#include <stdio.h>
#include <stdlib.h>
#include <errno.h>

#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

#include <pthread.h>
#include <sys/ioctl.h>

#include "../common/common.h"

/*=================================================================*/

#define LED_IOC_MAGIC	'L'
#define LED_ON	_IO(LED_IOC_MAGIC, 0)
#define LED_OFF	_IO(LED_IOC_MAGIC, 1)

static int led_fd = -1;
void led_set(int on)
{
   on? ioctl(led_fd,LED_ON) : ioctl(led_fd,LED_OFF);
    return;
}

/*=================================================================*/

int frequency;

const int trackX = 212, trackY=300, popupY=270;
inline void clear_screen();
int threading(void * m);
const int refreshX = 240, refreshY = 240, refreshW = 800, refreshH = 120;
fb_image *backgroundImage, *refreshBackground;

int main(int argc, char* argv[])
{
    fb_init("/dev/graphics/fb0");
    font_init("/data/local/font.ttc");
    touch_init("/dev/input/event3");

    if(led_fd == -1)
    {
        led_fd = open("/dev/led", O_RDWR);
        if(led_fd < 0){
            printf("open /dev/led failed, errno = %d\n", errno);
        }
    }

	// create thread
	pthread_t t1;
	int i,ret;
	ret = pthread_create(&t1,NULL,threading,NULL);
	if(ret!=0)
	{
		printf("Create pthread error!\n");
		exit(1);
	}


    // draw background image
    backgroundImage = fb_read_jpeg_image("/background.jpg");
    refreshBackground = fb_get_sub_image(
            backgroundImage, refreshX, refreshY, refreshW, refreshH);
    clear_screen();

    // draw controller
    fb_draw_track(trackX, trackY);
    fb_draw_infill(trackX, trackY, 0);
    fb_draw_slider(trackX, trackY);
    // fb_draw_popup(x, y);
    // fb_draw_poptext(x, y, val);

    fb_update();

    int type, x, y, finger;
    int isPressed, lastSliderX;

    while(1){
        type = touch_read(&x, &y, &finger);
        switch (type) {
            case TOUCH_PRESS:
                printf("TOUCH_PRESS：x=%d,y=%d,finger=%d\n", x, y, finger);
                if( x > lastSliderX - 25 && x < lastSliderX + 25 &&
                    y > trackY - 25 && y < trackY + 25 ){
                    fb_draw_popup(lastSliderX, popupY);
                    fb_draw_poptext(lastSliderX, popupY, (lastSliderX - trackX)/6);
                    fb_update();
                    isPressed = 1;
                }
                break;
            case TOUCH_MOVE: {
                printf("TOUCH_MOVE：x=%d,y=%d,finger=%d\n", x, y, finger);
                if (x < trackX)
                    x = trackX;
                if (x > trackX + 600)
                    x = trackX + 600;
                if (isPressed){
                    //redraw image
                    //fb_draw_image(refreshX, refreshY, refreshBackground, 0);
                    fb_draw_image(0, 0, backgroundImage, 0);
                    fb_draw_track(trackX, trackY);
                    fb_draw_infill(trackX, trackY, x - trackX);
                    fb_draw_slider(x, trackY);
                    fb_draw_popup(x, popupY);
                    fb_draw_poptext(x, popupY, (x - trackX)/6);
                    lastSliderX = x;
                    fb_update();

                    //get frequency
                    frequency = (x - trackX) / 6;
                }
                break;
            }
            case TOUCH_RELEASE:
                printf("TOUCH_RELEASE：x=%d,y=%d,finger=%d\n", x, y, finger);
                if (x < trackX)
                    x = trackX;
                if (x > trackX + 600)
                    x = trackX + 600;
                if (isPressed){
                    // redraw image, clear popup and popup text
                    //fb_draw_image(refreshX, refreshY, refreshBackground, 0);
                    fb_draw_image(0, 0, backgroundImage, 0);
                    fb_draw_track(trackX, trackY);
                    fb_draw_infill(trackX, trackY, x - trackX);
                    fb_draw_slider(x, trackY);
                    fb_update();
                    lastSliderX = x;
                    isPressed = 0;
                }
                break;
            default:
                printf("unknown\n");
                break;
        }
    }

    return 0;
}

void clear_screen(){
    fb_draw_image(0, 0, backgroundImage, 0);
    fb_draw_image(974, 550, fb_read_png_image("/button_50.png"), 0);
    //fb_draw_rect(0,0,1024,600,0);
    fb_update();
}

int threading(void *m)
{
    int cmd;
	while(1)
	{
        if(frequency == 0){
            printf("-- 1\n");
            led_set(0);
        } else if (frequency == 100) {
            printf("-- 2\n");
            led_set(1);
        } else {
            printf("-- 3, %d\n", 10000000 / (frequency + 100));
            led_set(1);
            usleep(10000000 / (frequency + 50));
            led_set(0);
            usleep(10000000 / (frequency + 50));
        }
	}
}
