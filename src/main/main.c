#include "3ds.h"
#include "stdio.h"
#include "lvgl-8.3.11/lvgl.h"

#define WIDTH_BTM 320
#define HEIGHT_BTM 240

#define WIDTH_TOP 400
#define HEIGHT_TOP 240

// User input place holder
static touchPosition touch;
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
            // u16 data = img_16[j * w + i];

            u8 r = color[j * w + i].ch.red << 3;    //((data >> 11) & 0x1F) << 3;
            u8 g = color[j * w + i].ch.green << 2;  //((data >> 5) & 0x3F) << 2;
            u8 b = color[j * w + i].ch.blue << 3;   //(data & 0x1F) << 3;

            fb_8[v] = b;
            fb_8[v + 1] = g;
            fb_8[v + 2] = r;
        }
    }

}

void flush_cb_3ds(lv_disp_drv_t * disp, const lv_area_t * area, lv_color_t * color_p)
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
    gspWaitForVBlank();
    gfxSwapBuffers();

    lv_disp_flush_ready(disp);
}

void touch_cb_3ds(lv_indev_drv_t * drv, lv_indev_data_t*data)
{

    hidTouchRead(&touch);
    printf("\x1b[2;0H%03d; %03d", touch.px, touch.py);
    if(touch.px >=5 && touch.py >=5)
    {
        printf("Ja\n");
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
    PrintConsole topScreen;
    consoleInit(GFX_TOP, &topScreen);
    consoleSelect(&topScreen);

    // IVGL init
    lv_init();

    // Draw buffer
    static lv_disp_draw_buf_t draw_buf;
    static lv_color_t buf1[WIDTH_BTM * HEIGHT_BTM];
    lv_disp_draw_buf_init(&draw_buf, buf1, NULL, WIDTH_BTM * HEIGHT_BTM);

    // Display init
    static lv_disp_drv_t disp_drv;        /*Descriptor of a display driver*/
    lv_disp_drv_init(&disp_drv);          /*Basic initialization*/
    disp_drv.flush_cb = flush_cb_3ds;    /*Set your driver function*/
    disp_drv.draw_buf = &draw_buf;        /*Assign the buffer to the display*/
    disp_drv.hor_res = WIDTH_BTM;   /*Set the horizontal resolution of the display*/
    disp_drv.ver_res = HEIGHT_BTM;   /*Set the vertical resolution of the display*/
    disp_drv.direct_mode = 1;           /*Enable direct mode*/
    lv_disp_drv_register(&disp_drv);      /*Finally register the driver*/

    // Touchpad init
    static lv_indev_drv_t indev_drv_touch;
    lv_indev_drv_init(&indev_drv_touch);      /*Basic initialization*/
    indev_drv_touch.type = LV_INDEV_TYPE_POINTER;
    indev_drv_touch.read_cb = touch_cb_3ds;
    lv_indev_t *my_indev = lv_indev_drv_register(&indev_drv_touch);

    /* Choose one example or demo from below*/
    // Examples
    // lv_example_btnmatrix_2();
    // lv_example_calendar_1();
    // lv_example_style_13();
    // lv_example_spinner_3ds();
    // lv_example_anim_2();

    // Demo
    lv_demo_widgets();
    // lv_demo_benchmark();
    // lv_example_get_started_1();



    printf("Hello, LVGL on 3ds\n");
    while(aptMainLoop())
    {
        // User input
        hidScanInput();
        kDown = hidKeysDown();
        kHeld = hidKeysHeld();
        // hidTouchRead(&touch);

        // Quit App
        if(kHeld & KEY_START) break;

        lv_timer_handler();
        usleep(5000);
        lv_tick_inc(5);
    }
    return 0;
}