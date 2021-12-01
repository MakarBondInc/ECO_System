#include "GSM.h"
#include "Delay.h"

void power_M66(uint8_t state)
{
    if(state == 1)
    {
        SysTick_ms(100);
        GPIOC->BSRR |= GPIO_BSRR_BS_3;
        SysTick_ms(2000);
        GPIOC->BSRR |= GPIO_BSRR_BR_3;
    }
    else
    {
        GPIOC->BSRR |= GPIO_BSRR_BS_3;
        SysTick_ms(800);
        GPIOC->BSRR |= GPIO_BSRR_BR_3;
    }
}

void sleepMode_M66(uint8_t state)
{
    if(state == 1){send_string_UART1("AT+QSCLK=1\n");}
    else{send_string_UART1("AT+QSCLK=0\n");}
}