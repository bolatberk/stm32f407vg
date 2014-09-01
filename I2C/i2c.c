/* berk bolat i2c library */

#include "i2c.h"
void i2c_start(){
	while(I2C_GetFlagStatus(I2C1, I2C_FLAG_BUSY));
	I2C_GenerateSTART(I2C1,ENABLE);
	while(!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_MODE_SELECT));
}

void i2c_write(uint8_t slave_add, uint8_t reg_add, uint8_t val){
	i2c_start();
	I2C_Send7bitAddress(I2C1, slave_add, I2C_Direction_Transmitter );

	        while (!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED ));

	        I2C_SendData(I2C1, reg_add);

	        while (!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_BYTE_TRANSMITTING ));

	        I2C_SendData(I2C1, val);

	        while (!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_BYTE_TRANSMITTING ));

	        I2C_GenerateSTOP(I2C1, ENABLE);
	

}
uint8_t I2C_read(uint8_t slave_add, uint8_t reg_add)
{
        uint8_t value;
        i2c_start();

        I2C_Send7bitAddress(I2C1, slave_add, I2C_Direction_Transmitter );

        while (!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED ));

        I2C_SendData(I2C1, reg_add);

        while (I2C_GetFlagStatus(I2C1, I2C_FLAG_BTF ) == RESET);

        I2C_GenerateSTOP(I2C1, ENABLE);

        I2C_GenerateSTART(I2C1, ENABLE);

        while (!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_MODE_SELECT ));

        I2C_Send7bitAddress(I2C1, slave_add, I2C_Direction_Receiver );

        while (I2C_GetFlagStatus(I2C1, I2C_FLAG_ADDR ) == RESET);

    I2C_AcknowledgeConfig(I2C1, DISABLE);

    while (!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_BYTE_RECEIVED ));

    value = I2C_ReceiveData(I2C1);

        I2C_GenerateSTOP(I2C1, ENABLE);

        return value;
}

void i2c_init(void)
{
    GPIO_InitTypeDef GPIO_InitStruct;
    I2C_InitTypeDef I2C_InitStruct;

    RCC_APB1PeriphClockCmd(RCC_APB1Periph_I2C1, ENABLE);

    RCC_APB1PeriphResetCmd(RCC_APB1Periph_I2C1, ENABLE);
    RCC_APB1PeriphResetCmd(RCC_APB1Periph_I2C1, DISABLE);
    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB, ENABLE);

    GPIO_InitStruct.GPIO_Pin = GPIO_Pin_6 | GPIO_Pin_7;
    GPIO_InitStruct.GPIO_Mode = GPIO_Mode_AF;
    GPIO_InitStruct.GPIO_Speed = GPIO_Speed_100MHz;
    GPIO_InitStruct.GPIO_OType = GPIO_OType_OD;
    GPIO_InitStruct.GPIO_PuPd = GPIO_PuPd_UP;
    GPIO_Init(GPIOB, &GPIO_InitStruct);

    GPIO_PinAFConfig(GPIOB, GPIO_PinSource6, GPIO_AF_I2C1);	// SCL pini
    GPIO_PinAFConfig(GPIOB, GPIO_PinSource7, GPIO_AF_I2C1); // SDA pini

    I2C_InitStruct.I2C_ClockSpeed = 100000;    //i2c hýzý normal modda
    I2C_InitStruct.I2C_Mode = I2C_Mode_I2C;
    I2C_InitStruct.I2C_DutyCycle = I2C_DutyCycle_2;
    I2C_InitStruct.I2C_OwnAddress1 = 0x00;
    I2C_InitStruct.I2C_Ack = I2C_Ack_Enable;
    I2C_InitStruct.I2C_AcknowledgedAddress = I2C_AcknowledgedAddress_7bit;
    I2C_Init(I2C1, &I2C_InitStruct);

    I2C_Cmd(I2C1, ENABLE);
}
