// // Includes
// #include <stdint.h>
// #include <stdbool.h>
// #include "inc/hw_memmap.h"
// #include "inc/hw_types.h"
// #include "driverlib/gpio.h"
// #include "driverlib/pin_map.h"
// #include "driverlib/sysctl.h"
// #include "driverlib/uart.h"
// #include "driverlib/adc.h"
// #include "utils/uartstdio.h"
// #include "grlib/grlib.h"
// #include "drivers/cfal96x64x16.h"

// // Constants
// #define CURSOR_SIZE      5       // Size of cursor in pixels
// #define BUTTON_WIDTH     80      // Width of clickable button
// #define BUTTON_HEIGHT    20      // Height of clickable button
// #define BUTTON_X         32      // Button X position
// #define BUTTON_Y         40      // Button Y position
// #define MAX_ADC_VALUE    4095    // Maximum ADC value (12-bit)
// #define DISPLAY_WIDTH    96      // Display width in pixels
// #define DISPLAY_HEIGHT   64      // Display height in pixels

// // Function prototypes
// void ConfigureADC(void);
// void ConfigureButtonInput(void);
// void DrawCursor(tContext *pContext, int32_t x, int32_t y);
// void DrawButton(tContext *pContext, uint32_t clickCount);
// bool IsButtonPressed(int32_t cursorX, int32_t cursorY);

// // Global variables
// tContext g_sContext;              // Graphics context
// uint32_t g_ui32SysClock;          // System clock rate
// uint32_t g_ui32ButtonClickCount;  // Button click counter
// bool g_bButtonWasPressed;         // Track button state for edge detection

// int main(void)
// {
//     // Initialize system clock
//     g_ui32SysClock = SysCtlClockFreqSet((SYSCTL_XTAL_25MHZ | SYSCTL_OSC_MAIN | SYSCTL_USE_PLL | SYSCTL_CFG_VCO_480), 120000000);

//     // Initialize the display driver
//     CFAL96x64x16Init(g_ui32SysClock);
    
//     // Initialize the graphics context
//     GrContextInit(&g_sContext, &g_sCFAL96x64x16);
    
//     // Configure ADC for analog stick input
//     ConfigureADC();
    
//     // Configure button input
//     ConfigureButtonInput();
    
//     // Initial values
//     int32_t cursorX = DISPLAY_WIDTH / 2;
//     int32_t cursorY = DISPLAY_HEIGHT / 2;
//     g_ui32ButtonClickCount = 0;
//     g_bButtonWasPressed = false;
    
//     // Clear the screen
//     GrContextForegroundSet(&g_sContext, ClrBlack);
//     GrRectFill(&g_sContext, &(tRectangle){0, 0, DISPLAY_WIDTH - 1, DISPLAY_HEIGHT - 1});
    
//     // Draw the button initially
//     DrawButton(&g_sContext, g_ui32ButtonClickCount);
    
//     while(1)
//     {
//         uint32_t adcValues[2];
        
//         // Trigger the ADC conversion
//         ADCProcessorTrigger(ADC0_BASE, 0);
        
//         // Wait for conversion to complete
//         while(!ADCIntStatus(ADC0_BASE, 0, false)) {}
        
//         // Clear the interrupt
//         ADCIntClear(ADC0_BASE, 0);
        
//         // Get the ADC values
//         ADCSequenceDataGet(ADC0_BASE, 0, adcValues);
        
//         // Map the ADC values to cursor positions
//         int32_t newCursorX = (adcValues[0] * (DISPLAY_WIDTH - CURSOR_SIZE)) / MAX_ADC_VALUE;
//         int32_t newCursorY = (adcValues[1] * (DISPLAY_HEIGHT - CURSOR_SIZE)) / MAX_ADC_VALUE;
        
//         // Check if button is pressed
//         bool buttonPressed = !GPIOPinRead(GPIO_PORTF_BASE, GPIO_PIN_4);
        
//         // Check for a click
//         if(buttonPressed && !g_bButtonWasPressed)
//         {
//             // Check if cursor is over the button
//             if(IsButtonPressed(cursorX, cursorY))
//             {
//                 g_ui32ButtonClickCount++;
                
//                 // Redraw the button with updated count
//                 DrawButton(&g_sContext, g_ui32ButtonClickCount);
//             }
//         }
//         g_bButtonWasPressed = buttonPressed;
        
//         // Only redraw cursor if it moved
//         if(cursorX != newCursorX || cursorY != newCursorY)
//         {
//             // Clear the old cursor position
//             GrContextForegroundSet(&g_sContext, ClrBlack);
//             GrRectFill(&g_sContext, &(tRectangle){cursorX, cursorY, cursorX + CURSOR_SIZE - 1, cursorY + CURSOR_SIZE - 1});
            
//             // Update cursor position
//             cursorX = newCursorX;
//             cursorY = newCursorY;
            
//             // Draw new cursor
//             DrawCursor(&g_sContext, cursorX, cursorY);
//         }
        
//         // Short delay to prevent flickering
//         SysCtlDelay(g_ui32SysClock / 100);
//     }
// }

// // Configure ADC for analog stick
// void ConfigureADC(void)
// {
//     // Enable ADC0 peripheral
//     SysCtlPeripheralEnable(SYSCTL_PERIPH_ADC0);
    
//     // Enable GPIO port for ADC pins
//     SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOE);
    
//     // Configure pins for ADC input
//     GPIOPinTypeADC(GPIO_PORTE_BASE, GPIO_PIN_2 | GPIO_PIN_3); // PE2 for X, PE3 for Y
    
//     // Configure ADC sequence
//     ADCSequenceConfigure(ADC0_BASE, 0, ADC_TRIGGER_PROCESSOR, 0);
    
//     // Configure sequence steps
//     ADCSequenceStepConfigure(ADC0_BASE, 0, 0, ADC_CTL_CH1); // X-axis on AIN1 (PE2)
//     ADCSequenceStepConfigure(ADC0_BASE, 0, 1, ADC_CTL_CH0 | ADC_CTL_IE | ADC_CTL_END); // Y-axis on AIN0 (PE3)
    
//     // Enable the sequence
//     ADCSequenceEnable(ADC0_BASE, 0);
    
//     // Clear any pending interrupts
//     ADCIntClear(ADC0_BASE, 0);
// }

// // Configure button input
// void ConfigureButtonInput(void)
// {
//     // Enable GPIO port F
//     SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOF);
    
//     // Configure PF4 as input with pull-up (SW1 on many TM4C boards)
//     GPIOPinTypeGPIOInput(GPIO_PORTF_BASE, GPIO_PIN_4);
//     GPIOPadConfigSet(GPIO_PORTF_BASE, GPIO_PIN_4, GPIO_STRENGTH_2MA, GPIO_PIN_TYPE_STD_WPU);
// }

// // Draw cursor at the given position
// void DrawCursor(tContext *pContext, int32_t x, int32_t y)
// {
//     GrContextForegroundSet(pContext, ClrWhite);
//     GrRectFill(pContext, &(tRectangle){x, y, x + CURSOR_SIZE - 1, y + CURSOR_SIZE - 1});
// }

// // Draw button with click count text
// void DrawButton(tContext *pContext, uint32_t clickCount)
// {
//     char countText[16];
    
//     // Clear the text area above button
//     GrContextForegroundSet(pContext, ClrBlack);
//     GrRectFill(pContext, &(tRectangle){BUTTON_X, BUTTON_Y - 15, BUTTON_X + BUTTON_WIDTH, BUTTON_Y - 1});
    
//     // Draw the button
//     GrContextForegroundSet(pContext, ClrBlue);
//     GrRectFill(pContext, &(tRectangle){BUTTON_X, BUTTON_Y, BUTTON_X + BUTTON_WIDTH, BUTTON_Y + BUTTON_HEIGHT});
    
//     // Draw button border
//     GrContextForegroundSet(pContext, ClrWhite);
//     GrRectDraw(pContext, &(tRectangle){BUTTON_X, BUTTON_Y, BUTTON_X + BUTTON_WIDTH, BUTTON_Y + BUTTON_HEIGHT});
    
//     // Draw "Click Me" text inside button
//     GrContextForegroundSet(pContext, ClrWhite);
//     GrStringDrawCentered(pContext, "Click Me", -1, BUTTON_X + (BUTTON_WIDTH / 2), BUTTON_Y + (BUTTON_HEIGHT / 2), false);
    
//     // Format and draw click count text
//     sprintf(countText, "Clicks: %d", clickCount);
//     GrContextForegroundSet(pContext, ClrYellow);
//     GrStringDrawCentered(pContext, countText, -1, BUTTON_X + (BUTTON_WIDTH / 2), BUTTON_Y - 8, false);
// }

// // Check if cursor is over the button
// bool IsButtonPressed(int32_t cursorX, int32_t cursorY)
// {
//     return (cursorX >= BUTTON_X && 
//             cursorX <= BUTTON_X + BUTTON_WIDTH &&
//             cursorY >= BUTTON_Y && 
//             cursorY <= BUTTON_Y + BUTTON_HEIGHT);
// }