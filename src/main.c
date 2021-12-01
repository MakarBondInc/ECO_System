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

uint8_t SPI_data_sent;
uint8_t IRQ_nRF;
uint8_t data_SPI;

uint8_t IRQ_Data_Ready_nRF24;
uint8_t IRQ_Sent_Ready_nRF24;
uint8_t IRQ_Maximum_number_of_TX;
uint8_t impulse = 0;

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
        
    //Debug();            //Добавление функций необходимых для отладки

    init_SysTick();     //Инициализация системного таймера

    //init_USART1();      //Инициализация USART1 для связи с GSM
    init_USART2();      //Инициализация USART2 для связи с ПК
    init_SPI1();

    pin_CSN(HIGH);        //Сигнал выбора
    pin_CE(LOW);
    Delay_ms(1000);
/*
    read_status_AXL();
    UART2_send_string("\nSTATUS: ");
    send_to_PC(SPI_data_rx, 2);
    while(1){;}
*/    
    //UART2_send_string("I'm ready!\n");  //Отправка строки по UART на ПК
/*
                            GPIOB->BSRR |= GPIO_BSRR_BR_8;
                            GPIOC->BSRR |= GPIO_BSRR_BR_13;
                            while(1)
                            {
                                for(impulse = 0; impulse < 9; impulse++)
                                {
                                    GPIOC->BSRR |= GPIO_BSRR_BS_13;
                                    Delay_us(8);
                                    GPIOC->BSRR |= GPIO_BSRR_BR_13;
                                    Delay_us(8);
                                }
                                Delay_ms(500);
                            }
*/
/*
    read_register_nRF24(EN_RXADDR);
    send_to_PC(SPI_data_rx, 2);
    data_pipe(5, Off);
    read_register_nRF24(EN_RXADDR);
    send_to_PC(SPI_data_rx, 2);
    
    while(1){;}
    Delay_ms(1000);       //Задержка длительностью 1 секунда
    //Delay_ms(1000);     //Задержка длительностью 1 секунда
    //Delay_ms(1000);     //Задержка длительностью 1 секунда
    //write_register_nRF24(RX_ADDR_P0);

    write_register_nRF24(CONFIG, 0b00001000);
    write_register_nRF24(STATUS, 0b01111000);

    write_register_nRF24(RF_SETUP, 0b00000110);
    read_register_nRF24(RF_SETUP);
    UART2_send_string("\nRF_SETUP: ");
    send_to_PC(SPI_data_rx, 2);

    Delay_ms(100);     //Задержка длительностью 1 секунда

    flush_TX();
    
    read_status_nRF24();
    UART2_send_string("\nSTATUS: ");
    send_to_PC(SPI_data_rx, 1);
    
    write_register_nRF24(CONFIG, 0b00001010);    //Запуск модуля и превод в режим передатчика

    Delay_ms(1);

    read_register_nRF24(CONFIG);
    UART2_send_string("\nCONFIG: ");
    send_to_PC(SPI_data_rx, 2);

    write_register_nRF24_5_byte(TX_ADDR, 0x78, 0x78, 0x78, 0x78, 0x78);
    //send_to_PC(SPI_data_rx, 5);
    
    read_register_nRF24_5_byte(TX_ADDR);
    UART2_send_string("\nTX_ADDR: ");
    send_to_PC(SPI_data_rx, 6);

    write_register_nRF24_5_byte(RX_ADDR_P0, 0x78, 0x78, 0x78, 0x78, 0x78);
    //send_to_PC(SPI_data_rx, 5);
    
    read_register_nRF24_5_byte(RX_ADDR_P0);
    UART2_send_string("\nRX_ADDR_P0: ");
    send_to_PC(SPI_data_rx, 6);

    write_register_nRF24(RF_CH, 110);    //Установка канала номер 100
    //send_to_PC(SPI_data_rx, 2);

    read_register_nRF24(RF_CH);
    UART2_send_string("\nRF_CH: ");
    send_to_PC(SPI_data_rx, 2);
    
*/
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

    /*
    read_register_nRF24(CONFIG);
    data_SPI = SPI_data_rx[0]; dataTranslate(data_SPI); data_SPI = SPI_data_rx[1]; dataTranslate(data_SPI);
    
    write_register_nRF24(CONFIG, 9);    //Перевод модуля в режим передатчика
    data_SPI = SPI_data_rx[0]; dataTranslate(data_SPI);

    UART2_send_string("\n");
    read_register_nRF24(CONFIG);
    data_SPI = SPI_data_rx[0]; dataTranslate(data_SPI); data_SPI = SPI_data_rx[1]; dataTranslate(data_SPI);
    */
/*
    UART2_send_string("\n");
    read_register_nRF24(CONFIG);
    data_SPI = SPI_data_rx[0]; dataTranslate(data_SPI); data_SPI = SPI_data_rx[1]; dataTranslate(data_SPI);
    UART2_send_string("\n");
*/
    //UART2_send_string("\n");
    //read_register_nRF24(TX_ADDR);
    //data_SPI = SPI_data_rx[0]; dataTranslate(data_SPI); data_SPI = SPI_data_rx[1]; dataTranslate(data_SPI); data_SPI = SPI_data_rx[2]; dataTranslate(data_SPI); data_SPI = SPI_data_rx[3]; dataTranslate(data_SPI); data_SPI = SPI_data_rx[4]; dataTranslate(data_SPI); data_SPI = SPI_data_rx[5]; dataTranslate(data_SPI);

    //write_register_nRF24_5_byte(TX_ADDR, 0xB3, 0xB4, 0xB5, 0xB6, 0x05);
    //transmit_data();
    
    __ASM("WFI");
    while (1)
    {
/*
        Delay_ms(500);
        Debug_LED(On);
        Delay_ms(500);
        Debug_LED(Off);
*/
        //if(SPI_data_ready){SPI_data_ready = 0; spi = 0; pin_CSN(HIGH);}
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
/*
void blink_1(void)
{
    Debug_LED(HIGH);
    Delay_ms(100);
    Debug_LED(LOW);
}
void blink_2(void)
{
    Debug_LED(HIGH);
    Delay_ms(100);
    Debug_LED(LOW);
    Delay_ms(100);
    Debug_LED(HIGH);
    Delay_ms(100);
    Debug_LED(LOW);
}
void blink_3(void)
{
    Debug_LED(HIGH);
    Delay_ms(100);
    Debug_LED(LOW);
    Delay_ms(100);
    Debug_LED(HIGH);
    Delay_ms(100);
    Debug_LED(LOW);
    Delay_ms(100);
    Debug_LED(HIGH);
    Delay_ms(100);
    Debug_LED(LOW);
}
*/
/*Область обработчиков прерывания*/

    /*
    void SPI2_IRQHandler()  //Обработчик прерывания SPI интерфейса
    {
        if(SPI2->SR & SPI_SR_RXNE)  //Если прием байта закончен
        {
            Debug_LED(On);
            SPI_data_rx[spi] = SPI2->DR;    //Копируем даннные в переменную (флаг RXNE сбрасывается аппаратно после чтения DR)
            spi++;
            if(spi == (spi_size)){SPI_data_ready = 1;}     //Поднамаем флаг готовности байта, обработка осуществляется в общем цикле
            else{SPI2->DR = 0xFF; SPI_data_ready = 0;}
        }
    }
    */
    void USART1_IRQHandler()
    {
        ;
    }

    void USART2_IRQHandler()
    {
        ;
    }

    void I2C1_IRQHandler()
    {
        ;
    }
    void EXTI0_1_IRQHandler()
    {
        IRQ_nRF = 1;                //Поднамаем флаг внешнего прерывания, требуется обработать статусный регистр nRF
        EXTI->PR |= EXTI_PR_PIF0;   //Сброс флага прерывания на лиции PA0
    }
    void EXTI2_3_IRQHandler()
    {
        ;
    }
    void EXTI4_15_IRQHandler()
    {
        ;
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

/*
PA2     ART2
PA3

PA10    UART1
PA9

PB10     SPI2
PC2
PC3

PA0     IRQ nRF24l01

PA5     LED-Nucleo

PA8     CSN nRF24l01
PA1     CE
*/