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
#ifndef __TIMER_H__
#define __TIMER_H__

#include <stdint.h>

void timer_counter_init(void);
void timer_counter_start(void);
void timer_counter_stop(void);
void timer_counter_reset(void);
uint32_t timer_counter_getValue(void);

#endif /* __TIMER_H__ */