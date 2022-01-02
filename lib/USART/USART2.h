#include "stm32l051xx.h"    //Файл описания регистров микроконтроллера

#define Fcpu_USART2 16000000    //Частота тактирования
#define speed_USART2 9600      //Бод
#define BRR_speed_USART2 Fcpu_USART2/speed_USART2

uint32_t stringUART2_TX[30];
uint8_t i;

void init_USART2(void);
void UART2_send_string(char *stringUART2_TX);
void UART2_send_byte(uint32_t data);