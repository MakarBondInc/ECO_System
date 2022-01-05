#include "ADC.h"
void Init_ADC(void)
{
    F_EOCAL = 0;    //Сброс флага окончания процедуры калибровки АЦП.
    F_ADRDY = 0;    //Сброс флага готовности АЦП.
    F_Data = 0;     //Сброс флага завершения преобразования.

    RCC->APB2ENR |= RCC_APB2ENR_ADCEN;  //Включение тактирования блока АЦП.

    ADC1->CFGR2 |= ADC_CFGR2_CKMODE_0 | ADC_CFGR2_CKMODE_1; //Тактирование от шины PCLK (Synchronous clock mode).

    //ADC1->CFGR1 |= ADC_CFGR1_AUTOFF;
    ADC1->SMPR |= ADC_SMPR_SMP_0 | ADC_SMPR_SMP_1 | ADC_SMPR_SMP_2; //160.5 циклов

    ADC1->IER |= ADC_IER_EOCALIE;   //Разрешить прерывание по завершению калибровки.

    ADC1->IER |= ADC_IER_ADRDYIE;   //Разрешить прерывание по готовности АЦП для преобразования.

    ADC1->IER |= ADC_IER_EOCIE;     //Разрешить прерывание по завершению преобразования.
    
    //ADC1->SMPR |= ADC_SMPR_SMP_0 | ADC_SMPR_SMP_1 | ADC_SMPR_SMP_2;
}
void Calib_ADC(void)
{
    ADC1->CR |= ADC_CR_ADCAL;   //Запуск процедуры калибровки (регулятора напряжения запускается автоматически)

    __ASM("WFI");
    while(F_EOCAL == 0){;}
    F_EOCAL = 0;    //Сброс флага окончания процедуры калибровки АЦП.
}
uint32_t Alone_convert_ADC(void)
{
    ADC1->CR |= ADC_CR_ADSTART; //Запуск преобразования.
    __ASM("WFI");
    while(F_Data == 0){;}
    F_Data = 0;     //Сброс флага завершения преобразования.
    Delay_us(50);
    ADC1->CR |= ADC_CR_ADSTART; //Запуск преобразования.
    __ASM("WFI");
    while(F_Data == 0){;}
    F_Data = 0;     //Сброс флага завершения преобразования.

    return ADC_data;
}

void Connect_IN_17_ADC(void)
{
    ADC->CCR |= ADC_CCR_VREFEN;             //Включение источника опорного напряжения (ИОН).
    ADC1->CHSELR |= ADC_CHSELR_CHSEL17;     //Подключение ИОН к 17-му входу АЦП.
}

void Connect_IN_18_ADC(void)
{
    ADC->CCR |= ADC_CCR_VREFEN;           //Включение источника опорного напряжения (ИОН).
    ADC->CCR |= ADC_CCR_TSEN;             //Включение сенсора температуры.
    ADC1->CHSELR |= ADC_CHSELR_CHSEL18;   //Подключение сенсора температуры к АЦП.
}

void Connect_IN_4_ADC(void)
{
    ADC1->CHSELR |= ADC_CHSELR_CHSEL4;   //Подключение
}

void Connect_IN_9_ADC(void)
{
    ADC1->CHSELR &=~ ADC_CHSELR_CHSEL;
    ADC1->CHSELR |= ADC_CHSELR_CHSEL9;   //Подключение
}

void En_ADC(void)
{
    ADC1->ISR |= ADC_ISR_ADRDY;     //Сброс флага готовности АЦП.
    
    ADC1->CR |= ADC_CR_ADEN;    //Запуск АЦП.

    __ASM("WFI");
    while(F_ADRDY == 0){;}
    F_ADRDY = 0;    //Сброс флага готовности АЦП.
}

uint32_t Read_Vref(void)
{
    Vref = 0;
    ADC1->CHSELR &=~ ADC_CHSELR_CHSEL;
    Connect_IN_17_ADC();
    Vref = ((*VREFINT_CAL * 3000) / Alone_convert_ADC());
    return Vref;
}

uint32_t Read_IN4(void)
{
    Vm = 0;
    Vr = Read_Vref();
    ADC1->CHSELR &=~ ADC_CHSELR_CHSEL;
    Connect_IN_4_ADC();
    Vm = (Vr * Alone_convert_ADC()) / 4095;
    return Vm;
}

uint32_t Read_IN9(void)
{
    Vm = 0;
    Vr = Read_Vref();
    ADC1->CHSELR &=~ ADC_CHSELR_CHSEL;
    Connect_IN_9_ADC();
    Vm = (Vr * Alone_convert_ADC()) / 4095;
    return Vm;
}

uint32_t Read_Temper(void)
{
    Temper = 0;
    Temper_norm = 0;

    Temper_norm = Read_Vref();
    
    ADC1->CHSELR &=~ ADC_CHSELR_CHSEL;
    Connect_IN_18_ADC();
    Delay_us(50);
    Temper_norm = Alone_convert_ADC();
    Delay_us(50);
    Temper_norm = Alone_convert_ADC();
/*
    Temper = 130 - 30;
    Temper = Temper * (Temper_norm - (*TEMP_CAL1_ADDR));
    Temper = Temper / (*TEMP_CAL2_ADDR - *TEMP_CAL1_ADDR);
    Temper = Temper + 30;
*/
    return Temper_norm;
}