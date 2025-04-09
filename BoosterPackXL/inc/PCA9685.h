#ifndef PCA9685_H
#define PCA9685_H

#include "includes.h"
#include "driverlib/debug.h"
#include "driverlib/i2c.h"
#include "driverlib/gpio.h"
#include "driverlib/sysctl.h"
#include "inc/hw_memmap.h"
#include "bsp_i2c.h"

void PCA9685_Init();


/**
 * Sets the frequency of PWM signals.
 * Frequency will be capped to range [40..1000] Hertz. Try 50 for servos.
 */
void PCA9685_PWM_Freq(uint16_t freq);

/**
 * Set all leds back to default values (: fullOff = 1)
 */
void PCA9685_PWM_Reset(uint8_t fd);

/**
 * Write on and off ticks manually to a pin
 * (Deactivates any full-on and full-off)
 */
void PCA9685_PWM_Write(uint8_t fd, uint8_t pin, uint8_t on, uint8_t off);

/**
 * Reads both on and off registers as 16 bit of data
 * To get PWM: mask each value with 0xFFF
 * To get full-on or off bit: mask with 0x1000
 * Note: ALL_LED pin will always return 0
 */
void PCA9685_PWM_Read(uint8_t fd, uint8_t pin, uint16_t *on, uint16_t *off);

/**
 * Enables or deactivates full-on
 * tf = true: full-on
 * tf = false: according to PWM
 */
void PCA9685_Full_On(uint8_t fd, uint8_t pin, uint8_t tf);

/**
 * Enables or deactivates full-off
 * tf = true: full-off
 * tf = false: according to PWM or full-on
 */
void PCA9685_Full_Off(uint8_t fd, uint8_t pin, uint8_t tf);

/**
 * Helper function to get to register
 */
uint8_t PCA9685_Base_Reg(uint8_t pin);


/**
 * Simple PWM control which sets on-tick to 0 and off-tick to value.
 * If value is <= 0, full-off will be enabled
 * If value is >= 4096, full-on will be enabled
 * Every value in between enables PWM output
 */
static void myPwmWrite(uint8_t pin, uint8_t value);

/**
 * Simple full-on and full-off control
 * If value is 0, full-off will be enabled
 * If value is not 0, full-on will be enabled
 */
static void myOnOffWrite(uint8_t pin, uint8_t value);

/**
 * Reads off registers as 16 bit of data
 * To get PWM: mask with 0xFFF
 * To get full-off bit: mask with 0x1000
 * Note: ALL_LED pin will always return 0
 */
static uint8_t myOffRead(uint8_t pin);

/**
 * Reads on registers as 16 bit of data
 * To get PWM: mask with 0xFFF
 * To get full-on bit: mask with 0x1000
 * Note: ALL_LED pin will always return 0
 */
static uint8_t myOnRead(uint8_t pin);

#endif