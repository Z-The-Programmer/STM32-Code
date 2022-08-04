
#define MODE_SLAVE      0
#define MODE_MASTER     1

#ifndef STM32L432KC
#define STM32L432KC
#include "stm32l432xx.h"
#endif

void initSPIMaster(void);
unsigned int spi_write(uint8_t *data, int size);
