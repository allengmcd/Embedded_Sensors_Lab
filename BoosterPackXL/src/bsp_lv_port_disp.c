#include "bsp_lv_port_disp.h"

#include "st7735.h"

#define BYTES_PER_PIXEL (LV_COLOR_FORMAT_GET_SIZE(LV_COLOR_FORMAT_RGB565))
static uint8_t buf1[ST7735_TFTWIDTH * ST7735_TFTHEIGHT / 10 * 2];
static uint8_t buf2[ST7735_TFTWIDTH * ST7735_TFTHEIGHT / 10 * 2];


static void lvgl_tick_cb(void);
void st7735_flush_cb(lv_display_t * display, const lv_area_t * area, uint8_t * px_map);

uint32_t BSP_LVGL_Tick_Handler();

void BSP_LVGL_Init()
{
    // Initialize LVGL core first
    lv_init();

    // Register the LVGL tick incrementing function to be called on each SysTick
    BSP_RegisterTickCallback(lvgl_tick_cb);
    
    // Register the tick getter function with LVGL
    lv_tick_set_cb(BSP_GetTickCount);

    lv_display_t * display1 = lv_display_create(128, 128);

    lv_display_set_buffers(display1, buf1, buf2, sizeof(buf1), LV_DISPLAY_RENDER_MODE_PARTIAL);
}

static void lvgl_tick_cb(void)
{
    lv_tick_inc(1);  // Increment LVGL's internal tick counter on each SysTick
}

void st7735_flush_cb(lv_display_t * display, const lv_area_t * area, uint8_t * px_map)
{
    BSP_ST7735_PixelDrawMultiple(&st7735_display, area->x1, area->y1, area->x2, area->y2, (uint8_t *)px_map);

    /* IMPORTANT!!!
     * Inform LVGL that flushing is complete so buffer can be modified again. */
    lv_display_flush_ready(display);
}