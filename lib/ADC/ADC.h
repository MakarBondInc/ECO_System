#include "stm32l051xx.h"
#include "Delay.h"

#define TEMP_CAL2_ADDR ((uint16_t*) ((uint32_t) 0x1FF8007E))
#define TEMP_CAL1_ADDR ((uint16_t*) ((uint32_t) 0x1FF8007A))
#define VREFINT_CAL ((uint16_t*) ((uint32_t) 0x1FF80078))

uint8_t F_EOCAL;    //Флаг окончания процедуры калибровки АЦП.
uint8_t F_ADRDY;    //Флаг готовности АЦП.
uint8_t F_Data;     //Флаг завершения преобразования.

uint32_t ADC_data;
uint32_t Vref;
uint32_t Vm;
uint32_t Vr;
uint32_t Temper;
uint32_t Temper_norm;

void Init_ADC(void);
void Calib_ADC(void);
uint32_t Alone_convert_ADC(void);
void Connect_IN_17_ADC(void);
void Connect_IN_18_ADC(void);
void En_ADC(void);
uint32_t Read_Vref(void);
uint32_t Read_Temper(void);
void Connect_IN_4_ADC(void);
uint32_t Read_IN4(void);
/*
    *Доступные входы АЦП
        ADC_IN4 PA4 (Fast Channel)
        ADC_IN8 PB0
        ADC_IN9 PB1
    
    *АЦП имеет встроенный внутренний регулятор напряжения.
    *Перед использованием АЦП его необходимо запустить и дождаться запуска.
    !Перед переходом в режим сна, регулятор необходимо выключить! 
    !Раздел: (Analog reference for the ADC internal voltage regulator).
    *Запуск регулятора напряжения АЦП можно выполнить тремя способами:
        1) Установить ADVREGEN = 1;
        2) Запустить колибровки АЦП ADCAL = 1 (Бит ADVREGEN уст. автоматически);
        3) Включить АЦП ADEN = 1.
    *Отключение регулятора напряжения АЦП:
        1) Убедиться, что ADEN = 1;
        2) Установить ADVREGEN = 0.

    *Калибровка:
        Для запуска калибровки ADCAL = 1. //! Только при отключённом АЦП (когда ADEN = 0).
        Если ADCAL = 1, идет процесс калибровки, иначе калибровка закончена.
    *Во время калибровки АЦП вычисляет коэффициент, который используется для устранения ошибки смещения.
        Коэффициент хранится в регистре ADC_DR (биты с 6 по 0) или ADC_CALFACT.
        Коэффициент сохраняется в режимах LPRun, LPSleep и Stop.
    !Процедуру калибровки необходимо производить после каждого отключения питания.
    !В случаях Standby mode, ADC peripheral is reset значение коэффициента теряется.
    *Калибровочный коэффициент можно записывать в регистр ADC_DR из памяти, чтобы сэкономить время на калибровке.
        !Запись производить при включ. АЦП но не осуществ. преобразование (ADEN = 1 и ADSTART = 0).
    *Запуск процедуры калибровки:
        1) Уделиться, что ADEN = 0 и DMAEN = 0;
        2) Установить ADCAL = 1;
        3) Подождать пока ADCAL = 0 после того, как EOCAL будет установлен в 1. (Прерывание вкл. EOCALIE в рег. ADC_IER);
        4) После чего включить АЦП ADEN = 1.
    *Запуск АЦП:
        1) Установить ADEN = 1 и дождаться установки ADRDY. (Регулятор автом. включается.)
    *Процедура запуска АЦП:
        1) Сбросить ADRDY в рег. ADC_ISR (записью 1);
        2) Установить ADEN = 1 в регистре ADC_CR.
        3) Подождать пока ADRDY = 1 в регистре ADC_ISR. (Преерывание вкл. ADRDYIE в рег. ADC_IER);
    *Отключение или перевод в режим пониж. энерг. АЦП:
        1) ADDIS = 1, биты ADEN и ADDIS сбрасываются аппаратно.
    *Начало преобразования:
        1) ADSTART или про помощи внешнего триггера. (см. Раздел 13.4).
    *Процедура отключения АЦП:
        1) Уделиться, что ADSTART = 0 в регистре ADC_CR. (Что процедура преобразования не выпол.);
        2) Если требуется остановить преобр. установите ADSTP = 1 в регистре ADC_CR;
        3) Установить ADDIS = 1 в регистре ADC_CR;
        4) Если требуется подождите пока ADEN = 0 в регистре ADC_CR (значит АЦП полность отключ.)
            (ADDIS автоматически сбрасывается, как только ADEN = 0);
        5) Сбросить бит ADRDY в регистре ADC_ISR. (записью 1).
    !Max_f_ADC = 16 МГц (Table 58. ADC characteristics)
    *Выбор источника тактирования:
        1) HSI16 MHz clock
        2) PCLK
        (Table 59. Latency between trigger and start of conversion)
*/