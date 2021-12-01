#include "Delay.h"

void Delay_ms(int time)
{
    SysTick->LOAD = (time * SYSTICK_CONST_ms) - 1;  //Загрузка значения перезагрузки
    SysTick->VAL = (time * SYSTICK_CONST_ms) - 1;   //Загрузка текущего значения
    SysTick->CTRL |= SysTick_CTRL_ENABLE_Msk;       //Запуск системного таймера
    __ASM("WFI");
}
void Delay_us(int time)
{
    SysTick->LOAD = (time * SYSTICK_CONST_us) - 1;  //Загрузка значения перезагрузки
    SysTick->VAL = (time * SYSTICK_CONST_us) - 1;   //Загрузка текущего значения
    SysTick->CTRL |= SysTick_CTRL_ENABLE_Msk;       //Запуск системного таймера
    __ASM("WFI");
}
void init_SysTick(void)
{
    SysTick->CTRL |= SysTick_CTRL_CLKSOURCE_Msk;        //Тактирование от ядра
    SysTick->CTRL |= SysTick_CTRL_TICKINT_Msk;          //Разрешение исключения при сбросе счетчика
}