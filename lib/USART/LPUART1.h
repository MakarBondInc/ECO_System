#include "stm32l051xx.h"    //Файл описания регистров микроконтроллера

//#define Fcpu_LPUSART1 16000000   //Частота тактирования
//#define speed_LPUSART1 9600      //Бод
//#define BRR_speed_LPUSART1 (Fcpu_LPUSART1)/speed_LPUSART1

uint32_t stringLPUART1_TX[30];
char stringLPUART1_RX[30];
uint8_t j;
uint8_t error;

void init_LPUSART1(void);

void send_string_LPUART1(char *stringLPUART1_TX);

void LPUART1_read_string(void);