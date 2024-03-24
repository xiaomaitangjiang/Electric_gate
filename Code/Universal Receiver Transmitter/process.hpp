#ifndef __process_H_
#define __process_H_

#ifdef __cplusplus
extern "C" {
#endif

#include "stm32f1xx_hal.h"
#include "stm32f1xx_hal_i2c.h" 
//#include "oledfont.h"
#include "gpio.h"
#include "can.h"
#include "usart.h"
#include "i2c.h"
#include <string.h>
#include <stdint.h>
#include "typeport.h"


#define GPIO_Init_Q (GPIO_PIN_10|GPIO_PIN_11)
#define GPIO_Init_P (GPIO_PIN_8|GPIO_PIN_9)
#define GPIO_Init_type GPIOB
#ifdef __cplusplus
}
#endif

#include <vector>

using std::vector;








#define B1 0x1
#define B10 0x2
#define B100 0x4
#define B1000 0x8
#define B10000 0x10
#define B100000 0x20
#define B1000000 0x40
#define B10000000 0x80




enum TransmissionProtocols
	{
		none,
		uart,
		iic,
		can,
		smbus,
		IT_fall,//�½����ж�
		IT_rise,//�������ж�
		IT_rise_fall,//����/�½����ж�
		devices_transmit//���豸����
	};//����Э��	
typedef struct 
{	
	TransmissionProtocols Transmit;//����Э��
	int page;//ҳ��
}expand;






		
		

class communciation{
	public:

		void expandinit(expand * modedata);
		void expanddata(uint8_t * data_in,action  *action_store ,uint16_t  *V_SET ,uint16_t  *P_SET ,uint16_t  *I_SET ,uint16_t  *E_SET);
		
	private: 
		
		int actiontype();
		void modeprocessing(TransmissionProtocols initmode);
	
		void UART_RECEIVE(void);
		void IIC_RECEIVE(void);
		void SMBUS_RECEIVE(void);
		void CAN_RECEIVE(void);
	
		void Receive_port(uint8_t *receive,action  *action_out ,uint16_t  *V_SET_out ,uint16_t  *P_SET_out ,uint16_t  *I_SET_out ,uint16_t  *E_SET_out);
		
		void Firstbyteprocess (uint8_t	Bytein,action *reaction);
	  void dataprocessing (uint8_t  *data_IN ,action  *action_store ,uint16_t  *V_SET ,uint16_t  *P_SET ,uint16_t  *I_SET ,uint16_t  *E_SET);
		
		

	};

void Highresistance_IO(uint32_t pin,GPIO_TypeDef * pin_type);
void empty(void);


	
	
/*
	�������ݴ����������ṹ���ⲿ������ڲ�����
	�ⲿ�������ݸ�ʽ�������ⲿ��ʽת��Ϊ�ڲ�Ĭ�ϸ�ʽ��
	�ڲ�����������ⲿ��������ڲ�Ĭ�ϸ�ʽ�����ݣ�������������Ҫִ�еĲ���
	*/

/*
	�ڲ���ʽ��
	��СΪ5��uint8_t��������
	�����0��Ԫ��Ϊ�����޸�λ
	��1��Ԫ��Ϊ��ǰ��բ������ѹ
	��2��Ԫ��Ϊ��ǰ��բ��������
	��3��Ԫ��Ϊ��ǰ��բ�������
	��4��Ԫ��Ϊ��ǰ��բ������ۼ��õ����
	*/	
	
/*
	�����޸�λ
	����8λ 
	1λ żУ��λ��1������Ϊż����0��
	2λ ����/�رյ�բ
	3λ ����/�رհ�ȫ��������
	4λ ����/�ر���չ����
	5λ żУ��仯λ �������壩
	6-8λ Ĭ��Ϊ0
	*/
	
	
	/*
	uart��iic��can���߹���ʹ��������ͬ��gpio
	can������PB8��PB9ʵ��
	uart��iic��PB10��PB11ʵ��
	
	*/
#endif /* process_H_ */

