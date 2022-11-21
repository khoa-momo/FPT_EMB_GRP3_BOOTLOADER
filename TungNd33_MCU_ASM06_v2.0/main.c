

/** Read the ADC value and convert to Vin once every second
when receiving 0x55, then transfer to the computer nearest 10 values **/


#include "ADC.h"
#include "UART.h"
#include "sys_delay.h"
#include "FTOA.h"


volatile uint32_t ADC_Value;

//for convert FTOA
uint8_t Buffer[15];

// save ADC value
float_t Vin_ValueBuffer[10];

// Vin value
volatile float_t Vin = 0;

// Vref value 
float_t Vref = 3.3;

// count of Vin_ValueBuffer[]
uint8_t Count; 



int main(void)
{
    UART0_Init();
    
    UART0_SetupReceiveINT();
    
    ADC_Init();
    
    ADC_SetupINT();
    
    __enable_irq();
    
    while(1)
    {      
        // ADC start convert
         ADC_Start();  
               
        // write on string       
          Vin_ValueBuffer[Count] = Vin;
          
          Count++;
          if(Count == 10)
          {
            Count = 0;
          }
          
        // Delay 1s
          Sys_Delay(1000);
    }
}

// Convert ADC to Vin
float_t ADCToVin(uint32_t ADCValue)
{
    float_t VinTemp = 0;
    VinTemp = ((float_t)ADCValue / (65534U / Vref));   //65534 = (2^16) - 1
    return VinTemp;
}


void UART0_DriverIRQHandler()
{
    /* Application Code */
    // recevie 0x55 = 'P'
      if((UART0->D) == (uint8_t)0x55)
      {
        for(uint8_t i = 0; i < 10; i++)
        {
          // Convert ADC to Vin
          ftoa(Vin_ValueBuffer[i] , (char*)Buffer , 5);
          
          UART0_TransmitData(0x20);             // transmit "space"
          UART0_TransmitBuffer(Buffer, 5);      // transmit Value         
        }          
      }
    
    /* Clear Pending Interrupt */
    NVIC_ClearPendingIRQ(UART0_IRQn);
}


void ADC0_IRQHandler()
{
      
      // Get ADC value
      ADC_Value = ADC0->R[0];
      
      // convert ADC to Vin
      Vin = ADCToVin(ADC_Value);
       
      /* Clear Pending Interrupt */
      NVIC_ClearPendingIRQ(ADC0_IRQn); //15
}























