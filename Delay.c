
#include <INTRINS.H>
void Delay(unsigned int xms)		//@11.0592MHz
{
	unsigned char i, j;
	while(xms){
		_nop_();
		i = 2;
		j = 199;
		do
		{
			while (--j);
		} while (--i);
		
		xms--;
	}

}

//void Delay1ms()		//@12.000MHz
//{
//	unsigned char i, j;

//	i = 2;
//	j = 239;
//	do
//	{
//		while (--j);
//	} while (--i);
//}
