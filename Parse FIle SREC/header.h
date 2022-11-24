#ifndef __HEADER_H__
#define __HEADER_H__

#include<stdio.h>
#include<stdlib.h>
#include<string.h>


#define SIZE_STRING 	80
#define NumToNum(x) 	((x)-48)
#define CharToNum(x)	((x)-55)



void Return_Addr_Cnt(char *str, int *Addr, int *Cnt,int *line);
void Export_Data_In_File(FILE *fptr, char *str);
void Export_Byte_Empty(FILE *fptr, char *str,int byte);
int Read_One_Byte(int a,int b);

#endif

