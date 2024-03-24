#ifndef MENU_H_
#define MENU_H_

#ifdef __cplusplus
extern "C"{
#endif
	
	
#include "stm32f1xx_hal.h"
#include "oled.h"
	




extern uint16_t V_set,C_set,P_set,E_con_set;
extern uint16_t currentMenuIndex,enter,enter_storage,time_storage;
extern double V,C,P,E_con;
extern uint16_t time_close,time_on;
extern float HZ,HZ_S;
extern uint8_t caring;
extern uint16_t currentMenuIndex_storage,lineindex,lineindex_storage,data;
extern uint8_t cross_user;
extern uint16_t errorcode;
extern float temperature,V_ppm,V_adc;
extern uint8_t RH,T,Td;






typedef struct 
{
int page;//当前页码
int line;//当前所选行
int left;//左侧菜单页码
int right;//右侧菜单页码
int enter;//当前页码是否具有可进入的设置
}menu;

extern menu menulist[];




void page0(int index,int sw_state);
void page1(int index,int line, int choise);
void page2(int index,int line,int choise);
void page3(int index,int type);
void page5(int index);
void page4(int index);
void menuprocess(void);

#ifdef __cplusplus
}
#endif

#include "funtion.hpp"


#endif 
