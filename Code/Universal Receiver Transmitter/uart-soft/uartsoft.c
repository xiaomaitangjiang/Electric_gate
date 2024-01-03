#include "uartsoft.h"

TIM_HandleTypeDef htim4;




/*!
 * @brief	��ʱ��14��ʼ�� 
 * @param
 * @return	NONE
 * @note	103us��ʱ��,���ڴ������ݲ���     //û��ɣ����˼�ʱ����ʱʱ��ļ��㹫ʽ
 */



uint32_t delayTime =99;	//!<9600������ֵΪ104��ʵ�ʲ�����99ʱЧ�����
/*!
 * @brief 	ģ�⴮��1����һ���ֽ�
 * @param	
 * @return	none
 * @note	���ݵ�λ��ǰ��λ�ں�
 */
void MUSART1_SendData(uint8_t data)
{
	uint8_t i = 0;
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
				HAL_TIM_Base_Start_IT(&htim4);	//!<�򿪶�ʱ������������
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
			HAL_TIM_Base_Stop_IT(&htim4);   //!<�رն�ʱ��
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



