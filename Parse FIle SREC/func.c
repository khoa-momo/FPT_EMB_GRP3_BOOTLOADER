
#include"header.h"


void Return_Addr_Cnt(char *str, int *Addr, int *Cnt,int *line)
{
	int check_type=(str[1]=='1')|(str[1]=='2')|(str[1]=='3');
	int type=(str[1]>53)?(str[1]-53):(53-str[1]);
	type=(type==3||type==1)?6:(type==2)?8:4;
	int i,shift=0;
	int addr=0;//address value
	for(i=4;i<type+4;i++){
		addr=(addr<<shift)+Read_One_Byte(str[i],str[i+1]);
		shift+=8;
		i++;
		}
	if(check_type&(addr!=0x400))
	{
		//read count
		*Cnt=Read_One_Byte(str[2],str[3]);
		//read address
		*Addr=addr;
		(*line)++;
	}
}
void Export_Data_In_File(FILE *fptr, char *str)
{
	int addr=0;
	int count=Read_One_Byte(str[2],str[3]);
	int type=(str[1]>53)?(str[1]-53):(53-str[1]);
	type=(type==3||type==1)?6:(type==2)?8:4;
	int i,shift=0;
	//read address
	for(i=4;i<type+4;i++){
		addr=(addr<<shift)+Read_One_Byte(str[i],str[i+1]);
		shift+=8;
		i++;
		}
	int check_type=(str[1]=='0')|(str[1]=='7')|(str[1]=='8')|(str[1]=='9');
	if(check_type|(addr == 0x400))
	{}
	else{
		for(i=type+4;i<(count*2+2);i++){
			fputc(str[i],fptr);
		}
		fputc('\n',fptr);
	}
}
//export Byte empty
void Export_Byte_Empty(FILE *fptr, char *str,int byte)
{
	int i;
	for(i=0;i<byte*2;i++)
	{
		fputc('F',fptr);
	}
	fputc('\n',fptr);
}


int Read_One_Byte(int a,int b)
{
	return ((a>'9'?CharToNum(a):NumToNum(a))*16)+(b>'9'?CharToNum(b):NumToNum(b));
}



