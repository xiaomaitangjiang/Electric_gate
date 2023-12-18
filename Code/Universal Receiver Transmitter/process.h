#ifndef process_H_
#define process_H_

#include "stm32f1xx_hal.h"
#include "oledfont.h"
#include "gpio.h"
#include <string.h>
#include "i2c.h"

#define GPIO_Init_P GPIO_PIN_8|GPIO_PIN_9
#define GPIO_Init_type GPIOB

void UARTSOFT_RECEIVE(void);
void IIC_RECEIVE(void);
void SMBUS_RECEIVE(void);

typedef struct 
{	
	int mode;//ģʽ
	char TransmissionProtocols;//����Э��
	int page;//ҳ��
}expand;

void expandinit (int initmode);
void modeprocessing (expand * data);


#endif /* process_H_ */

