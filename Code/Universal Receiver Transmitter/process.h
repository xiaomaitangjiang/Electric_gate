#ifndef process_H_
#define process_H_

#include "stm32f1xx_hal.h"
#include "oledfont.h"
#include "gpio.h"
#include <string.h>
#include "i2c.h"

#define GPIO_Init_P GPIO_PIN_8|GPIO_PIN_9
#define GPIO_Init_type GPIOB
void empty(void);
void UARTSOFT_RECEIVE(void);
void IIC_RECEIVE(void);
void SMBUS_RECEIVE(void);



typedef struct 
{	
	int mode;//模式
	char TransmissionProtocols;//传输协议
	int page;//页码
}expand;

void expandinit (int initmode);
void modeprocessing (expand * data);


#endif /* process_H_ */

