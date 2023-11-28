/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2023 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */
/* USER CODE END Header */
/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "adc.h"
#include "dma.h"
#include "i2c.h"
#include "iwdg.h"
#include "rtc.h"
#include "tim.h"
#include "usart.h"
#include "gpio.h"
#include "oled.h"
#include <cstring>

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */

/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */

/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/

/* USER CODE BEGIN PV */

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
/* USER CODE BEGIN PFP */
double V=0,C=0,P=0,E_con=0;
int leakage=0,cross=0,cross_C=0,cross_S=0;
uint16_t errorcode=0x000;
uint16_t currentMenuIndex=2,time_close=0,time_on=1,enter=0,enter_storage=0,time_storage=0;
uint16_t V_set=0,C_set=0,P_set=0,E_con_set=0;
int time=0;
int RH=0,T=0,T1=0;
int caring=0;
float HZ=0,HZ_S=1;
int time_h=0,t=0;
uint8_t rxBuffer[24],receive[50];
uint16_t V_storage=0,P_storage=0;

void ms_Delay(uint16_t t_ms)
{
	uint32_t t=t_ms*3127;
	while(t--);
}


void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{
    time++;
    if (htim->Instance== htim2.Instance)
    {
			
			if(time_close+time_on!=0)
			{
				time++;
        if (time == time_close*65)
				{
					cross_C=0;
					time_close=0;
					time=0;
				}
        if(time == time_on*65)
				{
					cross_C=1;
					time_on=0;
					time=0;
				}
				if(time_storage!=time_close+time_on)
				{
					time=0;
					time_storage =time_close+time_on;
				}
				
			}
        
    }
		if (htim->Instance== htim3.Instance)
		{
			time_h++;
			if (time_h==2)
			{
				HZ=(float)(t/time_h);
				ms_Delay(1);
				t=0;
				time_h=0;
			}
		}
}





void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin)
{
	
	//Zero-crossing voltage detection

if(HAL_GPIO_ReadPin(GPIOA, GPIO_PIN_6) == GPIO_PIN_SET)
{
  // PA0 is low
	cross=1;
	t++;
}

//Leakage detection

if(HAL_GPIO_ReadPin(GPIOB, GPIO_PIN_15) == GPIO_PIN_RESET)
{
  // PA0 is low
	HAL_GPIO_WritePin(GPIOB, GPIO_PIN_10, GPIO_PIN_RESET);
	errorcode=0x004|errorcode;
}
  
}

//control
void control(void)
{
if(cross==1||cross_C==1||cross_S==1)
	{
		HAL_GPIO_WritePin(GPIOB, GPIO_PIN_10, GPIO_PIN_SET);
		ms_Delay(1);
		cross=0;
	}
	else
	{
		HAL_GPIO_WritePin(GPIOB, GPIO_PIN_10, GPIO_PIN_RESET);
	}
}


//hlw8032 uart receive
void HAL_UARTEx_RxEventCallback(UART_HandleTypeDef *huart, uint16_t Size)
{
	if(huart==&huart1)
	{
		for(uint16_t i=0;i<=Size;i++)
		{
			if (Size>(sizeof(rxBuffer)))
			{
				memset(receive,0,50);
			break;}
			rxBuffer[i]=receive[i];
			
		}
		//存储上次有效电流、功率数据
		 V_storage=V , P_storage=P;
		 errorcode = HLW8032_Data_processing(receive,&V,&C,&P,&E_con);
	   //V 电压,C 电流,P 功率,E_con 度
		HAL_UARTEx_ReceiveToIdle_IT(&huart1,receive,24);
	__HAL_DMA_DISABLE_IT(&hdma_usart1_rx,DMA_IT_HT);
		memset(receive,0,50);
	}
}


typedef struct 
{
int page;//当前页码
int line;//当前所选行
int left;//左侧菜单页码
int right;//右侧菜单页码
int enter;//当前页码是否具有可进入的设置
}menu;
	



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

void page3(int index)
{
	if (index==3)
	{
		OLED_Clear();	
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
	OLED_ShowNum(86,0,25,5,12,0);/*湿度*/
	OLED_ShowNum(86,2,26,5,12,0);/*温度*/
	OLED_ShowNum(86,4,14,2,12,0);	/*结露温度*/

	}
}		



	
//page,line,left,right,enter
menu m[]=
{
	{0,0,1,5,1},//主页
	{1,1,2,0,1},//定时
	{2,1,3,1,1},//安全设置
	{3,0,4,2,0},//故障显示
	{4,0,5,3,0},//温度，湿度，露点显示
	{5,0,0,4,0},//关于
};
/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */

/* USER CODE END 0 */

/**
  * @brief  The application entry point.
  * @retval int
  */
int main(void)
{
  /* USER CODE BEGIN 1 */
uint32_t ADC_RV;
  /* USER CODE END 1 */

  /* MCU Configuration--------------------------------------------------------*/

  /* Reset of all peripherals, Initializes the Flash interface and the Systick. */
  HAL_Init();

  /* USER CODE BEGIN Init */

  /* USER CODE END Init */

  /* Configure the system clock */
  SystemClock_Config();

  /* USER CODE BEGIN SysInit */

  /* USER CODE END SysInit */

  /* Initialize all configured peripherals */
  MX_GPIO_Init();
  MX_DMA_Init();
  MX_I2C1_Init();
 // MX_IWDG_Init();
  MX_USART1_UART_Init();
  MX_USART2_UART_Init();
  MX_TIM2_Init();
  MX_RTC_Init();
  MX_I2C2_Init();
  MX_TIM3_Init();
  MX_ADC1_Init();
	
	OLED_Init();
  OLED_Clear();
  /* USER CODE BEGIN 2 */
  uint16_t currentMenuIndex_storage=1,lineindex=1,lineindex_storage=0,data=0;
	
  /*使能定时器1中断*/
  HAL_TIM_Base_Start_IT(&htim2);
  HAL_TIM_Base_Start_IT(&htim3);
	
	HAL_ADCEx_Calibration_Start(&hadc1);
	ms_Delay(10);
	HAL_ADC_Start_DMA(&hadc1,&ADC_RV,240);
	
	
  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
    /* USER CODE END WHILE */
		//iwdg refresh
//HAL_IWDG_Refresh(&hiwdg);
ms_Delay(50);
HAL_UARTEx_ReceiveToIdle_IT(&huart1,receive,24);
	__HAL_DMA_DISABLE_IT(&hdma_usart1_rx,DMA_IT_HT);


//safety funtion
if(V>=V_set)
{
  cross_S=0;
	errorcode=0x008|errorcode;
	control();
	
}

if(C>=C_set)
{
  cross_S=0;
	errorcode=0x010|errorcode;
	control();
}

if(P>=P_set)
{
  cross_S=0;
	errorcode=0x020|errorcode;
	control();
}

if(E_con>=E_con_set)
{
  cross_S=0;
	errorcode=0x040|errorcode;
	control();
}
if(V-22>V_storage)
{
	cross_S=0;
	errorcode=0x080|errorcode;
	control();
}
if(P_storage>P_set)
{
	cross_S=0;
  errorcode=0x100|errorcode;
	control();
}
/*
//异常警告
if(errorcode!=0)
{
	HAL_GPIO_WritePin(GPIOA, GPIO_PIN_11, GPIO_PIN_SET);
}
else 
{HAL_GPIO_WritePin(GPIOA, GPIO_PIN_11, GPIO_PIN_RESET);}
*/
//OLED




// 读取按键状态
        GPIO_PinState key1State = HAL_GPIO_ReadPin(GPIOB, GPIO_PIN_0); // KEY1 left
        GPIO_PinState key2State = HAL_GPIO_ReadPin(GPIOA, GPIO_PIN_7); // KEY2  right
        GPIO_PinState key3State = HAL_GPIO_ReadPin(GPIOB, GPIO_PIN_1); // KEY3  enter
        GPIO_PinState key4State = HAL_GPIO_ReadPin(GPIOB, GPIO_PIN_12); // KEY4  line

	
if (caring==0&&key1State == GPIO_PIN_RESET && key2State == GPIO_PIN_RESET && key3State == GPIO_PIN_RESET )
{
	ms_Delay(10);
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
	ms_Delay(10);
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
            ms_Delay(25); // 消抖延时

            if (key1State == GPIO_PIN_RESET) {
                currentMenuIndex = m[currentMenuIndex].left; // 向左选择
                while (key1State == GPIO_PIN_RESET) {
                    key1State = HAL_GPIO_ReadPin(GPIOB, GPIO_PIN_0); // KEY1连接到GPIOA的7引脚
                }
							
            }

            if (key2State == GPIO_PIN_RESET) {
                currentMenuIndex = m[currentMenuIndex].right; // 向右选择
                while (key2State == GPIO_PIN_RESET) {
                    key2State = HAL_GPIO_ReadPin(GPIOA, GPIO_PIN_7); // KEY2连接到GPIOB的0引脚
                }
            }

           
						  if (key3State == GPIO_PIN_RESET) {
							while (key3State == GPIO_PIN_RESET) {
                    key3State = HAL_GPIO_ReadPin(GPIOB, GPIO_PIN_1);}
							lineindex=m[currentMenuIndex].line++;
				
						}
					 
						if (key4State == GPIO_PIN_RESET) {
                enter = m[currentMenuIndex].enter; // 确认
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
								ms_Delay(10); // 消抖延时
								enter = 0;
								while (key4State == GPIO_PIN_RESET) {
                    key4State = HAL_GPIO_ReadPin(GPIOB, GPIO_PIN_12); // KEY4连接到GPIOB的11引脚
								}
							}
							if(currentMenuIndex==0)
							{
								ms_Delay(10); // 消抖延时
								if(cross_C==0)
								{
									cross_C=1;
									enter=0;
								}
								else if(cross_C==1)
								{
									cross_C=0;
									enter=0;
								}
								
							}
					if(currentMenuIndex==1)
					{
						if(m[currentMenuIndex].line==1)
						{
							if(key1State == GPIO_PIN_RESET)
							{
								ms_Delay(10); // 消抖延时
								time_close--;
							  while(key1State == GPIO_PIN_RESET)
							  {key1State = HAL_GPIO_ReadPin(GPIOB, GPIO_PIN_0);}
							}
							
							if(key2State == GPIO_PIN_RESET)
							{
								ms_Delay(10); // 消抖延时
								time_close++;
							  while(key2State == GPIO_PIN_RESET)
							  {key2State = HAL_GPIO_ReadPin(GPIOA, GPIO_PIN_7);}
							}
							
							if(key3State == GPIO_PIN_RESET)
							{
								ms_Delay(10); // 消抖延时
								time_close=time_close+10;
							  while(key3State == GPIO_PIN_RESET)
							  {key3State = HAL_GPIO_ReadPin(GPIOB, GPIO_PIN_1);}
							}
							
							if(key1State == GPIO_PIN_RESET && key3State == GPIO_PIN_RESET)
							{
								ms_Delay(10); // 消抖延时
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
						if(m[currentMenuIndex].line==2)
						{
							
							if(key1State == GPIO_PIN_RESET)
							{
								ms_Delay(10); // 消抖延时
								time_on--;
							  while(key1State == GPIO_PIN_RESET)
							  {key1State = HAL_GPIO_ReadPin(GPIOB, GPIO_PIN_0);}
							}
							
							if(key2State == GPIO_PIN_RESET)
							{
								ms_Delay(10); // 消抖延时
								time_on++;
							  while(key2State == GPIO_PIN_RESET)
							  {key2State = HAL_GPIO_ReadPin(GPIOA, GPIO_PIN_7);}
							}
							
							if(key3State == GPIO_PIN_RESET)
							{
								ms_Delay(10); // 消抖延时
								time_on=time_on+10;
							  while(key3State == GPIO_PIN_RESET)
							  {key3State = HAL_GPIO_ReadPin(GPIOB, GPIO_PIN_1);}
							}
							
							if(key1State == GPIO_PIN_RESET && key3State == GPIO_PIN_RESET)
							{
								ms_Delay(10); // 消抖延时
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
						if(m[currentMenuIndex].line==1)
						{
							if(key1State == GPIO_PIN_RESET)
							{
								ms_Delay(10); // 消抖延时
								V_set--;
							  while(key1State == GPIO_PIN_RESET)
							  {key1State = HAL_GPIO_ReadPin(GPIOB, GPIO_PIN_0);}
							}
							
							if(key2State == GPIO_PIN_RESET)
							{
								ms_Delay(10); // 消抖延时
								V_set++;
							  while(key2State == GPIO_PIN_RESET)
							  {key2State = HAL_GPIO_ReadPin(GPIOA, GPIO_PIN_7);}
							}
							
							if(key3State == GPIO_PIN_RESET)
							{
								ms_Delay(10); // 消抖延时
								V_set=V_set+10;
							  while(key3State == GPIO_PIN_RESET)
							  {key3State = HAL_GPIO_ReadPin(GPIOB, GPIO_PIN_1);}
							}
							
							if(key1State == GPIO_PIN_RESET && key3State == GPIO_PIN_RESET)
							{
								ms_Delay(10); // 消抖延时
								V_set=V_set-10;
							  while(key1State == GPIO_PIN_RESET&&key3State == GPIO_PIN_RESET)
							  {
									key1State = HAL_GPIO_ReadPin(GPIOB, GPIO_PIN_0);
								  key3State = HAL_GPIO_ReadPin(GPIOB, GPIO_PIN_1);
								}
							}
							
						
						}
						if(m[currentMenuIndex].line==2)
						{
							
							if(key1State == GPIO_PIN_RESET)
							{
								ms_Delay(10); // 消抖延时
								C_set--;
							  while(key1State == GPIO_PIN_RESET)
							  {key1State = HAL_GPIO_ReadPin(GPIOB, GPIO_PIN_0);}
							}
							
							if(key2State == GPIO_PIN_RESET)
							{
								ms_Delay(10); // 消抖延时
								C_set++;
							  while(key2State == GPIO_PIN_RESET)
							  {key2State = HAL_GPIO_ReadPin(GPIOA, GPIO_PIN_7);}
							}
							
							if(key3State == GPIO_PIN_RESET)
							{
								ms_Delay(10); // 消抖延时
								C_set=C_set+10;
							  while(key3State == GPIO_PIN_RESET)
							  {key3State = HAL_GPIO_ReadPin(GPIOB, GPIO_PIN_1);}
							}
							
							if(key1State == GPIO_PIN_RESET && key3State == GPIO_PIN_RESET)
							{
								ms_Delay(10); // 消抖延时
								C_set=C_set-10;
							  while(key1State == GPIO_PIN_RESET&&key3State == GPIO_PIN_RESET)
							  {
									key1State = HAL_GPIO_ReadPin(GPIOB, GPIO_PIN_0);
								  key3State = HAL_GPIO_ReadPin(GPIOB, GPIO_PIN_1);
								}
							}


						
						}
						if(m[currentMenuIndex].line==3)
						{
							
							if(key1State == GPIO_PIN_RESET)
							{
								ms_Delay(10); // 消抖延时
								P_set--;
							  while(key1State == GPIO_PIN_RESET)
							  {key1State = HAL_GPIO_ReadPin(GPIOB, GPIO_PIN_0);}
							}
							
							if(key2State == GPIO_PIN_RESET)
							{
								ms_Delay(10); // 消抖延时
								P_set++;
							  while(key2State == GPIO_PIN_RESET)
							  {key2State = HAL_GPIO_ReadPin(GPIOA, GPIO_PIN_7);}
							}
							
							if(key3State == GPIO_PIN_RESET)
							{
								ms_Delay(10); // 消抖延时
								P_set=P_set+10;
							  while(key3State == GPIO_PIN_RESET)
							  {key3State = HAL_GPIO_ReadPin(GPIOB, GPIO_PIN_1);}
							}
							
							if(key1State == GPIO_PIN_RESET && key3State == GPIO_PIN_RESET)
							{
								ms_Delay(10); // 消抖延时
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
				lineindex=m[currentMenuIndex].line;
				if(lineindex>2)
				{
					if(currentMenuIndex==1)
					{m[currentMenuIndex].line=1;}
					if(currentMenuIndex==2&&lineindex>3)
					{m[currentMenuIndex].line=1;}
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
						ms_Delay(1);
						if(caring==0)
						{
	          page0(currentMenuIndex,cross_C);
            page1(currentMenuIndex,lineindex,enter);
	          page2(currentMenuIndex,lineindex,enter);
						page3(currentMenuIndex);
						page4(currentMenuIndex);
						page5(currentMenuIndex);
					  }
						else if (caring==1)
						{
							 page0(currentMenuIndex,cross_C);
               page1(currentMenuIndex,lineindex,enter);
							 page3(currentMenuIndex);
							 page4(currentMenuIndex);
							
							
							
						}
          }
					
				
				  currentMenuIndex_storage=currentMenuIndex;
					lineindex_storage=lineindex;
					data=V_set+C_set+P_set+time_close+time_on;
					enter_storage=enter;
					HZ_S=HZ;

    /* USER CODE BEGIN 3 */
  }
  /* USER CODE END 3 */
}

/**
  * @brief System Clock Configuration
  * @retval None
  */
void SystemClock_Config(void)
{
  RCC_OscInitTypeDef RCC_OscInitStruct = {0};
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};
  RCC_PeriphCLKInitTypeDef PeriphClkInit = {0};

  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_LSI|RCC_OSCILLATORTYPE_HSE
                              |RCC_OSCILLATORTYPE_LSE;
  RCC_OscInitStruct.HSEState = RCC_HSE_ON;
  RCC_OscInitStruct.HSEPredivValue = RCC_HSE_PREDIV_DIV1;
  RCC_OscInitStruct.LSEState = RCC_LSE_ON;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.LSIState = RCC_LSI_ON;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
  RCC_OscInitStruct.PLL.PLLMUL = RCC_PLL_MUL9;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }

  /** Initializes the CPU, AHB and APB buses clocks
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV2;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_2) != HAL_OK)
  {
    Error_Handler();
  }
  PeriphClkInit.PeriphClockSelection = RCC_PERIPHCLK_RTC|RCC_PERIPHCLK_ADC;
  PeriphClkInit.RTCClockSelection = RCC_RTCCLKSOURCE_LSE;
  PeriphClkInit.AdcClockSelection = RCC_ADCPCLK2_DIV6;
  if (HAL_RCCEx_PeriphCLKConfig(&PeriphClkInit) != HAL_OK)
  {
    Error_Handler();
  }
}

/* USER CODE BEGIN 4 */

/* USER CODE END 4 */

/**
  * @brief  This function is executed in case of error occurrence.
  * @retval None
  */
void Error_Handler(void)
{
  /* USER CODE BEGIN Error_Handler_Debug */
  /* User can add his own implementation to report the HAL error return state */
  __disable_irq();
  while (1)
  {
  }
  /* USER CODE END Error_Handler_Debug */
}

#ifdef  USE_FULL_ASSERT
/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t *file, uint32_t line)
{
  /* USER CODE BEGIN 6 */
  /* User can add his own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */
