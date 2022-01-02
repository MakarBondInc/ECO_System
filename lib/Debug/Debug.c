#include "Debug.h"

void LED_1(uint8_t led1)
{
    if(led1){GPIOA->BSRR |= GPIO_BSRR_BS_11;}
    else{GPIOA->BSRR |= GPIO_BSRR_BR_11;}
}
void LED_2(uint8_t led2)
{
    if(led2){GPIOA->BSRR |= GPIO_BSRR_BS_12;}
    else{GPIOA->BSRR |= GPIO_BSRR_BR_12;}
}
void LED_3(uint8_t led3)
{
    if(led3){GPIOB->BSRR |= GPIO_BSRR_BS_5;}
    else{GPIOB->BSRR |= GPIO_BSRR_BR_5;}
}