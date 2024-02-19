#include "process.h"


void empty(void)
{}

	
void Highresistance_IO(uint32_t pin,GPIO_TypeDef * pin_type)
{
	  GPIO_InitTypeDef GPIO_InitStruct = {0};
		
		GPIO_InitStruct.Pin = pin;
		GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_OD;
		GPIO_InitStruct.Pull = GPIO_NOPULL;
		HAL_GPIO_Init(pin_type, &GPIO_InitStruct);
		HAL_GPIO_WritePin(pin_type, pin, GPIO_PIN_RESET);
}
	
//�����ⲿ��������ݣ�ת��Ϊ�ڲ���ʽ
void communication::IIC_RECEIVE(void)
{
	
}


void communication::UARTSOFT_RECEIVE(void)
{empty();}

void communication::SMBUS_RECEIVE(void)
{empty();}

void communication::CAN_RECEIVE(void)
{
	
}

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
		CAN_RECEIVE();
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
	
	//Ĭ�ϸ���̬
		Highresistance_IO(GPIO_Init_P,GPIO_Init_type);
		Highresistance_IO(GPIO_Init_Q,GPIO_Init_type);
	if(initmode==1)
	{ //iicģʽ
		MX_I2C2_Init();
	}
	else if(initmode==2)
	{//uartģʽ
		MX_USART3_UART_Init();
	}
	else if(initmode==3)
	{//canģʽ
		MX_CAN_Init();
	}
	else if(initmode==4)
	{//�½����жϴ���
		GPIO_InitStruct.Pin = GPIO_Init_Q;
		GPIO_InitStruct.Mode = GPIO_MODE_IT_FALLING;
		GPIO_InitStruct.Pull = GPIO_PULLDOWN;
		HAL_GPIO_Init(GPIO_Init_type, &GPIO_InitStruct);
		
	}
	else if(initmode==5)
	{//�������жϴ���
		GPIO_InitStruct.Pin = GPIO_Init_Q;
		GPIO_InitStruct.Mode = GPIO_MODE_IT_RISING;
		GPIO_InitStruct.Pull = GPIO_PULLDOWN;
		HAL_GPIO_Init(GPIO_Init_type, &GPIO_InitStruct);
		
	}
	else if(initmode==6)
	{//�����½��жϴ���
		GPIO_InitStruct.Pin = GPIO_Init_Q;
		GPIO_InitStruct.Mode = GPIO_MODE_IT_RISING_FALLING;
		GPIO_InitStruct.Pull = GPIO_PULLDOWN;
		HAL_GPIO_Init(GPIO_Init_type, &GPIO_InitStruct);
		
	}
	
}


/**
 * @function: void Firstbyteprocess (int Bytein,action *reaction)
 * @brief�������ڲ���ʽ�Ĺ����޸�λ������
 * @param: ���빦���޸�λ
 * @param: ����һ������
 * @return {*}
 */
void communication::Firstbyteprocess (uint8_t Bytein,action *reaction)
{
		uint8_t bitcheck[]={B1,B10,B100,B1000,B10000,B100000,B1000000,B10000000};//���������������ͷ�ļ�
		

		int ParityCheckdata=0;
		//��żУ��
		for (int bit=1 ; bit<=8 ; bit++)
		{
			if ((Bytein&bitcheck[bit])==1)
			{
				ParityCheckdata++;
			}
		}
		if(ParityCheckdata%2==0)
		{ParityCheckdata=0;}
		else
		{ParityCheckdata=1;}
		if(ParityCheckdata!=(Bytein&bitcheck[0]))
		{
			*reaction=byteerror;
		}
		
		//λ���
		for (int bit=1,actionways=1,actionbit=0 ; bit<=8 && *reaction!=byteerror ; bit++,actionways++,actionbit++)
		{
			if ((Bytein&bitcheck[bit])==1)
			{
				(*(reaction+actionbit))=action(actionways);
			}
		}
		
}

/**
 * @function: void Firstbyteprocess (int Bytein,action *reaction)
 * @brief�������ڲ���ʽ���������
 * @param: �����ڲ���ʽ����
 * @param: ������������
 * @param: �ⲿ�豸������趨����ѹ
 * @param: �ⲿ�豸������趨�����
 * @param: �ⲿ�豸������趨������
 * @param: �ⲿ�豸������趨����õ����
 * @return {*}
 */
	
void communication::dataprocessing (uint8_t  *data_IN[5] ,action  *action_store,
	uint16_t  *V_SET ,uint16_t  *P_SET ,uint16_t  *I_SET ,uint16_t  *E_SET)

{
	uint8_t data[5];
	memcpy(data,data_IN,5);
	
	uint8_t funtionswitchdata=data[0];
	Firstbyteprocess(funtionswitchdata,action_store);
	*V_SET=(uint16_t)data[1];
	*I_SET=(uint16_t)data[2];
	*P_SET=(uint16_t)data[3];
	*E_SET=(uint16_t)data[4];
}


