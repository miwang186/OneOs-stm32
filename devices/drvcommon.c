#include <os_types.h>
#include <os_stddef.h>
#include <os_irq.h>
#include <os_clock.h>
#include <os_hw.h>
#include <os_memory.h>
#include <os_util.h>
#include "main.h"
#include "SEGGER_RTT.h"

#define STM32_FLASH_START_ADRESS ((uint32_t)0x08000000)
#define STM32_FLASH_SIZE         (512 * 1024)
#define STM32_FLASH_END_ADDRESS  ((uint32_t)(STM32_FLASH_START_ADRESS + STM32_FLASH_SIZE))

#define STM32_SRAM1_SIZE  (64)
#define STM32_SRAM1_START (0x20000000)
#define STM32_SRAM1_END   (STM32_SRAM1_START + STM32_SRAM1_SIZE * 1024)

#if defined(__CC_ARM) || defined(__CLANG_ARM)
extern int Image$$RW_IRAM1$$ZI$$Limit;
#define HEAP_BEGIN (&Image$$RW_IRAM1$$ZI$$Limit)
#elif __ICCARM__
#pragma section = "HEAP"
#define HEAP_BEGIN (__segment_end("HEAP"))
#else
extern int __bss_end;
#define HEAP_BEGIN (&__bss_end)
#endif

#define HEAP_END STM32_SRAM1_END


void SysTick_Handler(void)
{
    os_interrupt_enter();
    os_tick_increase();
#ifdef OS_USING_CLOCKSOURCE
    os_clocksource_update();
#endif
    os_interrupt_leave();
}


OS_WEAK void cortexm_systick_init(void)
{
	/* systick for kernel tick */
    SysTick_Config(SystemCoreClock/OS_TICK_PER_SECOND); /* 1ms */
}


#ifdef OS_USING_CONSOLE
void os_hw_console_output(const char *str)
{    
    SEGGER_RTT_printf(0,str);
}
#endif 

void hardware_init()
{
	os_kprintf("console init \n");
}

#ifdef USE_FULL_ASSERT
void assert_failed(uint8_t* file, uint32_t line)
{
	os_kprintf("assert_failed %s ,%d \n",file, line);	
	
	while(1);
}
#endif 
/**
 ***********************************************************************************************************************
 * @brief           This function will initial STM32 board.
 *
 * @param[in]       none
 *
 * @return          none
 ***********************************************************************************************************************
 */
void os_hw_board_init()
{
#ifdef OS_USE_BOOTLOADER
#ifdef SOC_SERIES_STM32F0
    memcpy((void*)0x20000000, (void*)USER_APP_ENTRY, 0xBC);
//    __HAL_RCC_SYSCFG_CLK_ENABLE();
//    __HAL_SYSCFG_REMAPMEMORY_SRAM();
#else
    SCB->VTOR = USER_APP_ENTRY;
#endif
#endif

#ifdef SCB_EnableICache
    /* Enable I-Cache---------------------------------------------------------*/
    SCB_EnableICache();
#endif

#ifdef SCB_EnableDCache
    /* Enable D-Cache---------------------------------------------------------*/
    SCB_EnableDCache();
#endif

    os_hw_interrupt_enable(0);

    hardware_init();

    os_hw_interrupt_disable();

    /* hardware init end, disable irq */

    /* Pin driver initialization is open by default */
#ifdef OS_USING_PIN
    os_hw_pin_init();
#endif

#ifdef HAL_SDRAM_MODULE_ENABLED
    SDRAM_Init();
#endif

    /* Heap initialization */
#if defined(OS_USING_HEAP)
    os_system_heap_init((void *)HEAP_BEGIN, (void *)HEAP_END);
#endif

//    os_dma_mem_init();

#if defined(OS_USING_CLOCKSOURCE_CORTEXM) && defined(DWT)
    cortexm_dwt_init();
#endif

    os_board_auto_init();

    cortexm_systick_init();
}
