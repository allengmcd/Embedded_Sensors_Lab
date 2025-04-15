// #include "PCA9685.h"



// #define PCA9685_I2C_BASE_MODULE_ADDRESS 0x40
// #define PCA9685_I2C_BASE_MODULE_ADRMASK 0x3F
// #define PCA9685_I2C_BASE_PROXY_ADDRESS  0xE0
// #define PCA9685_I2C_BASE_PROXY_ADRMASK  0xFE

// // Register addresses from data sheet
// #define PCA9685_MODE1_REG               0x00
// #define PCA9685_MODE2_REG               0x01
// #define PCA9685_SUBADR1_REG             0x02
// #define PCA9685_SUBADR2_REG             0x03
// #define PCA9685_SUBADR3_REG             0x04
// #define PCA9685_ALLCALL_REG             0x05
// #define PCA9685_LED0_REG                0x06          // Start of LEDx regs, 4B per reg, 2B on phase, 2B off phase, little-endian
// #define PCA9685_PRESCALE_REG            0xFE
// #define PCA9685_ALLLED_REG              0xFA

// // Mode1 register values
// #define PCA9685_MODE1_RESTART           0x80
// #define PCA9685_MODE1_EXTCLK            0x40
// #define PCA9685_MODE1_AUTOINC           0x20
// #define PCA9685_MODE1_SLEEP             0x10
// #define PCA9685_MODE1_SUBADR1           0x08
// #define PCA9685_MODE1_SUBADR2           0x04
// #define PCA9685_MODE1_SUBADR3           0x02
// #define PCA9685_MODE1_ALLCALL           0x01

// // Mode2 register values
// #define PCA9685_MODE2_OUTDRV_TPOLE      0x04
// #define PCA9685_MODE2_INVRT             0x10
// #define PCA9685_MODE2_OUTNE_TPHIGH      0x01
// #define PCA9685_MODE2_OUTNE_HIGHZ       0x02
// #define PCA9685_MODE2_OCH_ONACK         0x08

// #define PCA9685_SW_RESET                0x06          // Sent to address 0x00 to reset all devices on Wire line
// #define PCA9685_PWM_FULL                (uint16_t)0x1000    // Special value for full on/full off LEDx modes
// #define PCA9685_PWM_MASK                (uint16_t)0x0FFF    // Mask for 12-bit/4096 possible phase positions

// #define PCA9685_CHANNEL_COUNT           16
// #define PCA9685_MIN_CHANNEL             0
// #define PCA9685_MAX_CHANNEL             (PCA9685_CHANNEL_COUNT - 1)
// #define PCA9685_ALLLED_CHANNEL          -1                  // Special value for ALLLED registers

// uint8_t i2c_buffer[64]; 

// I2C_Struct pca9685 = {
//     I2C1_BASE,
//     SYSCTL_PERIPH_I2C1,
//     SYSCTL_PERIPH_GPIOA,
//     GPIO_PORTA_BASE,
//     GPIO_PIN_6,
//     SYSCTL_PERIPH_GPIOA,
//     GPIO_PORTA_BASE,
//     GPIO_PIN_7,
//     PCA9685_I2C_BASE_MODULE_ADDRESS
// };


// void PCA9685_Init()
// {
//     uint16_t freq = 0;

//     PCA9685_PWM_Freq(freq);
// }


// /**
//  * Sets the frequency of PWM signals.
//  * Frequency will be capped to range [40..1000] Hertz. Try 50 for servos.
//  */
// void PCA9685_PWM_Freq(uint16_t freq)
// {
//     // float freq = 0.0f;

//     // Cap at min and max
//     freq = (freq > 1000 ? 1000 : (freq < 40 ? 40 : freq));

//     // To set pwm frequency we have to set the prescale register. The formula is:
//     // prescale = round(osc_clock / (4096 * frequency))) - 1 where osc_clock = 25 MHz
//     // Further info here: http://www.nxp.com/documents/data_sheet/PCA9685.pdf Page 24
//     uint8_t prescale = (int)(25000000.0f / (4096 * freq) - 0.5f);

//     // Initialize the I2C module with the PCA9685 pins
//     BSP_I2C_Init(&pca9685);

//     uint8_t settings;
//     BSP_I2C_Receive8(&pca9685, PCA9685_MODE1_REG, &settings);
// 	uint8_t sleep	= settings | PCA9685_MODE1_SLEEP;			// Set sleep bit to 1
// 	uint8_t wake 	= settings & ~PCA9685_MODE1_SLEEP;			// Set sleep bit to 0
//     uint8_t restart = wake | PCA9685_MODE1_RESTART;	        // Set restart bit to 1

//     i2c_buffer[0] = PCA9685_MODE1_REG;
//     i2c_buffer[1] = sleep; // Sleep mode
//     i2c_buffer[2] = PCA9685_PRESCALE_REG;   
//     i2c_buffer[3] = prescale;   
//     i2c_buffer[4] = PCA9685_MODE1_REG;
//     i2c_buffer[5] = wake; // Wake up
//     BSP_I2C_Send_Burst(&pca9685, &i2c_buffer[0], 6);

//     BSP_Delay_ms(10);

//     i2c_buffer[0] = PCA9685_MODE1_REG;
//     i2c_buffer[1] = restart; // Sleep mode
//     BSP_I2C_Send_Burst(&pca9685, &i2c_buffer[0], 2);
// }

// /**
//  * Set all leds back to default values (: fullOff = 1)
//  */
// void PCA9685_PWM_Reset(uint8_t fd)
// {
//     i2c_buffer[0] = PCA9685_ALLLED_REG;
//     i2c_buffer[1] = 0x00;
//     i2c_buffer[2] = 0x10;
//     i2c_buffer[3] = 0x00;
//     BSP_I2C_Send_Burst(&pca9685, &i2c_buffer[0], 4);
// }

// /**
//  * Write on and off ticks manually to a pin
//  * (Deactivates any full-on and full-off)
//  */
// void PCA9685_PWM_Write(uint8_t fd, uint8_t pin, uint8_t on, uint8_t off)
// {
// 	uint8_t reg = PCA9685_Base_Reg(pin);

//     i2c_buffer[0] = reg;
//     i2c_buffer[1] = ((on  & 0x0FFF) >> 8) & 0xFF;
//     i2c_buffer[2] = (on  & 0x0FFF) & 0xFF;
//     i2c_buffer[3] = reg + 2;
//     i2c_buffer[4] = ((off  & 0x0FFF) >> 8) & 0xFF;
//     i2c_buffer[5] = (off  & 0x0FFF) & 0xFF;

//     BSP_I2C_Send_Burst(&pca9685, &i2c_buffer[0], 6);
// }

// /**
//  * Reads both on and off registers as 16 bit of data
//  * To get PWM: mask each value with 0xFFF
//  * To get full-on or off bit: mask with 0x1000
//  * Note: ALL_LED pin will always return 0
//  */
// void PCA9685_PWM_Read(uint8_t fd, uint8_t pin, uint16_t *on, uint16_t *off)
// {
// 	uint8_t reg = PCA9685_Base_Reg(pin);

// 	if (on)
//         BSP_I2C_Receive(&pca9685, reg, &on);
// 	if (off)
//         BSP_I2C_Receive(&pca9685, reg + 2, &off);
// }

// /**
//  * Enables or deactivates full-on
//  * tf = true: full-on
//  * tf = false: according to PWM
//  */
// void PCA9685_Full_On(uint8_t fd, uint8_t pin, uint8_t tf)
// {
// 	uint8_t reg = PCA9685_Base_Reg(pin) + 1;		// LEDX_ON_H

// 	uint8_t state;
//     BSP_I2C_Receive(&pca9685, reg, &state);

// 	// Set bit 4 to 1 or 0 accordingly
// 	state = tf ? (state | 0x10) : (state & 0xEF);

//     i2c_buffer[0] = reg;
//     i2c_buffer[1] = state;
//     BSP_I2C_Send_Burst(&pca9685, &i2c_buffer[0], 2);

// 	// For simplicity, we set full-off to 0 because it has priority over full-on
// 	if (tf)
//         PCA9685_Full_Off(fd, pin, 0);
// }

// /**
//  * Enables or deactivates full-off
//  * tf = true: full-off
//  * tf = false: according to PWM or full-on
//  */
// void PCA9685_Full_Off(uint8_t fd, uint8_t pin, uint8_t tf)
// {
// 	uint8_t reg = PCA9685_Base_Reg(pin) + 3;		// LEDX_OFF_H

// 	uint8_t state;
//     BSP_I2C_Receive(&pca9685, reg, &state);

// 	// Set bit 4 to 1 or 0 accordingly
// 	state = tf ? (state | 0x10) : (state & 0xEF);

//     i2c_buffer[0] = reg;
//     i2c_buffer[1] = state;
//     BSP_I2C_Send_Burst(&pca9685, &i2c_buffer[0], 2);
// }

// /**
//  * Helper function to get to register
//  */
// uint8_t PCA9685_BaseReg(uint8_t pin)
// {
// 	return (pin >= PCA9685_CHANNEL_COUNT ? PCA9685_ALLLED_REG : PCA9685_LED0_REG + 4 * pin);
// }




// //------------------------------------------------------------------------------------------------------------------
// //
// //	WiringPi functions
// //
// //------------------------------------------------------------------------------------------------------------------




// /**
//  * Simple PWM control which sets on-tick to 0 and off-tick to value.
//  * If value is <= 0, full-off will be enabled
//  * If value is >= 4096, full-on will be enabled
//  * Every value in between enables PWM output
//  */
// static void myPwmWrite(uint8_t pin, uint8_t value)
// {
// 	uint8_t fd   = 0;//node->fd;
// 	uint8_t ipin = pin;// - node->pinBase;

// 	if (value >= 4096)
// 		PCA9685_Full_On(fd, ipin, 1);
// 	else if (value > 0)
// 		PCA9685_PWM_Write(fd, ipin, 0, value);	// (Deactivates full-on and off by itself)
// 	else
// 		PCA9685_Full_Off(fd, ipin, 1);
// }

// /**
//  * Simple full-on and full-off control
//  * If value is 0, full-off will be enabled
//  * If value is not 0, full-on will be enabled
//  */
// static void myOnOffWrite(uint8_t pin, uint8_t value)
// {
// 	uint8_t fd   = 0;// node->fd;
// 	uint8_t ipin = pin;// - node->pinBase;

// 	if (value)
// 		PCA9685_Full_On(fd, ipin, 1);
// 	else
// 		PCA9685_Full_Off(fd, ipin, 1);
// }

// /**
//  * Reads off registers as 16 bit of data
//  * To get PWM: mask with 0xFFF
//  * To get full-off bit: mask with 0x1000
//  * Note: ALL_LED pin will always return 0
//  */
// static uint8_t myOffRead(uint8_t pin)
// {
// 	uint8_t fd   = 0;//node->fd;
// 	uint8_t ipin = pin;// - node->pinBase;

// 	uint8_t off;
// 	PCA9685_PWM_Read(fd, ipin, 0, &off);

// 	return off;
// }

// /**
//  * Reads on registers as 16 bit of data
//  * To get PWM: mask with 0xFFF
//  * To get full-on bit: mask with 0x1000
//  * Note: ALL_LED pin will always return 0
//  */
// static uint8_t myOnRead(uint8_t pin)
// {
// 	uint8_t fd   = 0;//node->fd;
// 	uint8_t ipin = pin;// - node->pinBase;

// 	uint8_t on;
// 	PCA9685_PWM_Read(fd, ipin, &on, 0);

// 	return on;
// }