
#include "UART.h"
#include "FLASH.h"
#include "PORT_GPIO.h"
#include "string_handle.h"
#include "RingBuffer.h"

/**************************************************/
#define BUF_SIZE (10)

static RingQueue_Type RX_BUF;

static uint8_t QueueArr[BUF_SIZE] = {0};

static uint8_t EraseOk[9] = "ERASE OK";

static uint8_t Ready[6] = "READY";

static uint8_t FIRMOK[7] = "FIRM OK";

static uint8_t tempRX;

static uint8_t txData;

static volatile uint32_t DataRecevie = 0;

static uint32_t DataWriteFlash;

static volatile uint32_t Address = 0;

static volatile uint8_t TotalSector = 0;

static uint8_t FirmReceiveOk = 0;

static uint8_t Status1 = 2;

static uint8_t Status2 = 2;

/******************************************************/
void Load_Firm();

void bootloader(uint32_t AppAddress);

void All_DeInit();

void UART0_DriverIRQHandler();

void PORTC_PORTD_IRQHandler();



/***********************************************************/
/***********************************************************/
/***********************************************************/

int main(void)
{
    LED_SW_Init();
    
    RingQueue_Init(&RX_BUF, QueueArr, BUF_SIZE);  
    
    UART0_Init();
    
    UART0_SetupReceiveINT();
    
    
    __enable_irq();
    
    FPTD->PDOR ^= GREEN_LED_PIN;
    FPTE->PDOR ^= RED_LED_PIN;
    
    while(1)
    {    
        
      //delay 5s check button is press jum to bootloader 2
        delay_Ms(50000);
        
        // jum to FIRM 1
        bootloader(0x00001000UL);
        if( (Status1 == 1) && (Status2 == 0))
        {
              //set priority
            NVIC_SetPriority (PORTC_PORTD_IRQn, 2);
            NVIC_SetPriority (UART0_IRQn, 1);
            
            // clear Flag
            PORTC->ISFR |= (0xFFFFFFFF);
            
            // clear pending
            NVIC_ClearPendingIRQ(PORTC_PORTD_IRQn);
            
            // jum to FIRM 2
            bootloader(0x00008000UL);
        }
        else if ((Status1 == 0) && (Status2 == 1))
        {
                //set priority
            NVIC_SetPriority (PORTC_PORTD_IRQn, 2);
            NVIC_SetPriority (UART0_IRQn, 1);
            
            // sent flag
            UART0_TransmitBuffer(Ready, 6);
            
            // load FIRM Program
            Load_Firm();
            
            // clear Flag
            PORTC->ISFR |= (0xFFFFFFFF);
            
            // clear pending
            NVIC_ClearPendingIRQ(PORTC_PORTD_IRQn);
        }
    }
}


void PORTC_PORTD_IRQHandler()
{
    // press SW1
    if((PORTC->PCR[3] & (PORT_PCR_ISF_MASK)) == PORT_PCR_ISF_MASK)
    {   
        Status1 = 1;
        Status2 = 0;
    }
  
    // press WS2
    if((PORTC->PCR[12] & (PORT_PCR_ISF_MASK)) == PORT_PCR_ISF_MASK)
    {       
        Status1 = 0;
        Status2 = 1;
    }
}


void UART0_DriverIRQHandler()
{
    /* code */
    FirmReceiveOk = 0;
   
    // recevie data
    tempRX = UART0_ReceiveData();
    
    // check data 
    if(((47 <tempRX) && (tempRX < 58)) || ((64 <tempRX) && (tempRX < 71)))
    {
      // write to ring buffer
      RingQueue_Push(&RX_BUF , tempRX);
      
      DataRecevie++;
    }
    // check end of file
    else if(tempRX == 'P')
    {
        FirmReceiveOk = 1;
    }  
    
    /* Clear Pending Interrupt */
    NVIC_ClearPendingIRQ(UART0_IRQn);
}


void bootloader(uint32_t AppAddress)
{
    /* Turn off all interrup */
    All_DeInit();

    __set_MSP(*((uint32_t*)AppAddress));

    /* ensuring this instruction is completed */
    __DMB();
    SCB->VTOR = AppAddress;
    __DSB();
    
    /* Jump to Reset Handler */
    uint32_t JumpAddress = *((volatile uint32_t*) (AppAddress + 4));
    
    void (*reset_handler)(void)= (void(*)(void))JumpAddress;
       
    /* reset MCU */
    reset_handler();
}


// load FIRM program
void Load_Firm()
{   
  // check flag set to 1 when recevie full data
  while(FirmReceiveOk == 0)
    {
        if(!RingQueue_Empty(&RX_BUF))
          {                            
            if(DataRecevie == 12)
            {              
              RingQueue_Pop(&RX_BUF , &txData);
              
              DataWriteFlash = (uint32_t)(Char2Dec(txData) << 28);
            }
            else if(DataRecevie == 13)  
            {
              RingQueue_Pop(&RX_BUF , &txData);
              DataWriteFlash |= (uint32_t)(Char2Dec(txData) << 24);
            }  
            else if(DataRecevie == 14)
            {  
              RingQueue_Pop(&RX_BUF , &txData);
              DataWriteFlash |= (uint32_t)(Char2Dec(txData) << 20);
            }  
            else if(DataRecevie == 15)
            {  
              RingQueue_Pop(&RX_BUF , &txData);
              DataWriteFlash |= (uint32_t)(Char2Dec(txData) <<16);
            }  
            else if(DataRecevie == 16)
            {  
              RingQueue_Pop(&RX_BUF , &txData);
              DataWriteFlash |= (uint32_t)(Char2Dec(txData) <<12);
            }  
            else if(DataRecevie == 17)
            {  
              RingQueue_Pop(&RX_BUF , &txData);
              DataWriteFlash |= (uint32_t)(Char2Dec(txData) << 8);
            }  
            else if(DataRecevie == 18)
            {  
              RingQueue_Pop(&RX_BUF , &txData);
              DataWriteFlash |= (uint32_t)(Char2Dec(txData) << 4);
            }  
            else if(DataRecevie == 19)
            {  
              RingQueue_Pop(&RX_BUF , &txData);
              DataWriteFlash |= (uint32_t)(Char2Dec(txData));              
              
              Flash_Write(Address, (uint32_t)DataWriteFlash);
              Address = Address + (uint32_t)(0x04U);
              DataRecevie = 11;
            }
            else if(DataRecevie == 1)
            {
                RingQueue_Pop(&RX_BUF , &txData);
                Address |= (uint32_t)(Char2Dec(txData) << 28);
            }
            else if(DataRecevie == 2)
            {
                RingQueue_Pop(&RX_BUF , &txData);
                Address |= (uint32_t)(Char2Dec(txData) << 24);
            }
            else if(DataRecevie == 3)
            {
                RingQueue_Pop(&RX_BUF , &txData);
                Address |= (uint32_t)(Char2Dec(txData) << 20);                              
            }
            else if(DataRecevie == 4)
            {
                RingQueue_Pop(&RX_BUF , &txData);
                Address |= (uint32_t)(Char2Dec(txData) << 16);                              
            }
            else if(DataRecevie == 5)
            {
                RingQueue_Pop(&RX_BUF , &txData);
                Address |= (uint32_t)(Char2Dec(txData) << 12);                              
            }
            else if(DataRecevie == 6)
            {
                RingQueue_Pop(&RX_BUF , &txData);
                Address |= (uint32_t)(Char2Dec(txData) << 8);                              
            }
            else if(DataRecevie == 7)
            {
                RingQueue_Pop(&RX_BUF , &txData);
                Address |= (uint32_t)(Char2Dec(txData) << 4);                              
            }
            else if(DataRecevie == 8)
            {
                RingQueue_Pop(&RX_BUF , &txData);
                Address |= (uint32_t)(Char2Dec(txData));                  
            }
            else if(DataRecevie == 9)
            {
              
              RingQueue_Pop(&RX_BUF , &txData);
              TotalSector |= (uint32_t)(Char2Dec(txData) << 4);               
            }
            else if(DataRecevie == 10)
            {
              
              RingQueue_Pop(&RX_BUF , &txData);
              TotalSector |= (uint32_t)(Char2Dec(txData)); 
              for(uint8_t i = 0; i <TotalSector; i++)
              {
                  Flash_Erase(Address + (i * 1024));
              }
              UART0_TransmitBuffer(EraseOk, 8);
              DataRecevie = 11;  
            }
                   
          }             
  }
  
  // clear all flag
  DataWriteFlash = 0;
  DataRecevie = 0;
  Address = 0;
  TotalSector = 0;
  FirmReceiveOk = 0;
  TotalSector = 0;
  
  // sent flag 
  UART0_TransmitBuffer(FIRMOK, 7);  
}

// deinit all peripheral
void All_DeInit()
{
  __disable_irq();
  /* Turn off systick */
  SysTick->CTRL = 0x0;
  SysTick->LOAD = 0;
    
  PORT_DeInit();
  
  GPIO_DeInit();
  
  UART0_DeInit();
  
  Clock_DeInit();
  
  // clear pending
    NVIC_ClearPendingIRQ(UART0_IRQn);
    NVIC_ClearPendingIRQ(PORTC_PORTD_IRQn);
    
    // disable interrup 
    NVIC_DisableIRQ(UART0_IRQn);
    NVIC_DisableIRQ(PORTC_PORTD_IRQn); 
    
    
}
