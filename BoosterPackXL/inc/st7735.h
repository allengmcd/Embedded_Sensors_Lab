// BSP.h
// Runs on either the TM4C123 or MSP432 with an Educational BoosterPack MKII (BOOSTXL-EDUMKII)
// This file contains the function prototypes for the software interface to the MKII BoosterPack.
// This board support package (BSP) is an abstraction layer,
//   forming a bridge between the low-level hardware and the high-level software.

// Daniel and Jonathan Valvano
// June 8, 2016

/* This example accompanies the books
   "Embedded Systems: Introduction to the MSP432 Microcontroller",
   ISBN: 978-1512185676, Jonathan Valvano, copyright (c) 2016

   "Embedded Systems: Real-Time Interfacing to the MSP432 Microcontroller",
   ISBN: 978-1514676585, Jonathan Valvano, copyright (c) 2016

   "Embedded Systems: Introduction to ARM Cortex M Microcontrollers"
   ISBN: 978-1469998749, Jonathan Valvano, copyright (c) 2016

   "Embedded Systems: Real Time Interfacing to Arm Cortex M Microcontrollers",
   ISBN: 978-1463590154, Jonathan Valvano, copyright (c) 2016

  "Embedded Systems: Real-Time Operating Systems for ARM Cortex-M Microcontrollers",
      ISBN: 978-1466468863, , Jonathan Valvano, copyright (c) 2016

 Copyright 2016 by Jonathan W. Valvano, valvano@mail.utexas.edu
    You may use, edit, run or distribute this file
    as long as the above copyright notice remains
 THIS SOFTWARE IS PROVIDED "AS IS".  NO WARRANTIES, WHETHER EXPRESS, IMPLIED
 OR STATUTORY, INCLUDING, BUT NOT LIMITED TO, IMPLIED WARRANTIES OF
 MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE APPLY TO THIS SOFTWARE.
 VALVANO SHALL NOT, IN ANY CIRCUMSTANCES, BE LIABLE FOR SPECIAL, INCIDENTAL,
 OR CONSEQUENTIAL DAMAGES, FOR ANY REASON WHATSOEVER.
 For more information about my classes, my research, and my books, see
 http://users.ece.utexas.edu/~valvano/
 */

//  J1   J3               J4   J2
// [ 1] [21]             [40] [20]
// [ 2] [22]             [39] [19]
// [ 3] [23]             [38] [18]
// [ 4] [24]             [37] [17]
// [ 5] [25]             [36] [16]
// [ 6] [26]             [35] [15]
// [ 7] [27]             [34] [14]
// [ 8] [28]             [33] [13]
// [ 9] [29]             [32] [12]
// [10] [30]             [31] [11]

// Connected pins in physical order
// J1.1 +3.3V (power)
// J1.2 joystick horizontal (X) (analog) {TM4C123 PB5/AIN11, MSP432 P6.0}
// J1.3 UART from Bluetooth to LaunchPad (UART) {TM4C123 PB0, MSP432 P3.2}
// J1.4 UART from LaunchPad to Bluetooth (UART) {TM4C123 PB1, MSP432 P3.3}
// J1.5 joystick Select button (digital) {TM4C123 PE4, MSP432 P4.1}
// J1.6 microphone (analog)              {TM4C123 PE5/AIN8, MSP432 P4.3}
// J1.7 LCD SPI clock (SPI)              {TM4C123 PB4, MSP432 P1.5}
// J1.8 ambient light (OPT3001) interrupt (digital) {TM4C123 PA5, MSP432 P4.6}
// J1.9 ambient light (OPT3001) and temperature sensor (TMP006) I2C SCL (I2C)  {TM4C123 PA6, MSP432 P6.5}
// J1.10 ambient light (OPT3001) and temperature sensor (TMP006) I2C SDA (I2C) {TM4C123 PA7, MSP432 P6.4}
//--------------------------------------------------
// J2.11 temperature sensor (TMP006) interrupt (digital) {TM4C123 PA2, MSP432 P3.6}
// J2.12 nothing                         {TM4C123 PA3, MSP432 P5.2}
// J2.13 LCD SPI CS (SPI)                {TM4C123 PA4, MSP432 P5.0}
// J2.14 nothing                         {TM4C123 PB6, MSP432 P1.7}
// J2.15 LCD SPI data (SPI)              {TM4C123 PB7, MSP432 P1.6}
// J2.16 nothing (reset)
// J2.17 LCD !RST (digital)              {TM4C123 PF0, MSP432 P5.7}
// J2.18 Profile 4                       {TM4C123 PE0, MSP432 P3.0}
// J2.19 servo PWM                       {TM4C123 PB2, MSP432 P2.5}
// J2.20 GND (ground)
//--------------------------------------------------
// J3.21 +5V (power)
// J3.22 GND (ground)
// J3.23 accelerometer X (analog)        {TM4C123 PD0/AIN7, MSP432 P6.1}
// J3.24 accelerometer Y (analog)        {TM4C123 PD1/AIN6, MSP432 P4.0}
// J3.25 accelerometer Z (analog)        {TM4C123 PD2/AIN5, MSP432 P4.2}
// J3.26 joystick vertical (Y) (analog)  {TM4C123 PD3/AIN4, MSP432 P4.4}
// J3.27 Profile 0                       {TM4C123 PE1, MSP432 P4.5}
// J3.28 Profile 1                       {TM4C123 PE2, MSP432 P4.7}
// J3.29 Profile 2                       {TM4C123 PE3, MSP432 P5.4}
// J3.30 Profile 3                       {TM4C123 PF1, MSP432 P5.5}
//--------------------------------------------------
// J4.31 LCD RS (digital)                {TM4C123 PF4, MSP432 P3.7}
// J4.32 user Button2 (bottom) (digital) {TM4C123 PD7, MSP432 P3.5}
// J4.33 user Button1 (top) (digital)    {TM4C123 PD6, MSP432 P5,1}
// J4.34 Profile 6/gator hole switch     {TM4C123 PC7, MSP432 P2.3}
// J4.35 nothing                         {TM4C123 PC6, MSP432 P6.7}
// J4.36 Profile 5                       {TM4C123 PC5, MSP432 P6.6}
// J4.37 RGB LED blue (PWM)              {TM4C123 PC4, MSP432 P5.6}
// J4.38 RGB LED green (PWM)             {TM4C123 PB3, MSP432 P2.4}
// J4.39 RGB LED red (jumper up) or LCD backlight (jumper down) (PWM) {TM4C123 PF3, MSP432 P2.6}
// J4.40 buzzer (PWM)                    {TM4C123 PF2, MSP432 P2.7}
//--------------------------------------------------
// Connected pins in logic order
// power and reset
// J1.1 +3.3V (power)
// J3.21 +5V (power)
// J3.22 GND (ground)
// J2.20 GND (ground)
// J2.16 nothing (reset)
//--------------------------------------------------
// LCD graphics
// J1.7 LCD SPI clock (SPI)              {TM4C123 PB4, MSP432 P1.5}
// J2.13 LCD SPI CS (SPI)                {TM4C123 PA4, MSP432 P5.0}
// J2.15 LCD SPI data (SPI)              {TM4C123 PB7, MSP432 P1.6}
// J2.17 LCD !RST (digital)              {TM4C123 PF0, MSP432 P5.7}
// J4.31 LCD RS (digital)                {TM4C123 PF4, MSP432 P3.7}
//--------------------------------------------------
// 3-color LED
// J4.37 RGB LED blue (PWM)              {TM4C123 PC4, MSP432 P5.6}
// J4.38 RGB LED green (PWM)             {TM4C123 PB3, MSP432 P2.4}
// J4.39 RGB LED red (jumper up) or LCD backlight (jumper down) (PWM) {TM4C123 PF3, MSP432 P2.6}
//--------------------------------------------------
// user buttons
// J4.32 user Button2 (bottom) (digital) {TM4C123 PD7, MSP432 P3.5}
// J4.33 user Button1 (top) (digital)    {TM4C123 PD6, MSP432 P5.1}
//--------------------------------------------------
// buzzer output
// J4.40 buzzer (PWM)                    {TM4C123 PF2, MSP432 P2.7}
//--------------------------------------------------
// Joystick
// J1.5 joystick Select button (digital) {TM4C123 PE4, MSP432 P4.1}
// J1.2 joystick horizontal (X) (analog) {TM4C123 PB5/AIN11, MSP432 P6.0}
// J3.26 joystick vertical (Y) (analog)  {TM4C123 PD3/AIN4, MSP432 P4.4}
//--------------------------------------------------
// accelerometer
// J3.23 accelerometer X (analog)        {TM4C123 PD0/AIN7, MSP432 P6.1}
// J3.24 accelerometer Y (analog)        {TM4C123 PD1/AIN6, MSP432 P4.0}
// J3.25 accelerometer Z (analog)        {TM4C123 PD2/AIN5, MSP432 P4.2}
//--------------------------------------------------
// microphone
// J1.6 microphone (analog)              {TM4C123 PE5/AIN8, MSP432 P4.3}
//--------------------------------------------------
// light and temperature sensors (I2C)
// J1.8 ambient light (OPT3001) interrupt (digital) {TM4C123 PA5, MSP432 P4.6}
// J1.9 ambient light (OPT3001) and temperature sensor (TMP006) I2C SCL (I2C)  {TM4C123 PA6, MSP432 P6.5}
// J1.10 ambient light (OPT3001) and temperature sensor (TMP006) I2C SDA (I2C) {TM4C123 PA7, MSP432 P6.4}
// J2.11 temperature sensor (TMP006) interrupt (digital) {TM4C123 PA2, MSP432 P3.6}
//--------------------------------------------------
// Bluetooth booster
// J1.3 UART from Bluetooth to LaunchPad (UART) {TM4C123 PB0, MSP432 P3.2}
// J1.4 UART from LaunchPad to Bluetooth (UART) {TM4C123 PB1, MSP432 P3.3}
//--------------------------------------------------
// profile pins
// J3.27 Profile 0                       {TM4C123 PE1, MSP432 P4.5}
// J3.28 Profile 1                       {TM4C123 PE2, MSP432 P4.7}
// J3.29 Profile 2                       {TM4C123 PE3, MSP432 P5.4}
// J3.30 Profile 3                       {TM4C123 PF1, MSP432 P5.5}
// J2.18 Profile 4                       {TM4C123 PE0, MSP432 P3.0}
// J4.36 Profile 5                       {TM4C123 PC5, MSP432 P6.6}
// J4.34 Profile 6                       {TM4C123 PC7, MSP432 P2.3}
//--------------------------------------------------
// unconnected pins
// J2.12 nothing                         {TM4C123 PA3, MSP432 P5.2}
// J2.14 nothing                         {TM4C123 PB6, MSP432 P1.7}
// J2.19 servo PWM                       {TM4C123 PB2, MSP432 P2.5}
// J4.35 nothing                         {TM4C123 PC6, MSP432 P6.7}


// ------------Includes------------
#include "includes.h"
#include "driverlib/sysctl.h"
#include "bsp_gpio.h"
#include "bsp_ssi.h"
#include "bsp_utils.h"
#include "bsp_joystick.h"
#include "grlib/grlib.h"




//color constants                  red  grn  blu
#define LCD_BLACK      0x0000   //   0,   0,   0
#define LCD_BLUE       0x001F   //   0,   0, 255
#define LCD_DARKBLUE   0x34BF   //  50, 150, 255
#define LCD_RED        0xF800   // 255,   0,   0
#define LCD_GREEN      0x07E0   //   0, 255,   0
#define LCD_LIGHTGREEN 0x07EF   //   0, 255, 120
#define LCD_ORANGE     0xFD60   // 255, 175,   0
#define LCD_CYAN       0x07FF   //   0, 255, 255
#define LCD_MAGENTA    0xF81F   // 255,   0, 255
#define LCD_YELLOW     0xFFE0   // 255, 255,   0
#define LCD_WHITE      0xFFFF   // 255, 255, 255
#define LCD_GREY       0x8410   // 128, 128, 128


// ------------BSP_LCD_Init------------
// Initialize the SPI and GPIO, which correspond with
// BoosterPack pins J1.7 (SPI CLK), J2.13 (SPI CS), J2.15
// (SPI MOSI), J2.17 (LCD ~RST), and J4.31 (LCD DC).
// Input: none
// Output: none
void BSP_LCD_Init(void);

void BSP_ST7735_PixelDraw(void *pvDisplayData, int32_t i32X, int32_t i32Y, uint32_t ui32Value);
void BSP_ST7735_PixelDrawMultiple(void *pvDisplayData, int32_t i32X, int32_t i32Y, int32_t i32X0, int32_t i32Count, 
  int32_t i32BPP, const uint8_t *pui8Data, const uint8_t *pui8Palette);
void BSP_ST7735_LineDrawH(void *pvDisplayData, int32_t i32X1, int32_t i32X2, int32_t i32Y, uint32_t ui32Value);
void BSP_ST7735_LineDrawV(void *pvDisplayData, int32_t i32X, int32_t i32Y1, int32_t i32Y2, uint32_t ui32Value);
void BSP_ST7735_RectFill(void *pvDisplayData, const tRectangle *psRect, uint32_t ui32Value);
uint32_t BSP_ST7735_ColorTranslate(void *pvDisplayData, uint32_t ui32Value);
void BSP_ST7735_Flush(void *pvDisplayData);

void Init_grlib();
void loop_grlib();

void BSP_Test_grlib();