#include "SysT.h"
#include "stm32f10x.h"

static volatile uint32_t usTicks;
void SysTick_Handler(void)
{
    usTicks++;
		//printf("%d\r\n",usTicks);
}


void InitSysT(void) {
    SysTick_CLKSourceConfig(SysTick_CLKSource_HCLK);
    SysTick_Config(SystemCoreClock / 1000000);
    SysTick->LOAD = 1;
    SysTick->CTRL |= SysTick_CTRL_TICKINT_Msk | SysTick_CTRL_ENABLE_Msk;
}

uint32_t miscos(void){
	return usTicks;
	}
