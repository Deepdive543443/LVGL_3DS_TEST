#include "sections.h"


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
