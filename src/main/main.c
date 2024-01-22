#include "3ds.h"
#include "stdio.h"
#include <pthread.h>
#include "lvgl-8.3.11/lvgl.h"
// #include "lv_drv_conf.h"
// #include "lv_conf.h"

#define WIDTH_BTM 320
#define HEIGHT_BTM 240


#define WIDTH_TOP 400
#define HEIGHT_TOP 240

static touchPosition touch;


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

            u8 b = color[j * w + i].ch.red << 3;    //((data >> 11) & 0x1F) << 3;
            u8 g = color[j * w + i].ch.green << 2;  //((data >> 5) & 0x3F) << 2;
            u8 r = color[j * w + i].ch.blue << 3;   //(data & 0x1F) << 3;

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

    lv_disp_flush_ready(disp);         /* Indicate you are ready with the flushing*/
}

void touch_cb_3ds(lv_indev_drv_t * drv, lv_indev_data_t*data)
{
    // printf("Nein\n");
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

// static void anim_x_cb(void * var, int32_t v)
// {
//     lv_obj_set_x(var, v);
// }

// static void anim_size_cb(void * var, int32_t v)
// {
//     lv_obj_set_size(var, v, v);
// }

// /**
//  * Create a playback animation
//  */
// void lv_example_anim_2(void)
// {

//     lv_obj_t * obj = lv_obj_create(lv_scr_act());
//     lv_obj_set_style_bg_color(obj, lv_palette_main(LV_PALETTE_RED), 0);
//     lv_obj_set_style_radius(obj, LV_RADIUS_CIRCLE, 0);

//     lv_obj_align(obj, LV_ALIGN_LEFT_MID, 10, 0);

//     lv_anim_t a;
//     lv_anim_init(&a);
//     lv_anim_set_var(&a, obj);
//     lv_anim_set_values(&a, 10, 50);
//     lv_anim_set_time(&a, 1000);
//     lv_anim_set_playback_delay(&a, 100);
//     lv_anim_set_playback_time(&a, 300);
//     lv_anim_set_repeat_delay(&a, 500);
//     lv_anim_set_repeat_count(&a, LV_ANIM_REPEAT_INFINITE);
//     lv_anim_set_path_cb(&a, lv_anim_path_ease_in_out);

//     lv_anim_set_exec_cb(&a, anim_size_cb);
//     lv_anim_start(&a);
//     lv_anim_set_exec_cb(&a, anim_x_cb);
//     lv_anim_set_values(&a, 10, 240);
//     lv_anim_start(&a);
// }

// void lv_example_style_13(void)
// {
//     static lv_style_t style_indic;
//     lv_style_init(&style_indic);
//     lv_style_set_bg_color(&style_indic, lv_palette_lighten(LV_PALETTE_RED, 3));
//     lv_style_set_bg_grad_color(&style_indic, lv_palette_main(LV_PALETTE_RED));
//     lv_style_set_bg_grad_dir(&style_indic, LV_GRAD_DIR_HOR);

//     static lv_style_t style_indic_pr;
//     lv_style_init(&style_indic_pr);
//     lv_style_set_shadow_color(&style_indic_pr, lv_palette_main(LV_PALETTE_RED));
//     lv_style_set_shadow_width(&style_indic_pr, 10);
//     lv_style_set_shadow_spread(&style_indic_pr, 3);

//     /*Create an object with the new style_pr*/
//     lv_obj_t * obj = lv_slider_create(lv_scr_act());
//     lv_obj_add_style(obj, &style_indic, LV_PART_INDICATOR);
//     lv_obj_add_style(obj, &style_indic_pr, LV_PART_INDICATOR | LV_STATE_PRESSED);
//     lv_slider_set_value(obj, 70, LV_ANIM_OFF);
//     lv_obj_center(obj);
// }

int main(int argc, char** argv)
{
    // Console init
    gfxInitDefault();
    u32 kDown;
    u32 kHeld;
    PrintConsole topScreen;
    consoleInit(GFX_TOP, &topScreen);
    consoleSelect(&topScreen);

    // Input init
    // static touchPosition touch;

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
    disp_drv.direct_mode = 1;
    lv_disp_drv_register(&disp_drv);      /*Finally register the driver*/

    // Touchpad init
    static lv_indev_drv_t indev_drv_touch;
    lv_indev_drv_init(&indev_drv_touch);      /*Basic initialization*/
    indev_drv_touch.type = LV_INDEV_TYPE_POINTER;
    indev_drv_touch.read_cb = touch_cb_3ds;
    lv_indev_t *my_indev = lv_indev_drv_register(&indev_drv_touch);

    // Examples
    // lv_example_btnmatrix_2();
    lv_example_style_13();
    // lv_example_spinner_3ds();
    // lv_example_anim_2();
    

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
        // hidTouchRead(&touch);

        // Quit App
        if(kHeld & KEY_START) break;

        lv_timer_handler();
        // TODO -- figure out correct way to tick
        usleep(1000);
        lv_tick_inc(1);

    }
    return 0;
}