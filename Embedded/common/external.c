#include <fcntl.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <linux/fb.h>

#include "common.h"

int fb_get_time(void)
{
	int32_t now;
	struct timespec ts;
	clock_gettime(CLOCK_MONOTONIC, &ts);
	now = ts.tv_sec*1000+ts.tv_nsec/1000000;
	return now;
}

/*w, h maybe == 0*/
fb_image * fb_new_image(int color_type, int w, int h, int line_byte)
{
	fb_image *image;

	if((w<0)||(h<0)) return;
	switch(color_type)
	{
	case FB_COLOR_RGB_8880:
	case FB_COLOR_RGBA_8888:
		if(line_byte < w*4) line_byte = w*4;
		break;
	case FB_COLOR_ALPHA_8:
		if(line_byte < w) line_byte = w;
		break;
	default:
		return NULL;
	}

	image = (fb_image *)malloc(sizeof(fb_image) + line_byte*h);
	if(image == NULL) return NULL;

	image->color_type = color_type;
	image->line_byte = line_byte;
	image->pixel_w = w;
	image->pixel_h = h;
	image->content = (char *)(image+1);
	return;
}

fb_image *fb_get_sub_image(fb_image *img, int x, int y, int w, int h)
{
	fb_image *ret;
	if(img == NULL) return NULL;
	if((x<0)||(y<0)||
		(w<0)||(h<0)||
		(x+w > img->pixel_w)||
		(y+h > img->pixel_w))
		return NULL;

	ret = (fb_image *)malloc(sizeof(fb_image));
	if(ret != NULL) {
		ret->color_type = img->color_type;
		ret->line_byte = img->line_byte;
		ret->pixel_w = w;
		ret->pixel_h = h;
		if(img->color_type != FB_COLOR_ALPHA_8) x*=4;
		ret->content = img->content + y*img->line_byte + x;
	}
	return ret;
}

void fb_free_image(fb_image *image)
{
	if(image) free(image);
}

/*================== read a jpeg image ===============*/
#include <jpeglib.h>
fb_image *fb_read_jpeg_image(char *file)
{
	fb_image *image;
	//指定错误处理器
	struct jpeg_error_mgr errpub;
	//申请jpeg解压对象
	struct jpeg_decompress_struct cinfo;
	//将错误处理结构对象绑定在JPEG对象上
	cinfo.err = jpeg_std_error(&errpub);
	//初始化cinfo结构
	jpeg_create_decompress(&cinfo);

	//指定解压数据源
	FILE *infile;
	if((infile = fopen(file, "rb")) == NULL){
		printf("fb_read_jpeg_image: Failed to open file %s", file);
		return NULL;
	}
	jpeg_stdio_src(&cinfo, infile);

	//获取文件信息
	jpeg_read_header(&cinfo, TRUE);
	//为解压缩设定参数
	cinfo.dct_method = JDCT_IFAST;
	cinfo.do_fancy_upsampling = FALSE;
	cinfo.out_color_space = JCS_EXT_BGRX; //输出图像的色彩空间
	
	//开始解压缩
	jpeg_start_decompress(&cinfo);

	image = (fb_image *)fb_new_image(FB_COLOR_RGB_8880, cinfo.output_width, cinfo.output_height, 0);
	if(image == NULL){
		jpeg_destroy_decompress(&cinfo);
		fclose(infile);
		return NULL;
	}
	//取出数据
	char *dst_row = image->content;
	int dst_line = image->line_byte;
	while(cinfo.output_scanline < cinfo.output_height){
		jpeg_read_scanlines(&cinfo, (JSAMPARRAY)&dst_row, 1);
		dst_row += dst_line;
	}

	//解压缩完毕
	jpeg_finish_decompress(&cinfo);
	//释放资源
	jpeg_destroy_decompress(&cinfo);
	fclose(infile);
	return image;
}

/*================== read a png image ===============*/
#include <png.h>
fb_image *fb_read_png_image(char *file)
{
	fb_image *image=NULL;
	png_structp png_ptr;
	png_infop info_ptr;
	FILE *fp;
	
	fp = fopen(file, "rb");
	if(fp == NULL) {
		printf("open png file failed: error %d\n",errno);
		return NULL;
	}

	png_ptr = png_create_read_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);
	if (png_ptr == NULL) {
		printf("png_create_read_struct failed\n");
		fclose(fp);
		return NULL;
	}
	
	// Allocate/initialize the memory for image information.  REQUIRED. 
	info_ptr = png_create_info_struct(png_ptr);
	if(info_ptr == NULL) {
		printf("png_create_info_struct failed\n");
		fclose(fp);
		png_destroy_read_struct(&png_ptr, NULL, NULL);
		return NULL;
	}
	
	/* Set error handling if you are using the setjmp/longjmp method (this is
	* the normal method of doing things with libpng).  REQUIRED unless you
	* set up your own error handlers in the png_create_read_struct() earlier.
	*/
	if (setjmp(png_jmpbuf(png_ptr))) {
		/* Free all of the memory associated with the png_ptr and info_ptr */
		png_destroy_read_struct(&png_ptr, &info_ptr, NULL);
		/* If we get here, we had a problem reading the file */
		/*TLogE("open png image error \n");*/
		fclose(fp);
		fb_free_image(image);
		return NULL;
	}
	
	//bind libpng with fp
	png_init_io(png_ptr, fp);
	png_set_sig_bytes(png_ptr, 0);
	
	png_read_png(png_ptr, info_ptr, 
		PNG_TRANSFORM_STRIP_16|
		PNG_TRANSFORM_PACKING|
		PNG_TRANSFORM_EXPAND|
		PNG_TRANSFORM_BGR, NULL);
	
	image = (fb_image *)malloc(sizeof(fb_image));
	if(image == NULL){
		png_destroy_read_struct(&png_ptr, &info_ptr, NULL);
		fclose(fp);
		return NULL;
	}

	if(info_ptr->bit_depth == 16) {
		png_set_strip_16(png_ptr);
	}
	/*if(png_get_valid(png_ptr,info_ptr,PNG_INFO_tRNS)) {
		png_set_tRNS_to_alpha(png_ptr);
	}*/

	//not support color type
	if(info_ptr->channels != 4) {
		printf("unrecognized image format.\n");
		png_destroy_read_struct(&png_ptr, &info_ptr, NULL);
		fclose(fp);
		return NULL; 
	}

	image = fb_new_image(FB_COLOR_RGBA_8888, info_ptr->width, info_ptr->height, 0);
	if(image == NULL){
		png_destroy_read_struct(&png_ptr, &info_ptr, NULL);
		fclose(fp);
		return NULL;
	}
	
	char *dst = image->content;
	char **row_pointers;
	char *src;
	row_pointers = (char **)png_get_rows(png_ptr, info_ptr);
	int i = 0;
	while(i < info_ptr->height)
	{
		src = row_pointers[i];
		memcpy(dst, src, info_ptr->width * 4);
		dst += image->line_byte;
		++i;
	}

	png_destroy_read_struct(&png_ptr, &info_ptr, NULL);
	fclose(fp);
	return image;
}

/*================== read a font image ===============*/

#include <ft2build.h>
#include FT_FREETYPE_H

/* typedef struct  FT_Bitmap_
  {
    unsigned int    rows;
    unsigned int    width;
    int             pitch;
    unsigned char*  buffer;
    unsigned short  num_grays;
    unsigned char   pixel_mode;
    unsigned char   palette_mode;
    void*           palette;

  } FT_Bitmap;
 */

static FT_Library library=NULL;
static FT_Face face;

/** Change UTF-8 to Unicode **/
static FT_ULong Utf8ToUnicode(const char *utf8, int len)
{
	FT_ULong unic=0;
	int i;
	unsigned char flag = (len==2?0x1F:(len==3?0x0F:(len==4?0x07:0xFF)));
	if(len == 1){
		return (FT_ULong)(unsigned char)utf8[0];
	}else{
		for(i=0;i<len-1;i++){
			unic |= (((FT_ULong)((unsigned char)utf8[len-i-1]&0x3f)) << 6*i);
		}
		unic |= (((FT_ULong)((unsigned char)utf8[0]&flag))<<(len-1)*6);
	}
	return unic;
}

void font_init(char *font_file)
{
	FT_Error error;

	if(library == NULL)
	{
		error = FT_Init_FreeType(&library);
		if(error){
			printf("FT_Init_FreeType: error %d\n",error);
			library = NULL;
			return;
		}
	}
	if(face == NULL)
	{
		error = FT_New_Face(library, font_file, 0, &face);
		if(error){
			printf("FT_New_Face(\"%s\"): error %d\n", font_file, error);
			face = NULL;
			return;
		}
		error = FT_Select_Charmap(face, FT_ENCODING_UNICODE);
		if(error){
			printf("FT_Select_Charmap: error %d\n",error);
			face = NULL;
			return;
		}
	}
	return;
}

/** read a font image **/ 
fb_image* fb_read_font_image(const char *text, int pixel_size, fb_font_info *info)
{
	if(face == NULL) {
		printf("call font_init(\"font_file\") first\n");
		return NULL;
	}
	if((text == NULL)||(pixel_size <= 0)) {
		printf("arg error\n");
		return NULL;
	}

	FT_Error error = FT_Set_Pixel_Sizes(face, 0, pixel_size);
	if(error){
		printf("FT_Set_Pixel_Sizes: error %d\n", error);
		return NULL;
	}

	fb_font_info sinfo;
	FT_ULong ucs4;
	if((text[0]&0x80) == 0){
		sinfo.bytes = 1;
		ucs4 = (unsigned char)text[0];
	}else if((text[0]&0xE0) == 0xC0){
		sinfo.bytes = 2;
		ucs4 = ((text[0]&0x1F)<<6)|(text[1]&0x3F);
	}else if((text[0]&0xF0) == 0xE0){
		sinfo.bytes = 3;
		ucs4 = ((text[0]&0x0F)<<12)|((text[1]&0x3F)<<6)|(text[2]&0x3F);
	}else if((text[0]&0xF8) == 0xF0){
		sinfo.bytes = 4;
		ucs4 = ((text[0]&0x07)<<18)|((text[1]&0x3F)<<12)|((text[2]&0x3F)<<6)|(text[3]&0x3F);
	}else{
		printf("code error!\n");
		return NULL;
	}

	error = FT_Load_Char(face, ucs4, FT_LOAD_RENDER);
	if(error){
		printf("FT_Load_Char: error %d", error);
		return NULL;
	}
	FT_GlyphSlot slot = face->glyph;

	fb_image* image;
	/*when ucs4 == 0x20 (blank), the bitmap.width/rows/pitch is 0*/
	image = fb_new_image(FB_COLOR_ALPHA_8, slot->bitmap.width, slot->bitmap.rows, slot->bitmap.pitch);
	if(image == NULL){
		printf("fb_new_image(\"%s\", %d,%d,%d) failed\n", text, slot->bitmap.width, slot->bitmap.rows, slot->bitmap.pitch);
		return NULL;
	}
	memcpy(image->content, slot->bitmap.buffer, slot->bitmap.rows * slot->bitmap.pitch);

	sinfo.advance_x = slot->advance.x >> 6;
	sinfo.left = slot->bitmap_left;
	sinfo.top = slot->bitmap_top;
	if(info) *info = sinfo;
	return image;
}

