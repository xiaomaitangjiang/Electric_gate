#include "main_program.h"
#include <stdio.h>
#include "oled.h"
#include "esp8266.h"


/*
*************************************
宏定义
*************************************
*/

/*
*************************************
变量声明
*************************************
*/
//extern uint16_t index_oled;
//extern uint16_t index_dht11;
//extern uint8_t index_send_msg;
//extern uint8_t index_led;
/*
*************************************
函数声明
*************************************
*/

/*
*************************************
变量定义
*************************************
*/
uint8_t led_status =0;
uint8_t temp_value =0;
uint8_t humi_value =0;
/*
*************************************
函数定义
*************************************
*/


/**
  * @brief          初始化函数,相当于Arduino的loop()函数,一直执行该函数
  * @param[in]      none
  * @retval         none
  */
void user_main_program(void)
{/*
  if(esp8266_receive_msg()!=1)
  {
  }
  if(index_dht11 ==1)
  {
    
    index_dht11=0;
  }
  if(index_led==1)
  {
    
    index_led=0;
  }
  if(index_oled==1)
  {
    
    index_oled=0;
  }
  if(index_send_msg==1)
	{
		index_send_msg =0;
		if(esp8266_send_msg()==0)
		{
		}
		else
		{
			printf("msg send error\r\n");
		}
	}
	*/
}

