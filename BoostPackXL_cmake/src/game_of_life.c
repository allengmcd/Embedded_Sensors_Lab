// #include "game_of_life.h"
// #include "lvgl.h"

// #define CANVAS_WIDTH  128
// #define CANVAS_HEIGHT  128

// static const char *labels[] = {
//     "Joystick X:", 
//     "Joystick Y:", 
//     "Temp:", 
//     "Voltage:", 
//     "Pressure:"
// };

// #define LABEL_COUNT (sizeof(labels) / sizeof(labels[0]))
// static lv_obj_t *label_objs[LABEL_COUNT];

// // Timer callback to update one pixel per frame
// void timer_cb(lv_timer_t *timer) {
//     (void)timer;
//     // lv_obj_t *container = lv_obj_create(lv_scr_act());
//     // lv_obj_set_size(container, 128, 128);
//     // lv_obj_center(container);

    
//     // lv_obj_t * container = (lv_obj_t *) lv_timer_get_user_data(timer);
    
//     // lv_label_set_text(label, "Hello world");
//     // lv_obj_set_style_text_color(lv_screen_active(), lv_color_hex(0xffffff), LV_PART_MAIN);
//     // lv_obj_align(label, LV_ALIGN_CENTER, 0, 0);



//     // // Enable flex layout in vertical column direction
//     // lv_obj_set_flex_flow(container, LV_FLEX_FLOW_COLUMN);
    
//     // // Align items to the start (left)
//     // lv_obj_set_flex_align(container,
//     //     LV_FLEX_ALIGN_START,  // Main axis: top-to-bottom
//     //     LV_FLEX_ALIGN_START,  // Cross axis: left-aligned
//     //     LV_FLEX_ALIGN_CENTER  // Track alignment (not as important here)
//     // );
    
    
//     for (unsigned int i = 0; i < LABEL_COUNT; i++) {
//         label_objs[i] = lv_label_create(lv_screen_active());
//         lv_label_set_text(label_objs[i], labels[i]);
//         lv_obj_set_style_text_color(label_objs[i], lv_color_white(), LV_PART_MAIN);
//         lv_obj_align(label_objs[i], LV_ALIGN_CENTER, -60, -60 + i * 20);
//     }
// }

// void create_canvas_with_pixel_timer(void) {
//     // /*Create a buffer for the canvas*/
//     // LV_DRAW_BUF_DEFINE_STATIC(draw_buf, CANVAS_WIDTH, CANVAS_HEIGHT, LV_COLOR_FORMAT_ARGB8888);
//     // LV_DRAW_BUF_INIT_STATIC(draw_buf);

//     // lv_obj_t * canvas = lv_canvas_create(lv_screen_active());
//     // lv_obj_set_size(canvas, CANVAS_WIDTH, CANVAS_HEIGHT);

//     // lv_obj_center(canvas);

//     // lv_canvas_set_draw_buf(canvas, &draw_buf);

//     lv_obj_t *container = lv_obj_create(lv_scr_act());
//     // lv_obj_set_size(container, 128, 128);
//     // lv_obj_center(container);


//     // // lv_timer_create(timer_cb, 1, container);


//     //     /*Change the active screen's background color*/
//     //     lv_obj_set_style_bg_color(lv_screen_active(), lv_color_hex(0x003a57), LV_PART_MAIN);

//         /*Create a white label, set its text and align it to the center*/
//         // lv_obj_t * label = lv_label_create(lv_screen_active());
//         // lv_label_set_text(label, "Hello world");
//         // lv_obj_set_style_text_color(lv_screen_active(), lv_color_hex(0xffffff), LV_PART_MAIN);
//         // lv_obj_align(label, LV_ALIGN_CENTER, 0, 0);
//         lv_timer_create(timer_cb, 1, container);
//         /*Change the active screen's background color*/
//         // lv_obj_set_style_bg_color(lv_screen_active(), lv_color_hex(0x003a57), LV_PART_MAIN);

//         // /*Create a white label, set its text and align it to the center*/
//         // lv_obj_t * label = lv_label_create(lv_screen_active());
//         // lv_label_set_text(label, "Joystick x:");
//         // lv_obj_set_style_text_color(lv_screen_active(), lv_color_hex(0xffffff), LV_PART_MAIN);
//         // lv_obj_align(label, LV_ALIGN_CENTER, -60, -60);
    
        
//         // lv_obj_t * label2 = lv_label_create(lv_screen_active());
//         // lv_label_set_text(label2, "Joystick x:");
//         // lv_obj_set_style_text_color(lv_screen_active(), lv_color_hex(0xffffff), LV_PART_MAIN);
//         // lv_obj_align(label2, LV_ALIGN_CENTER, -60, -40);
// }