#include "stm32l432xx.h"
#include "Timer.h"

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
* delay
*
* This function introduces a delay in milliseconds specified
* by the 'ms' parameter.
*****************************************************************/
void delay(unsigned int ms)
{
    /*HOLDS THE TOTAL COUNT FROM TM2_CNT REGISTER TO RECORD HOW
    MUCH TIME ELAPSED.*/
    unsigned int counter = 0;
    //HOLDS THE COUNT FOR THE COUNTER TO COUNT TO.
    unsigned int goalCount = ms * 4000u;
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
* delayUs
*
* This function introduces a delay in microseconds specified
* by the 'us' parameter.
*****************************************************************/
void delayUs(unsigned int us)
{
    /*HOLDS THE TOTAL COUNT FROM TM2_CNT REGISTER TO RECORD HOW
    MUCH TIME ELAPSED.*/
    unsigned int counter = 0;
    //HOLDS THE COUNT FOR THE COUNTER TO COUNT TO.
    unsigned int goalCount = us * 4;
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
