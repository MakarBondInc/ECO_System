#include "stm32l051xx.h"    //Файл описания регистров микроконтроллера

uint8_t SPI_data_rx[33];
uint8_t spi;
uint8_t spi_size;
uint8_t SPI_data_ready;

void init_SPI1(void);   //Процедура инициализации модуля SPI

void send_byte_SPI1(uint8_t data); //Функция отправки байта