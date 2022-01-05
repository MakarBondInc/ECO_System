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

void dataTranslate(uint8_t data);
void send_to_PC(uint8_t *SPI_data_rx, uint8_t w);
char convert_data_US(char data);

uint8_t curs;
uint8_t temp;
uint8_t k;
uint8_t y;
uint8_t q;

uint8_t SPI_data_sent;
uint8_t IRQ_nRF;
uint8_t data_SPI;

uint8_t IRQ_Data_Ready_nRF24;
uint8_t IRQ_Sent_Ready_nRF24;
uint8_t IRQ_Maximum_number_of_TX;
uint8_t impulse = 0;

char d_US;
char e_US;



int main(void)
{
    SPI_data_ready = 0;
    SPI_data_sent = 0;

    IRQ_Data_Ready_nRF24 = 0;
    IRQ_Sent_Ready_nRF24 = 0;
    IRQ_Maximum_number_of_TX = 0;

    spi = 0;

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
    Delay_ms(1000);

    Init_ADC();
    Calib_ADC();
    En_ADC();

    Delay_ms(1000);
    GPIOB->BSRR |= GPIO_BSRR_BR_6;
    while(1)
    {
        GPIOB->BSRR |= GPIO_BSRR_BS_6;
        Delay_ms(50);
        LPUART1_read_string();
        LED_1(On);
        GPIOB->BSRR |= GPIO_BSRR_BR_6;

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
    void ADC1_COMP_IRQHandler()
    {
        if(ADC1->ISR & ADC_ISR_EOCAL){ADC1->ISR |= ADC_ISR_EOCAL; F_EOCAL = true;}
        if(ADC1->ISR & ADC_ISR_ADRDY){ADC1->ISR |= ADC_ISR_ADRDY; F_ADRDY = true;}
        if(ADC1->ISR & ADC_ISR_EOC){ADC_data = ADC1->DR; F_Data = true;}
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
    void IK_read(void)
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