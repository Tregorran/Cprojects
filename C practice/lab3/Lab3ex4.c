#include <stdio.h>
#include <math.h>
#include <string.h>

void hexToDecimal(char HexNum[]);

//main method that passes in the arrays to the function
int main()
{ 
	char HexNum[] = "FF";
	hexToDecimal(HexNum);
	
	char HexNum2[] = "10";
	hexToDecimal(HexNum2);
	
	char HexNum3[] = "ABC";
	hexToDecimal(HexNum3);
	
	char HexNum4[] = "C2";
	hexToDecimal(HexNum4);
	
	char HexNum5[] = "-AB";
	hexToDecimal(HexNum5);
	return 0;
}

//function which convert hexidecial to decimal
void hexToDecimal(char HexNum[])
{
	double result =0;
	int sign = 1;
	int Numbers[3];
	int k = 0;
	//arry which has all the hexidecimal values in
	char array[18] = {'0','1','2','3','4','5','6','7','8','9','A','B','C','D','E','F',' ','\0'};
	
	//checks for a minus sign, gets actual values of hex equivalent and stores it backwards in another array
	for (int i=strlen(HexNum); i>-1; i--)
	{
			if (!(HexNum[i] == '-'))
			{
				for (int j=0; j<17; j++)
				{
					if (array[j] == HexNum[i])
					{
						Numbers[k] = j;
						k+=1;
					}
				}
			}
			else 
			{
				sign = -1;
			}
	}
	//if there is a minus sign calculate into decimal but not using the minus sign
	if (sign == -1)
	{
		for (int l=0; l<strlen(HexNum)-1; l++)
		{
			result += Numbers[l] * pow(16,l);
		}
	}
	//if there is not a minus sign calculate into decimal
	else
	{
		for (int l=0; l<strlen(HexNum); l++)
		{
		result += Numbers[l] * pow(16,l);
		}
	}
	//add sign by multiplying by minus 1
	result = result * sign;
	printf("%.f\n",result);	
	return;
}