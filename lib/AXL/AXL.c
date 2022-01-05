#include "AXL.h"
void AXL_CS(uint8_t state)
{
    if(state){GPIOB->BSRR |= GPIO_BSRR_BS_14;}
    else{GPIOB->BSRR |= GPIO_BSRR_BR_14;}
}
void read_status_AXL(void)
{
    spi_size = 1;
    SPI_data_ready = 0;
    spi = 0;

    AXL_CS(LOW);        //Сигнал выбора
    send_byte_SPI1(0b10001111);
    //send_byte_SPI1(0b10100000);
    while(1)
    {
        if(SPI1->SR & SPI_SR_RXNE)  //Если прием байта закончен
        {
            SPI1->DR;    //Копируем даннные в ячейку массива (флаг RXNE сбрасывается аппаратно после чтения DR)
            break;
        }
    }
    send_byte_SPI1(0x00);
    spi_size = 1;
    SPI_data_ready = 0;
    spi = 0;
    while(1)
    {
        if(SPI1->SR & SPI_SR_RXNE)  //Если прием байта закончен
        {
            SPI_data_rx[spi] = SPI1->DR;    //Копируем даннные в ячейку массива (флаг RXNE сбрасывается аппаратно после чтения DR)
            spi++;
            if(spi == spi_size){SPI_data_ready = 1; break;}     //
        }
    }
    AXL_CS(HIGH);       //Сигнал выбора
}