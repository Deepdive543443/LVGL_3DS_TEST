#include "3ds.h"
#include "stdio.h"
#include "lvgl-8.3.11/lvgl.h"

#include "sections.h"

// User input place holder
// static touchPosition touch;
static u32 kDown;
static u32 kHeld;

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
    static lv_disp_drv_t disp_drv_btm;        /*Descriptor of a display driver*/
    lv_disp_t *disp_btm = display_init(GFX_BOTTOM, &draw_buf_btm, &buf1_btm, &disp_drv_btm);


    static lv_disp_draw_buf_t draw_buf_top;
    static lv_color_t buf1_top[WIDTH_TOP * HEIGHT_TOP];
    static lv_disp_drv_t disp_drv_top;        /*Descriptor of a display driver*/
    lv_disp_t *disp_top = display_init(GFX_TOP, &draw_buf_top, &buf1_top, &disp_drv_top);

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