#include "main_program.h"
#include <stdio.h>
#include "oled.h"
#include "esp8266.h"


/*
*************************************
�궨��
*************************************
*/

/*
*************************************
��������
*************************************
*/
//extern uint16_t index_oled;
//extern uint16_t index_dht11;
//extern uint8_t index_send_msg;
//extern uint8_t index_led;
/*
*************************************
��������
*************************************
*/

/*
*************************************
��������
*************************************
*/
uint8_t led_status =0;
uint8_t temp_value =0;
uint8_t humi_value =0;
/*
*************************************
��������
*************************************
*/


/**
  * @brief          ��ʼ������,�൱��Arduino��loop()����,һֱִ�иú���
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

