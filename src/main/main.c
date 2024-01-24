#include "3ds.h"
#include "stdio.h"
#include "lvgl-8.3.11/lvgl.h"

#define WIDTH_BTM 320
#define HEIGHT_BTM 240

#define WIDTH_TOP 400
#define HEIGHT_TOP 240

// User input place holder
// static touchPosition touch;
static u32 kDown;
static u32 kHeld;

void writePic2FrameBuf565(void *fb, lv_color_t * color, u16 x, u16 y, u16 w, u16 h)
{
    /* Display the LVGL buffer on 3DS screen 
     * void *fb: 3DS SDK's screen framebuffer input
     * void *img: input 16 bit framebuffer in 16bit per pixel
     * u16 x, y, w, h: Input area
     * */

    u8 *fb_8 = (u8*) fb;

    u16 i, j, draw_x, draw_y;
    for(j = 0; j < h; j++)
    {
        for(i = 0; i < w; i++)
        {
            draw_y = y + h - j;
            draw_x = x + i;

            u32 v = (draw_y + draw_x * h) * 3;

            fb_8[v + 2] = color[j * w + i].ch.red << 3;    //((data >> 11) & 0x1F) << 3;
            fb_8[v + 1] = color[j * w + i].ch.green << 2;  //((data >> 5) & 0x3F) << 2;
            fb_8[v] = color[j * w + i].ch.blue << 3;   //(data & 0x1F) << 3;
        }
    }

}

void flush_cb_3ds_btm(lv_disp_drv_t * disp, const lv_area_t * area, lv_color_t * color_p)
{
    /* Update and swap frame buffer
     * TODO -- figure out a more efficient solution.
     * */
    writePic2FrameBuf565(
        gfxGetFramebuffer(GFX_BOTTOM, GFX_LEFT, NULL, NULL),
        color_p,
        0,
        0,
        WIDTH_BTM,
        HEIGHT_BTM
    );

    // These three lines swap and display new buffer on screen
    gfxFlushBuffers();
    gfxScreenSwapBuffers(GFX_BOTTOM,true);
    gspWaitForVBlank();
    // gfxSwapBuffersGpu();
    // gfxSwapBuffers();

    lv_disp_flush_ready(disp);
}

void flush_cb_3ds_top(lv_disp_drv_t * disp, const lv_area_t * area, lv_color_t * color_p)
{
    /* Update and swap frame buffer
     * TODO -- figure out a more efficient solution.
     * */
    writePic2FrameBuf565(
        gfxGetFramebuffer(GFX_TOP, GFX_LEFT, NULL, NULL),
        color_p,
        0,
        0,
        WIDTH_TOP,
        HEIGHT_TOP
    );

    // These three lines swap and display new buffer on screen
    gfxFlushBuffers();
    gfxScreenSwapBuffers(GFX_TOP,true);
    gspWaitForVBlank();
    // gfxSwapBuffersGpu();
    // gfxSwapBuffers();

    lv_disp_flush_ready(disp);
}

void touch_cb_3ds(lv_indev_drv_t * drv, lv_indev_data_t*data)
{
    touchPosition touch;
    hidTouchRead(&touch);
    if(touch.px >=5 && touch.py >=5)
    {
        data->point.x = touch.px;
        data->point.y = touch.py;
        data->state = LV_INDEV_STATE_PRESSED;
    }
    else 
    {
        data->state = LV_INDEV_STATE_RELEASED;
    }
}

int main(int argc, char** argv)
{
    // Console init
    gfxInitDefault();
    // PrintConsole topScreen;
    // consoleInit(GFX_TOP, &topScreen);
    // consoleSelect(&topScreen);

    // IVGL init
    lv_init();

    // Display init
    static lv_disp_draw_buf_t draw_buf_btm;
    static lv_color_t buf1_btm[WIDTH_BTM * HEIGHT_BTM];
    lv_disp_draw_buf_init(&draw_buf_btm, buf1_btm, NULL, WIDTH_BTM * HEIGHT_BTM);

    static lv_disp_drv_t disp_drv_btm;        /*Descriptor of a display driver*/
    lv_disp_drv_init(&disp_drv_btm);          /*Basic initialization*/
    disp_drv_btm.flush_cb = flush_cb_3ds_btm;    /*Set your driver function*/
    disp_drv_btm.draw_buf = &draw_buf_btm;        /*Assign the buffer to the display*/
    disp_drv_btm.hor_res = WIDTH_BTM;   /*Set the horizontal resolution of the display*/
    disp_drv_btm.ver_res = HEIGHT_BTM;   /*Set the vertical resolution of the display*/
    disp_drv_btm.direct_mode = 1;           /*Enable direct mode*/
    lv_disp_t *disp_btm = lv_disp_drv_register(&disp_drv_btm);      /*Register function will return the display ptr*/


    static lv_disp_draw_buf_t draw_buf_top;
    static lv_color_t buf1_top[WIDTH_TOP * HEIGHT_TOP];
    lv_disp_draw_buf_init(&draw_buf_top, buf1_top, NULL, WIDTH_BTM * HEIGHT_BTM);

    static lv_disp_drv_t disp_drv_top;        /*Descriptor of a display driver*/
    lv_disp_drv_init(&disp_drv_top);          /*Basic initialization*/
    disp_drv_top.flush_cb = flush_cb_3ds_top;    /*Set your driver function*/
    disp_drv_top.draw_buf = &draw_buf_top;        /*Assign the buffer to the display*/
    disp_drv_top.hor_res = WIDTH_TOP;   /*Set the horizontal resolution of the display*/
    disp_drv_top.ver_res = HEIGHT_TOP;   /*Set the vertical resolution of the display*/
    disp_drv_top.direct_mode = 1;           /*Enable direct mode*/
    lv_disp_t *disp_top = lv_disp_drv_register(&disp_drv_top);      /*Finally register the driver*/

    // Touchpad init
    static lv_indev_drv_t indev_drv_touch;
    lv_indev_drv_init(&indev_drv_touch);      /*Basic initialization*/
    indev_drv_touch.type = LV_INDEV_TYPE_POINTER;
    indev_drv_touch.read_cb = touch_cb_3ds;
    lv_indev_t *my_indev = lv_indev_drv_register(&indev_drv_touch);

    /* Choose one example or demo from below*/
    // Examples
    lv_disp_set_default(disp_top);
    // lv_example_btnmatrix_2();
    // lv_example_calendar_1();
    // lv_example_style_13();
    // lv_example_spinner_3ds();
    lv_example_anim_2();

    // Demo
    lv_disp_set_default(disp_btm);
    lv_demo_widgets();
    int demo_idx = 0;
    // lv_demo_benchmark();
    // lv_example_get_started_1();



    // printf("Hello, LVGL on 3ds\nPress SELECT to switch demo");
    while(aptMainLoop())
    {
        // User input
        hidScanInput();
        kDown = hidKeysDown();
        kHeld = hidKeysHeld();

        if(kDown & KEY_SELECT)
        {
            switch (demo_idx)
            {
                case 0:
                    lv_demo_widgets_close();
                    demo_idx++;
                    lv_demo_benchmark();
                    break;

                case 1:
                    lv_demo_benchmark_close();
                    demo_idx++;
                    lv_demo_keypad_encoder();
                    break;

                case 2:
                    lv_demo_keypad_encoder_close();
                    demo_idx = 0;
                    lv_demo_widgets();
                    break;
            
                default:
                    break;
            }
        }

        // Quit App
        if(kHeld & KEY_START) break;

        lv_timer_handler();
        usleep(1000);
        lv_tick_inc(1);
    }
    return 0;
}