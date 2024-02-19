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
#include <string.h>
#include "menu.h"
#include "can.h"

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
uint8_t leakage=0,cross=0,cross_C=0,cross_S=0;
uint16_t errorcode=0x000;
uint16_t currentMenuIndex=0,time_close=0,time_on=0,enter=0,enter_storage=0,time_storage=0;
uint16_t V_set=0,C_set=0,P_set=0,E_con_set=0;
uint8_t time=0;
uint8_t RH=0,T=0,T1=0;
uint8_t caring=0;
float HZ=0,HZ_S=1;
uint8_t time_h=0,t=0;
uint8_t rxBuffer[24],receive[50];
uint16_t V_storage=0,P_storage=0;
uint16_t ADC_RV[40]={0};
uint8_t type;
int16_t adc_RH,adc_T,adc_V;
uint16_t set_us =0; 
uint16_t currentMenuIndex_storage=1,lineindex=1,lineindex_storage=0,data=0;

float temperature,V_ppm,V_adc;


uint32_t V_tem;
int data_plus=0;



void ppm_processing (uint16_t V_ppm_in,uint16_t *error)
{
	if(V_ppm>=1825)//1825为计算估计值
	{
		uint16_t error_ppm=0;//忘了，看书
		error_ppm=error_ppm||*error;
	}
}






//delay us 
//TIM_HANDLE include in "main.h"
void delay_us_init(void)
{MX_TIM4_Init();}

void delay_us(uint16_t us)
{
	us--;
	__HAL_TIM_SET_COUNTER(TIM_HANDLE,0);
	__HAL_TIM_ENABLE(TIM_HANDLE);
	while(__HAL_TIM_GET_COUNTER(TIM_HANDLE)<us);
	__HAL_TIM_DISABLE(TIM_HANDLE);
}


uint16_t adc_data_processing (uint16_t  *adc_data, uint8_t listlength,uint8_t numberstart  ,uint8_t pernumber)
{
	data_plus=0;
	uint8_t tim=0;
	uint32_t min=0,max=0;
	max=*adc_data;
	min=max;
	for(uint8_t ergodic=numberstart;ergodic<=listlength ;ergodic=ergodic+pernumber)
	{
		data_plus=data_plus+(*(adc_data+ergodic));
		tim++;
		if(*(adc_data+ergodic)>max)
		{
			max=*(adc_data+ergodic);
		}
		if(*(adc_data+ergodic)<min)
			
		{min=*(adc_data+ergodic);}
	}
	data_plus=(data_plus-max-min)/(tim-2);
	data_plus=(uint16_t)abs((int)(data_plus*3300/4096));
	return data_plus;
}


//异常警告
uint8_t errorprocessing (uint16_t errortype,uint16_t * index)
{
	/*uint8_t type1=0;
	if(errortype!=0)
	{
		HAL_GPIO_WritePin(GPIOA, GPIO_PIN_11, GPIO_PIN_SET);
		*index=3;
		if((errortype&0x01)!=0)
		{type1=1;}
		if((errortype&0x02)!=0)
		{type1=2;}
		if((errortype&0x04)!=0)
		{type1=3;}
		if((errortype&0x08)!=0)
		{type1=4;}
		if((errortype&0x10)!=0)
		{type1=5;}
		if((errortype&0x20)!=0)
		{type1=6;}
		if((errortype&0x40)!=0)
		{type1=7;}
		if((errortype&0x80)!=0)
		{type1=8;}
		if((errortype&0x100)!=0)
		{type1=9;}
	}
	else if(errortype==0)
	{HAL_GPIO_WritePin(GPIOA, GPIO_PIN_11, GPIO_PIN_RESET);}
	
	return type1;*/
	return 0;
}



//串口dma接收错误不进入回调函数的罪魁祸首，该函数是弱定义
void HAL_UART_ErrorCallback(UART_HandleTypeDef *huart)
{
	if(huart->Instance ==USART2)
	{
		HAL_UARTEx_ReceiveToIdle_DMA(&huart1,receive,24);
	__HAL_DMA_DISABLE_IT(&hdma_usart1_rx,DMA_IT_HT);
	}
}




void HAL_Delay(uint32_t Delay)
{
	uint32_t t=Delay*3127;
	while(t--);
	
}
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
		if (htim->Instance == htim4.Instance)
		{
			// nothing here
		}
}



//control
void control(void)
{
if(cross==1||cross_C==1||cross_S==1)
	{
		HAL_GPIO_WritePin(GPIOB, GPIO_PIN_4, GPIO_PIN_SET);
		ms_Delay(1);
		cross=0;
	}
	else
	{
		HAL_GPIO_WritePin(GPIOB, GPIO_PIN_4, GPIO_PIN_RESET);
	}
}


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
		memset(receive,0,50);
		HAL_UARTEx_ReceiveToIdle_DMA(&huart1,receive,24);
	__HAL_DMA_DISABLE_IT(&hdma_usart1_rx,DMA_IT_HT);
	}
}



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
	delay_us_init();
  /* USER CODE BEGIN 2 */
  

  /*使能定时器1中断*/
  HAL_TIM_Base_Start_IT(&htim2);
  HAL_TIM_Base_Start_IT(&htim3);
	
	HAL_ADCEx_Calibration_Start(&hadc1);
	while(HAL_ADCEx_Calibration_Start(&hadc1));
	HAL_ADC_Start_DMA(&hadc1,(uint32_t *)ADC_RV,40);
	
	HAL_UARTEx_ReceiveToIdle_DMA(&huart1,receive,24);
	__HAL_DMA_DISABLE_IT(&hdma_usart1_rx,DMA_IT_HT);
  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
    //iwdg refresh
		//HAL_IWDG_Refresh(&hiwdg);

		errorprocessing(errorcode,&currentMenuIndex);	
	
		//safety funtion
		
		if(V>=V_set)
		{
			cross_S=0;
			errorcode=0x008|errorcode;
			control();	
		}
		else if(0x008&errorcode)
		{errorcode=0x008^errorcode;}

		if(C>=C_set)
		{
			cross_S=0;
			errorcode=0x010|errorcode;
			control();
		}
		else if(0x010&errorcode)
		{errorcode=0x010^errorcode;}

		if(P>=P_set)
		{
			cross_S=0;
			errorcode=0x020|errorcode;
			control();
		}
		else if(0x020&errorcode)
		{errorcode=0x020^errorcode;}

		if(E_con>=E_con_set)
		{
			cross_S=0;
			errorcode=0x040|errorcode;
			control();
		}
		else if(0x040&errorcode)
		{errorcode=0x040^errorcode;}
		if(V-22>V_storage)
		{
			cross_S=0;
			errorcode=0x080|errorcode;
			control();
		}
		else if(0x080&errorcode)
		{errorcode=0x080^errorcode;}
		if(P_storage>P_set)
		{
			cross_S=0;
			errorcode=0x100|errorcode;
			control();
		}
		else if(0x100&errorcode)
		{errorcode=0x100^errorcode;}

		//menu funtion
		menuprocess();

		//adc dataprocess
		V_tem=adc_data_processing(ADC_RV,40,1,3);
		temperature=abs(static_cast<int>(1430-V_tem))/4.3+25;
		
		V_ppm=adc_data_processing(ADC_RV,40,0,3);
		V_adc=adc_data_processing(ADC_RV,40,2,3);
		//
		//ppm_processing(V_ppm,&errorcode);	先不启用，原因见函数体		
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
