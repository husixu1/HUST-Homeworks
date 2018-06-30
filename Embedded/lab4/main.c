#include <stdio.h>
#include "../common/common.h"

inline void clear_screen();

fb_image* backgroundImage;

int main(int argc, char *argv[]) {
    fb_init("/dev/graphics/fb0");
    backgroundImage = fb_read_jpeg_image("/background.jpg");
    clear_screen();

    touch_init("/dev/input/event3");
    int type, x, y, finger, i;
    int lastX[5] = {0,0,0,0,0}, lastY[5] = {0,0,0,0,0};
    int lastPressX = 0, lastPressY = 0, lastPressFinger = 0;
    while (1) {
        type = touch_read(&x, &y, &finger);

        switch (type) {
            case TOUCH_PRESS:
                printf("TOUCH_PRESS：x=%d,y=%d,finger=%d\n", x, y, finger);
                lastPressX = x, lastPressY = y, lastPressFinger = finger;
                break;
            case TOUCH_MOVE: {
                int color = 0;
                int radius = 0;
                switch (finger) {
                    case 0:
                        radius = 17;
                        color = 0xf13aa2;
                        break;
                    case 1:
                        radius = 14;
                        color = 0xad41e8;
                        break;
                    case 2:
                        radius = 11;
                        color = 0x6d02a8;
                        break;
                    case 3:
                        radius = 8;
                        color = 0xff3d3d;
                        break;
                    case 4:
                        radius = 05;
                        color = 0xff3d3d;
                        break;
                }
                fb_draw_thick_line(lastX[finger], lastY[finger], x, y, radius, color);
                fb_update();

                printf("TOUCH_MOVE：x=%d,y=%d,finger=%d\n", x, y, finger);
                break;
            }
            case TOUCH_RELEASE:
                printf("TOUCH_RELEASE：x=%d,y=%d,finger=%d\n", x, y, finger);
                if(lastPressX > 974 && lastPressY > 550 && lastPressFinger == finger &&
                        x > 974 && y > 550){
                    clear_screen();
                }
                break;
            default:
                printf("unknown\n");
                break;
        }
        lastX[finger] = x, lastY[finger] = y;
    }
    return 0;
}

void clear_screen(){
    fb_draw_image(0, 0, backgroundImage, 0);
    fb_draw_image(974, 550, fb_read_png_image("/button_50.png"), 0);
    //fb_draw_rect(0,0,1024,600,0);
    fb_update();
}
