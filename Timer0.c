#include <REGX52.H>

  /**
    * @brief	定时器0初始化，1毫秒@11.0592MHz
	* @param	无
	* @retval 	无
	*/
void Timer0Init(void)	{ //1毫秒@11.0592MHz
	//AUXR &= 0x7F;		//定时器时钟12T模式
	// 寄存器 TMOD  不可位寻址 需要整体赋值
	//TMOD = 0x01; // 0000 0001
	//设置定时器模式
	TMOD = TMOD & 0xF0; // 把TMOD 的低4位清零，高4位保持不变
	TMOD = TMOD | 0x01; // 把TMOD 的最低位置1，高4位保持不变
	
	// 控制寄存器 TCON
	TL0 = 0x66;		//设置定时初值
	TH0 = 0xFC;		//设置定时初值
	TF0 = 0;		//清除TF0标志
	TR0 = 1;		//定时器0开始计时
	
	ET0 = 1;
	EA = 1;
	PT0 = 0;

}


/* 定时器中断函数模板
void Timer0_Routine() interrupt 1{
	static unsigned int T0Count;
	TL0 = 0x66;		//设置定时初值
	TH0 = 0xFC;		//设置定时初值
	T0Count ++;
	if(T0Count >= 1000){
		T0Count = 0;
		
	}
}

*/