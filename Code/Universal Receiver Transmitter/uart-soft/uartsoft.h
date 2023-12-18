#ifndef _UARTSOFT_H_
#define _UARTSOFT_H_

#include "stm32f1xx_hal.h"
#include "gpio.h"
#include "time.h"
#include "process.h"

typedef enum{
	STATE_START=0,
	STATE_BIT0,
	STATE_BIT1,
	STATE_BIT2,
	STATE_BIT3,
	STATE_BIT4,
	STATE_BIT5,
	STATE_BIT6,
	STATE_BIT7,
	STATE_STOP
}RX_STATE;
 
RX_STATE	rx_state = STATE_STOP;
uint8_t recvData=0;    //!<接收的一个字节数据，全局变量


#endif /* _UARTSOFT_H_ */