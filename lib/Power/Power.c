#include "Power.h"

void Power_4V(uint8_t p4v)
{
    if(p4v){GPIOA->BSRR |= GPIO_BSRR_BS_8;}
    else{GPIOA->BSRR |= GPIO_BSRR_BR_8;}
}
void Power_5V(uint8_t p5v)
{
    if(p5v){GPIOB->BSRR |= GPIO_BSRR_BS_15;}
    else{GPIOB->BSRR |= GPIO_BSRR_BR_15;}
}