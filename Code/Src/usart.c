/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file    usart.c
  * @brief   This file provides code for the configuration
  *          of the USART instances.
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
#include "usart.h"
#include <cstring>

/* USER CODE BEGIN 0 */


uint8_t rxBuffer[24];
uint8_t rx_temp;
uint8_t rx_len=0;


//串口接收中断回调函数
void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart)
{
    if(huart->Instance == USART1){
       
		rxBuffer[rx_len] = rx_temp;
    	rx_len++;   // 接收到上个数据，长度+1
        HAL_UART_Receive_IT(&huart1, &rx_temp, 1);    // 继续使能RX中断
        
    }
		
		if (huart == &huart2)
	{
		dataReceived = 1;
	}
}
//串口接收空闲中断回调函数，这个是从hal里面新加的
void HAL_UART_IdleCpltCallback(UART_HandleTypeDef *huart)
{
    if(huart->Instance == USART1){  
        
        //一般要在这里判断一下，是否已经接收到数据，防止误触发
        //经测试发现在开机刚开始使能idle中断的时候，会触发一次idle中断。
        if(rx_len >= 1){
            __NOP();//已经触发空闲中断回调，剩下内容由用户操作
        }
        
    }
}
//串口接收错误中断，下一节会说明为啥要有这个
void HAL_UART_ErrorCallback(UART_HandleTypeDef *huart)
{
	if(HAL_UART_GetError(huart) & HAL_UART_ERROR_PE){		/*!< Parity error            */
		//奇偶校验错误
		__HAL_UART_CLEAR_PEFLAG(huart);
	}else if(HAL_UART_GetError(huart) & HAL_UART_ERROR_NE){ /*!< Noise error             */
		//噪声错误
		__HAL_UART_CLEAR_NEFLAG(huart);
	}else if(HAL_UART_GetError(huart) & HAL_UART_ERROR_FE){ /*!< Frame error             */
		//帧格式错误
		__HAL_UART_CLEAR_FEFLAG(huart);
	}else if(HAL_UART_GetError(huart) & HAL_UART_ERROR_ORE){ /*!< Overrun error           */
		//数据太多串口来不及接收错误
		__HAL_UART_CLEAR_OREFLAG(huart);
	}
    //当这个串口发生了错误，一定要在重新使能接收中断
    if(huart ->Instance == USART1){
		HAL_UART_Receive_IT(&huart1, &rx_temp, 1);
	}
    //其他串口......
}

uint8_t HLW8032_Data_processing(uint8_t rxBuffer1[24],double * V1,double * C1,double * P1,double * E_con1)
{
	uint32_t VP_REG=0,V_REG=0,CP_REG=0,C_REG=0,PP_REG=0,P_REG=0,PF_COUNT=0,PF=0,dat_sum=0;
  uint8_t i=0;
  uint8_t k=0;
  uint16_t old_reg=0;
	int error =0x000;
	
	if(rxBuffer1[0]!=0xaa)//芯片误差修正功能正常，参数正常
	{
		for(i=2;i<23;i++)
		{
			dat_sum=dat_sum+rxBuffer1[i];//计算校验和
		}
		if(dat_sum%256==rxBuffer1[23])//检查校验位是否正确
		{
			VP_REG=rxBuffer1[2]*65536+rxBuffer1[3]*256+rxBuffer1[4];//计算电压参数寄存器
			V_REG=rxBuffer1[5]*65536+rxBuffer1[6]*256+rxBuffer1[7];//计算电压寄存器
			*V1 =(VP_REG/V_REG)*3.006;//计算电压值，1.88为电压系数，根据所采用的分压电阻大小来确定
			
			CP_REG=rxBuffer1[8]*65536+rxBuffer1[9]*256+rxBuffer1[10];//计算电流参数寄存器
			C_REG=rxBuffer1[11]*65536+rxBuffer1[12]*256+rxBuffer1[13];//计算电流寄存器
			*C1 =CP_REG/C_REG;//计算电流值
			
			if(rxBuffer1[0]>0xf0 )//判断实时功率是否未溢出
			{
				if(rxBuffer1[0]==0xf1)
				{
					error=0x001;
				}
				//屏幕驱动函数
				*P1=0;
			}
			else
			{
				PP_REG=rxBuffer1[14]*65536+rxBuffer1[15]*256+rxBuffer1[16];//计算功率参数寄存
				P_REG=rxBuffer1[17]*65536+rxBuffer1[18]*256+rxBuffer1[19];//计算功率寄存器
				*P1=(PP_REG/P_REG)*3.006;//计算有效功率
				
			}	
			if((rxBuffer1[20]&0x80)!=old_reg)//判断数据更新寄存器最高位有没有翻转
			{
				k++;
				old_reg=rxBuffer1[20]&0x80;
			}
			PF=(k*65536)+(rxBuffer1[21]*256)+rxBuffer1[22];//计算已用电量脉冲数
			PF_COUNT= 3600000000000/(PP_REG*3.006);//计算1度电对应的脉冲数量
			*E_con1=((PF*10000)/PF_COUNT)/10000.0;//计算已用电量
			memset((void *)rxBuffer1,0,24);//清空缓存
		}	
	}
	//屏幕驱动函数
	
	else if (rxBuffer1[0]==0xaa)
	{
		error=error|0x002;
	
	}
	
	return error;	
}

/* USER CODE END 0 */

UART_HandleTypeDef huart1;
UART_HandleTypeDef huart2;
DMA_HandleTypeDef hdma_usart1_rx;


/* USART1 init function */

void MX_USART2_UART_Init(void)
{

  /* USER CODE BEGIN USART1_Init 0 */

  /* USER CODE END USART1_Init 0 */

  /* USER CODE BEGIN USART1_Init 1 */

  /* USER CODE END USART1_Init 1 */
  huart2.Instance = USART2;
  huart2.Init.BaudRate = 115200;
  huart2.Init.WordLength = UART_WORDLENGTH_8B;
  huart2.Init.StopBits = UART_STOPBITS_1;
  huart2.Init.Parity = UART_PARITY_NONE;
  huart2.Init.Mode = UART_MODE_TX_RX;
  huart2.Init.HwFlowCtl = UART_HWCONTROL_NONE;
  huart2.Init.OverSampling = UART_OVERSAMPLING_16;
  if (HAL_UART_Init(&huart1) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN USART1_Init 2 */

  /* USER CODE END USART1_Init 2 */

}
/* USART2 init function */

void MX_USART1_UART_Init(void)
{

  /* USER CODE BEGIN USART2_Init 0 */

  /* USER CODE END USART2_Init 0 */

  /* USER CODE BEGIN USART2_Init 1 */

  /* USER CODE END USART2_Init 1 */
  huart1.Instance = USART1;
  huart1.Init.BaudRate = 4800;
  huart1.Init.WordLength = UART_WORDLENGTH_8B;
  huart1.Init.StopBits = UART_STOPBITS_1;
  huart1.Init.Parity = UART_PARITY_NONE;
  huart1.Init.Mode = UART_MODE_TX_RX;
  huart1.Init.HwFlowCtl = UART_HWCONTROL_NONE;
  huart1.Init.OverSampling = UART_OVERSAMPLING_16;
  if (HAL_UART_Init(&huart1) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN USART2_Init 2 */

  /* USER CODE END USART2_Init 2 */

}

void HAL_UART_MspInit(UART_HandleTypeDef* uartHandle)
{

  GPIO_InitTypeDef GPIO_InitStruct = {0};
  if(uartHandle->Instance==USART1)
  {
  /* USER CODE BEGIN USART1_MspInit 0 */

  /* USER CODE END USART1_MspInit 0 */
    /* USART1 clock enable */
    __HAL_RCC_USART1_CLK_ENABLE();

    __HAL_RCC_GPIOA_CLK_ENABLE();
    /**USART1 GPIO Configuration
    PA9     ------> USART1_TX
    PA10     ------> USART1_RX
    */
    GPIO_InitStruct.Pin = GPIO_PIN_9;
    GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
    HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

    GPIO_InitStruct.Pin = GPIO_PIN_10;
    GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

  /* USER CODE BEGIN USART1_MspInit 1 */
/* USART1 DMA Init */
    /* USART1_RX Init */
    hdma_usart1_rx.Instance = DMA1_Channel5;
    hdma_usart1_rx.Init.Direction = DMA_PERIPH_TO_MEMORY;
    hdma_usart1_rx.Init.PeriphInc = DMA_PINC_DISABLE;
    hdma_usart1_rx.Init.MemInc = DMA_MINC_ENABLE;
    hdma_usart1_rx.Init.PeriphDataAlignment = DMA_PDATAALIGN_BYTE;
    hdma_usart1_rx.Init.MemDataAlignment = DMA_MDATAALIGN_BYTE;
    hdma_usart1_rx.Init.Mode = DMA_CIRCULAR;
    hdma_usart1_rx.Init.Priority = DMA_PRIORITY_LOW;
    if (HAL_DMA_Init(&hdma_usart1_rx) != HAL_OK)
    {
      Error_Handler();
    }

    __HAL_LINKDMA(uartHandle,hdmarx,hdma_usart1_rx);

    
  /* USER CODE BEGIN USART2_MspInit 1 */

  /* USER CODE END USART2_MspInit 1 */
  
  /* USER CODE END USART1_MspInit 1 */
  }
  else if(uartHandle->Instance==USART2)
  {
  /* USER CODE BEGIN USART2_MspInit 0 */

  /* USER CODE END USART2_MspInit 0 */
    /* USART2 clock enable */
    __HAL_RCC_USART2_CLK_ENABLE();

    __HAL_RCC_GPIOA_CLK_ENABLE();
    /**USART2 GPIO Configuration
    PA2     ------> USART2_TX
    PA3     ------> USART2_RX
    */
    GPIO_InitStruct.Pin = GPIO_PIN_2;
    GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
    HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

    GPIO_InitStruct.Pin = GPIO_PIN_3;
    GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

    }
}

void HAL_UART_MspDeInit(UART_HandleTypeDef* uartHandle)
{

  if(uartHandle->Instance==USART1)
  {
  /* USER CODE BEGIN USART1_MspDeInit 0 */

  /* USER CODE END USART1_MspDeInit 0 */
    /* Peripheral clock disable */
    __HAL_RCC_USART1_CLK_DISABLE();

    /**USART1 GPIO Configuration
    PA9     ------> USART1_TX
    PA10     ------> USART1_RX
    */
    HAL_GPIO_DeInit(GPIOA, GPIO_PIN_9|GPIO_PIN_10);

  /* USER CODE BEGIN USART1_MspDeInit 1 */
/* USART2 DMA DeInit */
    HAL_DMA_DeInit(uartHandle->hdmarx);
    HAL_DMA_DeInit(uartHandle->hdmatx);
		
		
		
  /* USER CODE END USART1_MspDeInit 1 */
  }
  else if(uartHandle->Instance==USART2)
  {
  /* USER CODE BEGIN USART2_MspDeInit 0 */

  /* USER CODE END USART2_MspDeInit 0 */
    /* Peripheral clock disable */
    __HAL_RCC_USART2_CLK_DISABLE();

    /**USART2 GPIO Configuration
    PA2     ------> USART2_TX
    PA3     ------> USART2_RX
    */
    HAL_GPIO_DeInit(GPIOA, GPIO_PIN_2|GPIO_PIN_3);

    
  /* USER CODE BEGIN USART2_MspDeInit 1 */

  /* USER CODE END USART2_MspDeInit 1 */
  }
}

/* USER CODE BEGIN 1 */


/* USER CODE END 1 */
