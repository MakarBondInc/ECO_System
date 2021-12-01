#include "stm32l051xx.h"

#define F_SYSCLK 16000000               //Частота работы системной шины SYSCLK
#define SYSTICK_CONST_ms F_SYSCLK/1000
#define SYSTICK_CONST_us F_SYSCLK/1000000

void Delay_ms(int time);    //Задержка в миллисекундах с использованием системного таймера

void Delay_us(int time);

void init_SysTick(void);