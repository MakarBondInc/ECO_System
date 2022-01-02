#include "RTC.h"
void RTC_Init(void)
{
    RCC->APB1ENR |= RCC_APB1ENR_PWREN;  //Разрешаем тактирование модуля PWR
    PWR->CR |= PWR_CR_DBP;  //Снимаем защиту от записи

    RCC->CSR |= RCC_CSR_LSEON;  //Запуск генератора LSE
    //RCC->CSR |= RCC_CSR_LSEDRV_1 | RCC_CSR_LSEDRV_0;   //Driving capability
    while(!((RCC->CSR & RCC_CSR_LSERDY) == RCC_CSR_LSERDY)){;}
    RCC->CSR |= RCC_CSR_RTCSEL_0;   //Тактирование от генератора LSE
    RCC->CSR |= RCC_CSR_LSECSSON;

    RCC->CSR |= RCC_CSR_RTCEN;
}
void RTC_Read_Time(void)
{
    HT = ((RTC->TR & RTC_TR_HT) >> 20);
    HU = ((RTC->TR & RTC_TR_HU) >> 16);
    MNT = ((RTC->TR & RTC_TR_MNT) >> 12);
    MNU = ((RTC->TR & RTC_TR_MNU) >> 8);
    ST = ((RTC->TR & RTC_TR_ST) >> 4);
    SU = RTC->TR & RTC_TR_SU;

    Hours = (HT * 10) + HU;
    Minuts = (MNT * 10) + MNU;
    Seconds = (ST * 10) + SU;
}
