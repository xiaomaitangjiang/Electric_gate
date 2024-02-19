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
	
//处理外部传入的数据，转换为内部格式
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
 * @description: 传输模式处理函数
 * @param {expand} 传入的expand数组
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
		//uart传输函数
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
		//CAN总线
		expandinit(modedata->mode);
		CAN_RECEIVE();
	}
	else
	{modedata->Transmit=none;}
}


/**
 * @function: int modeprocessing (expand * data)
 * @description: 通用扩展功能初始化函数
 * @param {int} initmode通讯模式
 * @return {void} 
 */
void communication::expandinit (int initmode)
{
	GPIO_InitTypeDef GPIO_InitStruct = {0};
	
	//默认高阻态
		Highresistance_IO(GPIO_Init_P,GPIO_Init_type);
		Highresistance_IO(GPIO_Init_Q,GPIO_Init_type);
	if(initmode==1)
	{ //iic模式
		MX_I2C2_Init();
	}
	else if(initmode==2)
	{//uart模式
		MX_USART3_UART_Init();
	}
	else if(initmode==3)
	{//can模式
		MX_CAN_Init();
	}
	else if(initmode==4)
	{//下降沿中断触发
		GPIO_InitStruct.Pin = GPIO_Init_Q;
		GPIO_InitStruct.Mode = GPIO_MODE_IT_FALLING;
		GPIO_InitStruct.Pull = GPIO_PULLDOWN;
		HAL_GPIO_Init(GPIO_Init_type, &GPIO_InitStruct);
		
	}
	else if(initmode==5)
	{//上升沿中断触发
		GPIO_InitStruct.Pin = GPIO_Init_Q;
		GPIO_InitStruct.Mode = GPIO_MODE_IT_RISING;
		GPIO_InitStruct.Pull = GPIO_PULLDOWN;
		HAL_GPIO_Init(GPIO_Init_type, &GPIO_InitStruct);
		
	}
	else if(initmode==6)
	{//上升下降中断触发
		GPIO_InitStruct.Pin = GPIO_Init_Q;
		GPIO_InitStruct.Mode = GPIO_MODE_IT_RISING_FALLING;
		GPIO_InitStruct.Pull = GPIO_PULLDOWN;
		HAL_GPIO_Init(GPIO_Init_type, &GPIO_InitStruct);
		
	}
	
}


/**
 * @function: void Firstbyteprocess (int Bytein,action *reaction)
 * @brief：处理内部格式的功能修改位的数据
 * @param: 输入功能修改位
 * @param: 输入一个数组
 * @return {*}
 */
void communication::Firstbyteprocess (uint8_t Bytein,action *reaction)
{
		uint8_t bitcheck[]={B1,B10,B100,B1000,B10000,B100000,B1000000,B10000000};//二进制数，定义见头文件
		

		int ParityCheckdata=0;
		//奇偶校验
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
		
		//位检测
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
 * @brief：处理内部格式数组的数据
 * @param: 传入内部格式数组
 * @param: 传出动作数组
 * @param: 外部设备传入的设定最大电压
 * @param: 外部设备传入的设定最大功率
 * @param: 外部设备传入的设定最大电流
 * @param: 外部设备传入的设定最大用电度数
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


