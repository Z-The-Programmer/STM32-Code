#include "stm32l432xx.h"
#include "SPI.h"

void initTim2(void);
void delay1Sec(void);
void setupSPIMaster(void);
void configGpioSpiPins(void);
void enablePortClocks(void);


/*****************************************************************
* enablePortClocks
*
* This function enables the port clocks required to make use of
* the GPIO ports and their pins.
*****************************************************************/
void enablePortClocks(void)
{
     RCC->AHB2ENR |= (1 << 0);                  //GPIO A clock enable
     RCC->AHB2ENR |= (1 << 1);                  //GPIO B clock enable
}

/*****************************************************************
* initTim2
*
* This function initialises timer 2.
* The STM32L432KC runs at 4MHz by default so 4,000,000 has been
* written into the ARR register to reset the counter every second.
*****************************************************************/
void initTim2(void) 
{
    //ENABLE TIM2 CLOCK
    RCC->APB1ENR1 |= (1u << 0);
    
    //LEAVE THE COUNTER FREQUENCY UNCHANGED
    TIM2->PSC = 0;
    
    //SET TIMER RELOAD VALUE
    TIM2->ARR = (uint32_t)4000000;
    
    //SET INITIAL COUNTER VALUE
    TIM2->CNT = 0;
    
    //ENABLE TIM2 COUNTER
    TIM2->CR1 |= (1u << 0);
}

/*****************************************************************
* delay1Sec
*
* This function introduces a delay of 1 second approximately
* by tracking the values in the CNT register of timer 2.
* This function relies on the frequency of the microcontroller
* to be accurate. The STM32L432KC runs at 4MHz by default so the
* goalCount has been set to 4,000,000 to stop the count loop when
* the counter variable reaches or exceeds that that number as 1
* second (approx) would have passed.
* The value of the goalCount therefore depends on the frequency
* of the microcontroller.
*****************************************************************/
void delay1Sec(void)
{
    /*HOLDS THE TOTAL COUNT FROM TM2_CNT REGISTER TO RECORD HOW
    MUCH TIME ELAPSED.*/
    unsigned int counter = 0;
    //HOLDS THE COUNT FOR THE COUNTER TO COUNT TO.
    unsigned int goalCount = 4000000;
    //HOLDS THE MOST RECENT VALUE OBTAINED FROM TIM2_CNT.
    unsigned int currentCntVal = 0;
    //HOLDS THE PREVIOUS VALUE OBTAINED FROM TIM2_CNT.
    unsigned int prevCntVal = 0;
    //HOLDS RESULT OF CALCULATION BETWEEN CURRENT AND PREVIOUS COUNTS.
    unsigned int countToAdd = 0;
    
    //GET INITIAL VALUE OF CNT
    prevCntVal = TIM2->CNT; 
    
    //LOOP UNTIL COUNTER IS EQUAL OR EXCEED GOAL COUNT
    while(counter < goalCount)
    {
        //GET NEWEST COUNT
        currentCntVal = TIM2->CNT;
        
        //HANDLE SITUATION WHERE TIM2_CNT RESET
        if(currentCntVal < prevCntVal)
        {
            //GET THE COUNT BEFORE THE CNT REGISTER RESET AND THEN 
            //ADD THE COUNT AFTER IT RESET TO GET ELAPSED COUNT
            countToAdd = (4000000 - prevCntVal) + currentCntVal;
        }
        else
        {
            //SSUBTRACT CURRENT COUNT FROM PREVIOUS COUNT TO GET
            //ELAPSED COUNT
            countToAdd = currentCntVal - prevCntVal;
        }
        
        //ADD ELAPSED COUNT TO THE COUNTER
        counter += countToAdd;
        
        //CURRENT COUNT NOW BECOMES PREVIOUS COUNT
        prevCntVal = currentCntVal;
    }
}

/*****************************************************************
* setupSPIMaster
*
* This function calls a function to configure the SPI peripheral.
*****************************************************************/
void setupSPIMaster(void)
{
    initSPIMaster();
}

/*****************************************************************
* configGpioSpiPins
*
* This function configures the GPIO pins to alternate function
* to serve as SPI1 pins.
*****************************************************************/
void configGpioSpiPins(void)
{
    GPIOA->MODER &= ~(
                      (3u << (2 * 7))   //CLEAR PA7
                     |(3u << (2 * 6))   //CLEAR PA6
                     |(3u << (2 * 5))   //CLEAR PA5
                     );
    
    GPIOB->MODER &= ~(3u << (2 * 0));   //CLEAR PB0
    
    GPIOA->MODER |= (
                     (2u << (2 * 7))    //SET PA7 TO AF
                    |(2u << (2 * 6))    //SET PA6 TO AF
                    |(2u << (2 * 5))    //SET PA5 TO AF
                    );
    
    GPIOB->MODER |= (2u << (2 * 0));    //SET PB0 TO AF
    
    GPIOA->AFR[0] &= ~(                     //ACCESS AF LOWER
                       (15u << (4 * 7))     //CLEAR PA7 AF
                      |(15u << (4 * 6))     //CLEAR PA6 AF
                      |(15u << (4 * 5))     //CLEAR PA5 AF
                      );
                      
    GPIOB->AFR[0] &= ~(15u << (4 * 0));     //CLEAR PB0 AF
    
    GPIOA->AFR[0] |= (
                      (5u << (4 * 7))       //SET SPI1 MOSI
                     |(5u << (4 * 6))       //SET SPI1 MISO
                     |(5u << (4 * 5))       //SET SPI1 SCLK
                     );
                     
    GPIOB->AFR[0] |= (5u << (4 * 0));       //SET SPI1 SSEL (CHIP SELECT)
}

int main (void)
{
    //MPU9250 ADDRESSES
    uint8_t data[4] = {187, 188, 189, 190};
    
    //SET UP THE TIMER
    initTim2();
    
    //ENABLE GPIO PORT A AND B CLOCKS.
    enablePortClocks();
    
    //CONFIGURE GPIO PINS PA7,6,5 AND PB0 TO SPI1 ALTERNATE FUNCTION
    configGpioSpiPins();
    
    //SETUP SPI MASTER
    setupSPIMaster();

    while(1)
    {
        //WAIT FOR 1 SECOND
        delay1Sec();

        //WRITE TO SPI
        spi_write(&data[0], 4);
    }
}





