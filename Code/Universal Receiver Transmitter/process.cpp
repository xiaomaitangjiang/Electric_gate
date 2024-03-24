#include "process.hpp"

communciation URTP;
uint8_t URTP_UART_Receive[5];
uint8_t IIC_data[5];

void empty(void)
{}


	
void Highresistance_IO(uint32_t pin,GPIO_TypeDef * pin_type)
{
	  GPIO_InitTypeDef GPIO_InitStruct = {0};
		
		GPIO_InitStruct.Pin = pin;
		GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
		GPIO_InitStruct.Pull = GPIO_NOPULL;
		HAL_GPIO_Init(pin_type, &GPIO_InitStruct);
}
	


void communciation::expanddata(uint8_t * data_in,action  *action_store ,
	uint16_t  *V_SET ,uint16_t  *P_SET ,uint16_t  *I_SET ,uint16_t  *E_SET)
{
	Receive_port(data_in,action_store,V_SET,P_SET,I_SET,E_SET);
}

//�����ⲿ��������ݣ�ת��Ϊ�ڲ���ʽ
//������������
void communciation::Receive_port(uint8_t *receive,action  *action_out ,uint16_t  *V_SET_out ,
														uint16_t  *P_SET_out ,uint16_t  *I_SET_out ,uint16_t  *E_SET_out)
{
	
	dataprocessing(receive,action_out,V_SET_out,P_SET_out,I_SET_out,E_SET_out);
}


void communciation::IIC_RECEIVE(void)
{
	HAL_I2C_Slave_Receive_IT(&hi2c2,IIC_data,sizeof(IIC_data));
}

void communciation::UART_RECEIVE(void)
{
	HAL_UARTEx_ReceiveToIdle_DMA(&huart3,URTP_UART_Receive,5);//dma
	__HAL_DMA_DISABLE_IT(&hdma_usart3_rx,DMA_IT_HT);
}

void communciation::SMBUS_RECEIVE(void)
{empty();}

void communciation::CAN_RECEIVE(void)
{empty();}



/**
 * @function: int modeprocessing (expand * data)
 * @description: URTP�����ʼ������
 * @param {expand} �����expand���飬���ô���Э��
 * @return *
 */
void communciation::expandinit (expand * modedata)
{
		if(modedata->Transmit==uart)
		{
			//uart���亯��
			modeprocessing(modedata->Transmit);
		}
		else if(modedata->Transmit==iic)
		{
			//IIC
			modeprocessing(modedata->Transmit);
		}
		else if(modedata->Transmit==smbus)
		{
			//SMBUS
			modeprocessing(modedata->Transmit);
		}
		else if(modedata->Transmit==can)
		{
			//CAN����
			modeprocessing(modedata->Transmit);
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
void communciation::modeprocessing (TransmissionProtocols initmode)
{
	GPIO_InitTypeDef GPIO_InitStruct = {0};
	
	//Ĭ�ϸ���̬
		Highresistance_IO(GPIO_Init_P,GPIO_Init_type);
		Highresistance_IO(GPIO_Init_Q,GPIO_Init_type);
	if(initmode==iic)
	{ //iicģʽ
		MX_I2C2_Init();
		IIC_RECEIVE();
	}
	else if(initmode==uart)
	{//uartģʽ
		MX_USART3_UART_Init();
		UART_RECEIVE();
	}
	else if(initmode==can)
	{//canģʽ
		MX_CAN_Init();
		CAN_RECEIVE();
	}
	else if(initmode==IT_fall)
	{//�½����жϴ���
		GPIO_InitStruct.Pin = GPIO_Init_Q;
		GPIO_InitStruct.Mode = GPIO_MODE_IT_FALLING;
		GPIO_InitStruct.Pull = GPIO_PULLDOWN;
		HAL_GPIO_Init(GPIO_Init_type, &GPIO_InitStruct);
		
	}
	else if(initmode==IT_rise)
	{//�������жϴ���
		GPIO_InitStruct.Pin = GPIO_Init_Q;
		GPIO_InitStruct.Mode = GPIO_MODE_IT_RISING;
		GPIO_InitStruct.Pull = GPIO_PULLDOWN;
		HAL_GPIO_Init(GPIO_Init_type, &GPIO_InitStruct);
		
	}
	else if(initmode==IT_rise_fall)
	{//�����½��жϴ���
		GPIO_InitStruct.Pin = GPIO_Init_Q;
		GPIO_InitStruct.Mode = GPIO_MODE_IT_RISING_FALLING;
		GPIO_InitStruct.Pull = GPIO_PULLDOWN;
		HAL_GPIO_Init(GPIO_Init_type, &GPIO_InitStruct);
		
	}
	else if(initmode==devices_transmit)
	{
		//���豸��/�ӻ�ģʽ��ʼ��
		
	}
	
}


/**
 * @function: void Firstbyteprocess (int Bytein,action *reaction)
 * @brief�������ڲ���ʽ�Ĺ����޸�λ������
 * @param: ���빦���޸�λ
 * @param: ����һ������
 * @return {*}
 */
void communciation::Firstbyteprocess (uint8_t Bytein,action *reaction)
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
	
void communciation::dataprocessing (uint8_t  *data_IN ,action  *action_store,
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


