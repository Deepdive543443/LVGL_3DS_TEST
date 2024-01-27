#ifndef SECTIONS_H
#define SECTIONS_H

#include "stdlib.h"

#include "3ds.h"
#include "lvgl-8.3.11/lvgl.h"

#define WIDTH_BTM 320
#define HEIGHT_BTM 240

#define WIDTH_TOP 400
#define HEIGHT_TOP 240

// display.c
void writePic2FrameBuf565(void *fb, lv_color_t * color, u16 x, u16 y, u16 w, u16 h);
void flush_cb_3ds_btm(lv_disp_drv_t *disp, const lv_area_t *area, lv_color_t *color_p);
void flush_cb_3ds_top(lv_disp_drv_t *disp, const lv_area_t *area, lv_color_t *color_p);
void flush_cb_3ds(gfxScreen_t gfx_scr, lv_disp_drv_t *disp, const lv_area_t *area, lv_color_t *color_p);
lv_disp_t *display_init(gfxScreen_t gfx_scr, lv_disp_draw_buf_t *draw_buf, lv_color_t *buf1, lv_disp_drv_t *disp_drv);

// ui.c
typedef struct
{
    lv_obj_t *L;
    lv_obj_t *R;
} ui_LR_t;

typedef struct
{
    lv_obj_t *A;
    lv_obj_t *B;
    lv_obj_t *X;
    lv_obj_t *Y;
} ui_ABXY_t;


lv_obj_t *create_joystick();
ui_LR_t create_shoulder_button();
lv_obj_t *create_bottom_container();
lv_obj_t *put_text_example(const char *string);
void update_joy_stick(lv_obj_t *js, circlePosition *js_read);

// input.c
void virtual_L_cb(lv_indev_drv_t *drv, lv_indev_data_t *data);
void virtual_R_cb(lv_indev_drv_t *drv, lv_indev_data_t *data);
void virtual_LR_cb(u32 key, lv_indev_drv_t *drv, lv_indev_data_t *data);
void touch_cb_3ds(lv_indev_drv_t *drv, lv_indev_data_t *data);
void encoder_cb_3ds(lv_indev_drv_t *drv, lv_indev_data_t *data);

#endif // SECTIONS_H