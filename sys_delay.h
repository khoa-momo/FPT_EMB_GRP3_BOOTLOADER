#ifndef __SYS_DELAY_H__
#define __SYS_DELAY_H__

#include "MKL46Z4.h"

#define SYS_MAX_DELAY      0xFFFFFFFFU

typedef enum
{
  SYS_TICK_FREQ_10HZ         = 100U,
  SYS_TICK_FREQ_100HZ        = 10U,
  SYS_TICK_FREQ_1KHZ         = 1U,
  SYS_TICK_FREQ_DEFAULT      = SYS_TICK_FREQ_1KHZ
} Sys_TickFreqTypeDef;

/* Function Prototypes */
void Sys_InitTick(uint32_t TickPriority);
void Sys_Delay(uint32_t Delay);

#endif	/* __SYS_DELAY_H__ */
