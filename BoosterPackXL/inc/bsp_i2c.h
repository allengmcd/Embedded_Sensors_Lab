#ifndef BSP_I2C_H
#define BSP_I2C_H

#include "includes.h"
#include "inc/hw_memmap.h"
#include "driverlib/debug.h"
#include "driverlib/gpio.h"
#include "driverlib/i2c.h"
#include "driverlib/sysctl.h"

typedef struct {
    uint32_t i2c_base;
    uint32_t sysctl_periph;
    uint32_t scl_gpio_periph;
    uint32_t scl_port;
    uint32_t scl_pin;
    uint32_t sda_gpio_periph;
    uint32_t sda_port;
    uint32_t sda_pin;
    uint32_t address;
  } I2C_Struct;

void BSP_I2C_Init(I2C_Struct *i2c_struct);
void BSP_I2C_Send_Single(I2C_Struct *i2c_struct, uint8_t data);
void BSP_I2C_Send_Burst(I2C_Struct *i2c_struct, uint8_t *data, uint32_t length);
void BSP_I2C_Receive(I2C_Struct *i2c_struct, uint8_t register, int16_t *result);

#endif