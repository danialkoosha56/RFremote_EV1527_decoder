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
#include <stdbool.h>

#include <stm32f1xx_ll_bus.h>
#include <stm32f1xx_ll_rcc.h>
#include <stm32f1xx_ll_system.h>
#include <stm32f1xx_ll_utils.h>
#include <stm32f1xx_ll_gpio.h>
#include "stm32f1xx_ll_exti.h"

#include "gpio.h"
#include "timer.h"
#include "RFremote_deocder.h"


void gpio_inputInterrupt_init(void)
{
  LL_EXTI_InitTypeDef EXTI_InitStruct = {0};

  //peripheral clock enable
  LL_APB2_GRP1_EnableClock(LL_APB2_GRP1_PERIPH_GPIOA);

  /**/
  // LL_APB2_GRP1_EnableClock(LL_APB2_GRP1_PERIPH_AFIO); 
  LL_GPIO_AF_SetEXTISource(LL_GPIO_AF_EXTI_PORTA, LL_GPIO_AF_EXTI_LINE3);

  // Configure GPIO pin : PA3
  EXTI_InitStruct.Line_0_31 = LL_EXTI_LINE_3;
  EXTI_InitStruct.LineCommand = ENABLE;
  EXTI_InitStruct.Mode = LL_EXTI_MODE_IT;
  EXTI_InitStruct.Trigger = LL_EXTI_TRIGGER_RISING_FALLING;
  LL_EXTI_Init(&EXTI_InitStruct);

  LL_GPIO_SetPinMode(GPIOA, LL_GPIO_PIN_3, LL_GPIO_MODE_INPUT);
  LL_GPIO_SetPinPull(GPIOA, LL_GPIO_PIN_3, LL_GPIO_PULL_UP);

  /* EXTI interrupt init*/
  NVIC_SetPriority(EXTI3_IRQn, NVIC_EncodePriority(NVIC_GetPriorityGrouping(),0, 0));
  NVIC_EnableIRQ(EXTI3_IRQn);

}


void EXTI3_IRQHandler(void)
{
  /* USER CODE BEGIN EXTI3_IRQn 0 */

  if (LL_EXTI_IsActiveFlag_0_31(LL_EXTI_LINE_3) != RESET)
  {
    LL_EXTI_ClearFlag_0_31(LL_EXTI_LINE_3);
    
    uint32_t cnt = timer_counter_getValue();
    timer_counter_reset();
    bool edgeType = LL_GPIO_IsInputPinSet(GPIOA, LL_GPIO_PIN_3);
    RFremote_callBack(cnt, edgeType);
  }

}