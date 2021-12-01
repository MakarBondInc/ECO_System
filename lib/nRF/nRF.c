#include "nRF.h"


void pin_CSN(uint8_t state)
{
    if(state == 1) {GPIOB->BSRR |= GPIO_BSRR_BS_13;}
    else{GPIOB->BSRR |= GPIO_BSRR_BR_13;}
}

void pin_CE(uint8_t state)
{
    if(state == 1) {GPIOB->BSRR |= GPIO_BSRR_BS_2;}
    else{GPIOB->BSRR |= GPIO_BSRR_BR_2;}
}

void read_register_nRF24(uint8_t reg)
{
    SPI_data_ready = 0;
    spi = 0;

    pin_CSN(LOW);        //Сигнал выбора
    send_byte_SPI1(reg);
    while(1)
    {
        if(SPI1->SR & SPI_SR_RXNE)  //Если прием байта закончен
        {
            SPI_data_rx[0] = SPI1->DR;    //Копируем даннные в переменную (флаг RXNE сбрасывается аппаратно после чтения DR)
            break;
        }
    }
    send_byte_SPI1(0x00);
    while(1)
    {
        if(SPI1->SR & SPI_SR_RXNE)  //Если прием байта закончен
        {
            SPI_data_rx[1] = SPI1->DR;    //Копируем даннные в переменную (флаг RXNE сбрасывается аппаратно после чтения DR)
            SPI_data_ready = 1;
            break;
        }
    }
    pin_CSN(HIGH);       //Сигнал выбора
}

void read_register_nRF24_5_byte(uint8_t reg)
{
    SPI_data_ready = 0;
    spi = 0;

    pin_CSN(LOW);        //Сигнал выбора
    send_byte_SPI1(reg);
    while(1)
    {
        if(SPI1->SR & SPI_SR_RXNE)  //Если прием байта закончен
        {
            SPI_data_rx[0] = SPI1->DR;    //Копируем даннные в переменную (флаг RXNE сбрасывается аппаратно после чтения DR)
            break;
        }
    }
    send_byte_SPI1(0x00);
    while(1)
    {
        if(SPI1->SR & SPI_SR_RXNE)  //Если прием байта закончен
        {
            SPI_data_rx[1] = SPI1->DR;    //Копируем даннные в переменную (флаг RXNE сбрасывается аппаратно после чтения DR)
            SPI_data_ready = 1;
            break;
        }
    }
    send_byte_SPI1(0x00);
    while(1)
    {
        if(SPI1->SR & SPI_SR_RXNE)  //Если прием байта закончен
        {
            SPI_data_rx[2] = SPI1->DR;    //Копируем даннные в переменную (флаг RXNE сбрасывается аппаратно после чтения DR)
            SPI_data_ready = 1;
            break;
        }
    }
    send_byte_SPI1(0x00);
    while(1)
    {
        if(SPI1->SR & SPI_SR_RXNE)  //Если прием байта закончен
        {
            SPI_data_rx[3] = SPI1->DR;    //Копируем даннные в переменную (флаг RXNE сбрасывается аппаратно после чтения DR)
            SPI_data_ready = 1;
            break;
        }
    }
    send_byte_SPI1(0x00);
    while(1)
    {
        if(SPI1->SR & SPI_SR_RXNE)  //Если прием байта закончен
        {
            SPI_data_rx[4] = SPI1->DR;    //Копируем даннные в переменную (флаг RXNE сбрасывается аппаратно после чтения DR)
            SPI_data_ready = 1;
            break;
        }
    }
    send_byte_SPI1(0x00);
    while(1)
    {
        if(SPI1->SR & SPI_SR_RXNE)  //Если прием байта закончен
        {
            SPI_data_rx[5] = SPI1->DR;    //Копируем даннные в переменную (флаг RXNE сбрасывается аппаратно после чтения DR)
            SPI_data_ready = 1;
            break;
        }
    }
    pin_CSN(HIGH);       //Сигнал выбора
}

void write_register_nRF24(uint8_t reg, uint8_t data)
{
    SPI_data_ready = 0;
    spi = 0;

    pin_CSN(LOW);        //Сигнал выбора
    send_byte_SPI1(reg | 0x20);
    while(1)
    {
        if(SPI1->SR & SPI_SR_RXNE)  //Если прием байта закончен
        {
            SPI_data_rx[0] = SPI1->DR;    //Копируем даннные в переменную (флаг RXNE сбрасывается аппаратно после чтения DR)
            break;
        }
    }
    send_byte_SPI1(data);
    while(1)
    {
        if(SPI1->SR & SPI_SR_RXNE)  //Если прием байта закончен
        {
            SPI_data_rx[1] = SPI1->DR;    //Копируем даннные в переменную (флаг RXNE сбрасывается аппаратно после чтения DR)
            SPI_data_ready = 1;
            break;
        }
    }
    pin_CSN(HIGH);       //Сигнал выбора
}

void write_register_nRF24_5_byte(uint8_t reg, uint8_t data1, uint8_t data2, uint8_t data3, uint8_t data4, uint8_t data5)
{
    SPI_data_ready = 0;
    spi = 0;

    pin_CSN(LOW);        //Сигнал выбора
    send_byte_SPI1(reg | 0x20);
    while(1)
    {
        if(SPI1->SR & SPI_SR_RXNE)  //Если прием байта закончен
        {
            SPI_data_rx[0] = SPI1->DR;    //Копируем даннные в переменную (флаг RXNE сбрасывается аппаратно после чтения DR)
            break;
        }
    }
    send_byte_SPI1(data1);
    while(1)
    {
        if(SPI1->SR & SPI_SR_RXNE)  //Если прием байта закончен
        {
            SPI_data_rx[1] = SPI1->DR;    //Копируем даннные в переменную (флаг RXNE сбрасывается аппаратно после чтения DR)
            break;
        }
    }
    send_byte_SPI1(data2);
    while(1)
    {
        if(SPI1->SR & SPI_SR_RXNE)  //Если прием байта закончен
        {
            SPI_data_rx[2] = SPI1->DR;    //Копируем даннные в переменную (флаг RXNE сбрасывается аппаратно после чтения DR)
            break;
        }
    }
    send_byte_SPI1(data3);
    while(1)
    {
        if(SPI1->SR & SPI_SR_RXNE)  //Если прием байта закончен
        {
            SPI_data_rx[3] = SPI1->DR;    //Копируем даннные в переменную (флаг RXNE сбрасывается аппаратно после чтения DR)
            break;
        }
    }
    send_byte_SPI1(data4);
    while(1)
    {
        if(SPI1->SR & SPI_SR_RXNE)  //Если прием байта закончен
        {
            SPI_data_rx[4] = SPI1->DR;    //Копируем даннные в переменную (флаг RXNE сбрасывается аппаратно после чтения DR)
            SPI_data_ready = 1;
            break;
        }
    }
    send_byte_SPI1(data5);
    while(1)
    {
        if(SPI1->SR & SPI_SR_RXNE)  //Если прием байта закончен
        {
            SPI_data_rx[5] = SPI1->DR;    //Копируем даннные в переменную (флаг RXNE сбрасывается аппаратно после чтения DR)
            SPI_data_ready = 1;
            break;
        }
    }
    pin_CSN(HIGH);       //Сигнал выбора
}
/*
void read_payload_nRF24(void)
{
    Delay_ms(1);
    pin_CSN(LOW);        //Сигнал выбора
    send_byte_SPI1(0x61);
}
*/
void write_payload_nRF24(uint8_t *TX_data_nRF24, uint8_t size)
{
    pin_CSN(LOW);        //Сигнал выбора
    send_byte_SPI1(0xA0);
    while(1)
    {
        if(SPI1->SR & SPI_SR_RXNE)  //Если прием байта закончен
        {
            SPI_data_rx[0] = SPI1->DR;    //Копируем даннные в переменную (флаг RXNE сбрасывается аппаратно после чтения регистра DR)
            break;
        }
    }
    for(spi = 0; spi < size; spi++)
    {
        send_byte_SPI1(TX_data_nRF24[spi]);
        while(1)
        {
            if(SPI1->SR & SPI_SR_RXNE)  //Если прием байта закончен
            {
                SPI_data_rx[0] = SPI1->DR;    //Копируем даннные в переменную (флаг RXNE сбрасывается аппаратно после чтения DR)
                break;
            }
        }
    }
    pin_CSN(HIGH);       //Сигнал выбора
}

void flush_TX(void)
{
    pin_CSN(LOW);        //Сигнал выбора
    send_byte_SPI1(0xE1);
    while(1)
    {
        if(SPI1->SR & SPI_SR_RXNE)  //Если прием байта закончен
        {
            SPI_data_rx[0] = SPI1->DR;    //Копируем даннные в ячейку массива (флаг RXNE сбрасывается аппаратно после чтения DR)
            SPI_data_ready = 1;
            break;
        }
    }
    pin_CSN(HIGH);       //Сигнал выбора
}

void flush_RX(void)
{
    pin_CSN(LOW);        //Сигнал выбора
    send_byte_SPI1(0xE2);
    while(1)
    {
        if(SPI1->SR & SPI_SR_RXNE)  //Если прием байта закончен
        {
            SPI_data_rx[0] = SPI1->DR;    //Копируем даннные в ячейку массива (флаг RXNE сбрасывается аппаратно после чтения DR)
            SPI_data_ready = 1;
            break;
        }
    }
    pin_CSN(HIGH);       //Сигнал выбора
}

void reuse_TX_PL(void)
{
    pin_CSN(LOW);        //Сигнал выбора
    send_byte_SPI1(0xE3);
    while(1)
    {
        if(SPI1->SR & SPI_SR_RXNE)  //Если прием байта закончен
        {
            SPI_data_rx[0] = SPI1->DR;    //Копируем даннные в ячейку массива (флаг RXNE сбрасывается аппаратно после чтения DR)
            SPI_data_ready = 1;
            break;
        }
    }
    pin_CSN(HIGH);       //Сигнал выбора
}

void read_status_nRF24(void)
{
    spi_size = 1;
    SPI_data_ready = 0;
    spi = 0;

    pin_CSN(LOW);        //Сигнал выбора
    send_byte_SPI1(0xFF);
    while(1)
    {
        if(SPI1->SR & SPI_SR_RXNE)  //Если прием байта закончен
        {
            SPI_data_rx[spi] = SPI1->DR;    //Копируем даннные в ячейку массива (флаг RXNE сбрасывается аппаратно после чтения DR)
            spi++;
            if(spi == spi_size){SPI_data_ready = 1; break;}     //
        }
    }
    pin_CSN(HIGH);       //Сигнал выбора
}

void data_pipe(uint8_t n_pipe, uint8_t state)
{
    read_register_nRF24(EN_RXADDR);
    temp_nRF = SPI_data_rx[1];
    if(state == 1){temp_nRF = temp_nRF | (1 << n_pipe);}
    else{temp_nRF = temp_nRF & (~(1 << n_pipe));}
    write_register_nRF24(EN_RXADDR, temp_nRF);
}

void PWR_UP(uint8_t state)
{
    read_register_nRF24(CONFIG);
    temp_nRF = SPI_data_rx[1];
    if(state == 1){temp_nRF = temp_nRF | 0b00000010;}
    else{temp_nRF = temp_nRF & 0x11111101;}
    write_register_nRF24(CONFIG, temp_nRF);
}

void PRIM_RX(uint8_t state)
{
    read_register_nRF24(CONFIG);
    temp_nRF = SPI_data_rx[1];
    if(state == 1){temp_nRF = temp_nRF | 0b00000001;}
    else{temp_nRF = temp_nRF & 0x11111110;}
    write_register_nRF24(CONFIG, temp_nRF);
}

void EN_CRC(uint8_t state)
{
    read_register_nRF24(CONFIG);
    temp_nRF = SPI_data_rx[1];
    if(state == 1){temp_nRF = temp_nRF | 0b00001000;}
    else{temp_nRF = temp_nRF & 0x11110111;}
    write_register_nRF24(CONFIG, temp_nRF);
}

void settings_TX_mode(uint8_t data1, uint8_t data2, uint8_t data3, uint8_t data4, uint8_t data5, uint8_t channel)
{
    pin_CE(LOW);    //Перевод модуля в режим передачи
    Delay_ms(5);
    PRIM_RX(Off);   //Перевод модуля в режим передачи
    Delay_ms(5);
    PWR_UP(On);     //Включение модуля
    Delay_ms(5);
    EN_CRC(On);
    write_register_nRF24_5_byte(TX_ADDR, data1, data2, data3, data4, data5);
    write_register_nRF24_5_byte(RX_ADDR_P0, data1, data2, data3, data4, data5);
    write_register_nRF24(RF_CH, channel);
    flush_TX();
}