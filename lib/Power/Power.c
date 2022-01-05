#include "Power.h"

void Power_5V(uint8_t p5v)
{
    if(p5v){GPIOB->BSRR |= GPIO_BSRR_BS_15;}
    else{GPIOB->BSRR |= GPIO_BSRR_BR_15;}
}
void Power_US(uint8_t pUS)
{
    if(pUS){GPIOB->BSRR |= GPIO_BSRR_BS_3;}
    else{GPIOB->BSRR |= GPIO_BSRR_BR_3;}
}