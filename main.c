#include <REGX52.H>
#include "Delay.h"
#include "LCD1602.h"
#include "Timer0.h"

unsigned char Sec=58,Min=59,Hour=23;
bit PauseFlag = 1; // 启动标志
bit ResetFlag = 1; // 重置标志
bit KeyPauseEventP3_0 = 0; // 暂停事件
bit KeyPauseEventP3_1 = 0; // 重置事件

static unsigned int T0Count1000; // 周期1

void KeyScanP3(){
	static bit LastState_PauseP3_0 = 1;
	static bit LastState_PauseP3_1 = 1;
	
	// P3_0暂停事件处理
	if(P3_0 == 0 && LastState_PauseP3_0 == 1){
		//Delay(10);
		P2_1 = 0;
		if(P3_0 == 0){
			KeyPauseEventP3_0 = 1; // 记录事件
			LastState_PauseP3_0 = 0; // 改状态，防止重复触发
		}
	}
	else if(P3_0 == 1){
		LastState_PauseP3_0 = 1; // 松开后恢复检测
		//P2_1 = 1;
	}
	
	// P3_1重置事件处理
	if(P3_1 == 0 && LastState_PauseP3_1 == 1){
		//Delay(10);
		//P2_0 = 0;
		if(P3_1 == 0){
			KeyPauseEventP3_1 = 1; // 记录事件
			LastState_PauseP3_1 = 0; // 改变状态，防止重复触发
		}
	}
	else if(P3_1 == 1){
		LastState_PauseP3_1 = 1; //松开后恢复检测
		//P2_0 = 1;
	}
}

void main(){
	// 初始化
	LCD_Init();
	Timer0Init();
	
	// 显示字符串，
	LCD_ShowString(1,1,"Stopwatch:");
	LCD_ShowString(2,1,"  :  :");
	while(1){
		
		// 暂停计时
		KeyScanP3();
		if(KeyPauseEventP3_0){
			PauseFlag = !PauseFlag; // 切换暂停状态
			KeyPauseEventP3_0 = 0; // 清除事件
			// 显示暂停状态
			if(!PauseFlag){
				LCD_ShowString(1,11,"Pause!");
			}
			else{
				LCD_ShowString(1,11,"      ");
			}
		}
		else if(P3_0 == 1 && P3_2 == 1){
			P2_1 = 1;
		}
		
		LCD_ShowNum(2,1,Hour,2);
		LCD_ShowNum(2,4,Min,2);
		LCD_ShowNum(2,7,Sec,2);
		
		// 重置按钮
		if(KeyPauseEventP3_1){
			 //Delay(10);
			KeyPauseEventP3_1 = 0;
			if(P3_1 == 0){
				P2_0 = 0;
				T0Count1000 = 0;
				Sec = 0;
				Min = 0;
				Hour = 0;
			}
		}
		
//		if(P3_1 == 0 && KeyPauseEventP3_1 == 0){
//			KeyPauseEventP3_1 = 1;
//			P2_0 = 0;
//			Delay(20);
//			if(P3_1 == 0){
//				Sec = 0;
//				Min = 0;
//				Hour = 0;
//			}
//		}

		else if(P3_1 == 1){
			KeyPauseEventP3_1 = 0;
			P2_0 = 1;
		} // end
		
		// 亮灯测试
		if(P3_2 == 0){
			P2 = 0;
		}
		else if(PauseFlag == 0){
			P2_2 = 1;
			P2_3 = 1;
		}
		
	} // while end
}

void Timer0_Routine() interrupt 1{
	
	static unsigned int T0CountP2_4;
	static unsigned char flashCount = 0;
	static unsigned char Light = 0;
	TL0 = 0x66;		//设置定时初值
	TH0 = 0xFC;		//设置定时初值
	T0Count1000++;
	T0CountP2_4++;

	if(T0Count1000 >= 1000){ // 周期1
		T0Count1000 = 0;
		if(PauseFlag){
			P2_2 = 0; // 闪烁灯 秒
			flashCount = 80; // 设置亮灯时间
			
			Sec++;
			if(Sec >= 60){
				Sec = 0;
				P2_2 = 0; // 闪烁灯 分
				P2_3 = 0;
				Min++;
				if(Min >= 60){
					Min = 0;
					P2_4 = 0; // 闪烁灯 时
					P2_3 = 0;
					P2_2 = 0;
					P2_1 = 0;
					P2_0 = 0;
					Hour++;
					if(Hour >= 24){
						Hour = 0;
					}
				}
			}
		} // LED end
	} // 1000ms 计时 end
	if(flashCount > 0){
		flashCount--;
		if(flashCount == 0){
			P2_4 = 1; // 熄灭
			P2_3 = 1;
			P2_2 = 1;
//			P2_1 = 1;
//			P2_0 = 1;
		}
	}
	
	if(T0CountP2_4 >= 1100){ // 周期2
		T0CountP2_4 = 0;
		Light = 20; // 亮灯时长
	}

	if(Light > 0){
		P2_4 = 0;
		Light--;
		if(Light == 0){
			P2_4 = 1;
		}
	}
}