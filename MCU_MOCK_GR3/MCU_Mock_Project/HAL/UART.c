#include "UART.h"

void UART0_Clock2Mhz()
{  
  //set IRCS = 4MHz IRC and divide 2
  MCG->C2 &= ~(MCG_C2_IRCS_MASK);            //clear IRCS  
  MCG->C2 |= MCG_C2_IRCS(1);                 // set IRCS
  
  // Enables the internal reference clock for use as MCGIRCLK
  MCG->C1 &= ~MCG_C1_IRCLKEN_MASK;           // clear IRCLKEN
  MCG->C1 |= MCG_C1_IRCLKEN_MASK;            //set IRCLKEN
  
   /*set UART0_Clock = MCGIRCLK*/
    SIM->SOPT2 = (SIM->SOPT2 & ~SIM_SOPT2_UART0SRC_MASK) | SIM_SOPT2_UART0SRC(3);
    
    /* Enable clock for PORTA & UART0 */
    SIM->SCGC5 |= SIM_SCGC5_PORTA(1);
    SIM->SCGC4 |= SIM_SCGC4_UART0(1);  

}


void UART0_PortInit()
{
  /* Clear PCR_MUX Register */
  PORTA->PCR[UART0_RX_PIN] &= ~PORT_PCR_MUX_MASK;
  PORTA->PCR[UART0_TX_PIN] &= ~PORT_PCR_MUX_MASK;
  
  /* Set Pin PA1 & PA2 = ALT2 Mode */
  PORTA->PCR[UART0_RX_PIN] |= PORT_PCR_MUX(2);     
  PORTA->PCR[UART0_TX_PIN] |= PORT_PCR_MUX(2);
  
  /* Set up Pull-up for UART Pin */
  PORTA->PCR[UART0_RX_PIN] |= PORT_PCR_PE(1) | PORT_PCR_PS(1);
  PORTA->PCR[UART0_TX_PIN] |= PORT_PCR_PE(1) | PORT_PCR_PS(1);
}


void UART0_SetBaudrate()
{
    /*set Baudrate = 2.000.000 / (29*7) = 9600*/
    /*set OSR + 1 = 11*/
    UART0->C4 = (UART0->C4 & (~UART0_C4_OSR_MASK)) | UART0_C4_OSR(6);

   /*set BR = 33*/
    UART0->BDH &= (~UART0_BDH_SBR_MASK);
    UART0->BDL = (UART0->BDL & (~UART0_BDL_SBR_MASK)) | UART0_BDL_SBR(29);
}


void UART0_SetFrame()
{
    /* Select number of Data Bits - 8 bits */
    UART0->C4 &= (~UART0_C4_M10_MASK);
    UART0->C1 &= (~UART0_C1_M_MASK);
    
    /* Select Parity Bit - None */
    UART0->C1 &= (~UART0_C1_PE_MASK);
    
    /*set mode LSB*/
    UART0->S2 &= (~UART0_S2_MSBF_MASK);

   /*set receive data not inverted*/
    UART0->S2 &= (~UART0_S2_RXINV_MASK);

   /*set transmit data not inverted*/
    UART0->C3 &= (~UART0_C3_TXINV_MASK);
}


void UART0_Init()
{
  UART0_Clock2Mhz();
  UART0_PortInit();
  UART0_SetBaudrate();
  UART0_SetFrame();
  
  /* Enable Transmitter & Receiver */
  UART0->C2 |= UART0_C2_TE(1) | UART0_C2_RE(1);
}

void UART0_TransmitBuffer(uint8_t * buffer, uint32_t len)
{
  for(uint32_t i = 0; i <= len; i++)
  {
      /* Wait Data Register is Empty */
      while( (UART0->S1 & UART0_S1_TDRE_MASK) == 0 );
      /* send 8 bit data */
      UART0->D |= (uint8_t)(*buffer & (uint8_t)0xFF);
      /* increment buffer address */
      buffer++;
  }
}

void UART0_ReceiveBuffer(uint8_t * buffer, uint32_t len)
{
  for(uint32_t i = 0; i <= len; i++)
  {
      uint32_t timeout = 1000U;
      /* Wait Data Register is full */
       while(((UART0->S1 & UART0_S1_RDRF_MASK) == 0) && (timeout != 0))
       {
          timeout--;
       }
      /* Write to buffer */
      *buffer = (uint8_t)(UART0->D & (uint8_t)0xFF);
      /* increment buffer address */
      buffer++;  
  }
}

void UART0_TransmitData(uint8_t u8Data)
{
  /* Wait Data Register is Empty */
  while( (UART0->S1 & UART0_S1_TDRE_MASK) == 0 );
  
  /* Write to Data Register */
  UART0->D = u8Data;
}

uint8_t UART0_ReceiveData(void)
{
  /* Wait Data Register is full */
  while( (UART0->S1 & UART0_S1_RDRF_MASK) == 0 );
  
  /* Read and Return Data Register */
  return UART0->D;
}


void UART0_SetupReceiveINT()
{
  /* Setup UART Interrupt */
  UART0->C2 |= UART0_C2_RIE(1);
  
  /* Enable UART Interrupt by NVIC */
  NVIC_EnableIRQ(UART0_IRQn);
}

void UART0_ReceiveBufferINT(uint8_t * buffer, uint32_t len)
{
  for(uint32_t i = 0; i <= len; i++)
  {
      /* Wait Data Register is full */
       while( (UART0->S1 & UART0_S1_RDRF_MASK) == 0 );
      /* Write to buffer */
      *buffer = (uint8_t)(UART0->D & (uint8_t)0xFF);
      /* increment buffer address */
      buffer++;  
  }
}

void UART0_TransmitDataINT(uint8_t u8Data)
{
  /* Wait Data Register is Empty */
  while( (UART0->S1 & UART0_S1_TDRE_MASK) == 0 );
  
  /* Write to Data Register */
  UART0->D = u8Data;
}

// deinit uart
void UART0_DeInit()
{
    UART0->C4 = (0x0F);
    
    UART0->C1 = 0;
    
    UART0->S2 = 0;
    
    UART0->C3 = 0;
    
    UART0->C2 = 0;
}

// deinit clock
void Clock_DeInit()
{
    MCG->C2 = 0xC0;
    MCG->C1 = 0x04;
    
    SIM->SOPT2 = 0;
    SIM->SCGC5 = 0;
    SIM->SCGC4 = 0;
}