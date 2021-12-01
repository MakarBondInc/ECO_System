#include "stm32l051xx.h"
#include "SPI.h"
#include "Delay.h"

#define HIGH 1
#define LOW 0

#define On 1
#define Off 0

//Карта регистров nRF24l01
#define CONFIG 0x00
#define EN_AA 0x01
#define EN_RXADDR 0x02
#define SETUP_AW 0x03
#define SETUP_RETR 0x04
#define RF_CH 0x05
#define RF_SETUP 0x06
#define STATUS 0x07
#define OBSERVE_TX 0x08
#define CD 0x09
#define RX_ADDR_P0 0x0A
#define RX_ADDR_P1 0x0B
#define RX_ADDR_P2 0x0C
#define RX_ADDR_P3 0x0D
#define RX_ADDR_P4 0x0E
#define RX_ADDR_P5 0x0F
#define TX_ADDR 0x10
#define RX_PW_P0 0x11
#define RX_PW_P1 0x12
#define RX_PW_P2 0x13
#define RX_PW_P3 0x14
#define RX_PW_P4 0x15
#define RX_PW_P5 0x16
#define FIFO_STATUS 0x17

//Битовые маски прерываний в регистре STATUS
#define RX_DR 0x40
#define TX_DS 0x20
#define MAX_RT 0x10

//Массивы для приема и передачи данных
uint8_t TX_data_nRF24[32];
uint8_t RX_data_nRF24[33];

uint8_t temp_nRF;

void pin_CSN(uint8_t state);    //Управления сигналом выбора устройства на шине SPI

void pin_CE(uint8_t state);     //Управления сигналом выбора режима работы nRF24l01+

void read_register_nRF24(uint8_t reg);  //Чтение произвольного регистра с адресом "reg"

void read_register_nRF24_5_byte(uint8_t reg);   //Чтение произвольного 5-и байтового регистра с адресом "reg"

void write_register_nRF24(uint8_t reg, uint8_t data); //Запись в произвольный регистр с адресом "reg" и данными "data"

void write_register_nRF24_5_byte(uint8_t reg, uint8_t data1, uint8_t data2, uint8_t data3, uint8_t data4, uint8_t data5); //Запись в произвольный 5-и байтовый регистр с адресом "reg" и данными "data1, ..."

//void read_payload_nRF24(void);  //Чтение FIFO буфера

void write_payload_nRF24(uint8_t *TX_data_nRF24, uint8_t size); //Запись в буфер FIFO из массива "TX_data_nRF24" с числом элементов "size"

void flush_TX(void);    //Очистка передающего FIFO буфера

void flush_RX(void);    //Очистка приемного FIFO буфера 

void reuse_TX_PL(void);     //Повторное использование передающего буфера

void read_status_nRF24(void);   //Чтение статусного регистра

void transmit_data(void);   

void receive_data(void);

void data_pipe(uint8_t n_pipe, uint8_t state);   //Включение/отключение передающей трубы с номером "n_pipe"

void PWR_UP(uint8_t state);     //Вывод модуля из спящего режима 

void PRIM_RX(uint8_t state);    //Переключение модуля в режим приема

void EN_CRC(uint8_t state);    //

void settings_TX_mode(uint8_t data1, uint8_t data2, uint8_t data3, uint8_t data4, uint8_t data5, uint8_t channel); //Настройка адреса передатчика и номера канала
