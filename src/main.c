#include "stm32l051xx.h"
#include "Delay.h"
#include "RCC.h"
#include "GPIO.h"
#include "Debug.h"
#include "USART2.h"
#include "USART1.h"
#include "nRF.h"
#include "SPI.h"
#include "AXL.h"
#include "RTC.h"

#define On 1
#define Off 0
#define HIGH 1
#define LOW 0

//char data[8];
void dataTranslate(uint8_t data);
void send_to_PC(uint8_t *SPI_data_rx, uint8_t w);

uint8_t curs;
uint8_t temp;
uint8_t k;
uint8_t y;
uint8_t q;

uint8_t IRQ_nRF;
uint8_t IRQ_Data_Ready_nRF24;
uint8_t IRQ_Sent_Ready_nRF24;
uint8_t IRQ_Maximum_number_of_TX;

int main(void)
{
    IRQ_Data_Ready_nRF24 = 0;
    IRQ_Sent_Ready_nRF24 = 0;
    IRQ_Maximum_number_of_TX = 0;
    GPIO_Init();
    HSE_16MHz();    //Переключение тактировния на генератор HSE с частотой 16 МГц.
    init_SPI1();
    PWR_UP(Off);
    Stop_mode();

    init_SysTick();     //Инициализация системного таймера
    init_USART2();      //Инициализация USART2 для связи с ПК
    //Delay_ms(1000);
    

        //AXL_CS(HIGH);
        //read_status_AXL();
        //send_to_PC(SPI_data_rx, 1);

    
    LED_3(On);
    __ASM("NOP");
    while(1){;}
    
    RTC_Init();
    LED_3(On);
    
    
    
    NVIC_EnableIRQ(EXTI0_1_IRQn);
    NVIC_SetPriority(EXTI0_1_IRQn, 0);

    NVIC_EnableIRQ(SPI2_IRQn);
    NVIC_SetPriority(SPI2_IRQn, 1);

    NVIC_EnableIRQ(LPUART1_IRQn);
    NVIC_SetPriority(LPUART1_IRQn, 1);

    //init_USART1();      //Инициализация USART1 для связи с GSM
    
    

    while(1)
    {
        RTC_Read_Time();
        UART2_send_string("\n");
        UART2_send_byte(HT + 0x30);
        UART2_send_byte(HU + 0x30);
        UART2_send_string(":");
        UART2_send_byte(MNT + 0x30);
        UART2_send_byte(MNU + 0x30);
        UART2_send_string(":");
        UART2_send_byte(ST + 0x30);
        UART2_send_byte(SU + 0x30);
        Delay_ms(500);
    }

    pin_CSN(HIGH);        //Сигнал выбора
    pin_CE(LOW);
    Delay_ms(1000);

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
            read_status_nRF24();
            UART2_send_string("\nSTATUS: ");
            send_to_PC(SPI_data_rx, 1);

                temp = SPI_data_rx[0] & RX_DR; 
                if(temp == RX_DR){IRQ_Data_Ready_nRF24 = 1; write_register_nRF24(STATUS, SPI_data_rx[0]);}
                else{IRQ_Data_Ready_nRF24 = 0;}

                temp = SPI_data_rx[0] & TX_DS; 
                if(temp == TX_DS){IRQ_Sent_Ready_nRF24 = 1; UART2_send_string("\nОтправлено!"); write_register_nRF24(STATUS, SPI_data_rx[0]); LED_1(On); Delay_ms(100); LED_1(Off); NVIC_SystemReset();}
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

    void NMI_Handler()
    {
        if((RCC->CSR & RCC_CSR_LSECSSON) == RCC_CSR_LSECSSON){LED_1(On);}
        LED_2(On);
    }

    void LPUART1_IRQHandler()
    {
        if(first_byte){j = 0; first_byte = false;}
        else{j++;}
        if((LPUART1->ISR & USART_ISR_RXNE) == USART_ISR_RXNE)
        {
            stringLPUART1_RX[j] = LPUART1->RDR;
            if(stringLPUART1_RX[j] == 0x0D)
            {
                first_byte = true; 
                string_good = true;
            }
        }
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
