#include "stm32l051xx.h"

uint8_t Hours;
uint8_t Minuts;
uint8_t Seconds;
uint32_t HT;
uint32_t HU;
uint32_t MNT;
uint32_t MNU;
uint32_t ST;
uint32_t SU;

void RTC_Init(void);

void RTC_Read_Time(void);