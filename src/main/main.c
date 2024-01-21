#include "3ds.h"
#include "stdio.h"
#include <pthread.h>
#include "lvgl-8.3.11/lvgl.h"
// #include "lv_drv_conf.h"
// #include "lv_conf.h"

#define HEIGHT_BTM 240
#define WIDTH_BTM 320

#define WIDTH_TOP 400
#define HEIGHT_TOP 240

void writePic2FrameBuf565(void *fb, lv_color_t * color, u16 x, u16 y, u16 w, u16 h)
{
    /* Display the LVGL buffer on 3DS screen 
     * void *fb: 3DS SDK's screen framebuffer input
     * void *img: input 16 bit framebuffer in 16bit per pixel
     * u16 x, y, w, h: Input area
     * */

    u8 *fb_8 = (u8*) fb;
    // u16 *img_16 = (u16*) img;

    int i, j, draw_x, draw_y;
    for(j = 0; j < h; j++)
    {
        for(i = 0; i < w; i++)
        {
            draw_y = y + h - j;
            draw_x = x + i;

            u32 v = (draw_y + draw_x * h) * 3;
            // u16 data = img_16[j * w + i];

            u8 r = color[j * w + i].ch.red << 3;    //((data >> 11) & 0x1F) << 3;
            u8 g = color[j * w + i].ch.green << 2;  //((data >> 5) & 0x3F) << 2;
            u8 b = color[j * w + i].ch.blue << 3;   //(data & 0x1F) << 3;

            fb_8[v] = r;
            fb_8[v + 1] = g;
            fb_8[v + 2] = b;
        }
    }

}

void flush_cb_3ds(lv_disp_drv_t * disp, const lv_area_t * area, lv_color_t * color_p)
{
    // int32_t x, y;
    /*It's a very slow but simple implementation.
     *`set_pixel` needs to be written by you to a set pixel on the screen*/
    printf("Ja\n");
    writePic2FrameBuf565(
        gfxGetFramebuffer(GFX_BOTTOM, GFX_LEFT, NULL, NULL),
        color_p,
        area->x1,
        area->y1,
        area->x2 - area->x1,
        area->y2 - area->y1
    );

    // These three lines swap and display new buffer on screen
    gfxFlushBuffers();
    gspWaitForVBlank();
    gfxSwapBuffers();

    lv_disp_flush_ready(disp);         /* Indicate you are ready with the flushing*/
}

void lv_example_spinner_3ds(void)
{
    /*Create a spinner*/
    lv_obj_t * spinner = lv_spinner_create(lv_scr_act(), 60, 60);
    lv_obj_set_size(spinner, 50, 50);
    lv_obj_center(spinner);
}


int main(int argc, char** argv)
{
    // Console init
    gfxInitDefault();
    u32 kDown;
    u32 kHeld;
    PrintConsole topScreen;
    consoleInit(GFX_TOP, &topScreen);
    consoleSelect(&topScreen);

    // IVGL init
    lv_init();

    // Draw buffer
    static lv_disp_draw_buf_t draw_buf;
    static lv_color_t buf1[WIDTH_BTM * HEIGHT_BTM * 2];
    lv_disp_draw_buf_init(&draw_buf, buf1, NULL, WIDTH_BTM * HEIGHT_BTM * 2);
    
    static lv_disp_drv_t disp_drv;        /*Descriptor of a display driver*/
    lv_disp_drv_init(&disp_drv);          /*Basic initialization*/
    disp_drv.flush_cb = flush_cb_3ds;    /*Set your driver function*/
    disp_drv.draw_buf = &draw_buf;        /*Assign the buffer to the display*/
    disp_drv.hor_res = WIDTH_BTM;   /*Set the horizontal resolution of the display*/
    disp_drv.ver_res = HEIGHT_BTM;   /*Set the vertical resolution of the display*/
    lv_disp_drv_register(&disp_drv);      /*Finally register the driver*/


    // Examples
    // lv_example_btnmatrix_2();
    lv_example_spinner_3ds();
    

    /* Display init */
    // static lv_disp_draw_buf_t draw_buff;
    // lv_display_t *disp_top = lv_display_create(WIDTH, HEIGHT);
    // lv_display_set_flush_cb(disp_top, flush_cb_3ds);

    // static uint16_t buf[WIDTH * HEIGHT * 2];
    // lv_display_set_buffers(disp_top, buf, NULL, sizeof(buf), LV_DISP_RENDER_MODE_FULL);



    printf("Hello, LVGL on 3ds\n");
    while(aptMainLoop())
    {
        // User input
        hidScanInput();
        kDown = hidKeysDown();
        kHeld = hidKeysHeld();
        touchPosition touch;
        hidTouchRead(&touch);

        // Quit App
        if(kHeld & KEY_START) break;

        lv_timer_handler();

    }
    return 0;
}