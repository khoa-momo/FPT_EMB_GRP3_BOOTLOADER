#include "PORT_GPIO.h"

void Clock_Init()
{  
          /* choose 2MHz IRC */
      
        
        /* Enable clock for PORTE  PORTD  PORT C */
        SIM->SCGC5 |= ( SIM_SCGC5_PORTD_MASK | SIM_SCGC5_PORTE_MASK | SIM_SCGC5_PORTC_MASK);
}

void ledInit()
{ 
    // Initialize the Green LED (PTD5)
    // Set the PTD5 pin multiplexer to GPIO mode
    PORTD->PCR[5]= PORT_PCR_MUX(1);   
    // Set the pin's direction to output
    FPTD->PDDR |= GREEN_LED_PIN;    
    // Set the initial output state to high (reset LED off)
    FPTD->PSOR |= GREEN_LED_PIN;
    
    
        /* Initialize the RED LED (PTE29)*/
    /* Set the PTE29 pin multiplexer to GPIO mode */
    PORTE->PCR[29] = PORT_PCR_MUX(1);
    /* Set the pin's direction to output */
    FPTE->PDDR |= RED_LED_PIN;
    /* Set the initial output state to high (reset LED off) */
    FPTE->PSOR |= RED_LED_PIN;
	
	
}

void initButton_IRQ ()
{
  // Initialize the BUTTON 1 (PTC3)
    // Set the PTC3 pin multiplexer to GPIO mode
    PORTC->PCR[3]= PORT_PCR_MUX(1);
    // Enable and set BUTTON 1 is push pull
    PORTC->PCR[3]|= PORT_PCR_PE(1) | PORT_PCR_PS(1);
    /* Set up IROC -> Port-PCR -> Falling Edge */ 
    PORTC->PCR[3] &= ~(PORT_PCR_IRQC_MASK); // clear bit 
    PORTC->PCR[3] |= PORT_PCR_IRQC(10); 
  
  /* Set the pin's direction to INPUT (default INPUT) */
    
    
    
    // Initialize the BUTTON 2 (PTC12)
    // Set the PTC12 pin multiplexer to GPIO mode
    PORTC->PCR[12]= PORT_PCR_MUX(1);
    // Enable and set BUTTON 2 is push pull
    PORTC->PCR[12]|= PORT_PCR_PE(1) | PORT_PCR_PS(1);
    /* Set up IROC -> Port-PCR -> Falling Edge */ 
    PORTC->PCR[12] &= ~(PORT_PCR_IRQC_MASK); // clear bit 
    PORTC->PCR[12] |= PORT_PCR_IRQC(10); 
  
  /* Set the pin's direction to INPUT (default INPUT) */
  
  
	
      /* Set up NVIC */ 
    NVIC_EnableIRQ (PORTC_PORTD_IRQn); // 31
}

void LED_SW_Init()
{
    Clock_Init();
    initButton_IRQ ();
    ledInit();
}

/* Delay mili second with 2MHz clock*/
void delay_Ms(uint32_t Delay_CNT)
{
    
    for (uint32_t i = 0; i < Delay_CNT; i++)
    {
      
          /* Delay 1 micro second */
          for(uint32_t j = 0; j< 323; j++)
          {
              /* Do Nothing */
          }
          
    }
}

// deinit port
void PORT_DeInit()
{
    PORTD->PCR[5] = 0;
    PORTC->PCR[3] = 0;
    PORTC->PCR[12] = 0;
    PORTE->PCR[29] = 0;
    PORTA->PCR[1] = 0;
    PORTA->PCR[2] = 0;
    
    PORTD->ISFR |= (0xFFFFFFFF);
    PORTC->ISFR |= (0xFFFFFFFF);
    PORTE->ISFR |= (0xFFFFFFFF);
}

// deinit GPIO
void GPIO_DeInit()
{
    GPIOD->PDDR = 0;
    GPIOE->PDDR = 0;
}

