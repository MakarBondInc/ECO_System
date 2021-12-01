#include "Debug.h"

void Debug(void)
{
    //TODO Инициализация светодиодной индикации:
        //Светодиод на плате Nucleo
    GPIOA->MODER &=~ GPIO_MODER_MODE5;
    GPIOA->MODER |= GPIO_MODER_MODE5_0;
    GPIOA->OTYPER &=~ GPIO_OTYPER_OT_5;
    //GPIOA->OSPEEDR |= GPIO_OSPEEDER_OSPEED5_1 | GPIO_OSPEEDER_OSPEED5_0;
}

void Debug_LED(uint8_t state)
{
    //if(state == 1){GPIOA->BSRR |= GPIO_BSRR_BS_5;}
    //else{GPIOA->BSRR |= GPIO_BSRR_BR_5;}
}