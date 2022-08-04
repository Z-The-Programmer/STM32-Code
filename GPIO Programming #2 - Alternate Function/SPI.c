#include "stm32l432xx.h"
#include "SPI.h"


/*****************************************************************
* initSPIMaster
*
* This function configures SPI1
*****************************************************************/
void initSPIMaster(void)
{    
    //ENABLE SPI1 PERIPHERAL CLOCK
    RCC->APB2ENR|=RCC_APB2ENR_SPI1EN;
    
    //SET ALL BITS TO 0 IN CONTROL REGISTER 1
    SPI1->CR1 = 0;
    
    //MODE MASTER | CLOCK POLARITY 1 | CLOCK PHASE 1 | DIVIDE INPUT CLOCK BY 64 TO GET SPI OUTPUT FREQUENCY
    SPI1->CR1 |= (SPI_CR1_MSTR | SPI_CR1_CPOL | SPI_CR1_CPHA | (5u << 3));

    //SET ALL BITS TO 0 IN CONTROL REGISTER 2
    SPI1->CR2 = 0;
    
    //SET SSOE BIT AS THIS MICROCONTROLLER IS THE ONLY MASTER
    SPI1->CR2 |= SPI_CR2_SSOE;
}


/*****************************************************************
* spi_write
*
* This function is used for demonstration purposes only. Refer
* to the SPI Driver tutorial for a properly written SPI transmit
* function.
*****************************************************************/
unsigned int spi_write(uint8_t *data, int size)
{
    unsigned int rx_data = 0;
    int i = 0;
    
    //ENABLE SPI 1 TO SET SLAVE SELECT LOW (USING HARDWARE SLAVE MANAGEMENT)
    SPI1->CR1 |= (SPI_CR1_SPE);
    
    while(i < size)
    {
        while(!((SPI1->SR)&(1u << 1)));     //WAIT FOR TXE BIT TO SET WHICH WILL INDDICATE THAT THE BUFFER IS EMPTY
        
        SPI1->DR=data[i];                   //SEND DATA
        i++;
    }
    
    while(!((SPI1->SR)&(1u << 1)));     //WAIT FOR TXE BIT TO SET WHICH WILL INDDICATE THAT THE BUFFER IS EMPTY
    while(((SPI1->SR)&(1u << 7)));     //WAIT FOR BSY BIT TO RESET WHICH WILL INDICATE THAT THE SPI PERIPHERAL IS FINISHED
    
    //CLEAR OVERRUN FLAG BY READING DR AND SR
    uint8_t temp = (uint8_t)SPI1->DR;
            temp = (uint8_t)SPI1->SR;
   
    //DISABLE SPI 1 TO SET SLAVE SELECT HIGH (USING HARDWARE SLAVE MANAGEMENT)
    SPI1->CR1 &= ~(SPI_CR1_SPE);
    
    return rx_data;
}


