#include "stm32l432xx.h"

void initGPIO(void);

/*****************************************************************
* initGPIO
*
* This function configures the required GPIO clock and pins
* to either input or output.
*****************************************************************/
void initGPIO(void) 
{
	//enable clock for GPIO B
	RCC->AHB2ENR |= (1U << 1);

	//set bits 6,7,8 and 9 to 0. Since bits 8 and 9 were set to 0, PB_4 already set to input mode
	GPIOB->MODER &= ~(15U << 6);

	//set PB_3 to output mode
	GPIOB->MODER |= (1U << 6);
}

int main(void)
{		
	int btnPressed = 0;		//state of button pin (PB_4) saved here

	//initialise GPIO
	initGPIO();
	
	//loop around checking if button was pressed.
	while(!btnPressed)
	{
		//inspect the state of the button pin by checking bit 4 of GPIOB_IDR
		btnPressed = (GPIOB->IDR & (1U << 4));
	}
	
	//button was pressed, turn LED on
	GPIOB->ODR |= (1U << 3);
}
