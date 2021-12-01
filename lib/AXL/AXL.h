#include "stm32l051xx.h"
#include "SPI.h"

#define HIGH 1
#define LOW 0

void AXL_CS(uint8_t state);
void read_status_AXL(void);