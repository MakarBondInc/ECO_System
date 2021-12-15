#include "I2C.h"
void I2C_init(void)
{
    //PB6 -- SCL (I2C1)
    GPIOB->MODER &=~ GPIO_MODER_MODE6;          //Сброс режима работы вывода A5
    GPIOB->MODER |= GPIO_MODER_MODE6_1;         //Выбор режима для вывода А5 (альтернативная функция)
    GPIOB->OSPEEDR &=~ GPIO_OSPEEDER_OSPEED6;   //Сброс скорости для вывода А5
    GPIOB->AFR[0] |= (1 << 24);                 //Установка альтернативной функции (AF0)

    //PB7 -- SDA (I2C1)
    GPIOB->MODER &=~ GPIO_MODER_MODE7;          //Сброс режима работы вывода A5
    GPIOB->MODER |= GPIO_MODER_MODE7_1;         //Выбор режима для вывода А5 (альтернативная функция)
    GPIOB->OSPEEDR &=~ GPIO_OSPEEDER_OSPEED7;   //Сброс скорости для вывода А5
    GPIOB->AFR[0] |= (1 << 28);                 //Установка альтернативной функции (AF0)

    RCC->APB1ENR |= RCC_APB1ENR_I2C1EN;         //Разрешение тактирования интерфейса I2C

    I2C1->TIMINGR = 0x00303D5B;

    I2C1->CR2 |= (0b00000000000000100000000000000000);

    I2C1->CR2 |= (I2C_CR2_SADD_Msk & 0x53);

    I2C1->CR2 |= I2C_CR2_AUTOEND;

    I2C1->CR2 |= I2C_CR2_RD_WRN;

    I2C1->CR1 |= I2C_CR1_PE;                    //Включение модуля I2C
}
/*
void I2C_start(void)
{
    I2C1->CR1 |= I2C_CR1_START;
    while (!(I2C1->SR1 & I2C_SR1_SB)){;}
    flag = I2C1->SR1;
}

void I2C_stop(void)
{
    I2C1->CR1 |= I2C_CR1_STOP;
}

void I2C_send_address(uint8_t address)
{
    I2C1->DR = address;
    while (!(I2C1->SR1 & I2C_SR1_ADDR)){;}
    flag = I2C1->SR1;
    flag = I2C1->SR2;
}
*/
void I2C_send_byte(uint8_t byte)
{
    if((I2C1->ISR & I2C_ISR_TXE) == I2C_ISR_TXE)
    {
        I2C1->TXDR = byte;
        I2C1->CR2 |= I2C_CR2_START;
    }
}
uint8_t I2C_read_byte(void)
{
    while(1)
    {
        if((I2C1->ISR & I2C_ISR_RXNE) == I2C_ISR_RXNE)
        {
            data = I2C2->RXDR;
            return data;
        }
    }
}