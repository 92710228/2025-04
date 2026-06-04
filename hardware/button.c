#include "button.h"
#include "stm32f10x.h"
#include  "delay.h"
#include "oled.h"
// 按键初始化函数
void Button_Init(void)
{
    // 所有变量必须定义在函数最开头！！！
    GPIO_InitTypeDef  GPIO_INIT;
    EXTI_InitTypeDef  EXTI_INIT;
    NVIC_InitTypeDef  NVIC_INIT;
		NVIC_InitTypeDef  NVIC_INIT1;
		GPIO_InitTypeDef  GPIO_INIT1;
		GPIO_InitTypeDef  GPIO_INIT2;

    // 1. 开时钟
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA | RCC_APB2Periph_AFIO, ENABLE);
		RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB |RCC_APB2Periph_GPIOC,ENABLE);
    // 2. 配置 GPIO
    GPIO_INIT.GPIO_Pin = GPIO_Pin_5 | GPIO_Pin_6 | GPIO_Pin_4;
    GPIO_INIT.GPIO_Mode = GPIO_Mode_IPD;  // 下拉
    GPIO_Init(GPIOA, &GPIO_INIT);

    // 3. 配置外部中断 EXTI
    GPIO_EXTILineConfig(GPIO_PortSourceGPIOA, GPIO_PinSource5);
    GPIO_EXTILineConfig(GPIO_PortSourceGPIOA, GPIO_PinSource6);
    GPIO_EXTILineConfig(GPIO_PortSourceGPIOA, GPIO_PinSource4);


    EXTI_INIT.EXTI_Line = EXTI_Line5 | EXTI_Line6 | EXTI_Line4;
    EXTI_INIT.EXTI_Mode = EXTI_Mode_Interrupt;
    EXTI_INIT.EXTI_Trigger = EXTI_Trigger_Rising;
    EXTI_INIT.EXTI_LineCmd = ENABLE;
    EXTI_Init(&EXTI_INIT);

    // 4. 配置中断优先级
    NVIC_INIT.NVIC_IRQChannel = EXTI9_5_IRQn;
    NVIC_INIT.NVIC_IRQChannelPreemptionPriority = 1;
    NVIC_INIT.NVIC_IRQChannelSubPriority =1;
    NVIC_INIT.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_INIT);
		
			NVIC_INIT1.NVIC_IRQChannel = EXTI4_IRQn;
			NVIC_INIT1.NVIC_IRQChannelPreemptionPriority = 1;
			NVIC_INIT1.NVIC_IRQChannelSubPriority = 2;
			NVIC_INIT1.NVIC_IRQChannelCmd = ENABLE;
			NVIC_Init(&NVIC_INIT1);
		//语音初始化和继电器控制。

		GPIO_INIT1.GPIO_Mode=GPIO_Mode_Out_PP;
		GPIO_INIT1.GPIO_Pin=GPIO_Pin_10;
		GPIO_INIT1.GPIO_Speed=GPIO_Speed_50MHz;
	GPIO_Init(GPIOB,&GPIO_INIT1);
	
	
		GPIO_INIT2.GPIO_Mode=GPIO_Mode_Out_PP;
		GPIO_INIT2.GPIO_Pin=GPIO_Pin_14;
		GPIO_INIT2.GPIO_Speed=GPIO_Speed_50MHz;
	GPIO_Init(GPIOC,&GPIO_INIT2);
	
	//蜂鸣器初始化
	GPIO_WriteBit(GPIOB,GPIO_Pin_10,Bit_RESET);
	GPIO_WriteBit(GPIOC,GPIO_Pin_14,Bit_RESET);
}
volatile uint8_t num=0,motorManualSet=0,sum=0;
// 中断服务函数
void EXTI9_5_IRQHandler(void)
{
    if(EXTI_GetITStatus(EXTI_Line5) == SET)
    {
		motorManualSet = 1;  
        num=0;
    }
		EXTI_ClearITPendingBit(EXTI_Line5);

if(EXTI_GetITStatus(EXTI_Line6) == SET) {
    motorManualSet = 2;   // 请求开
	num=0;
}
 EXTI_ClearITPendingBit(EXTI_Line6);
}

void EXTI4_IRQHandler(void){
	
	if(EXTI_GetITStatus(EXTI_Line4) == SET) {
//    OLED_Clear();
        // 动作--模式切换
	//	Delay_ms(20);
		sum=1;
					num++;
					if(num>3){
						num=0;
					}
				//	Delay_ms(20);
					 
					}
 EXTI_ClearITPendingBit(EXTI_Line4);

}