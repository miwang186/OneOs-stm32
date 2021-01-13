#include "main.h"

#include "SEGGER_RTT.h"

#define LED_PIN		GPIO_Pin_8
#define LED_PORT	GPIOC


void delay_ms(uint32_t ms)
{
	volatile int i ,j;
	
	for(i = 0; i < us ;i++)
		for(j = 0; j < 7200 ;j++);
}	

void LED_GPIO_Init()
{
	GPIO_InitTypeDef GPIO_InitStructure;
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC, ENABLE);

	GPIO_InitStructure.GPIO_Pin = LED_PIN; 
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_Out_PP;
	GPIO_Init(LED_PORT, &GPIO_InitStructure);
}

int main(void)
{
	LED_GPIO_Init();
	SEGGER_RTT_printf(0,"hello world \r\n");
	
	while(1)
	{
		delay_ms(500);
		GPIO_SetBits(LED_PORT, LED_PIN);
		delay_ms(500);
		GPIO_ResetBits(LED_PORT, LED_PIN);
	}
}

