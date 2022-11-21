#include "sys_delay.h"

volatile uint32_t uwTick;
static Sys_TickFreqTypeDef uwTickFreq = SYS_TICK_FREQ_DEFAULT;  /* 1KHz */
static void Sys_IncTick(void);
static uint32_t Sys_GetTick(void);

/* Public Delay Function for Users */
void Sys_InitTick(uint32_t TickPriority)
{
    uint32_t ticks = SystemCoreClock / (1000U / uwTickFreq);

    SysTick->LOAD  = (uint32_t)(ticks - 1UL);                         /* set reload register */
    NVIC_SetPriority(SysTick_IRQn, TickPriority);
    SysTick->VAL   = 0UL;                                             /* Load the SysTick Counter Value */
    
    SysTick->CTRL  = SysTick_CTRL_CLKSOURCE_Msk |
                     SysTick_CTRL_TICKINT_Msk   |
                     SysTick_CTRL_ENABLE_Msk;                         /* Enable SysTick IRQ and SysTick Timer */    
    /* Enable Systick Interrupt */
    NVIC_EnableIRQ(SysTick_IRQn);
}

void Sys_Delay(uint32_t Delay)
{
    uint32_t tickstart = Sys_GetTick();
    uint32_t wait = Delay;

    /* Add a freq to guarantee minimum wait */
    if (wait < SYS_MAX_DELAY)
    {
            wait += (uint32_t)(uwTickFreq);
    }

    Sys_InitTick(0);
    while((Sys_GetTick() - tickstart) < wait);
    SysTick->CTRL &= ~SysTick_CTRL_ENABLE_Msk;
}

void SysTick_Handler(void)
{
    Sys_IncTick();
    NVIC_ClearPendingIRQ(SysTick_IRQn);
}

static uint32_t Sys_GetTick(void)
{
    return uwTick;
}

static void Sys_IncTick(void)
{
    uwTick += uwTickFreq;
}

