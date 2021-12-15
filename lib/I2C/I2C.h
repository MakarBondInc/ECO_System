#include "stm32l051xx.h"
void I2C_init(void);
void I2C_start(void);
void I2C_stop(void);
void I2C_send_address(uint8_t data);

uint8_t data;
void I2C_send_byte(uint8_t byte);
uint8_t I2C_read_byte(void);
