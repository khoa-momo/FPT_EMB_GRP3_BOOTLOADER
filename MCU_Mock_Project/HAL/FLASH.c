#include "FLASH.h"







void Flash_Write(uint32_t Address, uint32_t Data)
{
    
        /* Wait Previous Command Complete? */
        while ((FTFA->FSTAT & FTFA_FSTAT_CCIF_MASK) == 0);
        
        /* Check Error from Previous Command */
        if ( ((FTFA->FSTAT & FTFA_FSTAT_ACCERR_MASK) == FTFA_FSTAT_ACCERR_MASK) \
            || ((FTFA->FSTAT & FTFA_FSTAT_FPVIOL_MASK) == FTFA_FSTAT_FPVIOL_MASK) )
        {
            FTFA->FSTAT = FTFA_FSTAT_CLEAR_ERROR;
        }
        
        /* Write Command and Parameter to FCCOB Registers */
        FTFA->FCCOB0 = CMD_PROGRAM_LONGWORD;
        FTFA->FCCOB1 = (uint8_t)((Address) >> 16);
        FTFA->FCCOB2 = (uint8_t)((Address) >> 8);
        FTFA->FCCOB3 = (uint8_t)(Address);
        
        FTFA->FCCOB4 = (uint8_t)Data;
        FTFA->FCCOB5 = (uint8_t)(Data >> 8);
        FTFA->FCCOB6 = (uint8_t)(Data >> 16);
        FTFA->FCCOB7 = (uint8_t)(Data >> 24);
        
        __disable_irq();
        /* Clear CCIF Flag to launch command */
        FTFA->FSTAT = START_COMMAND;
        while ((FTFA->FSTAT & FTFA_FSTAT_CCIF_MASK) == 0);
        __enable_irq();
    
}



void Flash_Erase(uint32_t Address)
{
    
    /* Wait Previous Command Complete? */
    while ((FTFA->FSTAT & FTFA_FSTAT_CCIF_MASK) == 0);
    
    /* Check Error from Previous Command */
    if ( ((FTFA->FSTAT & FTFA_FSTAT_ACCERR_MASK) == FTFA_FSTAT_ACCERR_MASK) \
        || ((FTFA->FSTAT & FTFA_FSTAT_FPVIOL_MASK) == FTFA_FSTAT_FPVIOL_MASK) )
    {
        FTFA->FSTAT = FTFA_FSTAT_CLEAR_ERROR;
    }
    
    /* Write Command and Parameter to FCCOB Registers */
    FTFA->FCCOB0 = CMD_ERASE_SECTOR;
    
    FTFA->FCCOB1 = (uint8_t)(Address >> 16);
    FTFA->FCCOB2 = (uint8_t)(Address >> 8);
    FTFA->FCCOB3 = (uint8_t)Address;
    
    
    /* Clear CCIF Flag to launch command */
    __disable_irq();
    FTFA->FSTAT = START_COMMAND;
    while ((FTFA->FSTAT & FTFA_FSTAT_CCIF_MASK) == 0);
    __enable_irq();
}



uint32_t Flash_Read(uint32_t Address)
{
     return *(uint8_t *)(Address);
}


