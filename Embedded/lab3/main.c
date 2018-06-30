#include <stdio.h>
#include "../common/common.h"

#define RED		FB_COLOR(255,0,0)
#define ORANGE	FB_COLOR(255,165,0)
#define YELLOW	FB_COLOR(255,255,0)
#define GREEN	FB_COLOR(0,255,0)
#define CYAN	FB_COLOR(0,127,255)
#define BLUE	FB_COLOR(0,0,255)
#define PURPLE	FB_COLOR(139,0,255)
#define WHITE   FB_COLOR(255,255,255)
#define BLACK   FB_COLOR(0,0,0)

int* main(int argc, char *argv[])
{
	fb_init("/dev/graphics/fb0");
	font_init("/data/local/font.ttc");

	fb_draw_rect(0,0,SCREEN_WIDTH,SCREEN_HEIGHT,BLACK);
	fb_update();

	fb_image *img;
	img = fb_read_jpeg_image("/data/local/jpg_test.jpg");
	fb_draw_image(0,0,img,0);
	fb_update();
	fb_free_image(img);

	img = fb_read_png_image("/data/local/png_test.png");
	fb_draw_image(100,300,img,0);
	fb_update();
	fb_free_image(img);

	img = fb_read_font_image("嵌",30,NULL);
	fb_draw_image(400,350,img,RED);
	fb_update();
	fb_free_image(img);

	fb_draw_text(50,50,"床前明月光，疑是地上霜。",64,PURPLE);
	fb_draw_text(50,120,"举头望明月，低头思故乡。",64,PURPLE);
	fb_update();
	return 0;
}
