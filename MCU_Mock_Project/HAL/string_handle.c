#include "string_handle.h"

/* This function swaps values pointed by xp and yp */
static void Str_Swap(uint8_t *xp, uint8_t *yp)
{
    uint8_t temp = *xp;
    *xp = *yp;
    *yp = temp;
}

/* A utility function to reverse a string  */
static void Str_Reverse(uint8_t * str, uint32_t length)
{
    uint32_t start = 0;
    uint32_t end = length -1;
    while (start < end)
    {
        Str_Swap(&str[start], &str[end]);
        start++;
        end--;
    }
}

/*
 * Implementation of itoa() - Convert integer to String (Base = 10 or 16)
 */
uint8_t* itoa(uint32_t num, uint8_t* str, uint32_t base)
{
    uint32_t i = 0;
    bool isNegative = false;
 
    /* Handle 0 explicitly, otherwise empty string is printed for 0 */
    if (num == 0)
    {
        str[i++] = '0';
        str[i] = '\0';
        return str;
    }
 
    // In standard itoa(), negative numbers are handled only with
    // base 10. Otherwise numbers are considered unsigned.
    if (num < 0 && base == 10)
    {
        isNegative = true;
        num = -num;
    }
 
    // Process individual digits
    while (num != 0)
    {
        int rem = num % base;
        str[i++] = (rem > 9)? (rem-10) + 'a' : rem + '0';
        num = num/base;
    }
 
    // If number is negative, append '-'
    if (isNegative)
        str[i++] = '-';
 
    str[i] = '\0'; // Append string terminator
 
    // Reverse the string
    Str_Reverse(str, i);
 
    return str;
}

/*
 * Implementation of Char2Dec() - Convert character to Decimal
 * Intput = '0' to '9' and 'A' to 'F'
 * If Invalid input, program will be corrupted
 */
uint8_t Char2Dec(uint8_t chardata)
{
	uint8_t retVal;
	
	if ( (47 <chardata) && (chardata < 58) )
	{
		retVal = chardata - 48;
	}
	else if ( (64 < chardata) && (chardata < 71) )
	{
		retVal = chardata - 55;
	}
	else
	{
		while(1);
	}
	
	return retVal;
}
