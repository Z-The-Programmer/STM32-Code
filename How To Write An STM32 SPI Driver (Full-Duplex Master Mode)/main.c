#include "stm32l432xx.h"
#include "Timer.h"
#include "SPI.h"


int main (void)
{
    //MPU9250 Addresses
    uint8_t data[4] = {187, 188, 189, 190};
    
    //STORES DATA RECEIVED FROM MPU9250
    uint8_t rxd = 0;
    
    //SET UP THE TIMER
    initTim2();
    
    //SETUP SPI MASTER
    initSPI_SSM();
    
    while(1)
    {
        //WRITE TO SPI
        rxd = transferSPI_SSM(data[0]);
        
        //DELAY MS
        delay(50);
    }
}





