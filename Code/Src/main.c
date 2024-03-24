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

//warning::请使用c++编译

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
#include <string.h>
#include "menu.h"
#include "can.h"
#include "funtion.hpp"
#include "process.hpp"
#include "esp8266.h"
#include "SHT3x.h"


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
 uint8_t dat[8];
 action re[10];

// data of circuit
double V=0,C=0,P=0,E_con=0;
uint16_t V_set=220;
uint16_t C_set=10,P_set=100,E_con_set=50;
uint8_t leakage=0,cross=0,cross_t=0,cross_user=0;
uint16_t errorcode=0x000;
float HZ=0,HZ_S=1;//HZ is the fequence of circuit

//meun
uint16_t currentMenuIndex=0,time_close=0,time_on=0,enter=0,enter_storage=0,time_storage=0;
uint8_t caring=0;
uint16_t V_storage=0,P_storage=0;
uint16_t currentMenuIndex_storage=1,lineindex=1,lineindex_storage=0,data=0;
uint8_t type;


//TIM
uint8_t time=0;
uint8_t time_h=0,t=0;


// data of environment
uint8_t RH=0,T=0,Td=0;
float temperature,V_ppm,V_adc;


//UART
uint8_t receive[50];
extern uint8_t URTP_UART_Receive[5];

//ADC
uint16_t ADC_RV[3]={0};


//TIM_us
uint16_t set_us =0; 


//URTP
action reflect;
extern communciation URTP;




//浓度处理函数
void ppm_processing (const uint16_t V_ppm_in,uint16_t *error)
{
	if(V_ppm>=1825)//1825为计算估计值
	{
		uint16_t error_ppm=0x200;
		error_ppm=error_ppm||*error;
	}
}



//ms延时
void ms_Delay(uint16_t t_ms)
{
	uint32_t t=t_ms*3127;
	while(t--);
}




//串口dma接收错误不进入回调函数的罪魁祸首，该函数是弱定义
void HAL_UART_ErrorCallback(UART_HandleTypeDef *huart)
{
	if(huart->Instance ==USART1)
	{
		HAL_UARTEx_ReceiveToIdle_DMA(&huart1,receive,24);
	__HAL_DMA_DISABLE_IT(&hdma_usart1_rx,DMA_IT_HT);
	}
	if(huart->Instance ==USART3)
	{
		HAL_UARTEx_ReceiveToIdle_DMA(&huart3,URTP_UART_Receive,5);
	__HAL_DMA_DISABLE_IT(&hdma_usart3_rx,DMA_IT_HT);
	}
}



//TIM回调函数
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
					cross_t=0;
					time_close=0;
					time=0;
				}
        if(time == time_on*65)
				{
					cross_t=1;
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
				HAL_Delay(1);
				t=0;
				time_h=0;
			}
		}
		
}



//gpio中断回调函数
void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin)
{
	
	//Zero-crossing voltage detection

if(HAL_GPIO_ReadPin(GPIOB, GPIO_PIN_3) == GPIO_PIN_SET)
{
  // PA0 is low
	cross=1;
	t++;
}


//Leakage detection
if(HAL_GPIO_ReadPin(GPIOA, GPIO_PIN_5) == GPIO_PIN_RESET)
{
  // PA0 is low
	HAL_GPIO_WritePin(GPIOB, GPIO_PIN_10, GPIO_PIN_RESET);
	errorcode=0x004|errorcode;
}
  
}



//hlw8032 uart receive
void HAL_UARTEx_RxEventCallback(UART_HandleTypeDef *huart, uint16_t Size)
{
	if(huart==&huart1)
	{
		//存储上次有效电流、功率数据
		 V_storage=V , P_storage=P;
		 errorcode = HLW8032_Data_processing(receive,&V,&C,&P,&E_con);
	   //V 电压,C 电流,P 功率,E_con 度
		memset(receive,0,24);
		HAL_UARTEx_ReceiveToIdle_DMA(&huart1,receive,24);
	__HAL_DMA_DISABLE_IT(&hdma_usart1_rx,DMA_IT_HT);
	}
	if(huart==&huart3)
	{
		
		expanddata_C(URTP_UART_Receive,&reflect,&V_set,&P_set,&C_set,&E_con_set);
		
		memset(URTP_UART_Receive,0,5);
		HAL_UARTEx_ReceiveToIdle_DMA(&huart3,receive,5);
	__HAL_DMA_DISABLE_IT(&hdma_usart3_rx,DMA_IT_HT);
	}
}


extern uint8_t IIC_data[5];
// iic回调函数
void HAL_I2C_SlaveTxCpltCallback(I2C_HandleTypeDef *hi2c)
{
	if(hi2c==&hi2c2)
	{
		expanddata_C(IIC_data,&reflect,&V_set,&P_set,&C_set,&E_con_set);
		HAL_I2C_Slave_Transmit_IT(&hi2c2, IIC_data, sizeof(IIC_data));   // 启动中断发送
	}
}





	
//delay us 
//TIM_HANDLE include in "main.h"
/*void delay_us_init(void)
{MX_TIM4_Init();}

void delay_us(uint16_t us)
{
	us--;
	__HAL_TIM_SET_COUNTER(TIM_HANDLE,0);
	__HAL_TIM_ENABLE(TIM_HANDLE);
	while(__HAL_TIM_GET_COUNTER(TIM_HANDLE)<us);
	__HAL_TIM_DISABLE(TIM_HANDLE);
}*/






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
  //MX_IWDG_Init();
  MX_USART1_UART_Init();
  MX_USART2_UART_Init();
  MX_TIM2_Init();
  MX_RTC_Init();
	MX_I2C1_Init();
  MX_TIM3_Init();
  MX_ADC1_Init();
	
	OLED_Init();
	OLED_Clear();
  esp8266_init();
  /* USER CODE BEGIN 2 */
  

  /*使能定时器1中断*/
  HAL_TIM_Base_Start_IT(&htim2);
  HAL_TIM_Base_Start_IT(&htim3);
	
	HAL_ADCEx_Calibration_Start(&hadc1);
	while(HAL_ADCEx_Calibration_Start(&hadc1));
	
	
	HAL_UARTEx_ReceiveToIdle_DMA(&huart1,receive,24);
	__HAL_DMA_DISABLE_IT(&hdma_usart1_rx,DMA_IT_HT);
  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
    //iwdg refresh
		//HAL_IWDG_Refresh(&hiwdg);
		
		
		//
		HAL_ADC_Start_DMA(&hadc1,(uint32_t *)ADC_RV,3);
		
		//浓度检测
		ppm_processing((uint16_t)V_ppm,&errorcode);
		
		
		uint16_t user_set[]={V_set,C_set,P_set,E_con_set,V_storage,P_storage};
		//安全功能
		errorcode=(errorcode|safefuntion(V,C,P,E_con,(uint16_t *)user_set,(((uint16_t *)user_set)+5)));

		//错误处理
		errorprocessing(errorcode,&currentMenuIndex,&cross_user);
		
		//电闸通断控制
	  switch_control(cross,cross_user,cross_t);
		
		//
		T=T/10;
		RH=RH/10;
		//显示功能复合体
		menuprocess();

		//adc dataprocess
		int16_t ADC_Receive_T=adc_data_processing(ADC_RV,1);
		int32_t aa=1430;
		if (ADC_Receive_T>=aa)
		{temperature=abs((int)(ADC_Receive_T-aa))/43+25;}
		else
		{temperature=abs((int)(aa-ADC_Receive_T))/43+25;}
		
		V_ppm=adc_data_processing(ADC_RV,2);
		V_adc=adc_data_processing(ADC_RV,3);
		//浓度处理
		ppm_processing(V_ppm,&errorcode);	
		
		//结露温度计算
		Td=Dew_point_temperature(temperature,RH);
		
		expand a;
		a.Transmit=uart;
		URTP.expandinit(&a);
		
		URTP.expanddata(dat,re,&V_set,&P_set,&C_set,&E_con_set);
		
		//sht30
		uint8_t recv_dat[6] = {0};
		SHT30_reset();
		SHT30_Init();
		HAL_Delay(100);
		if(SHT30_Read_Dat(recv_dat) == HAL_OK)
        {
					HAL_Delay(10);
					SHT30_Dat_To_Float(recv_dat, (float *)&T, (float *)&RH);
				}
				
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
