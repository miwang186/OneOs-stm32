#include <oneos_config.h>
#include <bsp.h>
#include <os_stddef.h>
#include <os_errno.h>

os_err_t LED_GPIO_Init(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC, ENABLE);

	GPIO_InitStructure.GPIO_Pin = LED_PIN; 
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_Out_PP;
	GPIO_Init(LED_PORT, &GPIO_InitStructure);
	return OS_EOK;
}

OS_DEVICE_INIT(LED_GPIO_Init);
