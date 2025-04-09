#include "bsp_i2c.h"



void BSP_I2C_Init(I2C_Struct *i2c_struct)
{
    // Enable the I2C0 peripheral
    SysCtlPeripheralEnable(i2c_struct->sysctl_periph);
    while(!SysCtlPeripheralReady(i2c_struct->sysctl_periph)) { }

    // Initialize Master and Slave and Specify slave address
    I2CMasterInitExpClk(i2c_struct->i2c_base, SysCtlClockGet(), true);
    I2CMasterSlaveAddrSet(i2c_struct->i2c_base, i2c_struct->address, false);

    // Enable the SCL and SDA peripheral
    SysCtlPeripheralEnable(i2c_struct->scl_gpio_periph);
    while(!SysCtlPeripheralReady(i2c_struct->scl_gpio_periph)) { }
    SysCtlPeripheralEnable(i2c_struct->sda_gpio_periph);
    while(!SysCtlPeripheralReady(i2c_struct->sda_gpio_periph)) { }

    // Set the SCL and SDA pins to I2C mode
    GPIOPinTypeI2CSCL(i2c_struct->scl_port, i2c_struct->scl_pin);
    GPIOPinTypeI2C(i2c_struct->sda_port, i2c_struct->sda_pin);
}


void BSP_I2C_Send_Single(I2C_Struct *i2c_struct, uint8_t data)
{
    //specify that we are writing (a register address) to the
    //slave device
    I2CMasterSlaveAddrSet(i2c_struct->i2c_base, i2c_struct->address, false);

    //specify register to be read
    I2CMasterDataPut(i2c_struct->i2c_base, data);

    //send control byte and register address byte to slave device
    I2CMasterControl(i2c_struct->i2c_base, I2C_MASTER_CMD_SINGLE_SEND);

    // Wait until the slave has received and acknowledged the data.
    SysCtlDelay(100);
    while(I2CMasterBusy(i2c_struct->i2c_base));
}


void BSP_I2C_Send_Burst(I2C_Struct *i2c_struct, uint8_t *data, uint32_t length)
{
    //specify that we are writing (a register address) to the
    //slave device
    I2CMasterSlaveAddrSet(i2c_struct->i2c_base, i2c_struct->address, false);

    for(int i = 0; i < length; i++)
    {
        if(i == 0) // first send
        {
            // Send register address (1st byte)
            I2CMasterDataPut(i2c_struct->i2c_base, data[i]);
            I2CMasterControl(i2c_struct->i2c_base, I2C_MASTER_CMD_BURST_SEND_START);
            while (I2CMasterBusy(i2c_struct->i2c_base));
        }
        else if(i < length) // middle sends
        {   
            // Send register address (middle bytes)
            I2CMasterDataPut(i2c_struct->i2c_base, data[i]);
            I2CMasterControl(i2c_struct->i2c_base, I2C_MASTER_CMD_BURST_SEND_CONT);
            while (I2CMasterBusy(i2c_struct->i2c_base));
        }
        else // last send
        {
            // Send register address (last byte)
            I2CMasterDataPut(i2c_struct->i2c_base, data[i]);
            I2CMasterControl(i2c_struct->i2c_base, I2C_MASTER_CMD_BURST_SEND_FINISH);
            while (I2CMasterBusy(i2c_struct->i2c_base));
        }
    }
}

void BSP_I2C_Receive8(I2C_Struct *i2c_struct, uint8_t reg, int8_t *result)
{
    //specify that we are reading (a register address) to the
    //slave device
    I2CMasterSlaveAddrSet(i2c_struct->i2c_base, i2c_struct->address, true);


    // Send register address (1st byte)
    I2CMasterDataPut(i2c_struct->i2c_base, reg);
    I2CMasterControl(i2c_struct->i2c_base, I2C_MASTER_CMD_SINGLE_SEND);
    while (I2CMasterBusy(i2c_struct->i2c_base));


    //send control byte and read from the register we
    //specified
    I2CMasterControl(i2c_struct->i2c_base, I2C_MASTER_CMD_SINGLE_RECEIVE);

    //wait for MCU to finish transaction
    SysCtlDelay(100);
    while(I2CMasterBusy(i2c_struct->i2c_base));

    result = I2CMasterDataGet(i2c_struct->i2c_base);
}

void BSP_I2C_Receive16(I2C_Struct *i2c_struct, uint8_t reg, int16_t *result)
{
    //specify that we are reading (a register address) to the
    //slave device
    I2CMasterSlaveAddrSet(i2c_struct->i2c_base, i2c_struct->address, true);

    // Send register address (1st byte)
    I2CMasterDataPut(i2c_struct->i2c_base, reg);
    I2CMasterControl(i2c_struct->i2c_base, I2C_MASTER_CMD_SINGLE_SEND);
    while (I2CMasterBusy(i2c_struct->i2c_base));

    //send control byte and read from the register we
    //specified
    I2CMasterControl(i2c_struct->i2c_base, I2C_MASTER_CMD_BURST_RECEIVE_START);

    //wait for MCU to finish transaction
    SysCtlDelay(100);
    while(I2CMasterBusy(i2c_struct->i2c_base));

    uint8_t msb = (I2CMasterDataGet(i2c_struct->i2c_base) & 0xFF);

    I2CMasterControl(i2c_struct->i2c_base, I2C_MASTER_CMD_BURST_RECEIVE_FINISH); //Receive

    SysCtlDelay(100);
    while(I2CMasterBusy(i2c_struct->i2c_base));

    uint8_t lsb = (I2CMasterDataGet(i2c_struct->i2c_base) & 0xFF );

    *result = (msb << 8) | lsb;
}