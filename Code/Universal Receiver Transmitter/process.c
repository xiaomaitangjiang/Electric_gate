#include "process.h"


void empty(void)
{}

void IIC_RECEIVE(void)
{MX_I2C1_Init();}

void UARTSOFT_RECEIVE(void)
{empty();}

void SMBUS_RECEIVE(void)
{empty();}
	
/**
 * @function: int modeprocessing (expand * data)
 * @description: 传输模式处理函数
 * @param {expand} 传入的expand数组
 * @return {unsigned int} result
 */
void modeprocessing (expand * data)
{
	if(data->mode!=0)
	{
		expandinit(data->mode);	
	}
	if(strcmp(&(data->TransmissionProtocols),"usart")==0)
	{
		//uart传输函数
		expandinit(data->mode);
		UARTSOFT_RECEIVE();
	}
	else if(strcmp(&(data->TransmissionProtocols),"i2c")==0)
	{
		//IIC
		expandinit(data->mode);
		IIC_RECEIVE();
	}
	else if(strcmp(&(data->TransmissionProtocols),"smbus")==0)
	{
		//SMBUS
		expandinit(data->mode);
		SMBUS_RECEIVE();
	}
	else if(strcmp(&(data->TransmissionProtocols),"CAN")==0)
	{
		//CAN总线
		expandinit(data->mode);
		
	}
	else
	{data->TransmissionProtocols=NULL;}
}


/**
 * @function: int modeprocessing (expand * data)
 * @description: 通用扩展功能初始化函数
 * @param {int} initmode通讯模式
 * @return {void} 
 */
void expandinit (int initmode)
{
	GPIO_InitTypeDef GPIO_InitStruct = {0};
	if(initmode==0)
	{
		//开漏下拉，低电平
		//默认无输出模式
		GPIO_InitStruct.Pin = GPIO_Init_P;
		GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_OD;
		GPIO_InitStruct.Pull = GPIO_PULLDOWN;
		HAL_GPIO_Init(GPIO_Init_type, &GPIO_InitStruct);
		HAL_GPIO_WritePin(GPIO_Init_type, GPIO_Init_P, GPIO_PIN_RESET);
	}
	else if(initmode==1)
	{ //iic模式
		GPIO_InitStruct.Pin = GPIO_Init_P;
		GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
		GPIO_InitStruct.Pull = GPIO_PULLDOWN;
		HAL_GPIO_Init(GPIO_Init_type, &GPIO_InitStruct);
		MX_I2C1_Init();
	}
	else if(initmode==2)
	{//uart模式
		GPIO_InitStruct.Pin = GPIO_Init_P;
		GPIO_InitStruct.Mode = GPIO_MODE_IT_RISING;
		GPIO_InitStruct.Pull = GPIO_PULLDOWN;
		HAL_GPIO_Init(GPIO_Init_type, &GPIO_InitStruct);
		
	}
	else if(initmode==3)
	{//下降沿中断触发
		GPIO_InitStruct.Pin = GPIO_Init_P;
		GPIO_InitStruct.Mode = GPIO_MODE_IT_FALLING;
		GPIO_InitStruct.Pull = GPIO_PULLDOWN;
		HAL_GPIO_Init(GPIO_Init_type, &GPIO_InitStruct);
		
	}
	else if(initmode==4)
	{//上升沿中断触发
		GPIO_InitStruct.Pin = GPIO_Init_P;
		GPIO_InitStruct.Mode = GPIO_MODE_IT_RISING;
		GPIO_InitStruct.Pull = GPIO_PULLDOWN;
		HAL_GPIO_Init(GPIO_Init_type, &GPIO_InitStruct);
		
	}
	else if(initmode==5)
	{//上升下降中断触发
		GPIO_InitStruct.Pin = GPIO_Init_P;
		GPIO_InitStruct.Mode = GPIO_MODE_IT_RISING_FALLING;
		GPIO_InitStruct.Pull = GPIO_PULLDOWN;
		HAL_GPIO_Init(GPIO_Init_type, &GPIO_InitStruct);
		
	}
	
}



uint32_t dataprocessing (uint8_t * data_IN[24])
{
	uint8_t data[24];
	memcpy(data,data_IN,24);
	if()
	
}
