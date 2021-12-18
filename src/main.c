#include "stm32l051xx.h"
#include "Delay.h"
#include "RCC.h"
#include "GPIO.h"
#include "Debug.h"
#include "USART2.h"
#include "LPUSART1.h"
#include "nRF.h"
#include "SPI.h"
#include "AXL.h"
#include "ADC.h"

#define On 1
#define Off 0
#define HIGH 1
#define LOW 0
#define true 1
#define false 0

void dataTranslate(uint8_t data);
void send_to_PC(uint8_t *SPI_data_rx, uint8_t w);
char convert_data_US(char data);
void Send_Vref_to_PC(uint32_t data);
void Send_Temper_to_PC(uint32_t data);
void Send_data_to_PC(uint32_t data);
volatile uint16_t abs_f(int16_t data);

uint8_t curs;
uint8_t temp;
uint8_t k;
uint8_t y;
uint8_t q;
uint8_t p;
uint8_t i;

uint8_t SPI_data_sent;
uint8_t IRQ_nRF;
uint8_t data_SPI;

uint8_t IRQ_Data_Ready_nRF24;
uint8_t IRQ_Sent_Ready_nRF24;
uint8_t IRQ_Maximum_number_of_TX;
uint8_t impulse = 0;

uint8_t dt_data;
uint8_t t_data;
uint8_t s_data;
uint8_t d_data;
uint8_t e_data;

volatile uint32_t ADC_tem;
uint32_t ADC_Vref;
uint32_t ADC_Vm;
uint32_t ADC_V;
uint32_t mean;

uint16_t mod;
int16_t sum_0;
int16_t sum_1;
int16_t res;

char d_US;
char e_US;
/*
uint16_t table[131] = {2921, 2777, 2647, 2529, 2420, 2320, 2229, 2143, 2065, 1991, 1923, 1859, 1799, 1743, 1690, 
                        1640, 1593, 1549, 1507, 1467, 1429, 1393, 1359, 1327, 1296, 1266, 1238, 1210, 1184, 1159, 
                        1136, 1113, 1091, 1069, 1049, 1029, 1010, 992, 974, 957, 941, 925, 910, 895, 880, 866, 853, 
                        840, 827, 815, 803, 791, 780, 769, 758, 748, 737, 728, 718, 709, 699, 691, 682, 673, 665, 657, 
                        649, 642, 634, 627, 619, 612, 606, 599, 592, 586, 580, 573, 567, 562, 556, 550, 544, 539, 534, 
                        528, 523, 518, 513, 508, 504, 499, 494, 490, 485, 481, 477, 473, 469, 464, 460, 457, 453, 449, 
                        445, 442, 438, 434, 431, 427, 424, 421, 417, 414, 411, 408, 405, 402, 399, 396, 393, 390, 387, 
                        384, 382, 379, 376, 374, 371, 368, 366};
                        */
uint16_t table[131] = {2497, 2443, 2390, 2336, 2283, 2229, 2175, 2122, 2068, 2015, 
                        2043, 1971, 1904, 1841, 1782, 1726, 1674, 1624, 1578, 1533, 1491, 1452, 1414, 1378, 1344, 1311, 
                        1280, 1250, 1222, 1195, 1168, 1143, 1119, 1096, 1074, 1053, 1032, 1012, 993, 975, 957, 940, 923, 
                        907, 892, 877, 862, 848, 835, 821, 809, 796, 784, 772, 761, 750, 739, 729, 718, 708, 699, 689, 680, 
                        671, 662, 654, 646, 637, 630, 622, 614, 607, 600, 593, 586, 579, 572, 566, 560, 553, 547, 541, 536, 
                        530, 524, 519, 514, 508, 503, 498, 493, 488, 484, 479, 474, 470, 465, 461, 457, 453, 448, 444, 440, 
                        436, 433, 429, 425, 421, 418, 414, 411, 407, 403, 399, 395, 391, 387, 383, 379, 375, 371, 367, 363, 
                        359, 355, 351, 347, 343, 338, 334, 330};
int main(void)
{
    sum_0 = 0;
    sum_1 = 0;

    SPI_data_ready = 0;
    SPI_data_sent = 0;

    IRQ_Data_Ready_nRF24 = 0;
    IRQ_Sent_Ready_nRF24 = 0;
    IRQ_Maximum_number_of_TX = 0;

    spi = 0;

    ADC_tem = 0;
    HSE_16MHz();    //Переключение тактировния на генератор HSE с частотой 16 МГц.
    GPIO_Init();

    AXL_CS(HIGH);
    
    NVIC_EnableIRQ(EXTI0_1_IRQn);
    NVIC_SetPriority(EXTI0_1_IRQn, 0);

    NVIC_EnableIRQ(SPI2_IRQn);
    NVIC_SetPriority(SPI2_IRQn, 1);

    NVIC_EnableIRQ(ADC1_COMP_IRQn);
    NVIC_SetPriority(ADC1_COMP_IRQn, 1);

    init_SysTick();     //Инициализация системного таймера

    init_LPUSART1();      //Инициализация USART1 для связи с GSM
    init_USART2();      //Инициализация USART2 для связи с ПК
    init_SPI1();

    pin_CSN(HIGH);        //Сигнал выбора
    pin_CE(LOW);

    Init_ADC();
    Calib_ADC();
    En_ADC();

        while(1)
        {
            mean = 0;
            for(i = 0; i < 5; i++)
            {
                mean = mean + Read_IN4();
            }
            mean = mean / i;

            if(mean < 2497 && mean > 330)
            {
                for(i = 0; i < 131; i++)
                {
                    sum_0 = table[i] - mean;
                    if(sum_0 > 0)
                    {
                        sum_1 = sum_0;
                    }
                    if(sum_0 < 0)
                    {
                        sum_0 = sum_0 * (-1);
                        if(sum_0 < sum_1){p = i;break;}
                        else{p = i - 1; break;}
                    }
                }
                p = p + 20;
            }
            else{p = 0;}

            UART2_send_string("\n");
            UART2_send_string("\n");
            Send_data_to_PC(p);
            //Send_data_to_PC(mean);
            Delay_ms(500);
        }

    Delay_ms(1000);
    GPIOC->BSRR |= GPIO_BSRR_BR_13;
    while(1)
    {
        GPIOC->BSRR |= GPIO_BSRR_BS_13;
        Delay_ms(50);
        LPUART1_read_string();
        GPIOC->BSRR |= GPIO_BSRR_BR_13;

        d_US = convert_data_US(stringLPUART1_RX[2]) + 10;
        e_US = convert_data_US(stringLPUART1_RX[3]);

        UART2_send_string("\n");
        UART2_send_byte((d_US - 10) + 0x30);
        UART2_send_byte(e_US + 0x30);
        Delay_ms(100);
    }
    
    read_register_nRF24(CONFIG);
    UART2_send_string("\nCONFIG: ");
    send_to_PC(SPI_data_rx, 2);
    settings_TX_mode(0x78, 0x78, 0x78, 0x78, 0x78, 110);
    read_register_nRF24(CONFIG);
    UART2_send_string("\nCONFIG: ");
    send_to_PC(SPI_data_rx, 2);

    for(q = 0; q < 32; q++)
    {
        TX_data_nRF24[q] = 0b00110010;
    }
    write_payload_nRF24(TX_data_nRF24, 32);
    //send_to_PC(SPI_data_rx, 32);
/*
    read_status_nRF24();
    UART2_send_string("\nSTATUS: ");
    send_to_PC(SPI_data_rx, 1);
*/
    pin_CE(HIGH);
    Delay_us(50);
    pin_CE(LOW);
    
    __ASM("WFI");
    while (1)
    {
        if(IRQ_nRF)
        {
            //Debug_LED(On);
            read_status_nRF24();
            UART2_send_string("\nSTATUS: ");
            send_to_PC(SPI_data_rx, 1);
            //Delay_ms(10); 

                temp = SPI_data_rx[0] & RX_DR; 
                if(temp == RX_DR){IRQ_Data_Ready_nRF24 = 1; write_register_nRF24(STATUS, SPI_data_rx[0]);}
                else{IRQ_Data_Ready_nRF24 = 0;}

                temp = SPI_data_rx[0] & TX_DS; 
                if(temp == TX_DS){IRQ_Sent_Ready_nRF24 = 1; UART2_send_string("\nОтправлено!"); write_register_nRF24(STATUS, SPI_data_rx[0]); NVIC_SystemReset();}
                else{IRQ_Sent_Ready_nRF24 = 0;}

                temp = SPI_data_rx[0] & MAX_RT; 
                if(temp == MAX_RT){IRQ_Maximum_number_of_TX = 1; UART2_send_string("\nПревышено число попыток!"); write_register_nRF24(STATUS, SPI_data_rx[0]); NVIC_SystemReset();}
                else{IRQ_Maximum_number_of_TX = 0;}

            IRQ_nRF = 0;
        }
    }
}
    void ADC1_COMP_IRQHandler()
    {
        if(ADC1->ISR & ADC_ISR_EOCAL){ADC1->ISR |= ADC_ISR_EOCAL; F_EOCAL = true;}
        if(ADC1->ISR & ADC_ISR_ADRDY){ADC1->ISR |= ADC_ISR_ADRDY; F_ADRDY = true;}
        if(ADC1->ISR & ADC_ISR_EOC){ADC_data = ADC1->DR; F_Data = true;}
    }
    void EXTI0_1_IRQHandler()
    {
        IRQ_nRF = 1;                //Поднамаем флаг внешнего прерывания, требуется обработать статусный регистр nRF
        EXTI->PR |= EXTI_PR_PIF0;   //Сброс флага прерывания на лиции PA0
    }
//Обработчик исключения от системного таймера.
    void SysTick_Handler()
    {
        SysTick->CTRL &=~ SysTick_CTRL_ENABLE_Msk;     //Остановка системного таймера
    }
    void dataTranslate(uint8_t data)    //Копирование байта данных из SPI в UART
    {
        for(k = 0; k < 8; k++)
        {
            curs = 128 >> k;
            temp = data;
            temp = temp & curs;
            if(temp == curs){UART2_send_string("1");}
            else{UART2_send_string("0");}
        }
        UART2_send_string(" ");
    }
    void send_to_PC(uint8_t *SPI_data_rx, uint8_t w)
    {
        for(y = 0; y < w; y++)
        {
            dataTranslate(SPI_data_rx[y]);
        }
    }
    char convert_data_US(char data)
    {
        char rez;
        if(data == 0x30){rez = 0;}
        if(data == 0x31){rez = 1;}
        if(data == 0x32){rez = 2;}
        if(data == 0x33){rez = 3;}
        if(data == 0x34){rez = 4;}
        if(data == 0x35){rez = 5;}
        if(data == 0x36){rez = 6;}
        if(data == 0x37){rez = 7;}
        if(data == 0x38){rez = 8;}
        if(data == 0x39){rez = 9;}
        return rez;
    }
    void Send_Vref_to_PC(uint32_t data)
    {
        dt_data = (data / 10000) + 0x30;
        t_data = ((data % 10000) / 1000) + 0x30;
        s_data = ((data % 1000) / 100) + 0x30;
        d_data = ((data % 100) / 10) + 0x30;
        e_data = (data % 10) + 0x30;
        
        //UART2_send_byte(dt_data);
        UART2_send_byte(t_data);
        UART2_send_string(",");
        UART2_send_byte(s_data);
        UART2_send_byte(d_data);
        UART2_send_byte(e_data);
        UART2_send_string(" В");
    }
    void Send_Temper_to_PC(uint32_t data)
    {
        dt_data = (data / 10000) + 0x30;
        t_data = ((data % 10000) / 1000) + 0x30;
        s_data = ((data % 1000) / 100) + 0x30;
        d_data = ((data % 100) / 10) + 0x30;
        e_data = (data % 10) + 0x30;
        UART2_send_byte(dt_data);
        UART2_send_byte(t_data);
        UART2_send_byte(s_data);
        UART2_send_byte(d_data);
        UART2_send_string(",");
        UART2_send_byte(e_data);
    }
    void Send_data_to_PC(uint32_t data)
    {
        dt_data = (data / 10000) + 0x30;
        t_data = ((data % 10000) / 1000) + 0x30;
        s_data = ((data % 1000) / 100) + 0x30;
        d_data = ((data % 100) / 10) + 0x30;
        e_data = (data % 10) + 0x30;
        UART2_send_byte(dt_data);
        UART2_send_byte(t_data);
        UART2_send_byte(s_data);
        UART2_send_byte(d_data);
        UART2_send_byte(e_data);
    }
    