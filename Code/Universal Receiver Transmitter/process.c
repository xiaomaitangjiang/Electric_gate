#include "process.h"


void empty(void)
{}

void communication::IIC_RECEIVE(void)
{MX_I2C1_Init();}

void communication::UARTSOFT_RECEIVE(void)
{empty();}

void communication::SMBUS_RECEIVE(void)
{empty();}


communication::communication()
{}
communication::~communication()
{}

/**
 * @function: int modeprocessing (expand * data)
 * @description: ����ģʽ������
 * @param {expand} �����expand����
 * @return {unsigned int} result
 */
void communication::modeprocessing (expand * modedata)
{
	if(modedata->mode!=0)
	{
		expandinit(modedata->mode);	
	}
	if(modedata->Transmit==uart)
	{
		//uart���亯��
		expandinit(modedata->mode);
		UARTSOFT_RECEIVE();
	}
	else if(modedata->Transmit==iic)
	{
		//IIC
		expandinit(modedata->mode);
		IIC_RECEIVE();
	}
	else if(modedata->Transmit==smbus)
	{
		//SMBUS
		expandinit(modedata->mode);
		SMBUS_RECEIVE();
	}
	else if(modedata->Transmit==can)
	{
		//CAN����
		expandinit(modedata->mode);
		
	}
	else
	{modedata->Transmit=none;}
}


/**
 * @function: int modeprocessing (expand * data)
 * @description: ͨ����չ���ܳ�ʼ������
 * @param {int} initmodeͨѶģʽ
 * @return {void} 
 */
void communication::expandinit (int initmode)
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




action* communication::bitprocess (int Bytein,action reaction[8])
{
	enum bitcheck
		{
			bit0check=0b1,
			bit1check=0b10,
			bit2check=0b100,
			bit3check=0b1000,
			bit4check=0b10000,
			bit5check=0b100000,
			bit6check=0b1000000,
			bit7check=0b10000000
	  }bitchecker;
		
		
		int actioncode=0;
		int ParityCheckdata=0;
		//��żУ��
		for (int bit=1;bit<=0b10000000;bit=bit<<1)
		{
			if ((Bytein&(bitchecker=bitcheck(bit)))==1)
			{
				ParityCheckdata++;
			}
		}
		if(ParityCheckdata%2==0)
		{ParityCheckdata=1;}
		else
		{ParityCheckdata=0;}
		if(ParityCheckdata!=(Bytein&(bitchecker=bitcheck(0b1))))
		{
			reaction[0]=byteerror;
		}
		
		//λ���
		for (int bit=1,actionways=1,actionbit=0;bit<=0b10000000 && reaction[0]!=byteerror;bit=bit<<1,actionways++,actionbit++)
		{
			if ((Bytein&(bitchecker=bitcheck(bit)))==1)
			{
				reaction[actionbit]=action(actionways);
			}
		}
		
	return reaction;
}


	
action* communication::dataprocessing (uint8_t * data_IN[5])
{
	uint8_t data[5];
	memcpy(data,data_IN,5);
	
	int funtionswitchdata=data[1];
	action *actionway;
	actionway=bitprocess(funtionswitchdata,actionway);
	
	return actionway;
}


