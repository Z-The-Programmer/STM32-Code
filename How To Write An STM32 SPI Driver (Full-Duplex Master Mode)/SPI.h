#ifndef STM32L432KC
#define STM32L432KC
#include "stm32l432xx.h"
#endif

void initClocks(void);

void initSPI_SSM(void);
void configSpi1Pins_SSM(void);
void setPinMode_SSM(void);
void setAF_SSM(void);
void configSpi_SSM(void);
uint8_t transferSPI_SSM(uint8_t tx_data);


void initSPI_HSM(void);
void configSpi1Pins_HSM(void);
void setPinMode_HSM(void);
void setAF_HSM(void);
void configSpi_HSM(void);
uint8_t transferSPI_HSM(uint8_t tx_data);



