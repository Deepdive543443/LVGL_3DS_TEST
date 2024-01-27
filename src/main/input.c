#include "sections.h"

void virtual_LR_cb(lv_indev_drv_t *drv, lv_indev_data_t *data)
{
    u32 kDown = hidKeysDown();
    u32 kHeld = hidKeysHeld();
    u8 id = 0;
    
    if (kHeld & KEY_L)
    {
        id = 1;
    }
    else if (kHeld & KEY_R)
    {
        id = 2;
    }

    if (id > 0)
    {
        data->btn_id = id;
        data->state = LV_INDEV_STATE_PRESSED;
    }
    else
    {
        data->state = LV_INDEV_STATE_RELEASED;
    }
}

void touch_cb_3ds(lv_indev_drv_t *drv, lv_indev_data_t *data)
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

void encoder_cb_3ds(lv_indev_drv_t *drv, lv_indev_data_t *data)
{
    u32 kDown = hidKeysDown();
    u32 kHeld = hidKeysHeld();

    // Enter (A)
    data->state = LV_INDEV_STATE_PRESSED;
    if (kHeld & KEY_A)
    {
        data->key = LV_KEY_ENTER;
    }
    // LEFT (LEFT, UP)
    else if (kHeld & KEY_LEFT || kHeld & KEY_UP)
    {
        data->key = LV_KEY_LEFT;
    }
    // RIGHT (RIGHT, DOWN)
    else if (kHeld & KEY_RIGHT || kHeld & KEY_DOWN)
    {
        data->key = LV_KEY_RIGHT;
    }
    else
    {
        data->state = LV_INDEV_STATE_RELEASED;
    }

}