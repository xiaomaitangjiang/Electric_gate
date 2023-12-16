#include "UartSet.h"
#include "string.h"

typedef enum
{
    TimeRecvStartStep1 = 0,
    TimeRecvStep2 = 1,
    TimeRecvStep3 = 2,
    TimeRecvStep1 = 3,
    TimeSendStep = 4,

} TimeStep;

uint16_t TimeSet[5];

uint16_t TimeSetBuff[][5] = {{1199, 59, 59, 2378, 2498 }, //1200
    {539, 59, 59, 1128, 1247 },  //2400
    {239, 59, 59, 503, 624  },   //4800
    {149, 29, 29, 251, 311  },   //9600
    {0, 0, 0, 0, 0    },         //预留
};

UART gsUARTBuff;                //定义串口

uint8_t ucRecvData = 0;         //每次接收的字节
uint8_t ucAcquCx = 0;           //三次滤波计数
uint8_t ucRecvBitCx = 0;        //接收位计数
uint8_t ucSendBitCx = 0;        //发送位计数
uint8_t ucSendLengCx = 0;       //发送长度计数

uint8_t ucRecvBitBuff = 0;       //采集位保存


TIM_HandleTypeDef htim6;

void MX_TIM6_Init(void)
{

    __HAL_RCC_TIM6_CLK_ENABLE();

    htim6.Instance = TIM6;
    htim6.Init.Prescaler = 15;
    htim6.Init.CounterMode = TIM_COUNTERMODE_UP;
    htim6.Init.Period = TimeSet[TimeRecvStartStep1];
    htim6.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_ENABLE;

    HAL_NVIC_SetPriority(TIM6_IRQn, 0, 0);
    HAL_NVIC_EnableIRQ(TIM6_IRQn);

    HAL_TIM_Base_Init(&htim6);

}


void UART_GPIO_Init(void)
{
    GPIO_InitTypeDef GPIO_InitStruct = {0};

    __HAL_RCC_GPIOB_CLK_ENABLE();

    GPIO_InitStruct.Pin = COM_TX_Pin;
    GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
    GPIO_InitStruct.Pull = GPIO_PULLUP;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
    HAL_GPIO_Init(COM_TX_GPIO_Port, &GPIO_InitStruct);

    GPIO_InitStruct.Pin = COM_RX_Pin;
    GPIO_InitStruct.Mode = GPIO_MODE_IT_FALLING;
    GPIO_InitStruct.Pull = GPIO_PULLUP;
    HAL_GPIO_Init(COM_RX_GPIO_Port, &GPIO_InitStruct);

    HAL_NVIC_SetPriority(EXTI4_15_IRQn, 0, 0);
    HAL_NVIC_EnableIRQ(EXTI4_15_IRQn);

}


/*******************************************************************************
* @FunctionName   : UART_Init.
* @Description    : 模拟串口结构体初始化.
* @Input          : None.
* @Output         : None.
* @Return         : None.
*******************************************************************************/
void UART_Init(void)
{
    gsUARTBuff.CheckType = NONE;
    gsUARTBuff.UartMaxLength = Uartlength;
    gsUARTBuff.UartStat = COM_NONE_BIT_DEAL;
    UART_GPIO_Init();

    if(BaudRate == 1200)
    {
        memcpy(TimeSet, &TimeSetBuff[0][0], sizeof(TimeSet));
    }
    else if(BaudRate == 2400)
    {
        memcpy(TimeSet, &TimeSetBuff[1][0], sizeof(TimeSet));
    }
    else if(BaudRate == 4800)
    {
        memcpy(TimeSet, &TimeSetBuff[2][0], sizeof(TimeSet));
    }
    else if(BaudRate == 9600)
    {
        memcpy(TimeSet, &TimeSetBuff[3][0], sizeof(TimeSet));
    }
    else
    {

    }

    MX_TIM6_Init();
}


/*******************************************************************************
* @FunctionName   : UART_Send_Data.
* @Description    : 模拟串口发送数据接口.
* @Input          : None.
* @Output         : None.
* @Return         : None.
*******************************************************************************/
void UART_Send_Data(uint8_t * data, uint8_t size)
{
    gsUARTBuff.Sendlength = size;
    memcpy(gsUARTBuff.UART_Send_buf, data, size);
   
      if(gsUARTBuff.UartStat == COM_NONE_BIT_DEAL)
        {
            gsUARTBuff.TxEn = 1;
            gsUARTBuff.RxEn = 0;
            gsUARTBuff.UartStat = COM_START_BIT_DEAL;

            TIM6->ARR = TimeSet[TimeSendStep];
            TIM6->EGR = TIM_EGR_UG;

            HAL_TIM_Base_Start_IT(&htim6);
        }
      
}


/*******************************************************************************
* @FunctionName   : EXTI4_15_IRQHandler.
* @Description    : 接收引脚外部中断，下降沿触发，触发后即进入起始位判断.
* @Input          : None.
* @Output         : None.
* @Return         : None.
* @Author&Data    : MrShuCai  2019.4.11.
*******************************************************************************/
void EXTI4_15_IRQHandler(void)
{
    if(__HAL_GPIO_EXTI_GET_IT(GPIO_PIN_4) != RESET)
    {
        __HAL_GPIO_EXTI_CLEAR_IT(GPIO_PIN_4);

        if(gsUARTBuff.UartStat == COM_NONE_BIT_DEAL)
        {
            gsUARTBuff.RxEn = 1;
            ucRecvData = 0;
            gsUARTBuff.UartStat = COM_START_BIT_DEAL;

            TIM6->ARR = TimeSet[TimeRecvStartStep1];
            TIM6->EGR = TIM_EGR_UG;                  //初始化定时器
                      EXTI->IMR &= ~(0x10);
                      EXTI->EMR &= ~(0x10);
            HAL_TIM_Base_Start_IT(&htim6);
        }
    }

}


/*******************************************************************************
* @FunctionName   : BitValueChk.
* @Description    : 判断采集bit值，三次中为1的次数大于等于2则值为1否则为0.
* @Input          : n 采集记录的位值.
* @Output         : BitValue.
* @Return         : BitValue.
* @Author&Data    : MrShuCai  2019.5.1.
*******************************************************************************/
uint8_t BitValueChk(uint8_t n)
{
    uint8_t BitValCx = 0;

    for(BitValCx = 0; n; n >>= 1)
    {
        BitValCx += n & 0x01;
    }

    return (BitValCx < 2) ? (0) : (1);

}

/*******************************************************************************
* @FunctionName   : TIM6_IRQHandler.
* @Description    : 中断处理函数，包括发送和接收两部分.
* @Input          : None.
* @Output         : None.
* @Return         : None.
* @Author&Data    : MrShuCai  2019.4.11.
*******************************************************************************/
void TIM6_IRQHandler(void)
{

    HAL_TIM_IRQHandler(&htim6);

    if(gsUARTBuff.TxEn == 1)         /*数据发送，发送优先，无发送后才进入接收状态*/
    {
        switch(gsUARTBuff.UartStat)  /*串口发送位状态判断*/
        {
        case COM_START_BIT_DEAL :
        {
            HAL_GPIO_WritePin(COM_TX_GPIO_Port, COM_TX_Pin, GPIO_PIN_RESET);
            gsUARTBuff.UartStat = COM_DATA_BIT_DEAL;
            ucSendBitCx = 0;
        }
        break;

        case COM_DATA_BIT_DEAL :
        {
            HAL_GPIO_WritePin(COM_TX_GPIO_Port, COM_TX_Pin, (GPIO_PinState)((gsUARTBuff.UART_Send_buf[ucSendLengCx] >> ucSendBitCx) & 0x01));

            ucSendBitCx ++;

            if(ucSendBitCx >= 8)
            {
                if(gsUARTBuff.CheckType == NONE)
                {
                    gsUARTBuff.UartStat = COM_STOP_BIT_DEAL;
                }
                else
                {
                    gsUARTBuff.UartStat = COM_CHECK_BIT_DEAL;
                }
            }

        }
        break;

        case COM_CHECK_BIT_DEAL :
        {

        }
        break;

        case COM_STOP_BIT_DEAL :
        {
            HAL_GPIO_WritePin(COM_TX_GPIO_Port, COM_TX_Pin, GPIO_PIN_SET);

            ucSendBitCx = 0;

            if(ucSendLengCx < gsUARTBuff.Sendlength - 1)
            {
                gsUARTBuff.UartStat = COM_START_BIT_DEAL;
                ucSendLengCx ++;
            }
            else
            {
                ucSendLengCx = 0;
                gsUARTBuff.UartStat = COM_NONE_BIT_DEAL;
                gsUARTBuff.TxEn = 0;
                gsUARTBuff.RxEn = 1;
                HAL_TIM_Base_Stop_IT(&htim6);
                              EXTI->IMR |= 0x10;
                          EXTI->EMR |= 0x10;
                TIM6 ->CNT = 0;
            }

        }
        break;

        default:
            break;

        }
    }


    if(gsUARTBuff.RxEn == 1)
    {
        switch(gsUARTBuff.UartStat)
        {
                    case COM_START_BIT_DEAL :
                    {
                            ucRecvBitBuff = (ucRecvBitBuff << 1) | (HAL_GPIO_ReadPin(COM_RX_GPIO_Port, COM_RX_Pin) & 0x01);

                            if(++ ucAcquCx >= 3)
                            {
                                    if(BitValueChk(ucRecvBitBuff) == 0)                        
                                    {
                                            gsUARTBuff.UartStat = COM_DATA_BIT_DEAL;
                                            TIM6->ARR = TimeSet[ucAcquCx];
                                    }
                                    else
                                    {
                                            gsUARTBuff.UartStat = COM_STOP_BIT_DEAL;
                                    }
                                    
                                    ucRecvBitBuff = 0;
                                    ucAcquCx = 0;
                            }
                            else
                            {
                                    TIM6->ARR = TimeSet[ucAcquCx];
                            }


                    }
                    break;

                    case COM_DATA_BIT_DEAL :                         //数据位
                    {

                            ucRecvBitBuff = (ucRecvBitBuff << 1) | (HAL_GPIO_ReadPin(COM_RX_GPIO_Port, COM_RX_Pin) & 0x01);

                            if(++ ucAcquCx >= 3)
                            {
                                    ucRecvData |= (BitValueChk(ucRecvBitBuff) & 0x01) << ucRecvBitCx;

                                    if(ucRecvBitCx >= 7)
                                    {
                                            ucRecvBitCx = 0;

                                            if(gsUARTBuff.CheckType == NONE)
                                            {
                                                    gsUARTBuff.UartStat = COM_STOP_BIT_DEAL;
                                            }
                                            else
                                            {
                                                    gsUARTBuff.UartStat = COM_CHECK_BIT_DEAL;
                                            }
                                    }
                                    else
                                    {
                                            ucRecvBitCx ++;
                                    }

                                    TIM6->ARR = TimeSet[ucAcquCx];

                                    ucAcquCx = 0;
                                    ucRecvBitBuff = 0;
                            }
                            else
                            {
                                    TIM6->ARR = TimeSet[ucAcquCx];
                            }
                    }
                    break;

                    case COM_CHECK_BIT_DEAL :                         //校验位
                    {

                    }
                    break;

                    case COM_STOP_BIT_DEAL :                         //停止位
                    {

                        ucRecvBitBuff = (ucRecvBitBuff << 1) | (HAL_GPIO_ReadPin(COM_RX_GPIO_Port, COM_RX_Pin) & 0x01);

                            if( ++ ucAcquCx >= 3)
                            { 
                                    if(BitValueChk(ucRecvBitBuff) == 1)    
                                    {
                                            if(gsUARTBuff.Recvlength < gsUARTBuff.UartMaxLength)
                                            {
                                                    gsUARTBuff.UART_Recv_buf[gsUARTBuff.Recvlength] = ucRecvData;
                                                    gsUARTBuff.Recvlength ++;
                                            }

                                            gsUARTBuff.UartStat = COM_NONE_BIT_DEAL;
                                            HAL_TIM_Base_Stop_IT(&htim6);
                                            
                                            EXTI->IMR |= 0x10;
                                            EXTI->EMR |= 0x10;
                                            TIM6->CNT = 0;
                                    }
                                    else
                                    {
                                            ucAcquCx = 0;
                                    }
                                    
                                    ucRecvBitBuff = 0;
                                    ucAcquCx = 0;
                            }
                            else
                            {
                                    TIM6->ARR = TimeSet[ucAcquCx];
                            }
                    }
                    break;

                    default:
                            break;
                    }
    }
}