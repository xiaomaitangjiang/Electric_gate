#include "menu.h"

/*
waring
不要随意修改或重构
除非具有完美方案
（写的很烂）
*/


//page,line,left,right,enter
menu menulist[]=
{
	{0,0,1,5,1},//主页
	{1,1,2,0,1},//定时
	{2,1,3,1,1},//安全设置
	{3,0,4,2,0},//故障显示
	{4,0,5,3,0},//温度，湿度，露点显示
	{5,0,0,4,0},//关于
};




	



void page0(int index,int sw_state)
{
	if(index==0)
	{
	OLED_Clear();
	OLED_ShowCHinese(0,0,0,0);//电
  OLED_ShowCHinese(12,0,1,0);//压
  OLED_ShowCHinese(24,0,27,0);//:
	OLED_Showdecimal(32,0,V,3,2,12,0);//V
	OLED_ShowCHinese(0,2,0,0);//电
  OLED_ShowCHinese(12,2,2,0);//流
  OLED_ShowCHinese(24,2,27,0);//:
	OLED_Showdecimal(32,2,C,3,2,12,0);//C
	OLED_ShowCHinese(0,4,3,0);//功
  OLED_ShowCHinese(12,4,4,0);//率
  OLED_ShowCHinese(24,4,27,0);//:
	OLED_Showdecimal(32,4,P,3,2,12,0);//P
	OLED_ShowCHinese(0,6,9,0);//累
	OLED_ShowCHinese(12,6,10,0);//计
	OLED_ShowCHinese(24,6,5,0);//用
	OLED_ShowCHinese(36,6,0,0);//电
	OLED_ShowCHinese(48,6,27,0);//:
	OLED_Showdecimal(60,6,E_con,3,2,12,0);//E_con
	OLED_ShowCHinese(102,1,11,sw_state);//开
	OLED_ShowCHinese(114,1,23,sw_state);//启
	OLED_ShowCHinese(102,3,21,!sw_state);//关
  OLED_ShowCHinese(114,3,22,!sw_state);//闭
	}

}
	
void page1(int index,int line, int choise)
{
	int turn1=0;
	int turn2=0;
	
	
	if(choise==1)	
	{
		switch (line)
		{
			case 1:
				turn1=1;
			break;
			case 2:
				turn2=1;
			break;
		}
		
	}
	if (index==1)
	{
	OLED_Clear();	
	OLED_ShowCHinese(13,0,19,turn1);//定
	OLED_ShowCHinese(25,0,20,turn1);//时
	OLED_ShowCHinese(37,0,21,turn1);//关
	OLED_ShowCHinese(49,0,22,turn1);//闭
	OLED_ShowCHinese(13,2,19,turn2);//定
	OLED_ShowCHinese(25,2,20,turn2);//时
	OLED_ShowCHinese(37,2,23,turn2);//启
	OLED_ShowCHinese(49,2,24,turn2);//动
	OLED_ShowCHinese(0,line*2-2,40,0);//->
	OLED_ShowNum(110,0,time_close,2,12,0);/*关闭时间*/
	OLED_ShowNum(110,2,time_on,2,12,0);/*开启时间*/
		
	}
}

void page2(int index,int line,int choise)
{
	int turn1=0;
	int turn2=0;
	int turn3=0;
	
	
	if(choise==1)	
	{
		switch (line)
		{
			case 1:
				turn1=1;
			break;
			case 2:
				turn2=1;
			break;
			case 3:
				turn3=1;
			break;
		}
		
	}
	if(index==2)
	{
	OLED_Clear();	
	OLED_ShowCHinese(13,0,38,turn1);//最
	OLED_ShowCHinese(25,0,39,turn1);//大
	OLED_ShowCHinese(37,0,0,turn1);//电
	OLED_ShowCHinese(49,0,1,turn1);//压
	OLED_ShowCHinese(13,2,38,turn2);//最
	OLED_ShowCHinese(25,2,39,turn2);//大
	OLED_ShowCHinese(37,2,0,turn2);//电
	OLED_ShowCHinese(49,2,2,turn2);//流
	OLED_ShowCHinese(13,4,38,turn3);//最
	OLED_ShowCHinese(25,4,39,turn3);//大
	OLED_ShowCHinese(37,4,32,turn3);//实
	OLED_ShowCHinese(49,4,33,turn3);//时
  OLED_ShowCHinese(61,4,3,turn3);//功
  OLED_ShowCHinese(73,4,4,turn3);//率	
	OLED_ShowCHinese(0,line*2-2,40,0);//->
	OLED_ShowNum(86,0,V_set,3,12,0);/*最大电压*/
	OLED_ShowNum(86,2,C_set,5,12,0);/*最大电流*/
	OLED_ShowNum(86,4,E_con_set,5,12,0);/*最大实时功率*/

	}
}

void page3(int index,int type)
{
	if (index==3)
	{
		OLED_Clear();	
		switch(type)
		{
			case 1:
				OLED_ShowCHinese(0,6,54,0);//寄
				OLED_ShowCHinese(12,6,55,0);//存
				OLED_ShowCHinese(24,6,56,0);//器
				OLED_ShowCHinese(36,6,34,0);//错
			  OLED_ShowCHinese(48,6,35,0);//误
			  OLED_ShowCHinese(60,6,67,0);//清除符
			break;
			case 2:
				OLED_ShowCHinese(0,6,57,0);//误
				OLED_ShowCHinese(12,6,58,0);//差
				OLED_ShowCHinese(24,6,59,0);//修
				OLED_ShowCHinese(36,6,60,0);//正
			  OLED_ShowCHinese(48,6,61,0);//失
			  OLED_ShowCHinese(60,6,62,0);//效
			break;
			case 4:
				OLED_ShowCHinese(0,6,0,0);//电
				OLED_ShowCHinese(12,6,1,0);//压
				OLED_ShowCHinese(24,6,45,0);//过
				OLED_ShowCHinese(36,6,39,0);//大
			  OLED_ShowCHinese(48,6,67,0);//清除符
			  OLED_ShowCHinese(60,6,67,0);//清除符
			break;
			case 5:
				OLED_ShowCHinese(0,6,0,0);//电
				OLED_ShowCHinese(12,6,2,0);//流
				OLED_ShowCHinese(24,6,45,0);//过
				OLED_ShowCHinese(36,6,39,0);//大
			  OLED_ShowCHinese(48,6,67,0);//清除符
			  OLED_ShowCHinese(60,6,67,0);//清除符
			break;
			case 6:
				OLED_ShowCHinese(0,6,3,0);//功
				OLED_ShowCHinese(12,6,4,0);//率
				OLED_ShowCHinese(24,6,45,0);//过
				OLED_ShowCHinese(36,6,39,0);//大
			  OLED_ShowCHinese(48,6,67,0);//清除符
			  OLED_ShowCHinese(60,6,67,0);//清除符
			break;
			case 7:
				OLED_ShowCHinese(0,6,65,0);//达
				OLED_ShowCHinese(12,6,66,0);//到
				OLED_ShowCHinese(24,6,25,0);//设
				OLED_ShowCHinese(36,6,26,0);//置
			  OLED_ShowCHinese(48,6,7,0);//度
			  OLED_ShowCHinese(60,6,8,0);//数
			case 9:
				OLED_ShowCHinese(0,6,45,0);//过
				OLED_ShowCHinese(12,6,64,0);//载
			  OLED_ShowCHinese(24,6,67,0);//清除符
			  OLED_ShowCHinese(36,6,67,0);//清除符
			  OLED_ShowCHinese(48,6,67,0);//清除符
			  OLED_ShowCHinese(60,6,67,0);//清除符
			break;
			case 8:
				OLED_ShowCHinese(0,6,42,0);//短
				OLED_ShowCHinese(12,6,43,0);//路
			  OLED_ShowCHinese(24,6,67,0);//清除符
			  OLED_ShowCHinese(36,6,67,0);//清除符
			  OLED_ShowCHinese(48,6,67,0);//清除符
			  OLED_ShowCHinese(60,6,67,0);//清除符
			break;
			case 3:
				OLED_ShowCHinese(0,6,41,0);//漏
				OLED_ShowCHinese(12,6,0,0);//电
			  OLED_ShowCHinese(24,6,67,0);//清除符
			  OLED_ShowCHinese(36,6,67,0);//清除符
			  OLED_ShowCHinese(48,6,67,0);//清除符
			  OLED_ShowCHinese(60,6,67,0);//清除符
			break;
		}
		OLED_ShowCHinese(0,0,0,0);//电
	  OLED_ShowCHinese(12,0,43,0);//路
		OLED_ShowCHinese(24,0,52,0);//频
	  OLED_ShowCHinese(36,0,53,0);//率
		
		OLED_ShowCHinese(0,3,34,0);//错
	  OLED_ShowCHinese(12,3,35,0);//误
		OLED_ShowCHinese(24,3,27,0);//:
		
		/*频率*/
		OLED_ShowNum(86,0,(int)HZ,2,12,0);
	}
}

void page5(int index)
{
	if (index==5)
	{
		OLED_Clear();	
		OLED_DrawBMP(0,0,45,7,gImage_1,0);
		OLED_ShowCHinese(48,0,11,0);//开
	  OLED_ShowCHinese(60,0,12,0);//发
	  OLED_ShowCHinese(72,0,13,0);//者
		OLED_ShowCHinese(84,0,27,0);//:
		OLED_ShowString(96,0,"Wesp",12,0);
		OLED_ShowCHinese(48,3,14,0);//版
	  OLED_ShowCHinese(60,3,15,0);//本
	  OLED_ShowCHinese(72,3,16,0);//号
		OLED_ShowCHinese(84,3,27,0);//:
		OLED_ShowString(96,3,"1.0",12,0);
	}
}

void page4(int index)
{
	
	
	if(index==4)
	{
	OLED_Clear();	
	OLED_ShowCHinese(13,0,47,0);//温
	OLED_ShowCHinese(25,0,48,0);//度
		
	OLED_ShowCHinese(13,2,49,0);//湿
	OLED_ShowCHinese(25,2,48,0);//度
		
	OLED_ShowCHinese(13,4,50,0);//结
	OLED_ShowCHinese(25,4,51,0);//露
	OLED_ShowCHinese(37,4,47,0);//温
	OLED_ShowCHinese(49,4,48,0);//度
		
	OLED_ShowCHinese(13,6,68,0);//外
	OLED_ShowCHinese(25,6,69,0);//部
	OLED_ShowCHinese(37,6,47,0);//温
	OLED_ShowCHinese(49,6,48,0);//度	
	OLED_ShowNum(86,0, RH ,2,12,0);/*湿度*/
	OLED_ShowNum(86,2, temperature ,2,12,0);/*温度*/
	OLED_ShowNum(86,4, Td ,2,12,0);	/*结露温度*/
	OLED_ShowNum(86,6, T ,2,12,0);	/*结露温度*/
	}
}		



	




void menuprocess(void)
{
	// 读取按键状态
        GPIO_PinState key1State = HAL_GPIO_ReadPin(GPIOB, GPIO_PIN_0); // KEY1 left
        GPIO_PinState key2State = HAL_GPIO_ReadPin(GPIOA, GPIO_PIN_7); // KEY2  right
        GPIO_PinState key3State = HAL_GPIO_ReadPin(GPIOB, GPIO_PIN_1); // KEY3  enter
        GPIO_PinState key4State = HAL_GPIO_ReadPin(GPIOB, GPIO_PIN_12); // KEY4  line

	
if (caring==0&&key1State == GPIO_PIN_RESET && key2State == GPIO_PIN_RESET && key3State == GPIO_PIN_RESET )
{
	HAL_Delay(10);
	caring=1;	
	while (key1State == GPIO_PIN_RESET || key2State == GPIO_PIN_RESET  || key3State == GPIO_PIN_RESET)
	{
   key1State = HAL_GPIO_ReadPin(GPIOB, GPIO_PIN_0); // KEY1
   key2State = HAL_GPIO_ReadPin(GPIOA, GPIO_PIN_7); // KEY2
   key3State = HAL_GPIO_ReadPin(GPIOB, GPIO_PIN_1); // KEY3

	}
}
else if (caring==1&&key1State == GPIO_PIN_RESET && key2State == GPIO_PIN_RESET  && key3State == GPIO_PIN_RESET )
{
	HAL_Delay(10);
	caring=0;
	while (key1State == GPIO_PIN_RESET || key2State == GPIO_PIN_RESET || key3State == GPIO_PIN_RESET)
	{
   key1State = HAL_GPIO_ReadPin(GPIOB, GPIO_PIN_0); // KEY1
   key2State = HAL_GPIO_ReadPin(GPIOA, GPIO_PIN_7); // KEY2
   key3State = HAL_GPIO_ReadPin(GPIOB, GPIO_PIN_1); // KEY3

	}
}
        if(enter==0)
				{
					// 处理按键事件
        if (key1State == GPIO_PIN_RESET || key2State == GPIO_PIN_RESET || key4State == GPIO_PIN_RESET || key3State == GPIO_PIN_RESET ) {
            HAL_Delay(25); // 消抖延时

            if (key1State == GPIO_PIN_RESET) {
                currentMenuIndex = menulist[currentMenuIndex].left; // 向左选择
                while (key1State == GPIO_PIN_RESET) {
                    key1State = HAL_GPIO_ReadPin(GPIOB, GPIO_PIN_0); // KEY1连接到GPIOA的7引脚
                }
							
            }

            if (key2State == GPIO_PIN_RESET) {
                currentMenuIndex = menulist[currentMenuIndex].right; // 向右选择
                while (key2State == GPIO_PIN_RESET) {
                    key2State = HAL_GPIO_ReadPin(GPIOA, GPIO_PIN_7); // KEY2连接到GPIOB的0引脚
                }
            }

           
						  if (key3State == GPIO_PIN_RESET) {
							while (key3State == GPIO_PIN_RESET) {
                    key3State = HAL_GPIO_ReadPin(GPIOB, GPIO_PIN_1);}
							lineindex=menulist[currentMenuIndex].line++;
				
						}
					 
						if (key4State == GPIO_PIN_RESET) {
                enter = menulist[currentMenuIndex].enter; // 确认
                while (key4State == GPIO_PIN_RESET) {
                    key4State = HAL_GPIO_ReadPin(GPIOB, GPIO_PIN_12); // KEY4连接到GPIOB的11引脚
                }
            }
						
					
						
						
						
         } 
		    }

				else if(enter==1)
				{
					if (key4State == GPIO_PIN_RESET)//取消选择操作
							{
								HAL_Delay(10); // 消抖延时
								enter = 0;
								while (key4State == GPIO_PIN_RESET) {
                    key4State = HAL_GPIO_ReadPin(GPIOB, GPIO_PIN_12); // KEY4连接到GPIOB的11引脚
								}
							}
							if(currentMenuIndex==0)
							{
								HAL_Delay(10); // 消抖延时
								if(cross_user==0)
								{
									cross_user=1;
									enter=0;
								}
								else if(cross_user==1)
								{
									cross_user=0;
									enter=0;
								}
								
							}
					if(currentMenuIndex==1)
					{
						if(menulist[currentMenuIndex].line==1)
						{
							if(key1State == GPIO_PIN_RESET)
							{
								HAL_Delay(10); // 消抖延时
								time_close--;
							  while(key1State == GPIO_PIN_RESET)
							  {key1State = HAL_GPIO_ReadPin(GPIOB, GPIO_PIN_0);}
							}
							
							if(key2State == GPIO_PIN_RESET)
							{
								HAL_Delay(10); // 消抖延时
								time_close++;
							  while(key2State == GPIO_PIN_RESET)
							  {key2State = HAL_GPIO_ReadPin(GPIOA, GPIO_PIN_7);}
							}
							
							if(key3State == GPIO_PIN_RESET)
							{
								HAL_Delay(10); // 消抖延时
								time_close=time_close+10;
							  while(key3State == GPIO_PIN_RESET)
							  {key3State = HAL_GPIO_ReadPin(GPIOB, GPIO_PIN_1);}
							}
							
							if(key1State == GPIO_PIN_RESET && key3State == GPIO_PIN_RESET)
							{
								HAL_Delay(10); // 消抖延时
								time_close=time_close-10;
							  while(key1State == GPIO_PIN_RESET&&key3State == GPIO_PIN_RESET)
							  {
									key1State = HAL_GPIO_ReadPin(GPIOB, GPIO_PIN_0);
								  key3State = HAL_GPIO_ReadPin(GPIOB, GPIO_PIN_1);
								}
							}
							if(time_close+time_on!=time_close)
				      {time_on=0;}
							
						
						}
						if(menulist[currentMenuIndex].line==2)
						{
							
							if(key1State == GPIO_PIN_RESET)
							{
								HAL_Delay(10); // 消抖延时
								time_on--;
							  while(key1State == GPIO_PIN_RESET)
							  {key1State = HAL_GPIO_ReadPin(GPIOB, GPIO_PIN_0);}
							}
							
							if(key2State == GPIO_PIN_RESET)
							{
								HAL_Delay(10); // 消抖延时
								time_on++;
							  while(key2State == GPIO_PIN_RESET)
							  {key2State = HAL_GPIO_ReadPin(GPIOA, GPIO_PIN_7);}
							}
							
							if(key3State == GPIO_PIN_RESET)
							{
								HAL_Delay(10); // 消抖延时
								time_on=time_on+10;
							  while(key3State == GPIO_PIN_RESET)
							  {key3State = HAL_GPIO_ReadPin(GPIOB, GPIO_PIN_1);}
							}
							
							if(key1State == GPIO_PIN_RESET && key3State == GPIO_PIN_RESET)
							{
								HAL_Delay(10); // 消抖延时
								time_on=time_on-10;
							  while(key1State == GPIO_PIN_RESET&&key3State == GPIO_PIN_RESET)
							  {
									key1State = HAL_GPIO_ReadPin(GPIOB, GPIO_PIN_0);
								  key3State = HAL_GPIO_ReadPin(GPIOB, GPIO_PIN_1);
								}
							}
							if(time_close+time_on!=time_on)
				      {time_close=0;}

						
						}
						
					
					}
					if(currentMenuIndex==2)
					{
						if(menulist[currentMenuIndex].line==1)
						{
							if(key1State == GPIO_PIN_RESET)
							{
								HAL_Delay(10); // 消抖延时
								V_set--;
							  while(key1State == GPIO_PIN_RESET)
							  {key1State = HAL_GPIO_ReadPin(GPIOB, GPIO_PIN_0);}
							}
							
							if(key2State == GPIO_PIN_RESET)
							{
								HAL_Delay(10); // 消抖延时
								V_set++;
							  while(key2State == GPIO_PIN_RESET)
							  {key2State = HAL_GPIO_ReadPin(GPIOA, GPIO_PIN_7);}
							}
							
							if(key3State == GPIO_PIN_RESET)
							{
								HAL_Delay(10); // 消抖延时
								V_set=V_set+10;
							  while(key3State == GPIO_PIN_RESET)
							  {key3State = HAL_GPIO_ReadPin(GPIOB, GPIO_PIN_1);}
							}
							
							if(key1State == GPIO_PIN_RESET && key3State == GPIO_PIN_RESET)
							{
								HAL_Delay(10); // 消抖延时
								V_set=V_set-10;
							  while(key1State == GPIO_PIN_RESET&&key3State == GPIO_PIN_RESET)
							  {
									key1State = HAL_GPIO_ReadPin(GPIOB, GPIO_PIN_0);
								  key3State = HAL_GPIO_ReadPin(GPIOB, GPIO_PIN_1);
								}
							}
							
						
						}
						if(menulist[currentMenuIndex].line==2)
						{
							
							if(key1State == GPIO_PIN_RESET)
							{
								HAL_Delay(10); // 消抖延时
								C_set--;
							  while(key1State == GPIO_PIN_RESET)
							  {key1State = HAL_GPIO_ReadPin(GPIOB, GPIO_PIN_0);}
							}
							
							if(key2State == GPIO_PIN_RESET)
							{
								HAL_Delay(10); // 消抖延时
								C_set++;
							  while(key2State == GPIO_PIN_RESET)
							  {key2State = HAL_GPIO_ReadPin(GPIOA, GPIO_PIN_7);}
							}
							
							if(key3State == GPIO_PIN_RESET)
							{
								HAL_Delay(10); // 消抖延时
								C_set=C_set+10;
							  while(key3State == GPIO_PIN_RESET)
							  {key3State = HAL_GPIO_ReadPin(GPIOB, GPIO_PIN_1);}
							}
							
							if(key1State == GPIO_PIN_RESET && key3State == GPIO_PIN_RESET)
							{
								HAL_Delay(10); // 消抖延时
								C_set=C_set-10;
							  while(key1State == GPIO_PIN_RESET&&key3State == GPIO_PIN_RESET)
							  {
									key1State = HAL_GPIO_ReadPin(GPIOB, GPIO_PIN_0);
								  key3State = HAL_GPIO_ReadPin(GPIOB, GPIO_PIN_1);
								}
							}


						
						}
						if(menulist[currentMenuIndex].line==3)
						{
							
							if(key1State == GPIO_PIN_RESET)
							{
								HAL_Delay(10); // 消抖延时
								P_set--;
							  while(key1State == GPIO_PIN_RESET)
							  {key1State = HAL_GPIO_ReadPin(GPIOB, GPIO_PIN_0);}
							}
							
							if(key2State == GPIO_PIN_RESET)
							{
								HAL_Delay(10); // 消抖延时
								P_set++;
							  while(key2State == GPIO_PIN_RESET)
							  {key2State = HAL_GPIO_ReadPin(GPIOA, GPIO_PIN_7);}
							}
							
							if(key3State == GPIO_PIN_RESET)
							{
								HAL_Delay(10); // 消抖延时
								P_set=P_set+10;
							  while(key3State == GPIO_PIN_RESET)
							  {key3State = HAL_GPIO_ReadPin(GPIOB, GPIO_PIN_1);}
							}
							
							if(key1State == GPIO_PIN_RESET && key3State == GPIO_PIN_RESET)
							{
								HAL_Delay(10); // 消抖延时
								P_set=P_set-10;
							  while(key1State == GPIO_PIN_RESET&&key3State == GPIO_PIN_RESET)
							  {
									key1State = HAL_GPIO_ReadPin(GPIOB, GPIO_PIN_0);
								  key3State = HAL_GPIO_ReadPin(GPIOB, GPIO_PIN_1);
								}
							}
							


						
						}
						
						
					}
				
				
				}
				lineindex=menulist[currentMenuIndex].line;
				if(lineindex>2)
				{
					if(currentMenuIndex==1)
					{menulist[currentMenuIndex].line=1;}
					if(currentMenuIndex==2&&lineindex>3)
					{menulist[currentMenuIndex].line=1;}
				}
				if(caring==1)
					{
					    if(currentMenuIndex>4&&currentMenuIndex_storage==4)
							  {currentMenuIndex=0;}
							else if (currentMenuIndex>4&&currentMenuIndex_storage==0)
							  {currentMenuIndex=4;}
							if(currentMenuIndex==2&&currentMenuIndex_storage==1)
								{
									currentMenuIndex=3;
							  }
							else if(currentMenuIndex==2&&currentMenuIndex_storage==3)
								{
									currentMenuIndex=1;
							  }
					}
				
				  if(currentMenuIndex_storage!=currentMenuIndex)//页面切换后使行数归1
						{
						lineindex=1;
						}
					if(currentMenuIndex_storage!=currentMenuIndex ||lineindex_storage!=lineindex||data!=V_set+C_set+P_set+time_close+time_on||enter_storage!=enter||HZ_S!=HZ)
					{
						HAL_Delay(1);
						if(caring==0)
						{
	          page0(currentMenuIndex,cross_user);
            page1(currentMenuIndex,lineindex,enter);
	          page2(currentMenuIndex,lineindex,enter);
						page3(currentMenuIndex,errorprocessing(errorcode,&currentMenuIndex,&cross_user));
						page4(currentMenuIndex);
						page5(currentMenuIndex);
					  }
						else if (caring==1)
						{
							 page0(currentMenuIndex,cross_user);
               page1(currentMenuIndex,lineindex,enter);
							 page3(currentMenuIndex,errorprocessing(errorcode,&currentMenuIndex,&cross_user));
							 page4(currentMenuIndex);
							
							
							
						}
          }
					
				
				  currentMenuIndex_storage=currentMenuIndex;
					lineindex_storage=lineindex;
					data=V_set+C_set+P_set+time_close+time_on;
					enter_storage=enter;
					HZ_S=HZ;
}


