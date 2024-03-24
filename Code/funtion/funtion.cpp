#include "funtion.hpp"


//内置温度传感器数据处理
uint16_t adc_data_processing (uint16_t *ADC_data,uint8_t channel)
{
	return *(ADC_data+(channel-1));
	
		
}


void expanddata_C(uint8_t * data_user, action  *action_user ,uint16_t  *V_SET_user ,
								uint16_t  *P_SET_user ,uint16_t  *I_SET_user ,uint16_t  *E_SET_user)
{
	URTP.expanddata(data_user,action_user,V_SET_user,P_SET_user,I_SET_user,E_SET_user);
}


void HAL_Delay(uint32_t Delay)
{
	uint32_t t=Delay*3127;
	while(t--);
	
}

/**
 * @function: int modeprocessing (expand * data)
 * @description: 安全判断函数
 * @param {double} 当前电压
 * @param {double} 当前电流
 * @param {double} 当前功率
 * @param {double} 当前用电度数
 * @param {uint16_t} 设定的数组/vector的第一个元素的指针（设定的电压/电流/功率/用电度数,上一次的电压/上一次的功率,长度为6）
 * @param {uint16_t} 设定的数组/vector的尾后指针
 * @return *
 */
uint16_t safefuntion (double V_in,double C_in,double P_in, double E_in,uint16_t *beg, uint16_t *end)
{
	vector<uint16_t> set;
	for (;beg!=end;beg++)
	{
		set.push_back(*beg);
	}
	
	//safety funtion
		uint16_t error=0;
		if(V_in>=set[0])
		{
			error=0x008|error;	
		}
		else if(0x008&error)
		{error=0x008^error;}

		if(C_in>=set[1])
		{
			error=0x010|error;
		}
		else if(0x010&error)
		{error=0x010^error;}

		if(P_in>=set[2])
		{
			error=0x020|error;
		}
		else if(0x020&error)
		{error=0x020^error;}

		if(E_in>=set[3])
		{
			error=0x040|error;
		}
		else if(0x040&error)
		{error=0x040^error;}
		if(V_in-22>set[4])
		{
			error=0x080|error;
		}
		else if(0x080&error)
		{error=0x080^error;}
		if(set[5]>set[3])
		{
			error=0x100|error;
		}
		else if(0x100&error)
		{error=0x100^error;}
		return error;
}

//control
void switch_control(uint8_t cross,uint8_t switch_tim,uint8_t switch_user)
{
if(cross==1||switch_tim==1||switch_user==1)
	{
		HAL_GPIO_WritePin(GPIOB, GPIO_PIN_4, GPIO_PIN_SET);
		HAL_Delay(1);
		cross=0;
	}
	else
	{
		HAL_GPIO_WritePin(GPIOB, GPIO_PIN_4, GPIO_PIN_RESET);
	}
}


uint8_t errorprocessing (const uint16_t error,uint16_t * index,uint8_t *error_switchoff)
{
	if(0x008&error ||0x010&error ||0x020&error ||0x040&error ||0x080&error||0x100&error ||0x200&error)
		{
			*error_switchoff=1;
		}
		
	uint8_t typeback=errorwarning(error,index);
	return typeback;
}



//异常警告
uint8_t errorwarning (uint16_t errortype,uint16_t * index)
{
	
	uint8_t type1=0;
	if(errortype!=0)
	{
		HAL_GPIO_WritePin(GPIOA, GPIO_PIN_11, GPIO_PIN_SET);
		*index=3;
		if((errortype&0x01)!=0)
		{type1=1;}
		if((errortype&0x02)!=0)
		{type1=2;}
		if((errortype&0x04)!=0)
		{type1=3;}
		if((errortype&0x08)!=0)
		{type1=4;}
		if((errortype&0x10)!=0)
		{type1=5;}
		if((errortype&0x20)!=0)
		{type1=6;}
		if((errortype&0x40)!=0)
		{type1=7;}
		if((errortype&0x80)!=0)
		{type1=8;}
		if((errortype&0x100)!=0)
		{type1=9;}
	}
	else if(errortype==0)
	{HAL_GPIO_WritePin(GPIOA, GPIO_PIN_11, GPIO_PIN_RESET);}
	
	return type1;
}

uint16_t Dew_point_temperature(const int16_t T_In,const int16_t RH_In)
{
	uint16_t Td=T_In-((100-RH_In)/5);
	return Td;
}

