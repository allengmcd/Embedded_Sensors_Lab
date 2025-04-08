#include "PCA9685.h"



#define PCA9685_I2C_BASE_MODULE_ADDRESS 0x40
#define PCA9685_I2C_BASE_MODULE_ADRMASK 0x3F
#define PCA9685_I2C_BASE_PROXY_ADDRESS  0xE0
#define PCA9685_I2C_BASE_PROXY_ADRMASK  0xFE

// Register addresses from data sheet
#define PCA9685_MODE1_REG               0x00
#define PCA9685_MODE2_REG               0x01
#define PCA9685_SUBADR1_REG             0x02
#define PCA9685_SUBADR2_REG             0x03
#define PCA9685_SUBADR3_REG             0x04
#define PCA9685_ALLCALL_REG             0x05
#define PCA9685_LED0_REG                0x06          // Start of LEDx regs, 4B per reg, 2B on phase, 2B off phase, little-endian
#define PCA9685_PRESCALE_REG            0xFE
#define PCA9685_ALLLED_REG              0xFA

// Mode1 register values
#define PCA9685_MODE1_RESTART           0x80
#define PCA9685_MODE1_EXTCLK            0x40
#define PCA9685_MODE1_AUTOINC           0x20
#define PCA9685_MODE1_SLEEP             0x10
#define PCA9685_MODE1_SUBADR1           0x08
#define PCA9685_MODE1_SUBADR2           0x04
#define PCA9685_MODE1_SUBADR3           0x02
#define PCA9685_MODE1_ALLCALL           0x01

// Mode2 register values
#define PCA9685_MODE2_OUTDRV_TPOLE      0x04
#define PCA9685_MODE2_INVRT             0x10
#define PCA9685_MODE2_OUTNE_TPHIGH      0x01
#define PCA9685_MODE2_OUTNE_HIGHZ       0x02
#define PCA9685_MODE2_OCH_ONACK         0x08

#define PCA9685_SW_RESET                0x06          // Sent to address 0x00 to reset all devices on Wire line
#define PCA9685_PWM_FULL                (uint16_t)0x1000    // Special value for full on/full off LEDx modes
#define PCA9685_PWM_MASK                (uint16_t)0x0FFF    // Mask for 12-bit/4096 possible phase positions

#define PCA9685_CHANNEL_COUNT           16
#define PCA9685_MIN_CHANNEL             0
#define PCA9685_MAX_CHANNEL             (PCA9685_CHANNEL_COUNT - 1)
#define PCA9685_ALLLED_CHANNEL          -1                  // Special value for ALLLED registers


  
I2C_Struct pca9685 = {
    I2C1_BASE,
    SYSCTL_PERIPH_I2C1,
    SYSCTL_PERIPH_GPIOA,
    GPIO_PORTA_BASE,
    GPIO_PIN_6,
    SYSCTL_PERIPH_GPIOA,
    GPIO_PORTA_BASE,
    GPIO_PIN_7,
    PCA9685_I2C_BASE_MODULE_ADDRESS
};


void PCA9685_Init()
{
    // Initialize the I2C module with the PCA9685 pins
    BSP_I2C_Init(&pca9685);
}


// void PCA9685_Write_Reg(int motor, int speed)
// {
    
// }


// void PCA9685_Write_All(int motor, int speed)
// {
    
// }


// void PCA9685_Read_All(int motor)
// {
    
// }


// void PCA9685_Write_All_LED(int motor, int speed)
// {
    
// }

// void PCA9685_Write_All_LED_ON(int motor, int speed)
// {
    
// }


// void PCA9685_Write_All_LED_OFF(int motor, int speed)
// {
    
// }
