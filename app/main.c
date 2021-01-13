#include "main.h"
#include <os_task.h>
#include <os_util.h>
#include <os_clock.h>

int main(void)
{
	while(1)
	{
		os_task_delay(50);
		GPIO_SetBits(GPIOC, GPIO_Pin_8);
		os_task_delay(50);
		GPIO_ResetBits(GPIOC, GPIO_Pin_8);
		os_kprintf("os run rick:%lu \n", os_tick_get());
	}
}

