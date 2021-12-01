#include "stm32l051xx.h"    //Файл описания регистров микроконтроллера

#define Fcpu_USART1 2*2000000   //Частота тактирования
#define speed_USART1 9600      //Бод
#define BRR_speed_USART1 Fcpu_USART1/speed_USART1

uint32_t stringUART1_TX[30];
uint8_t j;

void init_USART1(void);

void send_string_UART1(char *stringUART1_TX);