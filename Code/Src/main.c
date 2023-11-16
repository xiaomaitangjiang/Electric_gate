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
#include "dma.h"
#include "i2c.h"
#include "iwdg.h"
#include "tim.h"
#include "usart.h"
#include "gpio.h"
#include "oled.h"

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
volatile int dataReceived = 0;
int leakage=0,cross=0,cross_C=0,cross_S=0;
uint16_t errorcode=0x000;
uint8_t HLW8032_Data_processing(uint8_t rxBuffer1[24],double * V,double * C,double * P,double * E_con);
uint16_t currentMenuIndex=0,time_close=0,time_on=0,enter=0,enter_storage=0,time_storage=0;
uint16_t V_set=0,C_set=0,P_set=0,E_con_set=0;
int time=0;
int RH=0,T=0,T1=0;
int caring=0;
float HZ=0;



void ms_Delay(uint16_t t_ms)
{
	uint32_t t=t_ms*3127;
	while(t--);
}


void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{
    
    if (htim == (&htim2))
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
}



void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart) {
  if (huart == &huart2)
	{
		dataReceived = 1;
	}
    
}

void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin)
{
	
	//Zero-crossing voltage detection

if(HAL_GPIO_ReadPin(GPIOA, GPIO_PIN_6) == GPIO_PIN_SET)
{
  // PA0 is low
	cross=1;
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

typedef struct 
{
int page;//��ǰҳ��
int line;//��ǰ��ѡ��
int left;//���˵�ҳ��
int right;//�Ҳ�˵�ҳ��
int enter;//��ǰҳ���Ƿ���пɽ��������
}menu;
	



void page0(int index,int sw_state)
{
	if(index==0)
	{
	OLED_Clear();
	OLED_ShowCHinese(0,0,0,0);//��
  OLED_ShowCHinese(12,0,1,0);//ѹ
  OLED_ShowCHinese(24,0,27,0);//:
	OLED_Showdecimal(32,0,V,3,2,12,0);//V
	OLED_ShowCHinese(0,2,0,0);//��
  OLED_ShowCHinese(12,2,2,0);//��
  OLED_ShowCHinese(24,2,27,0);//:
	OLED_Showdecimal(32,2,C,3,2,12,0);//C
	OLED_ShowCHinese(0,4,3,0);//��
  OLED_ShowCHinese(12,4,4,0);//��
  OLED_ShowCHinese(24,4,27,0);//:
	OLED_Showdecimal(32,4,P,3,2,12,0);//P
	OLED_ShowCHinese(0,6,9,0);//��
	OLED_ShowCHinese(12,6,10,0);//��
	OLED_ShowCHinese(24,6,5,0);//��
	OLED_ShowCHinese(36,6,0,0);//��
	OLED_ShowCHinese(48,6,27,0);//:
	OLED_Showdecimal(60,6,E_con,3,2,12,0);//E_con
	OLED_ShowCHinese(102,1,11,sw_state);//��
	OLED_ShowCHinese(114,1,23,sw_state);//��
	OLED_ShowCHinese(102,3,21,!sw_state);//��
  OLED_ShowCHinese(114,3,22,!sw_state);//��
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
	OLED_ShowCHinese(13,0,19,turn1);//��
	OLED_ShowCHinese(25,0,20,turn1);//ʱ
	OLED_ShowCHinese(37,0,21,turn1);//��
	OLED_ShowCHinese(49,0,22,turn1);//��
	OLED_ShowCHinese(13,2,19,turn2);//��
	OLED_ShowCHinese(25,2,20,turn2);//ʱ
	OLED_ShowCHinese(37,2,23,turn2);//��
	OLED_ShowCHinese(49,2,24,turn2);//��
	OLED_ShowCHinese(0,line*2-2,40,0);//->
	OLED_ShowNum(110,0,time_close,2,12,0);/*�ر�ʱ��*/
	OLED_ShowNum(110,2,time_on,2,12,0);/*����ʱ��*/
		
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
	OLED_ShowCHinese(13,0,38,turn1);//��
	OLED_ShowCHinese(25,0,39,turn1);//��
	OLED_ShowCHinese(37,0,0,turn1);//��
	OLED_ShowCHinese(49,0,1,turn1);//ѹ
	OLED_ShowCHinese(13,2,38,turn2);//��
	OLED_ShowCHinese(25,2,39,turn2);//��
	OLED_ShowCHinese(37,2,0,turn2);//��
	OLED_ShowCHinese(49,2,2,turn2);//��
	OLED_ShowCHinese(13,4,38,turn3);//��
	OLED_ShowCHinese(25,4,39,turn3);//��
	OLED_ShowCHinese(37,4,32,turn3);//ʵ
	OLED_ShowCHinese(49,4,33,turn3);//ʱ
  OLED_ShowCHinese(61,4,3,turn3);//��
  OLED_ShowCHinese(73,4,4,turn3);//��	
	OLED_ShowCHinese(0,line*2-2,40,0);//->
	OLED_ShowNum(86,0,V_set,3,12,0);/*����ѹ*/
	OLED_ShowNum(86,2,C_set,5,12,0);/*������*/
	OLED_ShowNum(86,4,E_con_set,5,12,0);/*���ʵʱ����*/

	}
}

void page3(int index)
{
	if (index==3)
	{
		OLED_Clear();	
		OLED_ShowCHinese(0,0,0,0);//��
	  OLED_ShowCHinese(12,0,43,0);//·
		OLED_ShowCHinese(24,0,52,0);//Ƶ
	  OLED_ShowCHinese(36,0,53,0);//��
		
		OLED_ShowCHinese(0,3,32,0);//��
	  OLED_ShowCHinese(12,3,35,0);//��
		OLED_ShowCHinese(24,3,27,0);//:
		
		OLED_ShowNum(86,0,HZ,3,12,0);/*Ƶ��*/
		
	}
}

void page5(int index)
{
	if (index==5)
	{
		OLED_Clear();	
		OLED_DrawBMP(0,0,45,7,gImage_1,0);
		OLED_ShowCHinese(48,0,11,0);//��
	  OLED_ShowCHinese(60,0,12,0);//��
	  OLED_ShowCHinese(72,0,13,0);//��
		OLED_ShowCHinese(84,0,27,0);//:
		OLED_ShowString(96,0,"Wesp",12,0);
		OLED_ShowCHinese(48,3,14,0);//��
	  OLED_ShowCHinese(60,3,15,0);//��
	  OLED_ShowCHinese(72,3,16,0);//��
		OLED_ShowCHinese(84,3,27,0);//:
		OLED_ShowString(96,3,"1.0",12,0);
	}
}

	void page4(int index)
{
	
	
	if(index==4)
	{
	OLED_Clear();	
	OLED_ShowCHinese(13,0,47,0);//��
	OLED_ShowCHinese(25,0,48,0);//��
		
	OLED_ShowCHinese(13,2,49,0);//ʪ
	OLED_ShowCHinese(25,2,48,0);//��
		
	OLED_ShowCHinese(13,4,50,0);//��
	OLED_ShowCHinese(25,4,51,0);//¶
	OLED_ShowCHinese(37,4,47,0);//��
	OLED_ShowCHinese(49,4,48,0);//��	
	OLED_ShowNum(86,0,RH,5,12,0);/*ʪ��*/
	OLED_ShowNum(86,2,T,5,12,0);/*�¶�*/
	OLED_ShowNum(86,4,T1,2,12,0);	/*��¶�¶�*/

	}
}		



	
//page,line,left,right,enter
menu m[]=
{
	{0,0,1,5,1},//��ҳ
	{1,1,2,0,1},//��ʱ
	{2,1,3,1,1},//��ȫ����
	{3,0,4,2,0},//������ʾ
	{4,0,5,3,0},//�¶ȣ�ʪ�ȣ�¶����ʾ
	{5,0,0,4,0},//����
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
  MX_TIM2_Init();
  MX_IWDG_Init();
  MX_USART1_UART_Init();
  MX_USART2_UART_Init();
	OLED_Init();
  OLED_Clear();
  /* USER CODE BEGIN 2 */
double V=0,C=0,P=0,E_con=0;
uint8_t rxBuffer[24];
uint16_t V_storage=0,P_storage=0;
HAL_UART_Receive_DMA(&huart2, rxBuffer, sizeof(rxBuffer));
uint16_t currentMenuIndex_storage=1,lineindex=1,lineindex_storage=0,data=0;

HAL_TIM_Base_Start_IT(&htim2);
  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while(1)
  {
    /* USER CODE END WHILE */

//iwdg refresh
HAL_IWDG_Refresh(&hiwdg);

 
//data of HLW8032

 if (dataReceived==1) 
	 {
		 //�洢�ϴ���Ч��������������
		 V_storage=V , P_storage=P;
		 errorcode = HLW8032_Data_processing(rxBuffer,&V,&C,&P,&E_con);
	   //V ��ѹ,C ����,P ����,E_con ��
		 dataReceived = 0;
   }


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

//�쳣����
if(errorcode!=0)
{HAL_GPIO_WritePin(GPIOA, GPIO_PIN_11, GPIO_PIN_SET);}

//OLED



// ��ȡ����״̬
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
					// �������¼�
        if (key1State == GPIO_PIN_RESET || key2State == GPIO_PIN_RESET || key4State == GPIO_PIN_RESET || key3State == GPIO_PIN_RESET ) {
            ms_Delay(25); // ������ʱ

            if (key1State == GPIO_PIN_RESET) {
                currentMenuIndex = m[currentMenuIndex].left; // ����ѡ��
                while (key1State == GPIO_PIN_RESET) {
                    key1State = HAL_GPIO_ReadPin(GPIOB, GPIO_PIN_0); // KEY1���ӵ�GPIOA��7����
                }
							
            }

            if (key2State == GPIO_PIN_RESET) {
                currentMenuIndex = m[currentMenuIndex].right; // ����ѡ��
                while (key2State == GPIO_PIN_RESET) {
                    key2State = HAL_GPIO_ReadPin(GPIOA, GPIO_PIN_7); // KEY2���ӵ�GPIOB��0����
                }
            }

           
						  if (key3State == GPIO_PIN_RESET) {
							while (key3State == GPIO_PIN_RESET) {
                    key3State = HAL_GPIO_ReadPin(GPIOB, GPIO_PIN_1);}
							lineindex=m[currentMenuIndex].line++;
							if(currentMenuIndex==2 && lineindex>2)
							{
								m[currentMenuIndex].line=1;
								lineindex=1;
							}
							if(currentMenuIndex==3 && lineindex>3)
							{
								m[currentMenuIndex].line=1;
								lineindex=1;
							}
						}
					 
						if (key4State == GPIO_PIN_RESET) {
                enter = m[currentMenuIndex].enter; // ȷ��
                while (key4State == GPIO_PIN_RESET) {
                    key4State = HAL_GPIO_ReadPin(GPIOB, GPIO_PIN_12); // KEY4���ӵ�GPIOB��11����
                }
            }
						
					
						
						
						
         } 
		    }

				else if(enter==1)
				{
					if (key4State == GPIO_PIN_RESET)//ȡ��ѡ�����
							{
								ms_Delay(10); // ������ʱ
								enter = 0;
								while (key4State == GPIO_PIN_RESET) {
                    key4State = HAL_GPIO_ReadPin(GPIOB, GPIO_PIN_12); // KEY4���ӵ�GPIOB��11����
								}
							}
							if(currentMenuIndex==0)
							{
								ms_Delay(10); // ������ʱ
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
								ms_Delay(10); // ������ʱ
								time_close--;
							  while(key1State == GPIO_PIN_RESET)
							  {key1State = HAL_GPIO_ReadPin(GPIOB, GPIO_PIN_0);}
							}
							
							if(key2State == GPIO_PIN_RESET)
							{
								ms_Delay(10); // ������ʱ
								time_close++;
							  while(key2State == GPIO_PIN_RESET)
							  {key2State = HAL_GPIO_ReadPin(GPIOA, GPIO_PIN_7);}
							}
							
							if(key3State == GPIO_PIN_RESET)
							{
								ms_Delay(10); // ������ʱ
								time_close=time_close+10;
							  while(key3State == GPIO_PIN_RESET)
							  {key3State = HAL_GPIO_ReadPin(GPIOB, GPIO_PIN_1);}
							}
							
							if(key1State == GPIO_PIN_RESET && key3State == GPIO_PIN_RESET)
							{
								ms_Delay(10); // ������ʱ
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
								ms_Delay(10); // ������ʱ
								time_on--;
							  while(key1State == GPIO_PIN_RESET)
							  {key1State = HAL_GPIO_ReadPin(GPIOB, GPIO_PIN_0);}
							}
							
							if(key2State == GPIO_PIN_RESET)
							{
								ms_Delay(10); // ������ʱ
								time_on++;
							  while(key2State == GPIO_PIN_RESET)
							  {key2State = HAL_GPIO_ReadPin(GPIOA, GPIO_PIN_7);}
							}
							
							if(key3State == GPIO_PIN_RESET)
							{
								ms_Delay(10); // ������ʱ
								time_on=time_on+10;
							  while(key3State == GPIO_PIN_RESET)
							  {key3State = HAL_GPIO_ReadPin(GPIOB, GPIO_PIN_1);}
							}
							
							if(key1State == GPIO_PIN_RESET && key3State == GPIO_PIN_RESET)
							{
								ms_Delay(10); // ������ʱ
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
								ms_Delay(10); // ������ʱ
								V_set--;
							  while(key1State == GPIO_PIN_RESET)
							  {key1State = HAL_GPIO_ReadPin(GPIOB, GPIO_PIN_0);}
							}
							
							if(key2State == GPIO_PIN_RESET)
							{
								ms_Delay(10); // ������ʱ
								V_set++;
							  while(key2State == GPIO_PIN_RESET)
							  {key2State = HAL_GPIO_ReadPin(GPIOA, GPIO_PIN_7);}
							}
							
							if(key3State == GPIO_PIN_RESET)
							{
								ms_Delay(10); // ������ʱ
								V_set=V_set+10;
							  while(key3State == GPIO_PIN_RESET)
							  {key3State = HAL_GPIO_ReadPin(GPIOB, GPIO_PIN_1);}
							}
							
							if(key1State == GPIO_PIN_RESET && key3State == GPIO_PIN_RESET)
							{
								ms_Delay(10); // ������ʱ
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
								ms_Delay(10); // ������ʱ
								C_set--;
							  while(key1State == GPIO_PIN_RESET)
							  {key1State = HAL_GPIO_ReadPin(GPIOB, GPIO_PIN_0);}
							}
							
							if(key2State == GPIO_PIN_RESET)
							{
								ms_Delay(10); // ������ʱ
								C_set++;
							  while(key2State == GPIO_PIN_RESET)
							  {key2State = HAL_GPIO_ReadPin(GPIOA, GPIO_PIN_7);}
							}
							
							if(key3State == GPIO_PIN_RESET)
							{
								ms_Delay(10); // ������ʱ
								C_set=C_set+10;
							  while(key3State == GPIO_PIN_RESET)
							  {key3State = HAL_GPIO_ReadPin(GPIOB, GPIO_PIN_1);}
							}
							
							if(key1State == GPIO_PIN_RESET && key3State == GPIO_PIN_RESET)
							{
								ms_Delay(10); // ������ʱ
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
								ms_Delay(10); // ������ʱ
								P_set--;
							  while(key1State == GPIO_PIN_RESET)
							  {key1State = HAL_GPIO_ReadPin(GPIOB, GPIO_PIN_0);}
							}
							
							if(key2State == GPIO_PIN_RESET)
							{
								ms_Delay(10); // ������ʱ
								P_set++;
							  while(key2State == GPIO_PIN_RESET)
							  {key2State = HAL_GPIO_ReadPin(GPIOA, GPIO_PIN_7);}
							}
							
							if(key3State == GPIO_PIN_RESET)
							{
								ms_Delay(10); // ������ʱ
								P_set=P_set+10;
							  while(key3State == GPIO_PIN_RESET)
							  {key3State = HAL_GPIO_ReadPin(GPIOB, GPIO_PIN_1);}
							}
							
							if(key1State == GPIO_PIN_RESET && key3State == GPIO_PIN_RESET)
							{
								ms_Delay(10); // ������ʱ
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
				if(lineindex>2)
				{
					if(currentMenuIndex==1)
					{lineindex=1;}
					if(currentMenuIndex==2&&lineindex>3)
					{lineindex=1;}
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
				
				  if(currentMenuIndex_storage!=currentMenuIndex)//ҳ���л���ʹ������1
						{
						lineindex=1;
						}
					if(currentMenuIndex_storage!=currentMenuIndex ||lineindex_storage!=lineindex||data!=V_set+C_set+P_set+time_close+time_on||enter_storage!=enter)
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
					

			
			
			



			
				
		}








    /* USER CODE BEGIN 3 */
  }
  /* USER CODE END 3 */


/**
  * @brief System Clock Configuration
  * @retval None
  */
void SystemClock_Config(void)
{
  RCC_OscInitTypeDef RCC_OscInitStruct = {0};
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};

  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_LSI|RCC_OSCILLATORTYPE_HSE;
  RCC_OscInitStruct.HSEState = RCC_HSE_ON;
  RCC_OscInitStruct.HSEPredivValue = RCC_HSE_PREDIV_DIV1;
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
