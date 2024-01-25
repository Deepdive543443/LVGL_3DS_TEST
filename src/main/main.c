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

void create_button()
{
    /* Create L, R button that aligned with top left and top right of screen
     * Width: 90,  Height: 30*/
    lv_obj_t *btn_L = lv_btn_create(lv_scr_act());     /*Add a button the current screen*/                            /*Set its position*/
    lv_obj_set_size(btn_L, 90, 30);
    lv_obj_align(btn_L, LV_ALIGN_TOP_LEFT, -3, -3);
    lv_obj_t *label_L = lv_label_create(btn_L);          /*Add a label to the button*/
    lv_label_set_text(label_L, "L");                     /*Set the labels text*/
    lv_obj_align(label_L, LV_ALIGN_RIGHT_MID, 0, 0);

    lv_obj_t *btn_R = lv_btn_create(lv_scr_act());     /*Add a button the current screen*/                            /*Set its position*/
    lv_obj_set_size(btn_R, 90, 30);
    lv_obj_align(btn_R, LV_ALIGN_TOP_RIGHT, 3, -3);
    lv_obj_t *label_R = lv_label_create(btn_R);          /*Add a label to the button*/
    lv_label_set_text(label_R, "R");                     /*Set the labels text*/
    lv_obj_align(label_R, LV_ALIGN_LEFT_MID, 0, 0);

}


lv_obj_t *put_text_example(const char *string)
{
    static lv_style_t style;
    lv_style_init(&style);
    lv_style_set_radius(&style, 5);

    /*Make a gradient*/
    lv_style_set_width(&style, 150);
    lv_style_set_height(&style, LV_SIZE_CONTENT);

    lv_style_set_pad_ver(&style, 20);
    lv_style_set_pad_left(&style, 5);

    lv_style_set_x(&style, lv_pct(2));
    lv_style_set_y(&style, 180);

    /*Create an object with the new style*/
    lv_obj_t *obj = lv_obj_create(lv_scr_act());
    lv_obj_add_style(obj, &style, 0);

    lv_obj_t *label = lv_label_create(obj);
    lv_label_set_text(label, string);

    return label;
}


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

    // Touchpad init
    static lv_indev_drv_t indev_drv_touch;
    lv_indev_drv_init(&indev_drv_touch);      /*Basic initialization*/
    indev_drv_touch.type = LV_INDEV_TYPE_POINTER;
    indev_drv_touch.read_cb = touch_cb_3ds;
    lv_indev_t *my_indev = lv_indev_drv_register(&indev_drv_touch);

    /* Choose one example or demo from below*/
    // Examples
    lv_disp_set_default(disp_top);
    lv_obj_t *top_text = put_text_example("init");
    create_button();
    // lv_example_btnmatrix_2();
    // lv_example_calendar_1();
    // lv_example_style_13();
    // lv_example_spinner_3ds();
    // lv_example_anim_2();
    // lv_example_style_1();

    // Demo
    lv_disp_set_default(disp_btm);
    lv_demo_widgets();
    int demo_idx = 0;
    // lv_demo_benchmark();
    // lv_example_get_started_1();



    // printf("Hello, LVGL on 3ds\nPress SELECT to switch demo");
    
    while(aptMainLoop())
    {
        clock_gettime(CLOCK_MONOTONIC, &start);
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
        while (ticker());

        // Display latency
        // uint64_t delta_us = (end.tv_sec - start.tv_sec) * 1000000 + (end.tv_nsec - start.tv_nsec) / 1000;
        // char top_string[30];
        // sprintf(top_string, "%d m sec", delta_us);
        // lv_label_set_text(top_text, top_string);
        
        lv_tick_inc(TICK_S);
    }
    return 0;
}