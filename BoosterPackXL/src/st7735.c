// BSP.c
// Runs on TM4C123 with an Educational BoosterPack MKII (BOOSTXL-EDUMKII)
// This file contains the software interface to the MKII BoosterPack.
// This board support package (BSP) is an abstraction layer,
//   forming a bridge between the low-level hardware and the high-level software.

// Remember to remove R9 and R10 to use the LCD since R10
// connects PD1 (accelerometer Y) to PB7 (LCD SPI data).
// Daniel and Jonathan Valvano
// June 8, 2016

/* This example accompanies the books
   "Embedded Systems: Introduction to ARM Cortex M Microcontrollers",
      ISBN: 978-1469998749, Jonathan Valvano, copyright (c) 2016
   "Embedded Systems: Real Time Interfacing to ARM Cortex M Microcontrollers",
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

#include <st7735.h>

#define __TI_COMPILER_VERSION__ 1

void DisableInterrupts(void); // Disable interrupts
void EnableInterrupts(void);  // Enable interrupts
long StartCritical (void);    // previous I bit, disable interrupts
void EndCritical(long sr);    // restore I bit to previous value
void WaitForInterrupt(void);  // low power mode

//static uint32_t ClockFrequency = 16000000; // cycles/second


/* ********************** */
/*      LCD Section       */
/* ********************** */
// This section is based on ST7735.c, which itself is based
// on example code originally from Adafruit.  Some sections
// such as the font table and initialization functions were
// copied verbatim from Adafruit's example and are subject
// to the following disclosure.
/***************************************************
  This is a library for the Adafruit 1.8" SPI display.
  This library works with the Adafruit 1.8" TFT Breakout w/SD card
  ----> http://www.adafruit.com/products/358
  as well as Adafruit raw 1.8" TFT displayun
  ----> http://www.adafruit.com/products/618

  Check out the links above for our tutorials and wiring diagrams
  These displays use SPI to communicate, 4 or 5 pins are required to
  interface (RST is optional)
  Adafruit invests time and resources providing this open source code,
  please support Adafruit and open-source hardware by purchasing
  products from Adafruit!

  Written by Limor Fried/Ladyada for Adafruit Industries.
  MIT license, all text above must be included in any redistribution
 ****************************************************/
// some flags for ST7735_InitR()
enum initRFlags{
  none,
  INITR_GREENTAB,
  INITR_REDTAB,
  INITR_BLACKTAB
};

#define ST7735_TFTWIDTH  128
#define ST7735_TFTHEIGHT 128

// Pin definitions
#define ST7735_RES_Pin       GPIO_PIN_3
#define ST7735_RES_GPIO_Port GPIO_PORTH_BASE
#define ST7735_CS_Pin        GPIO_PIN_2
#define ST7735_CS_GPIO_Port  GPIO_PORTN_BASE
#define ST7735_DC_Pin        GPIO_PIN_3
#define ST7735_DC_GPIO_Port  GPIO_PORTL_BASE

#define GPIO_PIN_LOW 0

#define DC_COMMAND  GPIO_PIN_LOW  // 0
#define DC_DATA     ST7735_DC_Pin // Pin number
#define DC_SET(state) BSP_GPIO_Write(ST7735_DC_GPIO_Port, ST7735_DC_Pin, (state))

#define TFT_CS_LOW  GPIO_PIN_LOW  // 0
#define TFT_CS_HIGH     ST7735_CS_Pin // Pin number
#define TFT_CS_SET(state) BSP_GPIO_Write(ST7735_CS_GPIO_Port, ST7735_CS_Pin, (state))

#define RESET_LOW  GPIO_PIN_LOW  // 0
#define RESET_HIGH     ST7735_RES_Pin // Pin number
#define RESET_SET(state) BSP_GPIO_Write(ST7735_RES_GPIO_Port, ST7735_RES_Pin, (state))


// Color definitions
#define ST7735_BLACK   0x0000
#define ST7735_BLUE    0x001F
#define ST7735_RED     0xF800
#define ST7735_GREEN   0x07E0
#define ST7735_CYAN    0x07FF
#define ST7735_MAGENTA 0xF81F
#define ST7735_YELLOW  0xFFE0
#define ST7735_WHITE   0xFFFF


// 12 rows (0 to 11) and 21 characters (0 to 20)
// Requires (11 + size*size*6*8) bytes of transmission for each character
uint32_t StX=0; // position along the horizonal axis 0 to 20
uint32_t StY=0; // position along the vertical axis 0 to 11
uint16_t StTextColor = ST7735_YELLOW;

#define ST7735_NOP     0x00
#define ST7735_SWRESET 0x01
#define ST7735_RDDID   0x04
#define ST7735_RDDST   0x09

#define ST7735_SLPIN   0x10
#define ST7735_SLPOUT  0x11
#define ST7735_PTLON   0x12
#define ST7735_NORON   0x13

#define ST7735_INVOFF  0x20
#define ST7735_INVON   0x21
#define ST7735_DISPOFF 0x28
#define ST7735_DISPON  0x29
#define ST7735_CASET   0x2A
#define ST7735_RASET   0x2B
#define ST7735_RAMWR   0x2C
#define ST7735_RAMRD   0x2E

#define ST7735_PTLAR   0x30
#define ST7735_COLMOD  0x3A
#define ST7735_MADCTL  0x36

#define ST7735_FRMCTR1 0xB1
#define ST7735_FRMCTR2 0xB2
#define ST7735_FRMCTR3 0xB3
#define ST7735_INVCTR  0xB4
#define ST7735_DISSET5 0xB6

#define ST7735_PWCTR1  0xC0
#define ST7735_PWCTR2  0xC1
#define ST7735_PWCTR3  0xC2
#define ST7735_PWCTR4  0xC3
#define ST7735_PWCTR5  0xC4
#define ST7735_VMCTR1  0xC5

#define ST7735_RDID1   0xDA
#define ST7735_RDID2   0xDB
#define ST7735_RDID3   0xDC
#define ST7735_RDID4   0xDD

#define ST7735_PWCTR6  0xFC

#define ST7735_GMCTRP1 0xE0
#define ST7735_GMCTRN1 0xE1

typedef struct {
  uint16_t *framebuffer;  // Optional buffer (if using off-screen rendering)
  uint16_t width;         // Screen width
  uint16_t height;        // Screen height
} ST7735_Display;

ST7735_Display st7735_display;


void BSP_ST7735_PixelDraw(void *pvDisplayData, int32_t i32X, int32_t i32Y, uint32_t ui32Value);
void BSP_ST7735_PixelDrawMultiple(void *pvDisplayData, int32_t i32X, int32_t i32Y, int32_t i32X0, int32_t i32Count, 
  int32_t i32BPP, const uint8_t *pui8Data, const uint8_t *pui8Palette);
void BSP_ST7735_LineDrawH(void *pvDisplayData, int32_t i32X1, int32_t i32X2, int32_t i32Y, uint32_t ui32Value);
void BSP_ST7735_LineDrawV(void *pvDisplayData, int32_t i32X, int32_t i32Y1, int32_t i32Y2, uint32_t ui32Value);
void BSP_ST7735_RectFill(void *pvDisplayData, const tRectangle *psRect, uint32_t ui32Value);
uint32_t BSP_ST7735_ColorTranslate(void *pvDisplayData, uint32_t ui32Value);
void BSP_ST7735_Flush(void *pvDisplayData);


tDisplay display_st7735 = {
  sizeof(tDisplay),             // i32Size
  (void *)&st7735_display,       // pvDisplayData
  ST7735_TFTWIDTH,              // ui16Width
  ST7735_TFTHEIGHT,             // ui16Height
  BSP_ST7735_PixelDraw,         // pfnPixelDraw
  BSP_ST7735_PixelDrawMultiple, // pfnPixelDrawMultiple
  BSP_ST7735_LineDrawH,         // pfnLineDrawH
  BSP_ST7735_LineDrawV,         // pfnLineDrawV
  BSP_ST7735_RectFill,          // pfnRectFill
  BSP_ST7735_ColorTranslate,    // pfnColorTranslate
  BSP_ST7735_Flush              // pfnFlush
};

uint16_t _ScreenBuffer[ST7735_TFTHEIGHT * ST7735_TFTWIDTH]; // Screen buffer for double buffering

static uint8_t ColStart, RowStart; // some displays need this changed
static int16_t _width = ST7735_TFTWIDTH;   // this could probably be a constant, except it is used in Adafruit_GFX and depends on image rotation
static int16_t _height = ST7735_TFTHEIGHT;


// The Data/Command pin must be valid when the eighth bit is
// sent.  The SSI module has hardware input and output FIFOs
// that are 8 locations deep; however, they are not used in
// this implementation.  Each function first stalls while
// waiting for any pending SSI2 transfers to complete.  Once
// the SSI2 module is idle, it then prepares the Chip Select
// pin for the LCD and the Data/Command pin.  Next it starts
// transmitting the data or command.  Finally once the
// hardware is idle again, it sets the chip select pin high
// as required by the serial protocol.  This is a
// significant change from previous implementations of this
// function.  It is less efficient without the FIFOs, but it
// should ensure that the Chip Select and Data/Command pin
// statuses all match the byte that is actually being
// transmitted.
// NOTE: These functions will crash or stall indefinitely if
// the SSI2 module is not initialized and enabled.

// This is a helper function that sends a piece of 8-bit data to the LCD.
// Inputs: c  8-bit data to transmit
// Outputs: 8-bit reply
// Assumes: SSI2 and ports have already been initialized and enabled
uint8_t static writedata(uint8_t c) {
  TFT_CS_SET(TFT_CS_LOW);
  DC_SET(DC_DATA);
  //UARTprintf("writedata: %x\n  ", c);
  BSP_SSI_Send(&c, 1);
  TFT_CS_SET(TFT_CS_HIGH);

  return 0; // For now, return 0 since we are not using the response
}

// This is a helper function that sends an 8-bit command to the LCD.
// Inputs: c  8-bit code to transmit
// Outputs: 8-bit reply
// Assumes: SSI2 and ports have already been initialized and enabled
uint8_t static writecommand(uint8_t c) {
  TFT_CS_SET(TFT_CS_LOW);
  DC_SET(DC_COMMAND);
  //UARTprintf("writecommand: %x\n  ", c);
  BSP_SSI_Send(&c, 1);
  TFT_CS_SET(TFT_CS_HIGH);

  return 0; // For now, return 0 since we are not using the response
}

uint8_t static writebuffer16(uint16_t *buffer, uint32_t length) {
  TFT_CS_SET(TFT_CS_LOW);
  DC_SET(DC_DATA);
  BSP_SSI_Send_16_DMA(buffer, length);
  TFT_CS_SET(TFT_CS_HIGH);

  return 0; // For now, return 0 since we are not using the response
}


// Rather than a bazillion writecommand() and writedata() calls, screen
// initialization commands and arguments are organized in these tables
// stored in ROM.  The table may look bulky, but that's mostly the
// formatting -- storage-wise this is hundreds of bytes more compact
// than the equivalent code.  Companion function follows.
#define DELAY 0x80
//static const uint8_t
//  Bcmd[] = {                  // Initialization commands for 7735B screens
//    18,                       // 18 commands in list:
//    ST7735_SWRESET,   DELAY,  //  1: Software reset, no args, w/delay
//      50,                     //     50 ms delay
//    ST7735_SLPOUT ,   DELAY,  //  2: Out of sleep mode, no args, w/delay
//      255,                    //     255 = 500 ms delay
//    ST7735_COLMOD , 1+DELAY,  //  3: Set color mode, 1 arg + delay:
//      0x05,                   //     16-bit color
//      10,                     //     10 ms delay
//    ST7735_FRMCTR1, 3+DELAY,  //  4: Frame rate control, 3 args + delay:
//      0x00,                   //     fastest refresh
//      0x06,                   //     6 lines front porch
//      0x03,                   //     3 lines back porch
//      10,                     //     10 ms delay
//    ST7735_MADCTL , 1      ,  //  5: Memory access ctrl (directions), 1 arg:
//      0x08,                   //     Row addr/col addr, bottom to top refresh
//    ST7735_DISSET5, 2      ,  //  6: Display settings #5, 2 args, no delay:
//      0x15,                   //     1 clk cycle nonoverlap, 2 cycle gate
//                              //     rise, 3 cycle osc equalize
//      0x02,                   //     Fix on VTL
//    ST7735_INVCTR , 1      ,  //  7: Display inversion control, 1 arg:
//      0x0,                    //     Line inversion
//    ST7735_PWCTR1 , 2+DELAY,  //  8: Power control, 2 args + delay:
//      0x02,                   //     GVDD = 4.7V
//      0x70,                   //     1.0uA
//      10,                     //     10 ms delay
//    ST7735_PWCTR2 , 1      ,  //  9: Power control, 1 arg, no delay:
//      0x05,                   //     VGH = 14.7V, VGL = -7.35V
//    ST7735_PWCTR3 , 2      ,  // 10: Power control, 2 args, no delay:
//      0x01,                   //     Opamp current small
//      0x02,                   //     Boost frequency
//    ST7735_VMCTR1 , 2+DELAY,  // 11: Power control, 2 args + delay:
//      0x3C,                   //     VCOMH = 4V
//      0x38,                   //     VCOML = -1.1V
//      10,                     //     10 ms delay
//    ST7735_PWCTR6 , 2      ,  // 12: Power control, 2 args, no delay:
//      0x11, 0x15,
//    ST7735_GMCTRP1,16      ,  // 13: Magical unicorn dust, 16 args, no delay:
//      0x09, 0x16, 0x09, 0x20, //     (seriously though, not sure what
//      0x21, 0x1B, 0x13, 0x19, //      these config values represent)
//      0x17, 0x15, 0x1E, 0x2B,
//      0x04, 0x05, 0x02, 0x0E,
//    ST7735_GMCTRN1,16+DELAY,  // 14: Sparkles and rainbows, 16 args + delay:
//      0x0B, 0x14, 0x08, 0x1E, //     (ditto)
//      0x22, 0x1D, 0x18, 0x1E,
//      0x1B, 0x1A, 0x24, 0x2B,
//      0x06, 0x06, 0x02, 0x0F,
//      10,                     //     10 ms delay
//    ST7735_CASET  , 4      ,  // 15: Column addr set, 4 args, no delay:
//      0x00, 0x02,             //     XSTART = 2
//      0x00, 0x81,             //     XEND = 129
//    ST7735_RASET  , 4      ,  // 16: Row addr set, 4 args, no delay:
//      0x00, 0x02,             //     XSTART = 1
//      0x00, 0x81,             //     XEND = 160
//    ST7735_NORON  ,   DELAY,  // 17: Normal display on, no args, w/delay
//      10,                     //     10 ms delay
//    ST7735_DISPON ,   DELAY,  // 18: Main screen turn on, no args, w/delay
//      255 };                  //     255 = 500 ms delay
static const uint8_t
  Rcmd1[] = {                 // Init for 7735R, part 1 (red or green tab)
    15,                       // 15 commands in list:
    ST7735_SWRESET,   DELAY,  //  1: Software reset, 0 args, w/delay
      150,                    //     150 ms delay
    ST7735_SLPOUT ,   DELAY,  //  2: Out of sleep mode, 0 args, w/delay
      255,                    //     500 ms delay
    ST7735_FRMCTR1, 3      ,  //  3: Frame rate ctrl - normal mode, 3 args:
      0x01, 0x2C, 0x2D,       //     Rate = fosc/(1x2+40) * (LINE+2C+2D)
    ST7735_FRMCTR2, 3      ,  //  4: Frame rate control - idle mode, 3 args:
      0x01, 0x2C, 0x2D,       //     Rate = fosc/(1x2+40) * (LINE+2C+2D)
    ST7735_FRMCTR3, 6      ,  //  5: Frame rate ctrl - partial mode, 6 args:
      0x01, 0x2C, 0x2D,       //     Dot inversion mode
      0x01, 0x2C, 0x2D,       //     Line inversion mode
    ST7735_INVCTR , 1      ,  //  6: Display inversion ctrl, 1 arg, no delay:
      0x07,                   //     No inversion
    ST7735_PWCTR1 , 3      ,  //  7: Power control, 3 args, no delay:
      0xA2,
      0x02,                   //     -4.6V
      0x84,                   //     AUTO mode
    ST7735_PWCTR2 , 1      ,  //  8: Power control, 1 arg, no delay:
      0xC5,                   //     VGH25 = 2.4C VGSEL = -10 VGH = 3 * AVDD
    ST7735_PWCTR3 , 2      ,  //  9: Power control, 2 args, no delay:
      0x0A,                   //     Opamp current small
      0x00,                   //     Boost frequency
    ST7735_PWCTR4 , 2      ,  // 10: Power control, 2 args, no delay:
      0x8A,                   //     BCLK/2, Opamp current small & Medium low
      0x2A,
    ST7735_PWCTR5 , 2      ,  // 11: Power control, 2 args, no delay:
      0x8A, 0xEE,
    ST7735_VMCTR1 , 1      ,  // 12: Power control, 1 arg, no delay:
      0x0E,
    ST7735_INVOFF , 0      ,  // 13: Don't invert display, no args, no delay
    ST7735_MADCTL , 1      ,  // 14: Memory access control (directions), 1 arg:
      0xC8,                   //     row addr/col addr, bottom to top refresh
    ST7735_COLMOD , 1      ,  // 15: set color mode, 1 arg, no delay:
      0x05 };                 //     16-bit color
static const uint8_t
  Rcmd2green[] = {            // Init for 7735R, part 2 (green tab only)
    2,                        //  2 commands in list:
    ST7735_CASET  , 4      ,  //  1: Column addr set, 4 args, no delay:
      0x00, 0x02,             //     XSTART = 0
      0x00, 0x7F+0x02,        //     XEND = 127
    ST7735_RASET  , 4      ,  //  2: Row addr set, 4 args, no delay:
      0x00, 0x01,             //     XSTART = 0
      0x00, 0x7F+0x01 };      //     XEND = 127
static const uint8_t
  Rcmd2red[] = {              // Init for 7735R, part 2 (red tab only)
    2,                        //  2 commands in list:
    ST7735_CASET  , 4      ,  //  1: Column addr set, 4 args, no delay:
      0x00, 0x00,             //     XSTART = 0
      0x00, 0x7F,             //     XEND = 127
    ST7735_RASET  , 4      ,  //  2: Row addr set, 4 args, no delay:
      0x00, 0x00,             //     XSTART = 0
      0x00, 0x7F };           //     XEND = 127
static const uint8_t
  Rcmd3[] = {                 // Init for 7735R, part 3 (red or green tab)
    4,                        //  4 commands in list:
    ST7735_GMCTRP1, 16      , //  1: Magical unicorn dust, 16 args, no delay:
      0x02, 0x1c, 0x07, 0x12,
      0x37, 0x32, 0x29, 0x2d,
      0x29, 0x25, 0x2B, 0x39,
      0x00, 0x01, 0x03, 0x10,
    ST7735_GMCTRN1, 16      , //  2: Sparkles and rainbows, 16 args, no delay:
      0x03, 0x1d, 0x07, 0x06,
      0x2E, 0x2C, 0x29, 0x2D,
      0x2E, 0x2E, 0x37, 0x3F,
      0x00, 0x00, 0x02, 0x10,
    ST7735_NORON  ,    DELAY, //  3: Normal display on, no args, w/delay
      10,                     //     10 ms delay
    ST7735_DISPON ,    DELAY, //  4: Main screen turn on, no args w/delay
      100 };                  //     100 ms delay


// Companion code to the above tables.  Reads and issues
// a series of LCD commands stored in ROM byte array.
void static commandList(const uint8_t *addr) {

  uint8_t numCommands, numArgs;
  uint16_t ms;

  numCommands = *(addr++);               // Number of commands to follow
  while(numCommands--) {                 // For each command...
    writecommand(*(addr++));             //   Read, issue command
    numArgs  = *(addr++);                //   Number of args to follow
    ms       = numArgs & DELAY;          //   If hibit set, delay follows args
    numArgs &= ~DELAY;                   //   Mask out delay bit
    while(numArgs--) {                   //   For each argument...
      writedata(*(addr++));              //     Read, issue argument
    }

    if(ms) {
      ms = *(addr++);             // Read post-command delay time (ms)
      if(ms == 255) ms = 500;     // If 255, delay for 500 ms
      BSP_Delay_ms(ms);
    }
  }
}


// Initialization code common to both 'B' and 'R' type displays
void static commonInit(const uint8_t *cmdList) {
  ColStart  = RowStart = 0; // May be overridden in init func

  TFT_CS_SET(TFT_CS_LOW);
  RESET_SET(RESET_HIGH);
  BSP_Delay_ms(500);
  RESET_SET(RESET_LOW);
  BSP_Delay_ms(500);
  RESET_SET(RESET_HIGH);
  BSP_Delay_ms(500);
  TFT_CS_SET(TFT_CS_HIGH);

  if(cmdList) commandList(cmdList);
}


//------------ST7735_InitR------------
// Initialization for ST7735R screens (green or red tabs).
// Input: option one of the enumerated options depending on tabs
// Output: none
void static ST7735_InitR(enum initRFlags option) {
  commonInit(Rcmd1);
  if(option == INITR_GREENTAB) {
    commandList(Rcmd2green);
    ColStart = 2;
    RowStart = 3;
  } else {
    // colstart, rowstart left at default '0' values
    commandList(Rcmd2red);
  }
  commandList(Rcmd3);

  // if black, change MADCTL color filter
  if (option == INITR_BLACKTAB) {
    writecommand(ST7735_MADCTL);
    writedata(0xC0);
  }
  
  st7735_display.framebuffer = _ScreenBuffer; // Set the framebuffer to the screen buffer
  st7735_display.width = ST7735_TFTWIDTH;
  st7735_display.height = ST7735_TFTHEIGHT;
}


// ------------BSP_LCD_Init------------
// Initialize the SPI and GPIO, which correspond with
// BoosterPack pins J1.7 (SPI CLK), J2.13 (SPI CS), J2.15
// (SPI MOSI), J2.17 (LCD ~RST), and J4.31 (LCD DC).
// Input: none
// Output: none
void BSP_LCD_Init(void){
  ST7735_InitR(INITR_GREENTAB);
}


// Set the region of the screen RAM to be modified
// Pixel colors are sent left to right, top to bottom
// (same as Font table is encoded; different from regular bitmap)
// Requires 11 bytes of transmission
void static setAddrWindow(uint8_t x0, uint8_t y0, uint8_t x1, uint8_t y1) {

  writecommand(ST7735_CASET); // Column addr set
  writedata(0x00);
  writedata(x0+ColStart);     // XSTART
  writedata(0x00);
  writedata(x1+ColStart);     // XEND

  writecommand(ST7735_RASET); // Row addr set
  writedata(0x00);
  writedata(y0+RowStart);     // YSTART
  writedata(0x00);
  writedata(y1+RowStart);     // YEND

  writecommand(ST7735_RAMWR); // write to RAM
}


void BSP_LCD_DrawBuffer(uint16_t *buffer) {
  // UARTprintf("setAddrWindow...\n  ");
  setAddrWindow(0, 0, _width-1, _height-1);
  // UARTprintf("writecommand...\n  ");
  writecommand(ST7735_RAMWR); // write to RAM
  // UARTprintf("writebuffer...\n  ");
  writebuffer16(buffer, _width * _height);
}


void BSP_ST7735_PixelDraw(void *pvDisplayData, int32_t i32X, int32_t i32Y, uint32_t ui32Value)
{
  ST7735_Display *display = (ST7735_Display *)pvDisplayData;

  if (display->framebuffer == NULL) {
      return; // No framebuffer, nothing to display
  }

  int buffer_length = 1;
  display->framebuffer[0] = (uint16_t)ui32Value;

  setAddrWindow((uint8_t)i32X, (uint8_t)i32Y, (uint8_t)i32X, (uint8_t)i32Y); // set the address window
  writecommand(ST7735_RAMWR); // write to RAM
  writebuffer16(display->framebuffer, buffer_length); // write the buffer to the screen
}

void BSP_ST7735_PixelDrawMultiple(void *pvDisplayData, int32_t i32X, int32_t i32Y, int32_t i32X0, int32_t i32Count, 
  int32_t i32BPP, const uint8_t *pui8Data, const uint8_t *pui8Palette)
{
  ST7735_Display *display = (ST7735_Display *)pvDisplayData;

  if (display->framebuffer == NULL) {
      return; // No framebuffer, nothing to display
  }

  // Select the starting pixel in pui8Data
  pui8Data += i32X0 * (i32BPP / 8); // Adjust for bytes per pixel
  
  // Process pixel data based on bit depth
  for (int32_t i = 0; i < i32Count; i++) {
      uint16_t color;

      if (i32BPP == 1) {
          // Monochrome: Each bit represents a pixel
          int byteIndex = (i32X0 + i) / 8;
          int bitIndex = 7 - ((i32X0 + i) % 8);
          int pixelValue = (pui8Data[byteIndex] >> bitIndex) & 0x01;
          color = ((uint16_t *)pui8Palette)[pixelValue];
      }
      else if (i32BPP == 4) {
          // 4BPP: Each nibble represents a pixel index
          int byteIndex = (i32X0 + i) / 2;
          int nibbleShift = ((i32X0 + i) % 2) ? 0 : 4;
          int pixelIndex = (pui8Data[byteIndex] >> nibbleShift) & 0x0F;
          color = ((uint16_t *)pui8Palette)[pixelIndex];
      }
      else if (i32BPP == 8) {
          // 8BPP: Each byte is a palette index
          color = ((uint16_t *)pui8Palette)[pui8Data[i]];
      }
      else if (i32BPP == 16) {
          // 16BPP: Direct 16-bit color
          color = ((uint16_t *)pui8Data)[i];
      }
      else {
          return;
      }

      display->framebuffer[i] = color; // Store in the screen buffer
  }

  // Set the address window for this line of pixels
  //ST7735_SetAddressWindow(i32X, i32Y, i32X + i32Count - 1, i32Y);
  setAddrWindow(i32X, i32Y, i32X + i32Count - 1, i32Y);

  writecommand(ST7735_RAMWR); // write to RAM

  // Send the color data to the display
  // ST7735_WriteData((uint8_t *)ui16Buffer, i32Count * 2); // 2 bytes per pixel
  writebuffer16(display->framebuffer, i32Count); // write the buffer to the screen
}



void BSP_ST7735_LineDrawH(void *pvDisplayData, int32_t i32X1, int32_t i32X2, int32_t i32Y, uint32_t ui32Value)
{
  ST7735_Display *display = (ST7735_Display *)pvDisplayData;

  display->framebuffer = _ScreenBuffer;
  if (display->framebuffer == NULL) {
      return; // No framebuffer, nothing to display
  }

  int buffer_length = i32X2 - i32X1+1; // Calculate the length of the line
  // prepare the screen buffer for the line drawing
  for(int i = 0; i < ST7735_TFTWIDTH*ST7735_TFTHEIGHT; i++) {
    display->framebuffer[i] = (uint16_t)ui32Value; // Update the screen buffer with the color
    // UARTprintf("framebuffer[%d]: %d\n  ", i, ui32Value);
  }
  
  UARTprintf("buffer_length: %d\n  ", buffer_length);
  UARTprintf("i32X1: %d\n  ", i32X1);
  UARTprintf("i32Y: %d\n  ", i32Y);
  UARTprintf("i32X2: %d\n  ", i32X2);
  UARTprintf("i32Y: %d\n  ", i32Y);
  
  setAddrWindow((uint8_t)i32X1, (uint8_t)i32Y, (uint8_t)i32X2, (uint8_t)i32Y); // set the address window
  writecommand(ST7735_RAMWR); // write to RAM
  writebuffer16(display->framebuffer, ST7735_TFTWIDTH*ST7735_TFTHEIGHT); // write the buffer to the screen
  UARTprintf("framebuffer4...\n  ");
}


void BSP_ST7735_LineDrawV(void *pvDisplayData, int32_t i32X, int32_t i32Y1, int32_t i32Y2, uint32_t ui32Value)
{
  ST7735_Display *display = (ST7735_Display *)pvDisplayData;

  if (display->framebuffer == NULL) {
      return; // No framebuffer, nothing to display
  }

  int buffer_length = i32Y2 - i32Y1 + 1; // Calculate the length of the line
  // prepare the screen buffer for the line drawing
  for(int i = 0; i < ST7735_TFTWIDTH*ST7735_TFTHEIGHT; i++) {
    display->framebuffer[i] = ui32Value; // Update the screen buffer with the color
  }
  
  setAddrWindow((uint8_t)i32X, (uint8_t)i32Y1, (uint8_t)i32X, (uint8_t)i32Y2); // set the address window
  writecommand(ST7735_RAMWR); // write to RAM
  writebuffer16(display->framebuffer, ST7735_TFTWIDTH*ST7735_TFTHEIGHT); // write the buffer to the screen
}


void BSP_ST7735_RectFill(void *pvDisplayData, const tRectangle *psRect, uint32_t ui32Value)
{
  ST7735_Display *display = (ST7735_Display *)pvDisplayData;

  if (display->framebuffer == NULL) {
      return; // No framebuffer, nothing to display
  }
  display->framebuffer = _ScreenBuffer;

  int width_length = psRect->i16XMax - psRect->i16XMin + 1; // Calculate the width of the rectangle
  int height_length = psRect->i16YMax - psRect->i16YMin + 1; // Calculate the height of the rectangle 
  int buffer_length = width_length * height_length; // Total number of pixels in the rectangle

  UARTprintf("ui32Value: %d\n  ", ui32Value);
  UARTprintf("buffer_length: %d\n  ", buffer_length);
  UARTprintf("i16XMin: %d\n  ", psRect->i16XMin);
  UARTprintf("i16YMin: %d\n  ", psRect->i16YMin);
  UARTprintf("i16XMax: %d\n  ", psRect->i16XMax);
  UARTprintf("i16YMax: %d\n\n  ", psRect->i16YMax);

  // Fill the buffer with the color
  // for(int i = 0; i < height_length; i++) {
  //   for(int j = 0; j < width_length; j++) {
  //     display->framebuffer[i * width_length + j] = (uint16_t)ui32Value; // Fill the buffer with the color
  //   }
  // }
    for(int j = 0; j < ST7735_TFTWIDTH*ST7735_TFTHEIGHT; j++) {
      display->framebuffer[j] = (uint16_t)ui32Value; // Fill the buffer with the color
    }
  

  setAddrWindow((uint8_t)psRect->i16XMin, (uint8_t)psRect->i16YMin, (uint8_t)psRect->i16XMax, (uint8_t)psRect->i16YMax); // set the address window
  writecommand(ST7735_RAMWR); // write to RAM
  writebuffer16(display->framebuffer, ST7735_TFTWIDTH*ST7735_TFTHEIGHT); // write the buffer to the screen
}


uint32_t BSP_ST7735_ColorTranslate(void *pvDisplayData, uint32_t ui32Value)
{
  // Translate the color value to the format used by the display
  // For ST7735, we need to convert RGB888 to RGB565
  uint8_t r = (ui32Value >> 16) & 0xFF;
  uint8_t g = (ui32Value >> 8) & 0xFF;
  uint8_t b = ui32Value & 0xFF;

  // Convert to RGB565
  uint16_t rgb565 = ((r & 0xF8) << 8) | ((g & 0xFC) << 3) | (b >> 3);
  
  return (uint32_t)rgb565;
}

void BSP_ST7735_Flush(void *pvDisplayData)
{
    ST7735_Display *display = (ST7735_Display *)pvDisplayData;
    uint32_t buffer_length = display->width * display->height; // Total number of pixels in the framebuffer

    if (display->framebuffer == NULL) {
        return; // No framebuffer, nothing to flush
    }

    setAddrWindow(0, 0, display->width - 1, display->height - 1);
    writecommand(ST7735_RAMWR); // write to RAM
    writebuffer16(display->framebuffer, buffer_length); // write the buffer to the screen
}



/* ********************** */
/*   End of LCD Section   */
/* ********************** */
#define CURSOR_SIZE      5       // Size of cursor in pixels
#define BUTTON_WIDTH     80      // Width of clickable button
#define BUTTON_HEIGHT    20      // Height of clickable button
#define BUTTON_X         32      // Button X position
#define BUTTON_Y         40      // Button Y position


tContext g_sContext;              // Graphics context
uint32_t g_ui32ButtonClickCount;  // Button click counter
bool g_bButtonWasPressed;         // Track button state for edge detection

// Draw cursor at the given position
void DrawCursor(tContext *pContext, int32_t x, int32_t y)
{
    // GrContextForegroundSet(pContext, ClrWhite);
    GrContextForegroundSet(pContext, ClrYellow);
    GrRectFill(pContext, &(tRectangle){x, y, x + CURSOR_SIZE - 1, y + CURSOR_SIZE - 1});
}

// Draw button with click count text
void DrawButton(tContext *pContext, uint32_t clickCount)
{
    char countText[16];
    
    // Clear the text area above button
    GrContextForegroundSet(pContext, ClrBlack);
    GrRectFill(pContext, &(tRectangle){BUTTON_X, BUTTON_Y - 15, BUTTON_X + BUTTON_WIDTH, BUTTON_Y - 1});

    
    // Draw the button
    GrContextForegroundSet(pContext, ClrBlue);
    GrRectFill(pContext, &(tRectangle){BUTTON_X, BUTTON_Y, BUTTON_X + BUTTON_WIDTH, BUTTON_Y + BUTTON_HEIGHT});
    
    // Draw button border
    GrContextForegroundSet(pContext, ClrRed);
    GrRectDraw(pContext, &(tRectangle){BUTTON_X, BUTTON_Y, BUTTON_X + BUTTON_WIDTH, BUTTON_Y + BUTTON_HEIGHT});
    
    // Draw "Click Me" text inside button
    GrContextForegroundSet(pContext, ClrWhite);
    GrStringDrawCentered(pContext, "Click Me", -1, BUTTON_X + (BUTTON_WIDTH / 2), BUTTON_Y + (BUTTON_HEIGHT / 2), false);
    
    // Format and draw click count text
    //sprintf(countText, "Clicks: %d", clickCount);
    GrContextForegroundSet(pContext, ClrYellow);
    GrStringDrawCentered(pContext, countText, -1, BUTTON_X + (BUTTON_WIDTH / 2), BUTTON_Y - 8, false);
}

// Check if cursor is over the button
bool IsButtonPressed(int32_t cursorX, int32_t cursorY)
{
    return (cursorX >= BUTTON_X && 
            cursorX <= BUTTON_X + BUTTON_WIDTH &&
            cursorY >= BUTTON_Y && 
            cursorY <= BUTTON_Y + BUTTON_HEIGHT);
}

void Init_grlib()
{
  // Initialize the graphics context
  GrContextInit(&g_sContext, &display_st7735);

      // Initial values
      int32_t cursorX = ST7735_TFTWIDTH / 2;
      int32_t cursorY = ST7735_TFTHEIGHT / 2;
      g_ui32ButtonClickCount = 0;
      g_bButtonWasPressed = false;
      
      // Clear the screen
      GrContextForegroundSet(&g_sContext, ClrBlack);
      GrRectFill(&g_sContext, &(tRectangle){0, 0, ST7735_TFTWIDTH - 1, ST7735_TFTHEIGHT - 1});
      
      // // Draw the button initially
      DrawButton(&g_sContext, g_ui32ButtonClickCount);
      

}

void loop_grlib()
{
  int32_t cursorX = ST7735_TFTWIDTH / 2;
  int32_t cursorY = ST7735_TFTHEIGHT / 2;
  DrawCursor(&g_sContext, cursorX, cursorY);
}


void BSP_Test_grlib()
{
  tRectangle myRect = {5, 5, ST7735_TFTWIDTH-1, ST7735_TFTHEIGHT-1};  // xMin, yMin, xMax, yMax
  BSP_ST7735_RectFill(&display_st7735, &myRect, 0xF00F);

  // UARTprintf("test 1...\n  ");
  // for(int j = 0; j < ST7735_TFTHEIGHT; j++)
  // {
  //   // int width_length = psRect->i16XMax - psRect->i16XMin + 1; // Calculate the width of the rectangle
  //   // int height_length = psRect->i16YMax - psRect->i16YMin + 1; // Calculate the height of the rectangle 
  //     myRect.i16XMin = j;
  //     myRect.i16YMin = ST7735_TFTHEIGHT-1-j;

  //      uint16_t color = (j % 2 == 0) ? 0xF800 : 0x07E0;
  //     BSP_ST7735_RectFill(&display_st7735, &myRect, color);
  //     // BSP_ST7735_LineDrawH(&display_st7735,0, ST7735_TFTWIDTH, j, 0xFF00);
  //     BSP_Delay_ms(500);
  // }
  
  // tRectangle myRect2 = {10, 0, 11, ST7735_TFTHEIGHT-1};  // xMin, yMin, xMax, yMax
  // BSP_ST7735_RectFill(&display_st7735, &myRect2, 0x07E0);
  // UARTprintf("test 2...\n  ");
  
  // for(int j = 0; j < ST7735_TFTWIDTH; j++)
  // {
  //   // int width_length = psRect->i16XMax - psRect->i16XMin + 1; // Calculate the width of the rectangle
  //   // int height_length = psRect->i16YMax - psRect->i16YMin + 1; // Calculate the height of the rectangle 
  //     myRect.i16XMin = ST7735_TFTWIDTH-1-j;
  //     myRect.i16YMin = j;

  //      uint16_t color = (j % 2 == 0) ? 0xF800 : 0x07E0;
  //     BSP_ST7735_RectFill(&display_st7735, &myRect, color);
  //     // BSP_ST7735_LineDrawH(&display_st7735,0, ST7735_TFTWIDTH, j, 0xFF00);
  //     BSP_Delay_ms(500);
  // }
  // UARTprintf("test 3...\n  ");
  // for(int j = 0; j < ST7735_TFTHEIGHT; j++)
  // {
  //   // int width_length = psRect->i16XMax - psRect->i16XMin + 1; // Calculate the width of the rectangle
  //   // int height_length = psRect->i16YMax - psRect->i16YMin + 1; // Calculate the height of the rectangle 
  //     myRect.i16XMin = ST7735_TFTWIDTH-1-j;
  //     myRect.i16YMin = ST7735_TFTWIDTH-1-j;

  //      uint16_t color = (j % 2 == 0) ? 0xF800 : 0x07E0;
  //     BSP_ST7735_RectFill(&display_st7735, &myRect, color);
  //     // BSP_ST7735_LineDrawH(&display_st7735,0, ST7735_TFTWIDTH, j, 0xFF00);
  //     BSP_Delay_ms(500);
  // }
  // UARTprintf("test 4...\n  ");
  // for(int j = 0; j < ST7735_TFTHEIGHT; j++)
  // {
  //   // int width_length = psRect->i16XMax - psRect->i16XMin + 1; // Calculate the width of the rectangle
  //   // int height_length = psRect->i16YMax - psRect->i16YMin + 1; // Calculate the height of the rectangle 
  //     myRect.i16XMin = j;
  //     myRect.i16YMin = j;

  //      uint16_t color = (j % 2 == 0) ? 0xF800 : 0x07E0;
  //     BSP_ST7735_RectFill(&display_st7735, &myRect, color);
  //     // BSP_ST7735_LineDrawH(&display_st7735,0, ST7735_TFTWIDTH, j, 0xFF00);
  //     BSP_Delay_ms(500);
  // }
  // UARTprintf("test 5...\n  ");
  // for(int j = 0; j < ST7735_TFTHEIGHT; j++)
  // {
  //   // int width_length = psRect->i16XMax - psRect->i16XMin + 1; // Calculate the width of the rectangle
  //   // int height_length = psRect->i16YMax - psRect->i16YMin + 1; // Calculate the height of the rectangle 
  //     myRect.i16XMin = 0;
  //     myRect.i16YMin = j;

  //      uint16_t color = (j % 2 == 0) ? 0xF800 : 0x07E0;
  //     BSP_ST7735_RectFill(&display_st7735, &myRect, color);
  //     // BSP_ST7735_LineDrawH(&display_st7735,0, ST7735_TFTWIDTH, j, 0xFF00);
  //     BSP_Delay_ms(500);
  // }
  // UARTprintf("test 6...\n  ");
  // for(int j = 0; j < ST7735_TFTHEIGHT; j++)
  // {
  //   // int width_length = psRect->i16XMax - psRect->i16XMin + 1; // Calculate the width of the rectangle
  //   // int height_length = psRect->i16YMax - psRect->i16YMin + 1; // Calculate the height of the rectangle 
  //     myRect.i16XMin = 0;
  //     myRect.i16YMin = ST7735_TFTWIDTH-1-j;

  //      uint16_t color = (j % 2 == 0) ? 0xF800 : 0x07E0;
  //     BSP_ST7735_RectFill(&display_st7735, &myRect, color);
  //     // BSP_ST7735_LineDrawH(&display_st7735,0, ST7735_TFTWIDTH, j, 0xFF00);
  //     BSP_Delay_ms(500);
  // }
  // UARTprintf("test 7...\n  ");
  // for(int j = 0; j < ST7735_TFTHEIGHT; j++)
  // {
  //   // int width_length = psRect->i16XMax - psRect->i16XMin + 1; // Calculate the width of the rectangle
  //   // int height_length = psRect->i16YMax - psRect->i16YMin + 1; // Calculate the height of the rectangle 
  //     myRect.i16XMin = j;
  //     myRect.i16YMin = 0;

  //      uint16_t color = (j % 2 == 0) ? 0xF800 : 0x07E0;
  //     BSP_ST7735_RectFill(&display_st7735, &myRect, color);
  //     // BSP_ST7735_LineDrawH(&display_st7735,0, ST7735_TFTWIDTH, j, 0xFF00);
  //     BSP_Delay_ms(500);
  // }
  // UARTprintf("test 7...\n  ");
  // for(int j = 0; j < ST7735_TFTHEIGHT; j++)
  // {
  //   // int width_length = psRect->i16XMax - psRect->i16XMin + 1; // Calculate the width of the rectangle
  //   // int height_length = psRect->i16YMax - psRect->i16YMin + 1; // Calculate the height of the rectangle 
  //     myRect.i16XMin = ST7735_TFTWIDTH-1-j;
  //     myRect.i16YMin = 0;

  //      uint16_t color = (j % 2 == 0) ? 0xF800 : 0x07E0;
  //     BSP_ST7735_RectFill(&display_st7735, &myRect, color);
  //     // BSP_ST7735_LineDrawH(&display_st7735,0, ST7735_TFTWIDTH, j, 0xFF00);
  //     BSP_Delay_ms(500);
  // }
  // for(int i = 0; i <  ST7735_TFTWIDTH; i++){
  //   BSP_ST7735_LineDrawH(&display_st7735, 0, ST7735_TFTWIDTH, i, 0x2FF0);
  //   BSP_Delay_ms(500);
  // }
  // for(int i = 0; i <  ST7735_TFTWIDTH; i++){
  //   BSP_ST7735_LineDrawH(&display_st7735, 0, ST7735_TFTWIDTH, ST7735_TFTWIDTH-i, 0x10FF);
  //   BSP_Delay_ms(500);
  // }
  // for(int i = 0; i <  ST7735_TFTWIDTH; i++){
  //   BSP_ST7735_LineDrawV(&display_st7735, i, 0, ST7735_TFTHEIGHT, 0x8833);
  //   BSP_Delay_ms(500);
  // }
  // for(int i = 0; i <  ST7735_TFTWIDTH; i++){
  //   BSP_ST7735_LineDrawV(&display_st7735, ST7735_TFTWIDTH-i, 0, ST7735_TFTHEIGHT, 0xAA22);
  //   BSP_Delay_ms(500);
  // }
//void BSP_ST7735_LineDrawH(void *pvDisplayData, int32_t i32X1, int32_t i32X2, int32_t i32Y, uint32_t ui32Value);
}