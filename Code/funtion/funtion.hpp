#ifndef __FUNTION_H_
#define __FUNTION_H_

#ifdef __cplusplus
extern "C"{
#endif

#include "main.h"
extern ADC_HandleTypeDef hadc1;

#ifdef __cplusplus
}
#endif
#include "vector"
#include "process.hpp"

using std::vector;

extern communciation URTP;



void HAL_Delay(uint32_t Delay);

uint16_t safefuntion (double V_in,double C_in,double P_in, double E_in,uint16_t *beg, uint16_t *end);
void switch_control(uint8_t cross,uint8_t switch_tim,uint8_t switch_user);

uint8_t errorprocessing (const uint16_t error,uint16_t * index,uint8_t *error_switchoff);
uint8_t errorwarning (uint16_t errortype,uint16_t * index);

uint16_t Dew_point_temperature(const int16_t T_In,const int16_t RH_In);

void expanddata_C(uint8_t * data_user,action  *action_user ,uint16_t  *V_SET_user ,uint16_t  *P_SET_user ,uint16_t  *I_SET_user ,uint16_t  *E_SET_user);



uint16_t adc_data_processing (uint16_t *ADC_data,uint8_t channel);

#endif
