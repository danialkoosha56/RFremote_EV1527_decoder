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
#ifndef __GPIO_H__
#define __GPIO_H__

#include <stm32f1xx_ll_gpio.h>

#define LED_PIN                           LL_GPIO_PIN_13
#define LED_GPIO_PORT                     GPIOC
#define LED_GPIO_CLK_ENABLE()             LL_APB2_GRP1_EnableClock(LL_APB2_GRP1_PERIPH_GPIOC)

#define LED_INIT()  LED_GPIO_CLK_ENABLE(); LL_GPIO_SetPinMode(LED_GPIO_PORT, LED_PIN, LL_GPIO_MODE_OUTPUT);
#define LED_ON()    LL_GPIO_SetOutputPin(LED_GPIO_PORT, LED_PIN)
#define LED_OFF()   LL_GPIO_ResetOutputPin(LED_GPIO_PORT, LED_PIN)
#define LED_TOG()   LL_GPIO_TogglePin(LED_GPIO_PORT, LED_PIN)

void gpio_inputInterrupt_init(void);

#endif /* __GPIO_H__ */