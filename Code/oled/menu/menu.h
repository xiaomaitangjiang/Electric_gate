#ifndef MENU_H_
#define MENU_H_

#include "stm32f1xx_hal.h"
#include "oled.h"
#include "main.h"


extern uint16_t V_set,C_set,P_set,E_con_set;
extern uint16_t currentMenuIndex,enter,enter_storage,time_storage;
extern double V,C,P,E_con;
extern uint16_t time_close,time_on;
extern float HZ,HZ_S;
extern uint8_t caring;
extern uint16_t currentMenuIndex_storage,lineindex,lineindex_storage,data;
extern uint8_t cross_C;
extern uint16_t errorcode;
extern float temperature,V_ppm,V_adc;







typedef struct 
{
int page;//��ǰҳ��
int line;//��ǰ��ѡ��
int left;//���˵�ҳ��
int right;//�Ҳ�˵�ҳ��
int enter;//��ǰҳ���Ƿ���пɽ��������
}menu;

extern menu menulist[];




void page0(int index,int sw_state);
void page1(int index,int line, int choise);
void page2(int index,int line,int choise);
void page3(int index,int type);
void page5(int index);
void page4(int index);
void menuprocess(void);

#endif 
