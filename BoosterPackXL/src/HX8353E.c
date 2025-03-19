//
// Screen_HX8353E.cpp
// Library C++ code
// ----------------------------------
// Developed with embedXcode
// http://embedXcode.weebly.com
//
// Project new_screen_HX8353
//
// Created by Rei VILO, mars 07, 2013 09:21
// embedXcode.weebly.com
//
// Apr 25, 2014
// Support for new LaunchPads with
// . MSP430F5529
// . LM4F120H5QR TM4C123GH6PM
// . TM4C1294NCPDT TM4C1294XNCZAD
//
//
// Copyright © Rei VILO, 2013-2014
// License CC = BY NC SA
//
// See Screen_HX8353E.h and ReadMe.txt for references
//
// Library header
#include "HX8353E.h"
///
#define HX8353E_WIDTH  128
#define HX8353E_HEIGHT 128
#define HX8353E_MADCTL_MY  0x80
#define HX8353E_MADCTL_MX  0x40
#define HX8353E_MADCTL_MV  0x20
#define HX8353E_MADCTL_ML  0x10
#define HX8353E_MADCTL_RGB 0x08
#define HX8353E_MADCTL_MH  0x04
#define HX8353E_NOP     0x00
#define HX8353E_SWRESET 0x01
#define HX8353E_RDDID   0x04
#define HX8353E_RDDST   0x09
#define HX8353E_SLPIN   0x10
#define HX8353E_SLPOUT  0x11
#define HX8353E_PTLON   0x12
#define HX8353E_NORON   0x13
#define HX8353E_INVOFF  0x20
#define HX8353E_INVON   0x21
#define HX8353E_GAMSET  0x26
#define HX8353E_DISPOFF 0x28
#define HX8353E_DISPON  0x29
#define HX8353E_CASET   0x2A
#define HX8353E_RASET   0x2B
#define HX8353E_RAMWR   0x2C
#define HX8353E_RGBSET  0x2d
#define HX8353E_RAMRD   0x2E
#define HX8353E_PTLAR   0x30
#define HX8353E_MADCTL  0x36
#define HX8353E_COLMOD  0x3A
#define HX8353E_SETPWCTR 0xB1
#define HX8353E_SETDISPL 0xB2
#define HX8353E_FRMCTR3  0xB3
#define HX8353E_SETCYC   0xB4
#define HX8353E_SETBGP   0xb5
#define HX8353E_SETVCOM  0xB6
#define HX8353E_SETSTBA  0xC0
#define HX8353E_SETID    0xC3
#define HX8353E_GETHID   0xd0
#define HX8353E_SETGAMMA 0xE0

#define LOW 0x00
#define HIGH 0x01

#define highByte(w) ((uint8_t)((w) >> 8))   // Extracts the high byte
#define lowByte(w)  ((uint8_t)((w) & 0xFF)) // Extracts the low byte


void _setPoint(uint16_t x1, uint16_t y1, uint16_t colour);
void _setWindow(uint16_t x0, uint16_t y0, uint16_t x1, uint16_t y1);
void _writeRegister(uint8_t command8, uint8_t data8);
void _writeCommand(uint8_t command8);
void _writeData(uint8_t data8);
void _writeData16(uint16_t data16);
void _writeData88(uint8_t dataHigh8, uint8_t dataLow8);
void _writeData8888(uint8_t dataHigh8, uint8_t dataLow8, uint8_t data8_3, uint8_t data8_4);
void _fastFill(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2, uint16_t colour);
// void _getRawTouch(uint16_t &x, uint16_t &y, uint16_t &z);

uint8_t _pinReset;
uint32_t _portReset;
uint8_t _pinDataCommand;
uint32_t _portDataCommand;
uint8_t _pinChipSelect;
uint32_t _portChipSelect;
uint8_t _pinBacklight;
uint32_t _portBacklight;


uint8_t      _fontX, _fontY, _fontSize;
uint8_t      _orientation;
bool         _penSolid, _fontSolid, _flagRead, _flagStorage;
uint16_t     _screenWidth, _screenHeigth;
uint8_t      _touchTrim;


void digitalWrite(uint32_t _port, uint8_t _pin, uint8_t _val)
{
    BSP_GPIO_Write(_port, _pin, _val);
}

void transfer(uint8_t command8)
{
    BSP_SSI_Send(&command8, sizeof(command8));
}


void Screen_HX8353E(
    uint32_t resetPort, 
    uint8_t resetPin, 
    uint32_t dataCommandPort, 
    uint8_t dataCommandPin, 
    uint32_t chipSelectPort, 
    uint8_t chipSelectPin, 
    uint32_t backlightPort, 
    uint8_t backlightPin)
{
    _portReset = resetPort;
    _pinReset = resetPin;
    _portDataCommand = dataCommandPort;
    _pinDataCommand = dataCommandPin;
    _portChipSelect = chipSelectPort;
    _pinChipSelect = chipSelectPin;
    _portBacklight = backlightPort;
    _pinBacklight = backlightPin;
};
void begin()
{
#if defined(__LM4F120H5QR__)
    SPI.setModule(2);
#endif
    // SPI.begin();
    // SPI.setClockDivider(SPI_CLOCK_DIV2);
    // SPI.setBitOrder(MSBFIRST);
    // SPI.setDataMode(SPI_MODE0);
    // if (_pinReset!=NULL) pinMode(_pinReset, OUTPUT);
    // if (_pinBacklight!=NULL) pinMode(_pinBacklight, OUTPUT);
    // pinMode(_pinDataCommand, OUTPUT);
    // pinMode(_pinChipSelect, OUTPUT);
    digitalWrite(_portBacklight, _pinBacklight, HIGH);
    digitalWrite(_portReset, _pinReset, HIGH);
    SysCtlDelay(100);
    digitalWrite(_portReset, _pinReset, LOW);
    SysCtlDelay(50);
    digitalWrite(_portReset, _pinReset, HIGH);
    SysCtlDelay(120);
    _writeCommand(HX8353E_SWRESET);
    SysCtlDelay(150);
    _writeCommand(HX8353E_SLPOUT);
    SysCtlDelay(200);
    _writeRegister(HX8353E_GAMSET, 0x04);
    _writeCommand(HX8353E_SETPWCTR);
    _writeData88(0x0A, 0x14);
    _writeCommand(HX8353E_SETSTBA);
    _writeData88(0x0A, 0x00);
    _writeRegister(HX8353E_COLMOD, 0x05);
    SysCtlDelay(10);
    _writeRegister(HX8353E_MADCTL, HX8353E_MADCTL_RGB);
    _writeCommand(HX8353E_CASET);
    _writeData8888(0x00, 0x00, 0x00, 0x79);
    _writeCommand(HX8353E_RASET);
    _writeData8888(0x00, 0x00, 0x00, 0x79);
    _writeCommand(HX8353E_NORON);
    SysCtlDelay(10);
    _writeCommand(HX8353E_DISPON);
    SysCtlDelay(120);
    _writeCommand(HX8353E_RAMWR);
    setBacklight(true);
    setOrientation(0);
    _screenWidth  = HX8353E_WIDTH;
    _screenHeigth = HX8353E_HEIGHT;
    _penSolid  = false;
    _fontSolid = true;
    _flagRead  = false;
    _touchTrim = 0;
    //clear();
}
void invert(bool flag)
{
    _writeCommand(flag ? HX8353E_INVON : HX8353E_INVOFF);
}
void setBacklight(bool flag)
{
    digitalWrite(_portBacklight, _pinBacklight, flag);
}
void setDisplay(bool flag)
{
    setBacklight(flag);
}
void setOrientation(uint8_t orientation)
{
    _orientation = orientation % 4;
    _writeCommand(HX8353E_MADCTL);
    switch (_orientation) {
        case 0:
            _writeData(HX8353E_MADCTL_MX | HX8353E_MADCTL_MY | HX8353E_MADCTL_RGB);
            break;
        case 1:
            _writeData(HX8353E_MADCTL_MY | HX8353E_MADCTL_MV | HX8353E_MADCTL_RGB);
            break;
        case 2:
            _writeData(HX8353E_MADCTL_RGB);
            break;
        case 3:
            _writeData(HX8353E_MADCTL_MX | HX8353E_MADCTL_MV | HX8353E_MADCTL_RGB);
            break;
    }
}
void _fastFill(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2, uint16_t colour)
{
    // if (x1 > x2) _swap(x1, x2);
    // if (y1 > y2) _swap(y1, y2);
    // _setWindow(x1, y1, x2, y2);
    // digitalWrite(_portDataCommand, _pinDataCommand, HIGH);
    // digitalWrite(_portChipSelect, _pinChipSelect, LOW);
    // uint8_t hi = highByte(colour);
    // uint8_t lo = lowByte(colour);
    // for (uint32_t t=(uint32_t)(y2-y1+1)*(x2-x1+1); t>0; t--) {
    //     transfer(hi);
    //     transfer(lo);
    // }
    // digitalWrite(_portChipSelect, _pinChipSelect, HIGH);
}
void _setPoint(uint16_t x1, uint16_t y1, uint16_t colour)
{
    if( (x1 < 0) || (x1 >= screenSizeX()) || (y1 < 0) || (y1 >= screenSizeY()) ) return;
    _setWindow(x1, y1, x1+1, y1+1);
    _writeData16(colour);
}
void _setWindow(uint16_t x0, uint16_t y0, uint16_t x1, uint16_t y1)
{
    switch (_orientation) {
        case 0:
            x0 += 2;
            y0 += 3;
            x1 += 2;
            y1 += 3;
            break;
        case 1:
            x0 += 3;
            y0 += 2;
            x1 += 3;
            y1 += 2;
            break;
        case 2:
            x0 += 2;
            y0 += 1;
            x1 += 2;
            y1 += 1;
            break;
        case 3:
            x0 += 1;
            y0 += 2;
            x1 += 1;
            y1 += 2;
            break;
        default:
            break;
    }
    _writeCommand(HX8353E_CASET);
    _writeData16(x0);
    _writeData16(x1);
    _writeCommand(HX8353E_RASET);
    _writeData16(y0);
    _writeData16(y1);
    _writeCommand(HX8353E_RAMWR);
}
void _writeRegister(uint8_t command8, uint8_t data8)
{
    _writeCommand(command8);
    _writeData(data8);
}
void _writeCommand(uint8_t command8)
{
    digitalWrite(_portDataCommand, _pinDataCommand, LOW);
    digitalWrite(_portChipSelect, _pinChipSelect, LOW);
    transfer(command8);
    digitalWrite(_portChipSelect, _pinChipSelect, HIGH);
}
void _writeData(uint8_t data8)
{
    digitalWrite(_portDataCommand, _pinDataCommand, HIGH);
    digitalWrite(_portChipSelect, _pinChipSelect, LOW);
    transfer(data8);
    digitalWrite(_portChipSelect, _pinChipSelect, HIGH);
}
void _writeData16(uint16_t data16)
{
    digitalWrite(_portDataCommand, _pinDataCommand, HIGH);
    digitalWrite(_portChipSelect, _pinChipSelect, LOW);
    transfer(highByte(data16));
    transfer(lowByte(data16));
    digitalWrite(_portChipSelect, _pinChipSelect, HIGH);
}
void _writeData88(uint8_t dataHigh8, uint8_t dataLow8)
{
    digitalWrite(_portDataCommand, _pinDataCommand, HIGH);
    digitalWrite(_portChipSelect, _pinChipSelect, LOW);
    transfer(dataHigh8);
    transfer(dataLow8);
    digitalWrite(_portChipSelect, _pinChipSelect, HIGH);
}
void _writeData8888(uint8_t dataHigh8, uint8_t dataLow8, uint8_t data8_3, uint8_t data8_4)
{
    _writeData(dataHigh8);
    _writeData(dataLow8);
    _writeData(data8_3);
    _writeData(data8_4);
}
// void _getRawTouch(uint16_t &x0, uint16_t &y0, uint16_t &z0)
// {
//     x0 = 0;
//     y0 = 0;
//     z0 = 0;
// }
uint16_t screenSizeX()
{
    switch (_orientation) {
        case 0:
        case 2:
            return _screenWidth;
            break;
        case 1:
        case 3:
            return _screenHeigth;
            break;
    }

    return 0;
}
uint16_t screenSizeY()
{
    switch (_orientation) {
        case 0:
        case 2:
            return _screenHeigth;
            break;
        case 1:
        case 3:
            return _screenWidth;
            break;
    }
    
    return 0;
}