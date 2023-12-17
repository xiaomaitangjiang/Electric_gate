#include "uartsoft.h"

/*!
 * @brief 	ģ�⴮��1 TX IO������
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
 * @brief 	ģ�⴮��1 RX IO������
 * @param	none
 * @return	none
 * @note	Rx(PA9)
 */
void MUSART1_RX_init(void)
{
    GPIO_InitTypeDef GPIO_InitStructure;
    EXTI_InitTypeDef EXTI_InitStructure;
    NVIC_InitTypeDef NVIC_InitStructure;
      
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_SYSCFG, ENABLE);    //!<�ⲿ�ж�ʱ��
	
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9;		 
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
    GPIO_InitStructure.GPIO_PuPd  = GPIO_PuPd_UP;
    GPIO_Init(GPIOA, &GPIO_InitStructure); 
	
    SYSCFG_EXTILineConfig(EXTI_PortSourceGPIOA,EXTI_PinSource9);  
    
    EXTI_InitStructure.EXTI_Line=EXTI_Line9;  
    EXTI_InitStructure.EXTI_Mode=EXTI_Mode_Interrupt;  
    EXTI_InitStructure.EXTI_Trigger=EXTI_Trigger_Falling;    //�½����ж�
    EXTI_InitStructure.EXTI_LineCmd=ENABLE;  
    EXTI_Init(&EXTI_InitStructure); 
     
    NVIC_InitStructure.NVIC_IRQChannel=EXTI4_15_IRQn;  
    NVIC_InitStructure.NVIC_IRQChannelPriority=0x01;  
    NVIC_InitStructure.NVIC_IRQChannelCmd=ENABLE;  
    NVIC_Init(&NVIC_InitStructure);
}


/*!
 * @brief	��ʱ��14��ʼ�� 
 * @param
 * @return	NONE
 * @note	103us��ʱ��,���ڴ������ݲ���
 */
void Time14Init(void)
{
    TIM_TimeBaseInitTypeDef TIM_TimerBaseStruct;
 
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM14, ENABLE);   //!<ʱ��ʹ��
    TIM_DeInit(TIM14);					    //!<Time1��ʱ������ȱʡֵ
    TIM_TimerBaseStruct.TIM_Period=103;     		    //!<�������ؼĴ�����ֵ (����Ϊ103��������ʱ104us)
    TIM_TimerBaseStruct.TIM_Prescaler=7;     	//!<ʹ���ڲ�8Mʱ�ӣ���Ƶ8(7+1)��8M/8 = 1000000������1000000(999999+1)�£���1��
    TIM_TimerBaseStruct.TIM_ClockDivision=0;		    //!<����Ƶ
    TIM_TimerBaseStruct.TIM_CounterMode=TIM_CounterMode_Up; //!<���ü��������ϼ���ģʽ
    TIM_TimeBaseInit(TIM14,&TIM_TimerBaseStruct);
 
    NVIC_InitTypeDef NVIC_InitStructure;
    NVIC_InitStructure.NVIC_IRQChannel=TIM14_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPriority=2;
    NVIC_InitStructure.NVIC_IRQChannelCmd=ENABLE;
    NVIC_Init(&NVIC_InitStructure);
    TIM_ClearITPendingBit(TIM14, TIM_FLAG_Update);
    TIM_ITConfig(TIM14,TIM_IT_Update,ENABLE);  		    //!<ʹ��TIM1�ж�Դ
    TIM_Cmd(TIM14,DISABLE);                     	    //!<����TIM1��ʱ��	
	
}


uint32 delayTime =99;	//!<9600������ֵΪ104��ʵ�ʲ�����99ʱЧ�����
/*!
 * @brief 	ģ�⴮��1����һ���ֽ�
 * @param	
 * @return	none
 * @note	���ݵ�λ��ǰ��λ�ں�
 */
void MUSART1_SendData(uint8 data)
{
	uint8 i = 0;
	TX_L();		//!<��ʼλ
	delay_us(delayTime);
	for(i = 0; i < 8; i++){
		if(data & 0x01)
			TX_H();
		else
			TX_L();
		delay_us(delayTime);
		data >>= 1;
	}
	TX_H();		//!<ֹͣλ
	delay_us(delayTime);
}



/*!
 * @brief	���ڽ���IO�жϴ�����
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
				TIM_Cmd(TIM14, ENABLE);	//!<�򿪶�ʱ������������
			}
		}
        EXTI_ClearITPendingBit(EXTI_Line9);
    }
}



/*!
 * @brief	��ʱ��1�жϴ�����
 * @param
 * @return	NONE
 * @note	 
 */
void TIM14_IRQHandler(void)
{
	if(TIM_GetITStatus(TIM14, TIM_IT_Update) != RESET){
	  	rx_state++;                         //!<�ı�״̬��
		if(rx_state == STATE_STOP){
			TIM_Cmd(TIM14, DISABLE);    //!<�رն�ʱ��
			usart_getByte();            //!<���յ�ֹͣλ֮�󣬴�������recvData
			return;                     //!<����
		}
		if(RX_READ()){
			recvData |= (1 << (rx_state - 1));
		}else{
			recvData &= ~(1 <<(rx_state - 1));
		}
		TIM_ClearITPendingBit(TIM14, TIM_FLAG_Update);
	}
}



