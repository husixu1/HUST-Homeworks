#include "common.h"
#include <linux/fb.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdio.h>
#include <sys/mman.h>
#include <string.h>
#include <math.h>

static int LCD_MEM_BUFFER[SCREEN_WIDTH * SCREEN_HEIGHT];
static int *LCD_FRAME_BUFFER = NULL;

static struct {
    int x1, y1, x2, y2;
} update_area = {0, 0, 0, 0};

void fb_init(char *dev) {
    int fd;
    struct fb_fix_screeninfo fb_fix;
    struct fb_var_screeninfo fb_var;

    if (LCD_FRAME_BUFFER != NULL)
        return; /*already done*/

    //First: Open the device
    if ((fd = open(dev, O_RDWR)) < 0) {
        printf("Unable to open framebuffer %s, errno = %d\n", dev, errno);
        return;
    }
    if (ioctl(fd, FBIOGET_FSCREENINFO, &fb_fix) < 0) {
        printf("Unable to FBIOGET_FSCREENINFO %s\n", dev);
        return;
    }
    if (ioctl(fd, FBIOGET_VSCREENINFO, &fb_var) < 0) {
        printf("Unable to FBIOGET_VSCREENINFO %s\n", dev);
        return;
    }

    printf("framebuffer info: bits_per_pixel=%u  width=%u  height=%u  line_length=%u  smem_len=%u\n",
            fb_var.bits_per_pixel, fb_var.xres, fb_var.yres, fb_fix.line_length, fb_fix.smem_len);

    //Second: mmap
    int *addr;
    size_t size = fb_var.xres * fb_var.yres * fb_var.bits_per_pixel / 8;
    addr = mmap(NULL, size, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);
    if ((int)addr == -1) {
        printf("failed to mmap memory for framebuffer.\n");
        return;
    }
    LCD_FRAME_BUFFER = addr;
    return;
}

/** copy data from mem buffer to frame buffer */
void fb_update(void) {
    if (LCD_FRAME_BUFFER == NULL) {
        printf("error: not allocate space for frame buffer\n");
        return;
    }

    if ((update_area.x1 >= SCREEN_WIDTH)
            || (update_area.x2 <= 0)
            || (update_area.y1 >= SCREEN_HEIGHT)
            || (update_area.y2 <= 0))
        return;

    int x, y, w, h;
    x = (update_area.x1 < 0) ? 0 : update_area.x1;
    y = (update_area.y1 < 0) ? 0 : update_area.y1;
    w = (update_area.x2 > SCREEN_WIDTH) ?
            SCREEN_WIDTH - x : update_area.x2 - x;
    h = (update_area.y2 > SCREEN_HEIGHT) ?
            SCREEN_HEIGHT - y : update_area.y2 - y;

    int *src, *dst;
    src = LCD_MEM_BUFFER + y * SCREEN_WIDTH + x;
    dst = LCD_FRAME_BUFFER + y * SCREEN_WIDTH + x;
    while (h-- > 0) {
        memcpy(dst, src, w * 4);
        src += SCREEN_WIDTH;
        dst += SCREEN_WIDTH;
    }

    update_area.x2 = 0;
    return;
}

static void _update_area(int x, int y, int w, int h) {
    //if((w <= 0)||(h <= 0)) return; /* sure */
    int x2 = x + w;
    int y2 = y + h;
    if (update_area.x2 == 0) {
        update_area.x1 = x;
        update_area.y1 = y;
        update_area.x2 = x2;
        update_area.y2 = y2;
    } else {
        if (update_area.x1 > x)
            update_area.x1 = x;
        if (update_area.y1 > y)
            update_area.y1 = y;
        if (update_area.x2 < x2)
            update_area.x2 = x2;
        if (update_area.y2 < y2)
            update_area.y2 = y2;
    }
    return;
}

/*======================================================================*/

void fb_draw_pixel(int x, int y, int color) {
    if (x < 0 || y < 0 || x >= SCREEN_WIDTH || y >= SCREEN_HEIGHT)
        return;
    _update_area(x, y, 1, 1);
    int *tmp = LCD_MEM_BUFFER + SCREEN_WIDTH * y + x;
    *tmp = color;
    return;
}

void fb_draw_rect(int x, int y, int w, int h, int color) {
    int i;
    int j;
    if (x < 0) {
        w += x;
        x = 0;
    }
    if (x + w > SCREEN_WIDTH) {
        w = SCREEN_WIDTH - x;
    }
    if (y < 0) {
        h += y;
        y = 0;
    }
    if (y + h > SCREEN_HEIGHT) {
        h = SCREEN_HEIGHT - y;
    }
    if (w <= 0 || h <= 0)
        return;
    _update_area(x, y, w, h);
    /*---------------------------------------------------*/
    // printf("you need implement fb_draw_rect()\n"); exit(0);
    for (i = y ; i < y + h ; ++i) {
        for (j = x ; j < x + w ; ++j) {
            fb_draw_pixel(j, i, color);
        }
    }
    /*---------------------------------------------------*/
    return;
}

void swap(int *a, int *b) {
    int temp = *a;
    *a = *b;
    *b = temp;
}

// returns absolute value of number
float absolute(float x ) {
    return x > 0 ? x : -x;
}

//returns integer part of a floating point number
int iPartOfNumber(float x) {
    return (int)x;
}

//rounds off a number
int roundNumber(float x) {
    return iPartOfNumber(x + 0.5) ;
}

//returns fractional part of a number
float fPartOfNumber(float x) {
    return x > 0 ? x - iPartOfNumber(x) : x - (iPartOfNumber(x) + 1);
}

//returns 1 - fractional part of number
float rfPartOfNumber(float x) {
    return 1 - fPartOfNumber(x);
}

// get new color according to current color and brightness
int getNewColor(float bri, int x) {
    return x;
//    return FB_COLOR( ((int)(bri * 0.2990 * ((x & 0xff0000) >> 16))),\
//        ((int)(bri * 0.5870 * ((x & 0xff00) >> 8))),\
//        ((int)(bri * 0.1140 * (x & 0xff))) );
}

void fb_draw_line(int x1, int y1, int x2, int y2, int color) {
    /*---------------------------------------------------*/
    // printf("you need implement fb_draw_line()\n"); exit(0);
    int steep = absolute(y2 - y1) > absolute(x2 - x1) ;

    // swap the co-ordinates if slope > 1 or we
    // draw backwards
    if (steep) {
        swap(&x1, &y1);
        swap(&x2, &y2);
    }
    if (x1 > x2) {
        swap(&x1, &x2);
        swap(&y1, &y2);
    }

    //compute the slope
    float dx = x2 - x1;
    float dy = y2 - y1;
    float gradient = dy / dx;
    if (dx == 0.0)
        gradient = 1;

    int xpxl1 = x1;
    int xpxl2 = x2;
    float intersectY = y1;

    // main loop
    if (steep) {
        int x;
        for (x = xpxl1 ; x <= xpxl2 ; x++) {
            // pixel coverage is determined by fractional
            // part of y co-ordinate
            fb_draw_pixel(iPartOfNumber(intersectY), x, getNewColor(intersectY, color));
            fb_draw_pixel(iPartOfNumber(intersectY) - 1, x, getNewColor(intersectY, color));
            intersectY += gradient;
        }
    } else {
        int x;
        for (x = xpxl1 ; x <= xpxl2 ; x++) {
            // pixel coverage is determined by fractional part of y co-ordinate
            fb_draw_pixel(x, iPartOfNumber(intersectY), getNewColor(intersectY, color));
            fb_draw_pixel(x, iPartOfNumber(intersectY) - 1, getNewColor(intersectY, color));
            intersectY += gradient;
        }
    }
    /*---------------------------------------------------*/
    return;
}

/*======================================================================*/

void fb_draw_image(int x, int y, fb_image *image, int color) {
    if (image == NULL)
        return;

    int ix = 0; //image x
    int iy = 0; //image y
    int w = image->pixel_w; //draw width
    int h = image->pixel_h; //draw height

    if (x < 0) {
        w += x;
        ix -= x;
        x = 0;
    }
    if (y < 0) {
        h += y;
        iy -= y;
        y = 0;
    }

    if (x + w > SCREEN_WIDTH) {
        w = SCREEN_WIDTH - x;
    }
    if (y + h > SCREEN_HEIGHT) {
        h = SCREEN_HEIGHT - y;
    }
    if ((w <= 0) || (h <= 0))
        return;

    _update_area(x, y, w, h);

    int *dst = (int *)(LCD_MEM_BUFFER + y * SCREEN_WIDTH + x);
    char *src = image->content + iy * image->line_byte + ix * 4;
    /*---------------------------------------------------------------*/

    int temp;
    int ww, i;

    if (image->color_type == FB_COLOR_RGB_8880) { /*lab3: jpg*/
        //printf("you need implement fb_draw_image() FB_COLOR_RGB_8880\n");
        //exit(0);
        int bytes = (image->line_byte >= 4 * w) ? w * 4 : image->line_byte;
        for(ww = 0; ww < h; ++ww){
            memcpy(dst, src, bytes);
            dst += SCREEN_WIDTH;
            src += image->line_byte;
        }
        // int i, j;
        // for (i = y ; i < y + h ; ++i) {
        //     for (j = x ; j < x + w ; ++j) {
        //         int * temp = LCD_MEM_BUFFER + i * SCREEN_WIDTH + j;
        //         char * pcolor = (image->content + (i - y) * image->line_byte + (j - x) * 4);
        //         *temp = *(int *)pcolor;
        //     }
        // }
        return;
    }

    if (image->color_type == FB_COLOR_RGBA_8888) { /*lab3: png*/
        //printf("you need implement fb_draw_image() FB_COLOR_RGBA_8888\n");
        //exit(0);
        int i, j;
        char B1, G1, R1, alpha;
        for (i = y ; i < y + h ; ++i) {
            for (j = x ; j < x + w ; ++j) {
                int * temp = (int *)LCD_MEM_BUFFER + i * SCREEN_WIDTH + j;

                char * pcolor = image->content + (i - y) * image->line_byte + (j - x) * 4;
                char * p = temp;

                alpha = *(char *)(pcolor + 3);
                R1 = *(char *)(pcolor + 2);
                G1 = *(char *)(pcolor + 1);
                B1 = *(char *)(pcolor);
                // printf("%d %d %d %d\t", alpha, R1, G1, B1);
                switch (alpha) {
                    case 0: break;
                    case 255: p[0] = B1; p[1] = G1; p[2] = R1; break;
                    default:
                        // p[0] = (255 - alpha) * p[0] + alpha * B1;
                        // p[1] = (255 - alpha) * p[1] + alpha * G1;
                        // p[2] = (255 - alpha) * p[2] + alpha * R1;
                        p[0] += (((B1 - p[0]) * alpha) >> 8);
                        p[1] += (((G1 - p[1]) * alpha) >> 8);
                        p[2] += (((R1 - p[2]) * alpha) >> 8);
                        break;
                }
            }
        }
        return;
    }

    if (image->color_type == FB_COLOR_ALPHA_8) { /*lab3: font*/
        //printf("you need implement fb_draw_image() FB_COLOR_ALPHA_8\n");
        //exit(0);
        char *i;
        int *j;
        int count;
        int R1, G1, B1, alpha;
        for(ww = 0; ww < h; ++ww){
            i = src;
            j = dst;

           for(count = 0; count < w; ++count){
                alpha = *(char *)(i);
                R1    = (color & 0xff0000) >> 16;
                G1    = (color & 0xff00) >> 8;
                B1    = (color & 0xff);

                *((char *)(j)    ) += (((B1 - *((char *)(j)    )) * alpha) >> 8);
                *((char *)(j) + 1) += (((G1 - *((char *)(j) + 1)) * alpha) >> 8);
                *((char *)(j) + 2) += (((R1 - *((char *)(j) + 2)) * alpha) >> 8);

                ++i;
                ++j;
            }
            dst += SCREEN_WIDTH;
            src += image->line_byte;
        }
        return;
    }
    /*---------------------------------------------------------------*/
    return;
}

/** draw a text string **/
void fb_draw_text(int x, int y, char *text, int font_size, int color) {
    fb_image *img;
    fb_font_info info;
    int i = 0;
    int len = strlen(text);
    while (i < len) {
        img = fb_read_font_image(text + i, font_size, &info);
        if (img == NULL)
            break;
        fb_draw_image(x + info.left, y - info.top, img, color);
        fb_free_image(img);

        x += info.advance_x;
        i += info.bytes;
    }
    return;
}

void fb_draw_circle(int x, int y, int r, int color) {
    if (x - r < 0 || y - r < 0 || x + r > SCREEN_WIDTH || y + r > SCREEN_HEIGHT) return;
    _update_area(x - r, y - r, 2 * r , 2 * r);
    int i, j;
    for (i = y - r ; i <= y + r ; ++i) {
        for (j = x - r ; j < x + r ; ++j) {
            if ((i - y) * (i - y) + (j - x) * (j - x) <= r * r) {
                fb_draw_pixel(j, i, color);
            }
        }
    }
}

void fb_draw_thick_line(int x1, int y1, int x2, int y2, int radius, int color) {
    int i;
    int steep = absolute(y2 - y1) > absolute(x2 - x1);
    if (steep) {
        double slope = (double)(x2 - x1) / (y2 - y1);

        if (y2 < y1) {
            swap(&y1, &y2);
            swap(&x1, &x2);
        }
        for (i = y1 ; i <= y2 ; ++i) {
            fb_draw_circle((int)((double)x1 + (double)(slope * (i - y1))), i, radius, color);
        }
    } else {
        double slope = (double)(y2 - y1) / (x2 - x1);
        if (x2 < x1) {
            swap(&x1, &x2);
            swap(&y1, &y2);
        }
        // int y = (y2 > y1) ? y1 : y2;
        // int y_diff    = (y2 > y1) ? (y2 - y1) : (y1 - y2);
        // _update_area(x1 - radius, y - radius, x2 - x1 + 2 * radius, y_diff + 2 * radius);
        for (i = x1 ; i <= x2 ; ++i) {
            fb_draw_circle(i, (int)((double)y1 + (double)(slope * (i - x1))), radius, color);
        }
    }
}

#define FB_SLIDER_WIDTH 600
#define FB_SLIDER_HEIGHT 20
#define FB_SLIDER_RADIUS 20
#define FB_SLIDER_BACKGROUND_COLOR FB_COLOR(87, 96, 111)
#define FB_SLIDER_FOREGROUND_COLOR FB_COLOR(241, 242, 246)
#define FB_SLIDER_FILL_COLOR FB_COLOR(112, 161, 255)
#define FB_SLIDER_STROKE_LINE_WIDTH 1
#define FB_SLIDER_POPUP_RADIUS 25
#define FB_SLIDER_POPUP_COLOR FB_COLOR(55, 66, 250)
#define FB_SLIDER_TEXT_COLOR FB_COLOR(241 , 242, 246)
#define FB_HALF_DIAGONAL ((double)FB_SLIDER_POPUP_RADIUS / sqrt(2.0))

void fb_draw_track(int x, int y) {
    fb_draw_circle(x, y, FB_SLIDER_HEIGHT / 2, FB_SLIDER_BACKGROUND_COLOR);
    fb_draw_circle(x, y, FB_SLIDER_HEIGHT / 2 - FB_SLIDER_STROKE_LINE_WIDTH, FB_SLIDER_FILL_COLOR);

    fb_draw_circle(x + FB_SLIDER_WIDTH, y, FB_SLIDER_HEIGHT / 2, FB_SLIDER_BACKGROUND_COLOR);
    fb_draw_circle(x + FB_SLIDER_WIDTH, y, FB_SLIDER_HEIGHT / 2 - FB_SLIDER_STROKE_LINE_WIDTH, FB_SLIDER_FOREGROUND_COLOR);

    fb_draw_rect(x, y - FB_SLIDER_HEIGHT / 2, FB_SLIDER_WIDTH, FB_SLIDER_HEIGHT, FB_SLIDER_BACKGROUND_COLOR);
    fb_draw_rect(x, y - FB_SLIDER_HEIGHT / 2 + FB_SLIDER_STROKE_LINE_WIDTH, FB_SLIDER_WIDTH, FB_SLIDER_HEIGHT - 2 * FB_SLIDER_STROKE_LINE_WIDTH, FB_SLIDER_FOREGROUND_COLOR);
}

void fb_draw_infill(x, y, length) {
    if (length <= 0 || length > FB_SLIDER_WIDTH) {
        return;
    }
    fb_draw_rect(x, y - FB_SLIDER_HEIGHT / 2 + FB_SLIDER_STROKE_LINE_WIDTH, length, FB_SLIDER_HEIGHT - 2 * FB_SLIDER_STROKE_LINE_WIDTH, FB_SLIDER_FILL_COLOR);
}

void fb_draw_slider(x, y) {
    fb_draw_circle(x, y, FB_SLIDER_RADIUS, FB_SLIDER_BACKGROUND_COLOR);
    fb_draw_circle(x, y, FB_SLIDER_RADIUS - FB_SLIDER_STROKE_LINE_WIDTH, FB_COLOR(30, 144, 255));
}

void fb_draw_popup(x, y) {
    // draw the below half triangle
    int i, j;
    for (i = y ; i >= y - FB_HALF_DIAGONAL ; --i) {
        for (j = x - (y - i) ; j <= x + (y - i) ; ++j) {
            fb_draw_pixel(j, i, FB_SLIDER_POPUP_COLOR);
        }
    }

    // draw the up half circle
    fb_draw_circle(x, y - 2 * FB_HALF_DIAGONAL , FB_SLIDER_POPUP_RADIUS, FB_SLIDER_POPUP_COLOR);
}

void fb_draw_poptext(int x, int y, int value) {
    if (value < 0 || value > 100) return;

    char number[3];
    memset(number, 0, 3);
    if (value == 100) {
        sprintf(number, "ON");
    } else {
        if (value < 10) {
            number[0] = '0';
            number[1] = '0' + value;
        } else {
            number[0] = '0' + value / 10;
            number[1] = '0' + value % 10;
        }
    }

    fb_draw_text(x - 15, y - 1.5 * (double)FB_HALF_DIAGONAL , number, 30, FB_SLIDER_TEXT_COLOR);
}


