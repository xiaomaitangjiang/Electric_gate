#ifndef process_H_
#define process_H_

#ifdef __cplusplus
extern "C" {
#endif

#include "stm32f1xx_hal.h"
#include "oledfont.h"
#include "gpio.h"
#include "can.h"
#include "usart.h"
#include "i2c.h"
#include <string.h>
#include <stdint.h>



#define GPIO_Init_Q (GPIO_PIN_10|GPIO_PIN_11)
#define GPIO_Init_P (GPIO_PIN_8|GPIO_PIN_9)
#define GPIO_Init_type GPIOB
#ifdef __cplusplus
}
#endif

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
		smbus
	};//传输协议	
typedef struct 
{	
	int mode;//模式
	TransmissionProtocols Transmit;
	int page;//页码
}expand;





enum action
		{
			openswitch=1,
			
			opensafefuntion=2,
			
			openexpandfuntion=3,
			
			closeswitch=4,
			closesafefuntion=5,
			closeexpandfuntion=6,
			closesafemode=7,
			byteerror=0,
			null=12
		};
		
		

class communication{
	public:
		communication();
		~communication();
		void expandinit(int initmode);
		void expanddata(uint8_t * data_IN[5]);
		int actiontype();
	private: 
		void modeprocessing (expand * modedata);
		void UARTSOFT_RECEIVE(void);
		void IIC_RECEIVE(void);
		void SMBUS_RECEIVE(void);
		void CAN_RECEIVE(void);
		void Firstbyteprocess (uint8_t	Bytein,action *reaction);
		action* parameterprocessing (uint8_t * data_IN[5]);
	  void dataprocessing (uint8_t  *data_IN[5] ,action  *action_store ,uint16_t  *V_SET ,uint16_t  *P_SET ,uint16_t  *I_SET ,uint16_t  *E_SET);
		
		

	};

void Highresistance_IO(uint32_t pin,GPIO_TypeDef * pin_type);
void empty(void);


/*
	程序数据处理包括两层结构，外部处理和内部处理。
	外部处理将数据格式由任意外部格式转换为内部默认格式，
	内部处理则接收外部处理传入的内部默认格式的数据，并处理和输出需要执行的操作
	*/

/*
	内部格式：
	大小为5的uint8_t类型数组
	数组第0个元素为功能修改位
	第1个元素为当前电闸的最大电压
	第2个元素为当前电闸的最大电流
	第3个元素为当前电闸的最大功率
	第4个元素为当前电闸的最大累计用电度数
	*/	
	
/*
	功能修改位
	包括8位 
	1位 偶校验位（1的数量为偶则置0）
	2位 开启/关闭电闸
	3位 开启/关闭安全保护功能
	4位 开启/关闭扩展功能
	5位 偶校验变化位 （无意义）
	6-8位 默认为0
	*/
	
	
	/*
	uart、iic和can总线功能使用两个不同的gpio
	can总线由PB8，PB9实现
	uart、iic由PB10，PB11实现
	
	*/
#endif /* process_H_ */

