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

void create_shoulder_button()
{
    /* Create L, R button that aligned with top left and top right of screen
     * Width: 90,  Height: 30*/
    lv_obj_t *btn_L = lv_btn_create(lv_scr_act());    /*Add a button the current screen*/                            /*Set its position*/
    lv_obj_set_size(btn_L, 90, 30);
    lv_obj_align(btn_L, LV_ALIGN_TOP_LEFT, -10, -5);
    lv_obj_t *label_L = lv_label_create(btn_L);          /*Add a label to the button*/
    lv_label_set_text(label_L, "L");                     /*Set the labels text*/
    lv_obj_align(label_L, LV_ALIGN_RIGHT_MID, 0, 0);

    lv_obj_t *btn_R = lv_btn_create(lv_scr_act());     /*Add a button the current screen*/                            /*Set its position*/
    lv_obj_set_size(btn_R, 90, 30);
    lv_obj_align(btn_R, LV_ALIGN_TOP_RIGHT, 10, -5);
    lv_obj_t *label_R = lv_label_create(btn_R);          /*Add a label to the button*/
    lv_label_set_text(label_R, "R");                     /*Set the labels text*/
    lv_obj_align(label_R, LV_ALIGN_LEFT_MID, 0, 0);
}

void create_bottom_container()
{
    // lv_obj_t *btm_container = lv_obj_create(lv_scr_act());
    // // lv_obj_set_align(btm_container, LV_ALIGN_BOTTOM_MID, 0, 0);
    // lv_obj_align(btm_container, LV_ALIGN_BOTTOM_MID, 0, 0);
    // lv_obj_set_size(btm_container, lv_pct(110), LV_SIZE_CONTENT);
    // lv_obj_set_flex_align(btm_container,
    //     LV_FLEX_ALIGN_START,
    //     LV_FLEX_ALIGN_START,
    //     LV_FLEX_ALIGN_START);

    // lv_obj_t *btn_A = lv_btn_create(btm_container);
    // lv_obj_t *btn_B = lv_btn_create(btm_container);
    
    // lv_obj_set_size(btn_A, lv_pct(50), 30);
    // lv_obj_set_size(btn_B, lv_pct(50), 30);


    static lv_style_t style_bg;
    lv_style_init(&style_bg);
    lv_style_set_pad_all(&style_bg, 0);
    lv_style_set_pad_gap(&style_bg, 0);
    lv_style_set_clip_corner(&style_bg, true);
    lv_style_set_radius(&style_bg, 14);
    
    lv_style_set_border_width(&style_bg, 0);


    static lv_style_t style_btn;
    lv_style_init(&style_btn);
    lv_style_set_radius(&style_btn, 0);

    lv_style_set_bg_color(&style_btn, lv_palette_main(LV_PALETTE_GREY));
    lv_style_set_bg_grad_color(&style_btn, lv_palette_lighten(LV_PALETTE_GREY, 3));
    lv_style_set_bg_grad_dir(&style_btn, LV_GRAD_DIR_VER);
    
    lv_style_set_border_width(&style_btn, 1);
    lv_style_set_border_opa(&style_btn, LV_OPA_100);
    lv_style_set_border_color(&style_btn, lv_color_hex(0x777777)); //lv_palette_main(LV_PALETTE_GREY)
    lv_style_set_border_side(&style_btn, LV_BORDER_SIDE_INTERNAL);
    lv_style_set_radius(&style_btn, 0);


    static const char *btnm_map[] = {"A", "B", "X", "Y", ""};
    lv_obj_t *btnm1 = lv_btnmatrix_create(lv_scr_act());
    lv_obj_align(btnm1, LV_ALIGN_BOTTOM_MID, 0, 19);
    lv_obj_set_size(btnm1, lv_pct(99), lv_pct(20));
    lv_btnmatrix_set_map(btnm1, btnm_map);
    lv_obj_add_style(btnm1, &style_bg, 0);
    lv_obj_add_style(btnm1, &style_btn, LV_PART_ITEMS);
    
}


lv_obj_t *put_text_example(const char *string)
{
    static lv_style_t style;
    lv_style_init(&style);
    lv_style_set_radius(&style, 5);

    /*Make a gradient*/
    lv_style_set_width(&style, 150);
    lv_style_set_height(&style, LV_SIZE_CONTENT); /* This enable a flexible width and height that change 
                                                   * during inputing*/


    lv_style_set_pad_ver(&style, 20);
    lv_style_set_pad_left(&style, 5);

    lv_style_set_align(&style, LV_ALIGN_CENTER);

    // lv_style_set_x(&style, lv_pct(2)); // percentage of 2 
    // lv_style_set_y(&style, 180);

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
    lv_obj_clear_flag(lv_scr_act(), LV_OBJ_FLAG_SCROLLABLE); // We don't want the top screen to be scrollable
    lv_obj_t *top_text = put_text_example("init");
    create_shoulder_button();
    create_bottom_container();
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