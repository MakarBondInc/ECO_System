#include "Power.h"

void Power_5V(uint8_t p5v)
{
    if(p5v){GPIOB->BSRR |= GPIO_BSRR_BS_15;}
    else{GPIOB->BSRR |= GPIO_BSRR_BR_15;}
}