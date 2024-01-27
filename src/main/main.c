#include "sys/time.h"
#include "stdio.h"

#include "3ds.h" 
#include "lvgl-8.3.11/lvgl.h"

#include "sections.h"


static struct timespec start, end;

bool ticker()
{
    /* Hands the main loop until it reach the tick time*/
    clock_gettime(CLOCK_MONOTONIC, &end);
    uint64_t delta_us = (end.tv_sec - start.tv_sec) * 1000000 + (end.tv_nsec - start.tv_nsec) / 1000;
    return delta_us < TICK_MS;
}

// User input place holder
static u32 kDown;
static u32 kHeld;
static circlePosition joy_stick;

int main(int argc, char** argv)
{
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



    /* Choose one example or demo from below*/
    // Examples
    lv_disp_set_default(disp_top);
    const lv_point_t points_array[] = {{-1, -1}/*Null point*/, {5,5},{WIDTH_TOP - 5,5} };
    static lv_indev_drv_t indev_drv_LR;
    indev_drv_LR.type = LV_INDEV_TYPE_BUTTON;
    indev_drv_LR.read_cb = virtual_LR_cb;
    lv_indev_t *lr_indev = lv_indev_drv_register(&indev_drv_LR);
    lv_indev_set_button_points(lr_indev, points_array);


    lv_obj_clear_flag(lv_scr_act(), LV_OBJ_FLAG_SCROLLABLE); // We don't want the top screen to be scrollable
    lv_obj_t *top_text = put_text_example("Hello\nLVGL 3DS");
    ui_LR_t ui_LR = create_shoulder_button();
    lv_obj_t *btnm1 = create_bottom_container();
    lv_obj_t *js = create_joystick();


    lv_disp_set_default(disp_btm);
    // Touchpad init
    static lv_indev_drv_t indev_drv_touch;
    lv_indev_drv_init(&indev_drv_touch);      /*Basic initialization*/
    indev_drv_touch.type = LV_INDEV_TYPE_POINTER;
    indev_drv_touch.read_cb = touch_cb_3ds;
    lv_indev_t *touch_indev = lv_indev_drv_register(&indev_drv_touch);


    static lv_indev_drv_t indev_drv_cross;
    lv_indev_drv_init(&indev_drv_cross);
    indev_drv_cross.type = LV_INDEV_TYPE_ENCODER;
    indev_drv_cross.read_cb = encoder_cb_3ds;
    lv_indev_t *enc_indev = lv_indev_drv_register(&indev_drv_cross);


    lv_group_t *g = lv_group_create();
    lv_group_add_obj(g, btnm1);
    lv_indev_set_group(enc_indev, g);

    // Demo
    lv_demo_widgets();
    int demo_idx = 0;
    // lv_demo_benchmark();
    // lv_example_get_started_1();
    // lv_example_btnmatrix_2();
    // lv_example_calendar_1();
    // lv_example_style_13();
    // lv_example_spinner_3ds();
    // lv_example_anim_2();
    // lv_example_style_1();

    while(aptMainLoop())
    {
        clock_gettime(CLOCK_MONOTONIC, &start);
        // User input
        hidScanInput();
        kDown = hidKeysDown();
        kHeld = hidKeysHeld();
        hidCircleRead(&joy_stick);


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

        if(kHeld & KEY_R)
        {
            lv_obj_set_style_bg_color(ui_LR.R, lv_color_hex(0xbcbcbc), NULL);
        }
        
        update_joy_stick(js, &joy_stick);

        lv_timer_handler();
        while (ticker());

        // Display joystick
        // char top_string[30];
        // sprintf(top_string, "Joy stick x: %5d\nJoy stick y: %5d", joy_stick.dx, joy_stick.dy);
        // lv_label_set_text(top_text, top_string);    
        lv_tick_inc(TICK_S);
    }
    return 0;
}