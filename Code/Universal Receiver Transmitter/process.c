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
 * @description: ����ģʽ������
 * @param {expand} �����expand����
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
		//uart���亯��
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
		//CAN����
		expandinit(data->mode);
		
	}
	else
	{data->TransmissionProtocols=NULL;}
}


/**
 * @function: int modeprocessing (expand * data)
 * @description: ͨ����չ���ܳ�ʼ������
 * @param {int} initmodeͨѶģʽ
 * @return {void} 
 */
void expandinit (int initmode)
{
	GPIO_InitTypeDef GPIO_InitStruct = {0};
	if(initmode==0)
	{
		//��©�������͵�ƽ
		//Ĭ�������ģʽ
		GPIO_InitStruct.Pin = GPIO_Init_P;
		GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_OD;
		GPIO_InitStruct.Pull = GPIO_PULLDOWN;
		HAL_GPIO_Init(GPIO_Init_type, &GPIO_InitStruct);
		HAL_GPIO_WritePin(GPIO_Init_type, GPIO_Init_P, GPIO_PIN_RESET);
	}
	else if(initmode==1)
	{ //iicģʽ
		GPIO_InitStruct.Pin = GPIO_Init_P;
		GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
		GPIO_InitStruct.Pull = GPIO_PULLDOWN;
		HAL_GPIO_Init(GPIO_Init_type, &GPIO_InitStruct);
		MX_I2C1_Init();
	}
	else if(initmode==2)
	{//uartģʽ
		GPIO_InitStruct.Pin = GPIO_Init_P;
		GPIO_InitStruct.Mode = GPIO_MODE_IT_RISING;
		GPIO_InitStruct.Pull = GPIO_PULLDOWN;
		HAL_GPIO_Init(GPIO_Init_type, &GPIO_InitStruct);
		
	}
	else if(initmode==3)
	{//�½����жϴ���
		GPIO_InitStruct.Pin = GPIO_Init_P;
		GPIO_InitStruct.Mode = GPIO_MODE_IT_FALLING;
		GPIO_InitStruct.Pull = GPIO_PULLDOWN;
		HAL_GPIO_Init(GPIO_Init_type, &GPIO_InitStruct);
		
	}
	else if(initmode==4)
	{//�������жϴ���
		GPIO_InitStruct.Pin = GPIO_Init_P;
		GPIO_InitStruct.Mode = GPIO_MODE_IT_RISING;
		GPIO_InitStruct.Pull = GPIO_PULLDOWN;
		HAL_GPIO_Init(GPIO_Init_type, &GPIO_InitStruct);
		
	}
	else if(initmode==5)
	{//�����½��жϴ���
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
