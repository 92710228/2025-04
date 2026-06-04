#include "stm32f10x.h"
#include "delay.h" 
#include "button.h"
#include "oled.h"
#include "dht11.h"
#include "MQ2.H"
#include "onenet.h"
#include "esp8266.h"
#include "sys.h"
#include "usart.h"

#define ESP8266_ONENET_INFO		"AT+CIPSTART=\"TCP\",\"mqtts.heclouds.com\",1883\r\n"

extern volatile uint8_t num,sum,motorManualSet;

u8 temperature=0,humidity=0;

void mode1();
void mode2();
void mode3();
int main(void)
{

		OLED_Init();
		Button_Init();
		DHT11_Init();
		Adc_Init();
		OLED_Clear();
			OLED_ShowChinese(0,0,10,16,1);  //物
			OLED_ShowChinese(18,0,11,16,1); //联
			OLED_ShowChinese(36,0,12,16,1); //网
			OLED_ShowChinese(54,0,13,16,1); //工
			OLED_ShowChinese(72,0,14,16,1); //程
			OLED_ShowChinese(90,0,20,16,1); //项
			OLED_ShowChinese(108,0,21,16,1); //目
			OLED_ShowChinese(0,36,15,16,0);//刘
			OLED_ShowChinese(18,36,16,16,0);//建
			OLED_ShowChinese(36,36,17,16,0);//伟
			OLED_ShowChinese(54,36,18,16,1);//小
			OLED_ShowChinese(72,36,19,16,1);//组
	  OLED_Refresh();
		//低电平开启继电器
    while(1)
    {
while(sum){
	OLED_Clear();
	sum=0;
}
			if(num==1){
				if(MQ2_GetData()>700){num=0;continue;}
				mode1();

			}
			else if(num==2){
				if(MQ2_GetData()>700){num=0;continue;}
				mode2();

			}
			else if(num==3){
				if(MQ2_GetData()>700){num=0;continue;}
				mode3();
	
			}
					else if(motorManualSet == 1) {
							// 手动开优先
							GPIO_WriteBit(GPIOB, GPIO_Pin_10, Bit_RESET);
					}else if(motorManualSet == 2){
						GPIO_WriteBit(GPIOB, GPIO_Pin_10, Bit_SET);
					}else if(MQ2_GetData()>700){//
						OLED_Clear();
								OLED_ShowChinese(18,30,39,16,0); //警告！！！
								OLED_ShowChinese(36,30,40,16,0); 
								OLED_ShowChinese(54,30,41,16,0); 
								OLED_ShowChinese(72,30,41,16,0); 
								OLED_ShowChinese(90,30,41,16,0); 
						OLED_ShowNum(58,0,MQ2_GetData()/10,3,16,1);
							OLED_Refresh();
							//蜂鸣器报警。
							GPIO_WriteBit(GPIOC,GPIO_Pin_14,Bit_SET);
							//电机旋转
							GPIO_WriteBit(GPIOB,GPIO_Pin_10,Bit_SET);

					}else{//安全运行
						
								OLED_ShowChinese(36,30,42,16,0); 
								OLED_ShowChinese(54,30,43,16,0); 
								OLED_ShowChinese(72,30,44,16,0); 
								OLED_ShowChinese(90,30,45,16,0); 
								//蜂鸣器不报警。
							GPIO_WriteBit(GPIOC,GPIO_Pin_14,Bit_RESET);
							//电机不旋转
							GPIO_WriteBit(GPIOB,GPIO_Pin_10,Bit_RESET);

							}
			}

}

void mode1(){
	
				Dht11_Get_Temp_Humi_Value(&temperature,&humidity);
					OLED_ShowChinese(0,0,0,16,1);  //温
					OLED_ShowChinese(18,0,1,16,1); //湿
					OLED_ShowChinese(36,0,2,16,1); //度
					OLED_ShowChinese(54,0,3,16,1); //测
					OLED_ShowChinese(72,0,4,16,1); //检
					OLED_ShowChinese(90,0,5,16,1); //装
					OLED_ShowChinese(108,0,6,16,1);  //置
					OLED_ShowChinese(0,30,0,16,0);//温
					OLED_ShowChinese(18,30,2,16,0);//度
					OLED_ShowChinese(0,48,1,16,0);//湿
					OLED_ShowChinese(18,48,2,16,0);//度
					OLED_ShowString(90,30,"C",16,0);//C
					OLED_ShowString(90,48,"%",16,0);//%
						OLED_ShowString(58,35,"    ",5,1);
					OLED_ShowNum(58,30,temperature+15,3,16,1);
					OLED_ShowNum(58,48,humidity/8,3,16,1);
					 OLED_Refresh();
}
void mode2(){
					OLED_ShowChinese(0,0,24,16,1);  //温
					OLED_ShowChinese(18,0,25,16,1); //湿
					OLED_ShowChinese(36,0,26,16,1); //度
					OLED_ShowChinese(54,0,27,16,1); //测
					OLED_ShowChinese(72,0,28,16,1); //检
					OLED_ShowChinese(90,0,29,16,1); //装
					OLED_ShowChinese(108,0,30,16,1);  //置
					OLED_ShowChinese(0,35,31,16,0);//温
					OLED_ShowChinese(18,35,32,16,0);//度
					OLED_ShowString(90,35,"%",16,0);//%
					OLED_ShowString(58,30,"    ",5,1);
					OLED_ShowString(58,48,"    ",5,1);
					
					OLED_ShowNum(58,35,MQ2_GetData()/10,3,16,1);
					OLED_Refresh();
}
void mode3(){
				#define ESP8266_ONENET_INFO		"AT+CIPSTART=\"TCP\",\"mqtts.heclouds.com\",1883\r\n"
				unsigned short timeCount = 0;	//发送间隔变量
	
				unsigned char *dataPtr = NULL;
				
				NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);	//中断控制器分组设置						
				Usart1_Init(115200);							//串口1，打印信息用
				Usart2_Init(115200);							//串口2，驱动ESP8266用
				OLED_Init();
				UsartPrintf(USART_DEBUG, " Hardware init OK\r\n");
				 while(DHT11_Init())
				{
					Delay_ms(200);
				}
				OLED_ShowString(20,24,"Networking",16,1);
				ESP8266_Init();					//初始化ESP8266
				OLED_ShowString(0,24,"                ",16,1);
				OLED_ShowString(0,24," Connected to",16,1);
				OLED_ShowString(30,48,"ONENET",16,1);
				UsartPrintf(USART_DEBUG, "Connect MQTTs Server...\r\n");
				while(ESP8266_SendCmd(ESP8266_ONENET_INFO, "CONNECT"))
					Delay_ms(500);
				UsartPrintf(USART_DEBUG, "NET_OK\r\n");
				while(OneNet_DevLink())			//接入OneNET
					Delay_ms(500);
					OLED_Clear();
				while(num==3)
					{
						temperature = rand() % 100 + 1;
						humidity = rand() % 100 + 1;
						
						if(++timeCount >= 50)									//发送间隔0.5s
						{
							if(num!=3){num=0;break;};
							Dht11_Get_Temp_Humi_Value(&temperature,&humidity);	//读取温湿度值	
							
							OLED_ShowChinese(10,5,33,16,0);//正在上传云端
							OLED_ShowChinese(28,5,34,16,0);//度
							OLED_ShowChinese(46,5,35,16,0);//温
							OLED_ShowChinese(64,5,36,16,0);//度
							OLED_ShowChinese(82,5,37,16,0);//温
							OLED_ShowChinese(100,5,38,16,0);//度
					OLED_ShowChinese(0,30,0,16,1);//温
					OLED_ShowChinese(18,30,2,16,1);//度
					OLED_ShowChinese(0,48,1,16,1);//湿
					OLED_ShowChinese(18,48,2,16,1);//度
					OLED_ShowString(90,30,"C",16,1);//C
					OLED_ShowString(90,48,"%",16,1);//%
							
					OLED_ShowNum(60,30,temperature,3,16,1);//显示温度	
				  OLED_ShowNum(60,48,humidity,3,16,1);	//显示湿度	
							UsartPrintf(USART_DEBUG,"TEMP:%d HUMI:%d\r\n",temperature,humidity);
						//	UsartPrintf(USART_DEBUG, "OneNet_SendData\r\n");
							OneNet_SendData();									//发送数据
							
							timeCount = 0;
							ESP8266_Clear();
						}
						
						dataPtr = ESP8266_GetIPD(0);
						if(dataPtr != NULL)
							OneNet_RevPro(dataPtr);
						Delay_ms(10);
					}
					while(num!=3){break;}
				}
	
