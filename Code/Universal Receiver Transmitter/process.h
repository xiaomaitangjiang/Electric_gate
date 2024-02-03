#ifndef process_H_
#define process_H_

#ifdef __cplusplus
extern "C" {
#endif

#include "stm32f1xx_hal.h"
#include "oledfont.h"
#include "gpio.h"
#include <string.h>
#include "i2c.h"
#include <stdint.h>





#define GPIO_Init_P GPIO_PIN_8|GPIO_PIN_9
#define GPIO_Init_type GPIOB
#ifdef __cplusplus
}
#endif


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


void empty(void);


enum action
		{
			openswitch=1,//双位检测
			openswitch1=2,
			
			opensafefuntion=3,//双位检测
			opensafefuntion1=4,
			
			openexpandfuntion=5,//双位检测
			openexpandfuntion1=6,
			
			opensafemode=7,
			
			closeswitch=8,
			closesafefuntion=9,
			closeexpandfuntion=10,
			closesafemode=11,
			byteerror=0
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
		action* parameterprocessing (uint8_t * data_IN[5]);
	  action* dataprocessing (uint8_t * data_IN[5]);
		action* bitprocess (int Bytein,action *reaction);
		

	};

	


#endif /* process_H_ */

