#include "stm32l432xx.h"


/*****************************************************************
 initClocks

 All clocks required are initialised here.
*****************************************************************/
void initClocks(void)
{
    RCC->AHB2ENR |= RCC_AHB2ENR_GPIOAEN;        // ENABLE GPIO PORT A CLOCK (BIT 0)
    RCC->APB2ENR |= RCC_APB2ENR_USART1EN;       // ENABLE UART1 CLOCK (BIT 14)
}


/*****************************************************************
 configPinMode

 Configures the mode of the pins.
*****************************************************************/
void configPinMode(void)
{
    // RESET PIN MODES
    GPIOA->MODER &= ~((3u << 18)    // CLEAR PA9
                     |(3u << 20));  // CLEAR PA10

    // SET PIN MODES
    GPIOA->MODER |= ((2u << 18)     // SET PA9 TO AF
                    |(2u << 20));   // SET PA10 TO AF
}

/*****************************************************************
 setAF

 Configures alternate function of the pins to UART1.
*****************************************************************/
void setAF(void)
{
    // RESET ALTERNATE FUNCTION. AFR[1] IS THE
    // ALTERNATE FUNCTION HIGH REGISTER
    GPIOA->AFR[1] &= ~((15u << 4)       // CLEAR PA9 AF
                      |(15u << 8));     // CLEAR PA10 AF

    // SET ALTERNATE FUNCTION
    GPIOA->AFR[1] |= ((7u << 4)         // SET PA9 AF
                     |(7u << 8));       // SET PA10 AF
}

/*****************************************************************
 configUart1Pins

 Configures required pins to to be used as UART1 pins.
*****************************************************************/
void configUart1Pins(void)
{
    // SET PIN MODE TO ALTERNATE FUNCTION
    configPinMode();

    // SET ALTERNATE FUNCTION TO BE UART1
    setAF();
}

/*****************************************************************
 configUart1

 This function configures the UART1 related registers to set up
 the peripheral.
*****************************************************************/
void configUart1(void)
{
    // CONFIGURE USART1 CR1 REGISTER
    // CLEAR BITS
    USART1->CR1 &= ~(USART_CR1_M1       // CLEAR M1 FOR 1 START BIT AND 8 DATA BITS (28)
                    |(0x03 << 26)       // INHIBIT INTERRUPTS AT BITS 26 AND 27     (27/26)
                    |USART_CR1_OVER8    // OVERSAMPLING BY 16                       (15)
                    |USART_CR1_CMIE     // INHIBIT CHARACTER MATCH INTERRUPT        (14)
                    |USART_CR1_MME      // DON'T ENABLE MUTE MODE                   (13)
                    |USART_CR1_M0       // CLEAR M0 FOR 1 START BIT AND 8 DATA BITS (12)
                    |USART_CR1_PCE      // NOT IMPLEMENTING PARITY CONTROL          (10)
                    |(0x1F << 3)        // INHIBIT INTERRUPTS AT BITS 4 TO 8        (4-8)
                    |USART_CR1_TE       // DON'T ENABLE TRANSMITTER JUST YET        (3)
                    |USART_CR1_RE       // DON'T ENABLE RECEIVER JUST YET           (2)
                    |USART_CR1_UE);     // DON'T ENABLE UART1 JUST YET              (0)

    // CONFIGURE USART CR2 REGISTER
    // CLEAR BITS
    USART1->CR2 &= ~(USART_CR2_RTOEN    // DISABLE RECEIVER TIMEOUT             (23)
                    |USART_CR2_ABREN    // NO AUTOMATIC BAUD RATE DETECTION     (20)
                    |USART_CR2_MSBFIRST // TRANSMIT/RECEIVE LSB FIRST           (19)
                    |(0x03 << 16)       // IDLE STATE HIGH FOR RX/TX PINS       (17/16)
                    |USART_CR2_SWAP     // DON'T SWAP FUNCTION OF RX/TX PINS    (15)
                    |USART_CR2_LINEN    // NO LIN MODE                          (14)
                    |(0x03 << 12)       // 1 STOP BIT                           (13/12)
                    |USART_CR2_CLKEN    // DON'T USE CLOCK WITH UART            (11)
                    |USART_CR2_LBDIE);  // NO LIN BREAK DETECTION INTERRUPT     (6)

    // CONFIGURE USART CR3 REGISTER
    // CLEAR BITS
    USART1->CR3 &= ~(USART_CR3_TCBGTIE  // NO TRANSMISSION COMPLETE BEFORE GUART TIME INTERRUPT (24)
                    |USART_CR3_DEM      // NO DRIVER ENABLE MODE                                (14)
                    |(0x7F << 3)        // DISABLE VARIOUS IRRELEVANT MODES                     (9-3)
                    |USART_CR3_IREN     // NO IrDA MODE                                         (1)
                    |USART_CR3_EIE);    // INHIBIT ERROR INTERRUPT                              (0)

    // SET BITS
    USART1->CR3 |= (USART_CR3_OVRDIS    // DISABLE OVERRUN FUNCTIONALITY (12)
                   |USART_CR3_ONEBIT);  // USE ONE SAMPLE BIT METHOD     (11)

    // SET BAUD RATE IN BRR REGISTER
    USART1->BRR = 35;                   // VALUE THAT SETS BAUD RATE TO 115,200 AT INPUT FREQUENCY OF 4MHZ

    // ENABLE UART
    USART1->CR1 |= (USART_CR1_TE        // ENABLE TRANSMITTER (3)
                   |USART_CR1_RE        // ENABLE RECEIVER    (2)
                   |USART_CR1_UE);      // ENABLE UART1       (0)

}

/*****************************************************************
 initUART

 This function encapsulates all the code and function calls
 related to setting up the UART.
*****************************************************************/
void initUART(void)
{
    // INITIALISE REQUIRED CLOCKS. MUST ALWAYS BE DONE FIRST
    // AS WITHOUT ENABLING THE CLOCKS THE PERIPHERALS CANNOT
    // BE CONFIGURED OR USED.
    initClocks();

    // CONFIGURE PINS FOR UART1
    configUart1Pins();

    // CONFIGURE UART1
    configUart1();
}

/*****************************************************************
 transmitUart

 This function sends a byte of data over UART.
*****************************************************************/
void transmitUart(uint8_t data)     // BYTE OF DATA TO BE TRANSMITTED
{
    // WAIT UNTIL TRANSMIT DATA REGISTER IS READY TO TAKE DATA
    // USART_ISR_TXE EXPANDS TO (1 << 7);
    while(!(USART1->ISR & USART_ISR_TXE));

    // LOAD DATA TO TRANSMIT REGISTER
    USART1->TDR = data;
}

/*****************************************************************
 transmitStrUart

 This function transmits a string over UART.
*****************************************************************/
void transmitStrUart(char* str) // POINTER TO A STRING TO BE TRANSMITTED
{
    // WHILE NOT END OF STRING
    while(*str)
    {
        // TRANSMIT CHARACTER THEN INCREMENT POINTER TO NEXT CHARACTER
        transmitUart(*str++);
    }
}

/*****************************************************************
 receiveUart

 This function reads a byte of data from the receive data register
 if there is data to be read.

 Returns
 a byte of data read from the receive data register
*****************************************************************/
uint8_t receiveUart(void)
{
    uint8_t rxData = 0;

    // IF THERE IS DATA IN THE RECEIVE DATA REGISTER
    // USART_ISR_RXNE EXPANDS TO (1 << 5)
    if(USART1->ISR & USART_ISR_RXNE)
    {
        // READ DATA FROM THE REGISTER
        rxData = USART1->RDR;
    }

    return rxData;
}

int main(void)
{
    //SET UP UART
    initUART();

    while(1)
    {
        // IF DATA WAS RECEIVED
        if(receiveUart() > 0)
        {
            //transmitStrUart("Character Received\n\r");

            // TRANSMIT AN ARBITRARY 'U' (ASCII 85) OVER UART
            transmitUart(85);
        }
    }
}
