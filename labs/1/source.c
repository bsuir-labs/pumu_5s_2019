#include "stm32f4xx.h"
#include "stm32f4xx_gpio.h"
#include "stm32f4xx_rcc.h"

void Delay(volatile uint32_t tick)
{
    for(uint32_t i = 0; i < tick; i++);
}


int main(void)
{
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOD, ENABLE);
	GPIO_InitTypeDef GPIO_InitStruct;
	GPIO_StructInit(&GPIO_InitStruct);

	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_12 | GPIO_Pin_14;
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_OUT;
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_2MHz;
	GPIO_InitStruct.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStruct.GPIO_PuPd = GPIO_PuPd_NOPULL;
	GPIO_Init(GPIOD, &GPIO_InitStruct);


    while(1)
    {
    	GPIO_SetBits(GPIOD, GPIO_Pin_14);
		Delay(500000);
		GPIO_ResetBits(GPIOD, GPIO_Pin_14);
		GPIO_SetBits(GPIOD, GPIO_Pin_12);
		Delay(500000);
		GPIO_ResetBits(GPIOD, GPIO_Pin_12);
    }
}
