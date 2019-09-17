#include "stm32f4xx.h"
#include "stm32f4xx_gpio.h"
#include "stm32f4xx_rcc.h"
#include "stm32f4xx_syscfg.h"
#include "stm32f4xx_exti.h"
#include "misc.h"

#define SEC025 2000000/4
#define SEC033 2000000/3
#define SEC05  2000000/2
#define SEC    2000000

unsigned short interruptState = 0;
unsigned short blinkType = 1;

void EXTI0_IRQHandler(void)
{
    if(EXTI_GetITStatus(EXTI_Line0) != RESET)
    {
        interruptState = 1;
        EXTI_ClearITPendingBit(EXTI_Line0);
    }
}

void Delay(int ticks)
{
    for (int i = 0; i < ticks; ++i);
}

void blink_type_1()
{
    GPIO_ToggleBits(GPIOD, GPIO_Pin_12);    // on for 1s
    GPIO_ToggleBits(GPIOD, GPIO_Pin_13);    // on for 0.5s
    GPIO_ToggleBits(GPIOD, GPIO_Pin_14);    // on for 0.3s
    GPIO_ToggleBits(GPIOD, GPIO_Pin_15);    // on for 0.25s
    Delay(SEC025);
    GPIO_ToggleBits(GPIOD, GPIO_Pin_15);    // off for 0.25s
    Delay(SEC033-SEC025);
    GPIO_ToggleBits(GPIOD, GPIO_Pin_14); // off for 0.33s
    Delay(SEC05-SEC033);
    GPIO_ToggleBits(GPIOD, GPIO_Pin_15);
    GPIO_ToggleBits(GPIOD, GPIO_Pin_13);
    Delay(2*SEC033 - SEC05);
    GPIO_ToggleBits(GPIOD, GPIO_Pin_14);
    Delay(3*SEC025 - 2 * SEC033);
    GPIO_ToggleBits(GPIOD, GPIO_Pin_15);
    Delay(SEC025);
}

void blink_type_2()
{
    GPIO_ToggleBits(GPIOD, GPIO_Pin_12);    // on for 1s
    GPIO_ToggleBits(GPIOD, GPIO_Pin_13);    // on for 0.5s
    GPIO_ToggleBits(GPIOD, GPIO_Pin_14);    // on for 0.3s
    GPIO_ToggleBits(GPIOD, GPIO_Pin_15);    // on for 0.25s
    Delay(SEC025);
    GPIO_ToggleBits(GPIOD, GPIO_Pin_12);    // off for 0.25s
    Delay(SEC033-SEC025);
    GPIO_ToggleBits(GPIOD, GPIO_Pin_13); // off for 0.33s
    Delay(SEC05-SEC033);
    GPIO_ToggleBits(GPIOD, GPIO_Pin_12);
    GPIO_ToggleBits(GPIOD, GPIO_Pin_14);
    Delay(2*SEC033 - SEC05);
    GPIO_ToggleBits(GPIOD, GPIO_Pin_13);
    Delay(3*SEC025 - 2 * SEC033);
    GPIO_ToggleBits(GPIOD, GPIO_Pin_12);
    Delay(SEC025);
}

void resetLEDs()
{
    GPIO_ResetBits(GPIOD, GPIO_Pin_12);
    GPIO_ResetBits(GPIOD, GPIO_Pin_13);
    GPIO_ResetBits(GPIOD, GPIO_Pin_14);
    GPIO_ResetBits(GPIOD, GPIO_Pin_15);
}

int main()
{
    // initializing LED lights
    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOD, ENABLE);
    GPIO_InitTypeDef GPIO_InitStruct;
    GPIO_StructInit(&GPIO_InitStruct);
    GPIO_InitStruct.GPIO_Pin = GPIO_Pin_12 | GPIO_Pin_13 | GPIO_Pin_14 | GPIO_Pin_15;
    GPIO_InitStruct.GPIO_Mode = GPIO_Mode_OUT;
    GPIO_InitStruct.GPIO_Speed = GPIO_Speed_2MHz;
    GPIO_InitStruct.GPIO_OType = GPIO_OType_PP;
    GPIO_InitStruct.GPIO_PuPd = GPIO_PuPd_NOPULL;
    GPIO_Init(GPIOD, &GPIO_InitStruct);

    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);
    GPIO_InitTypeDef GPIO_ButtonStruct;
    GPIO_StructInit(&GPIO_ButtonStruct);
    GPIO_ButtonStruct.GPIO_Pin = GPIO_Pin_0;
    GPIO_ButtonStruct.GPIO_Mode = GPIO_Mode_IN;
    GPIO_ButtonStruct.GPIO_Speed = GPIO_Speed_2MHz;
    GPIO_ButtonStruct.GPIO_OType = GPIO_OType_PP;
    GPIO_ButtonStruct.GPIO_PuPd = GPIO_PuPd_NOPULL;
    GPIO_Init(GPIOD, &GPIO_InitStruct);

    RCC_AHB2PeriphClockCmd(RCC_APB2Periph_SYSCFG, ENABLE);
    SYSCFG_EXTILineConfig(EXTI_PortSourceGPIOA, EXTI_PinSource0);
    EXTI_InitTypeDef EXTI_InitStruct;
    EXTI_StructInit(&EXTI_InitStruct);
    EXTI_InitStruct.EXTI_Mode = EXTI_Mode_Interrupt;
    EXTI_InitStruct.EXTI_Trigger = EXTI_Trigger_Rising;
    EXTI_InitStruct.EXTI_Line = EXTI_Line0;
    EXTI_InitStruct.EXTI_LineCmd = ENABLE;
    EXTI_Init(&EXTI_InitStruct);

    NVIC_InitTypeDef NVIC_InitStruct;
    NVIC_InitStruct.NVIC_IRQChannel = EXTI0_IRQn;
    NVIC_InitStruct.NVIC_IRQChannelPreemptionPriority = 0x01;
    NVIC_InitStruct.NVIC_IRQChannelSubPriority = 0x01;
    NVIC_InitStruct.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStruct);

    while (1)
    {
        if (interruptState == 1)
        {
            // interrupted
            Delay(100000);
            if (GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_0) == 1)
            {
                resetLEDs();
                blinkType = 3 - blinkType;
            }
            interruptState = 0;
        }

        if (blinkType == 1)
        {
            blink_type_1();
        } else
        {
            blink_type_2();
        }
    }

    return 0;
}
