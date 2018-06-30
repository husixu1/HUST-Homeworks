#ifndef _COMMON_H_
#define _COMMON_H_

#include <stdlib.h>
#include <unistd.h>
#include <errno.h>

/*======================== external.c ============================*/

int fb_get_time(void);

#define FB_COLOR(r,g,b)	(0xff000000|(r<<16)|(g<<8)|b)

#define FB_COLOR_RGB_8880	1
#define FB_COLOR_RGBA_8888	2
#define FB_COLOR_ALPHA_8	3

typedef struct {
	int color_type; /* FB_COLOR_XXXX */
	int pixel_w, pixel_h;
	int line_byte;
	char *content; /*4 byte align*/
} fb_image;

fb_image * fb_new_image(int color_type, int w, int h, int line_byte);
void fb_free_image(fb_image *image);

fb_image * fb_read_jpeg_image(char *file);
fb_image * fb_read_png_image(char *file);

/*得到一个图片的子图片,子图片和原图片共享颜色内存*/
fb_image *fb_get_sub_image(fb_image *img, int x, int y, int w, int h);

typedef struct {
	int bytes;	//UTF-8编码所占字节数
	int advance_x; //x方向步进距离
	int left;	//左跨距
	int top;	//上跨距
} fb_font_info;

void font_init(char *font_file);
fb_image * fb_read_font_image(const char *text, int pixel_size, fb_font_info *format);

/*=========================== graphic.c ===============================*/

#define SCREEN_WIDTH	1024
#define SCREEN_HEIGHT	600
#define BITS_PER_PIXEL	32

void fb_init(char *dev);
void fb_update(void);

/*lab2*/
void fb_draw_pixel(int x, int y, int color);
void fb_draw_rect(int x, int y, int w, int h, int color);
void fb_draw_line(int sx, int sy, int dx, int dy, int color);

/*lab3*/
void fb_draw_image(int x, int y, fb_image *image, int color);
void fb_draw_text(int x, int y, char *text, int font_size, int color);

/*lab4*/
#define TOUCH_NO_EVENT	0
#define TOUCH_PRESS	1
#define TOUCH_MOVE	2
#define TOUCH_RELEASE	3
#define FINGER_NUM_MAX	5

void touch_init(char *dev);
int touch_read(int *x, int *y, int *finger);

#endif /* _COMMON_H_ */
