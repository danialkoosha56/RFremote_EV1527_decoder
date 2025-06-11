/**
 * @file gpio.c
 * @author Danial Koosha 
 * @brief 
 * @version 0.1
 * @date 2025-06-11
 * 
 * @copyright Copyright (c) 2025
 * 
 */
#include <stm32f1xx_ll_bus.h>
#include <stm32f1xx_ll_rcc.h>
#include <stm32f1xx_ll_system.h>
#include <stm32f1xx_ll_utils.h>

#include <stm32f1xx_ll_tim.h>
#include <stm32f1xx_ll_gpio.h>


void timer_counter_init(void)
{
    TIM_TypeDef *TIMER = TIM2;
    LL_TIM_InitTypeDef TIM_InitStruct = {0};

    //prepheral clock enable
    LL_APB1_GRP1_EnableClock(LL_APB1_GRP1_PERIPH_TIM2);

    //TIM2 configuration
    uint32_t timerFreq = 1000000; // 1MHz
    // timerFreq = 10000; // 10khz
    uint16_t prescaler = (SystemCoreClock/timerFreq);

    TIM_InitStruct.Prescaler = prescaler-1;
    TIM_InitStruct.CounterMode = LL_TIM_COUNTERMODE_UP;
    TIM_InitStruct.Autoreload = 0xFFFF; //65535
    TIM_InitStruct.ClockDivision = LL_TIM_CLOCKDIVISION_DIV1;
    LL_TIM_Init(TIMER, &TIM_InitStruct);

    LL_TIM_DisableARRPreload(TIMER); // Disable auto-reload preload
    LL_TIM_SetTriggerOutput(TIMER, LL_TIM_TRGO_RESET); // Set trigger output to reset
    LL_TIM_DisableMasterSlaveMode(TIMER); // Disable master-slave mode
    
    // TIM2 interrupt configuration
    // NVIC_SetPriority(TIM2_IRQn, NVIC_EncodePriority(NVIC_GetPriorityGrouping(), 0, 0));
    // NVIC_EnableIRQ(TIM2_IRQn);
}

void timer_counter_start(void)
{
    TIM_TypeDef *TIMER = TIM2;
    LL_TIM_SetCounter(TIMER, 0); // Reset the counter to 0
    LL_TIM_EnableCounter(TIMER); // Enable the timer counter

    // LL_TIM_EnableIT_UPDATE(TIMER); // Enable update interrupt
}

void timer_counter_stop(void)
{
    TIM_TypeDef *TIMER = TIM2;
    LL_TIM_DisableCounter(TIMER); // Disable the timer counter
}

void timer_counter_reset(void)
{
    TIM_TypeDef *TIMER = TIM2;
    LL_TIM_SetCounter(TIMER, 0); // Reset the counter to 0
}

uint32_t timer_counter_getValue(void)
{
    TIM_TypeDef *TIMER = TIM2;
    return LL_TIM_GetCounter(TIMER); // Get the current counter value
}


void TIM2_IRQHandler(void)
{
  __NOP();
}