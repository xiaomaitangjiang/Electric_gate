#include "uartsoft.h"

/*!
 * @brief 	模拟串口1 TX IO口配置
 * @param	none
 * @return	none
 * @note	Tx(PA10)
 */
void MUSART1_TX_init(void)
{
    GPIO_InitTypeDef GPIO_InitStructure;
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;		 
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
    GPIO_InitStructure.GPIO_PuPd  = GPIO_PuPd_UP;
    GPIO_Init(GPIOA, &GPIO_InitStructure); 
    GPIO_SetBits(GPIOA, GPIO_Pin_10);
}



/*!
 * @brief 	模拟串口1 RX IO口配置
 * @param	none
 * @return	none
 * @note	Rx(PA9)
 */
void MUSART1_RX_init(void)
{
    GPIO_InitTypeDef GPIO_InitStructure;
    EXTI_InitTypeDef EXTI_InitStructure;
    NVIC_InitTypeDef NVIC_InitStructure;
      
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_SYSCFG, ENABLE);    //!<外部中断时钟
	
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9;		 
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
    GPIO_InitStructure.GPIO_PuPd  = GPIO_PuPd_UP;
    GPIO_Init(GPIOA, &GPIO_InitStructure); 
	
    SYSCFG_EXTILineConfig(EXTI_PortSourceGPIOA,EXTI_PinSource9);  
    
    EXTI_InitStructure.EXTI_Line=EXTI_Line9;  
    EXTI_InitStructure.EXTI_Mode=EXTI_Mode_Interrupt;  
    EXTI_InitStructure.EXTI_Trigger=EXTI_Trigger_Falling;    //下降沿中断
    EXTI_InitStructure.EXTI_LineCmd=ENABLE;  
    EXTI_Init(&EXTI_InitStructure); 
     
    NVIC_InitStructure.NVIC_IRQChannel=EXTI4_15_IRQn;  
    NVIC_InitStructure.NVIC_IRQChannelPriority=0x01;  
    NVIC_InitStructure.NVIC_IRQChannelCmd=ENABLE;  
    NVIC_Init(&NVIC_InitStructure);
}


/*!
 * @brief	定时器14初始化 
 * @param
 * @return	NONE
 * @note	103us定时器,用于串口数据采样
 */
void Time14Init(void)
{
    TIM_TimeBaseInitTypeDef TIM_TimerBaseStruct;
 
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM14, ENABLE);   //!<时钟使能
    TIM_DeInit(TIM14);					    //!<Time1定时器重设缺省值
    TIM_TimerBaseStruct.TIM_Period=103;     		    //!<设置重载寄存器初值 (设置为103，即：定时104us)
    TIM_TimerBaseStruct.TIM_Prescaler=7;     	//!<使用内部8M时钟，分频8(7+1)，8M/8 = 1000000，故数1000000(999999+1)下，达1秒
    TIM_TimerBaseStruct.TIM_ClockDivision=0;		    //!<不分频
    TIM_TimerBaseStruct.TIM_CounterMode=TIM_CounterMode_Up; //!<设置计数器向上计数模式
    TIM_TimeBaseInit(TIM14,&TIM_TimerBaseStruct);
 
    NVIC_InitTypeDef NVIC_InitStructure;
    NVIC_InitStructure.NVIC_IRQChannel=TIM14_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPriority=2;
    NVIC_InitStructure.NVIC_IRQChannelCmd=ENABLE;
    NVIC_Init(&NVIC_InitStructure);
    TIM_ClearITPendingBit(TIM14, TIM_FLAG_Update);
    TIM_ITConfig(TIM14,TIM_IT_Update,ENABLE);  		    //!<使能TIM1中断源
    TIM_Cmd(TIM14,DISABLE);                     	    //!<禁能TIM1定时器	
	
}


uint32 delayTime =99;	//!<9600，理论值为104但实际测下来99时效果最好
/*!
 * @brief 	模拟串口1发送一个字节
 * @param	
 * @return	none
 * @note	数据低位在前高位在后
 */
void MUSART1_SendData(uint8 data)
{
	uint8 i = 0;
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
				TIM_Cmd(TIM14, ENABLE);	//!<打开定时器，接收数据
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
			TIM_Cmd(TIM14, DISABLE);    //!<关闭定时器
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



