#ifndef __DHT11_H__
#define __DHT11_H__


/**********************************
包含头文件
**********************************/
#include "sys.h"


/**********************************
重定义关键词
**********************************/
#define DHT11_GPIO_CLK_ENABLE             			RCC_APB2Periph_GPIOA				//DHT11引脚
#define DHT11_PORT                           		GPIOA
#define DHT11_PIN                            		GPIO_Pin_1
#define DHT11_OUT 															PAout(1)
#define DHT11_IN  															PAin(1)


/**********************************
函数声明
**********************************/
void Dht11_Get_Temp_Humi_Value(u8 *temp,u8 *humi);							//DS18B20读取温度函数
uint8_t DHT11_Init(void);  																									//DS18B20初始化函数


#endif

