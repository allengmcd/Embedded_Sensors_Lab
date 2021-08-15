#define PART_TM4C123GH6PM
#define gcc 1

#include <stdbool.h>
#include <stdint.h>


#include "inc/hw_ints.h"
#include "inc/hw_memmap.h"
#include "inc/hw_adc.h"
#include "inc/hw_types.h"
#include "inc/hw_udma.h"
#include "driverlib/adc.h"
#include "driverlib/debug.h"
#include "driverlib/gpio.h"
#include "driverlib/interrupt.h"
#include "driverlib/pin_map.h"
#include "driverlib/rom.h"
#include "driverlib/rom_map.h"
#include "driverlib/sysctl.h"
#include "driverlib/systick.h"
#include "driverlib/timer.h"
#include "driverlib/uart.h"
#include "driverlib/udma.h"

#include "uartstdioTemp.h"

#define BUTTON_S1 GPIO_PIN_6
#define BUTTON_S2 GPIO_PIN_7
#define JOYSTICK_SEL GPIO_PIN_4

// ------------BSP_Joystick_Init------------
// Initialize a GPIO pin for input, which corresponds
// with BoosterPack pin J1.5 (Select button).
// Initialize two ADC pins, which correspond with
// BoosterPack pins J1.2 (X) and J3.26 (Y).
// Input: none
// Output: none
void BSP_Joystick_Init(void);

// ------------BSP_Joystick_Input------------
// Read and return the immediate status of the
// joystick.  Button de-bouncing for the Select
// button is not considered.  The joystick X- and
// Y-positions are returned as 10-bit numbers,
// even if the ADC on the LaunchPad is more precise.
// Input: x is pointer to store X-position (0 to 1023)
//        y is pointer to store Y-position (0 to 1023)
//        select is pointer to store Select status (0 if pressed)
// Output: none
// Assumes: BSP_Joystick_Init() has been called
//void BSP_Joystick_Input(uint16_t *x, uint16_t *y, uint8_t *select);
void BSP_Joystick_Input(uint32_t *x, uint32_t *y, uint32_t *z);

void BSP_Joystick_Select(uint32_t *isPressed);

// ------------BSP_Joystick_Init------------
// Initialize a GPIO pin for input, which corresponds
// with BoosterPack pin J1.5 (Select button).
// Initialize two ADC pins, which correspond with
// BoosterPack pins J1.2 (X) and J3.26 (Y).
// Input: none
// Output: none
void BSP_Microphone_Init(void);

// ------------BSP_Joystick_Input------------
// Read and return the immediate status of the
// joystick.  Button de-bouncing for the Select
// button is not considered.  The joystick X- and
// Y-positions are returned as 10-bit numbers,
// even if the ADC on the LaunchPad is more precise.
// Input: x is pointer to store X-position (0 to 1023)
//        y is pointer to store Y-position (0 to 1023)
//        select is pointer to store Select status (0 if pressed)
// Output: none
// Assumes: BSP_Joystick_Init() has been called
//void BSP_Joystick_Input(uint16_t *x, uint16_t *y, uint8_t *select);
void BSP_Microphone_Input(uint32_t *x);


void BSP_Accelerometer_Init(void);
void BSP_Accelerometer_Input(uint32_t *x, uint32_t *y, uint32_t *z);

void BSP_Button_Init(void);
void BSP_Button_Input(uint32_t *isPressed, uint8_t button);

void BSP_UART_Init(void);