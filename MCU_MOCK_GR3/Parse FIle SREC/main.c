#include"header.h"

int main()
{
	FILE *fptr1=fopen("APP18000.srec","r");/* open FPTR1 */
	FILE *fptr2=fopen("Output.txt","w");/* open FPTR2 */
	char str[SIZE_STRING];
	int i, line=0;/* count line */
	int AddrNew=0,  AddrOld=0;
	int CntNew=0, CntOld=0;
	while(fgets(str,SIZE_STRING,fptr1)!=NULL)
	{
		Return_Addr_Cnt(str,&AddrNew,&CntNew,&line);
		printf("\nAddNew:%d",AddrNew);
		printf("\nAddOld:%d",AddrOld);
		printf("\nCNTNew:%d",CntNew);
		printf("\nCNTOld:%d",CntOld);
		//line++;
		if(line>1)
		{
			//check Address distance
			int Distance = AddrNew - AddrOld;
			printf("\nDistance:%d\n",Distance);
			if(Distance == 16){
				Export_Data_In_File(fptr2,str);/* export data to Output.txt */
			}
			else
			{
				//check count empty
				int ByteEmpty= 0x13-CntOld + Distance - 0x10;
				//export data FF with 1 byte emty
				printf("\nBYTEempty:%d\n",ByteEmpty);
				Export_Byte_Empty(fptr2,str,ByteEmpty);

				Export_Data_In_File(fptr2,str);/* export data to Output.txt */
			}
		}
		else
		{
			Export_Data_In_File(fptr2,str);/* export data to Output.txt */
		}
		AddrOld=AddrNew;
		CntOld=CntNew;

	}
	fputc('P',fptr2);
	fclose(fptr2);/* close FPTR2 */
	fclose(fptr1);/* close FPRT1 */
	return 0;
}


