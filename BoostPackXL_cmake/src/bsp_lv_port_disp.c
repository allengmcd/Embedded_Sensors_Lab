#include "bsp_lv_port_disp.h"
#include "bsp_systick_handler.h"
#include "st7735.h"

#define BYTES_PER_PIXEL (LV_COLOR_FORMAT_GET_SIZE(LV_COLOR_FORMAT_RGB565))
#define BUFFER_SIZE (ST7735_TFTWIDTH * ST7735_TFTHEIGHT/ 10 * BYTES_PER_PIXEL)
static uint8_t buf1[BUFFER_SIZE];
static uint8_t buf2[BUFFER_SIZE];


// static void lvgl_tick_cb(void);
void st7735_flush_cb(lv_display_t * display, const lv_area_t * area, uint8_t * px_map);

uint32_t BSP_LVGL_Tick_Handler();

uint32_t lvgl_tick_get(void) {
    return (OSTimeGet() * 1000UL) / OS_TICKS_PER_SEC;
}

/* Platform-specific implementation of the LCD send command function. In general this should use polling transfer. */
static void lcd_send_cmd(lv_display_t *disp, const uint8_t *cmd, size_t cmd_size, const uint8_t *param, size_t param_size)
{
        LV_UNUSED(disp);
        // while (lcd_bus_busy);   /* wait until previous transfer is finished */
        // /* Set the SPI in 8-bit mode */
        // hspi1.Init.DataSize = SPI_DATASIZE_8BIT;
        // HAL_SPI_Init(&hspi1);
        // /* DCX low (command) */
        // HAL_GPIO_WritePin(LCD_DCX_GPIO_Port, LCD_DCX_Pin, GPIO_PIN_RESET);
        // /* CS low */
        // HAL_GPIO_WritePin(LCD_CS_GPIO_Port, LCD_CS_Pin, GPIO_PIN_RESET);
        // /* send command */
        // if (HAL_SPI_Transmit(&hspi1, cmd, cmd_size, BUS_SPI1_POLL_TIMEOUT) == HAL_OK) {
        //         /* DCX high (data) */
        //         HAL_GPIO_WritePin(LCD_DCX_GPIO_Port, LCD_DCX_Pin, GPIO_PIN_SET);
        //         /* for short data blocks we use polling transfer */
        //         HAL_SPI_Transmit(&hspi1, (uint8_t *)param, (uint16_t)param_size, BUS_SPI1_POLL_TIMEOUT);
        //         /* CS high */
        //         HAL_GPIO_WritePin(LCD_CS_GPIO_Port, LCD_CS_Pin, GPIO_PIN_SET);
        // }

        UARTprintf("lcd_send_cmd...\n  ");
        writecommand_test(cmd,cmd_size,param,param_size);
}

/* Platform-specific implementation of the LCD send color function. For better performance this should use DMA transfer.
 * In case of a DMA transfer a callback must be installed to notify LVGL about the end of the transfer.
 */
static void lcd_send_color(lv_display_t *disp, const uint8_t *cmd, size_t cmd_size, uint8_t *param, size_t param_size)
{
    
    LV_UNUSED(disp);
        // while (lcd_bus_busy);   /* wait until previous transfer is finished */
        // /* Set the SPI in 8-bit mode */
        // hspi1.Init.DataSize = SPI_DATASIZE_8BIT;
        // HAL_SPI_Init(&hspi1);
        // /* DCX low (command) */
        // HAL_GPIO_WritePin(LCD_DCX_GPIO_Port, LCD_DCX_Pin, GPIO_PIN_RESET);
        // /* CS low */
        // HAL_GPIO_WritePin(LCD_CS_GPIO_Port, LCD_CS_Pin, GPIO_PIN_RESET);
        // /* send command */
        // if (HAL_SPI_Transmit(&hspi1, cmd, cmd_size, BUS_SPI1_POLL_TIMEOUT) == HAL_OK) {
        //         /* DCX high (data) */
        //         HAL_GPIO_WritePin(LCD_DCX_GPIO_Port, LCD_DCX_Pin, GPIO_PIN_SET);
        //         /* for color data use DMA transfer */
        //         /* Set the SPI in 16-bit mode to match endianness */
        //         hspi1.Init.DataSize = SPI_DATASIZE_16BIT;
        //         HAL_SPI_Init(&hspi1);
        //         lcd_bus_busy = 1;
        //         HAL_SPI_Transmit_DMA(&hspi1, param, (uint16_t)param_size / 2);
        //         /* NOTE: CS will be reset in the transfer ready callback */
        // }
        UARTprintf("lcd_send_color...\n  ");
    writecommand_test(cmd,cmd_size,param,param_size);
}


void BSP_LVGL_Init()
{
    // Initialize LVGL core first
    lv_init();
    lv_tick_set_cb(lvgl_tick_get);
    lv_display_t *disp_drv = lv_st7735_create(ST7735_TFTWIDTH, ST7735_TFTHEIGHT, LV_LCD_FLAG_NONE, lcd_send_cmd, lcd_send_color);
    BSP_LCD_Init();
    lv_display_set_flush_cb(disp_drv, st7735_flush_cb);
    lv_display_set_buffers(disp_drv, buf1, buf2, BUFFER_SIZE, LV_DISPLAY_RENDER_MODE_PARTIAL);
}

// static void lvgl_tick_cb(void)
// {
//     lv_tick_inc(1);  // Increment LVGL's internal tick counter on each SysTick
// }

void st7735_flush_cb(lv_display_t * display, const lv_area_t * area, uint8_t * px_map)
{
    // UARTprintf("st7735_flush_cb...\n");
    uint32_t buffer_length = (area->x2-area->x1+1)*(area->y2-area->y1+1);
    BSP_LCD_Flush(area->x1, area->y1, area->x2, area->y2, (uint16_t *)px_map,buffer_length);

    /* IMPORTANT!!!
     * Inform LVGL that flushing is complete so buffer can be modified again. */
    lv_display_flush_ready(display);
}