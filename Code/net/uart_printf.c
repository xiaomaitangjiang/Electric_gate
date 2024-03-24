#include <stdio.h>
#include <stm32f1xx.h>
#include "uart_printf.h"


#pragma import(__use_no_semihosting) // 禁用半主机模式

// 请不要勾选Use MicroLib
#ifdef __MICROLIB
#error "Please do not use MicroLib!"
#endif

extern "C"
{
  void _sys_exit(int returncode)
  {
    printf("Exited! returncode=%d\n", returncode);
    while (1);
  }
  
  void _ttywrch(int ch)
  {
    if (ch == '\n')
      HAL_UART_Transmit(&huart2, (uint8_t *)"\r\n", 2, HAL_MAX_DELAY);
    else
      HAL_UART_Transmit(&huart2, (uint8_t *)&ch, 1, HAL_MAX_DELAY);
  }
}

namespace std
{
  struct __FILE
  {
    int handle;
    /* Whatever you require here. If the only file you are using is */
    /* standard output using printf() for debugging, no file handling */
    /* is required. */
  };
  
  FILE __stdin = {0};
  FILE __stdout = {1};
  FILE __stderr = {2};
  
  int fgetc(FILE *stream)
  {
    int c = 0;
    
    if (stream->handle == 0)
    {
      while (__HAL_UART_GET_FLAG(&huart2, UART_FLAG_RXNE) == RESET);
      HAL_UART_Receive(&huart2, (uint8_t *)&c, 1, HAL_MAX_DELAY);
      
      _ttywrch((c == '\r') ? '\n' : c); // 回显
      return c;
    }
    return EOF;
  }
  
  int fputc(int c, FILE *stream)
  {
    if (stream->handle == 1 || stream->handle == 2)
    {
      _ttywrch(c);
      return c;
    }
    return EOF;
  }
  
  int fclose(FILE * stream)
  {
    return 0;
  }
  
  int fseek(FILE *stream, long int offset, int whence)
  {
    return -1;
  }
  
  int fflush(FILE *stream)
  {
    if (stream->handle == 1 || stream->handle == 2)
      while (__HAL_UART_GET_FLAG(&huart2, UART_FLAG_TC) == RESET);
    return 0;
  }
}

