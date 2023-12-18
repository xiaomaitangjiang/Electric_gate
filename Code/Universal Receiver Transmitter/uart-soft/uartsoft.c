#include "uartsoft.h"

TIM_HandleTypeDef htim4;




/*!
 * @brief	定时器14初始化 
 * @param
 * @return	NONE
 * @note	103us定时器,用于串口数据采样     //没完成，忘了计时器定时时间的计算公式
 */
void MX_TIM4_Init(void)
{

  /* USER CODE BEGIN TIM3_Init 0 */

  /* USER CODE END TIM3_Init 0 */

  TIM_ClockConfigTypeDef sClockSourceConfig = {0};
  TIM_MasterConfigTypeDef sMasterConfig = {0};

  /* USER CODE BEGIN TIM3_Init 1 */

  /* USER CODE END TIM3_Init 1 */
  htim4.Instance = TIM3;
  htim4.Init.Prescaler = 7199;
  htim4.Init.CounterMode = TIM_COUNTERMODE_UP;
  htim4.Init.Period = 4999;
  htim4.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
  htim4.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_DISABLE;
  if (HAL_TIM_Base_Init(&htim4) != HAL_OK)
  {
    Error_Handler();
  }
  sClockSourceConfig.ClockSource = TIM_CLOCKSOURCE_INTERNAL;
  if (HAL_TIM_ConfigClockSource(&htim4, &sClockSourceConfig) != HAL_OK)
  {
    Error_Handler();
  }
  sMasterConfig.MasterOutputTrigger = TIM_TRGO_RESET;
  sMasterConfig.MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE;
  if (HAL_TIMEx_MasterConfigSynchronization(&htim4, &sMasterConfig) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN TIM3_Init 2 */

  /* USER CODE END TIM3_Init 2 */

}


uint32_t delayTime =99;	//!<9600，理论值为104但实际测下来99时效果最好
/*!
 * @brief 	模拟串口1发送一个字节
 * @param	
 * @return	none
 * @note	数据低位在前高位在后
 */
void MUSART1_SendData(uint8_t data)
{
	uint8_t i = 0;
	TX_L();		//!<起始位
	delay_us(delayTime);
	for(i = 0; i < 8; i++){
		if(data & 0x01)
			TX_H();
		else
			TX_L();
		delay_us(delayTime);
		data >>= 1;
	}
	TX_H();		//!<停止位
	delay_us(delayTime);
}



/*!
 * @brief	串口接收IO中断处理函数
 * @param	none
 * @return	NONE
 * @note	none
 */
void EXTI4_15_IRQHandler(void)
{
	if (EXTI_GetITStatus(EXTI_Line9) != RESET){
		if(RX_READ() == 0x00){
			if(rx_state >= STATE_STOP){
				recvData = 0;
				rx_state = STATE_START;
				delay_us(50);
				HAL_TIM_Base_Start_IT(&htim4);	//!<打开定时器，接收数据
			}
		}
        EXTI_ClearITPendingBit(EXTI_Line9);
    }
}



/*!
 * @brief	定时器1中断处理函数
 * @param
 * @return	NONE
 * @note	 
 */
void TIM14_IRQHandler(void)
{
	if(TIM_GetITStatus(TIM14, TIM_IT_Update) != RESET){
	  	rx_state++;                         //!<改变状态机
		if(rx_state == STATE_STOP){
			HAL_TIM_Base_Stop_IT(&htim4);   //!<关闭定时器
			usart_getByte();            //!<接收到停止位之后，处理数据recvData
			return;                     //!<返回
		}
		if(RX_READ()){
			recvData |= (1 << (rx_state - 1));
		}else{
			recvData &= ~(1 <<(rx_state - 1));
		}
		TIM_ClearITPendingBit(TIM14, TIM_FLAG_Update);
	}
}



