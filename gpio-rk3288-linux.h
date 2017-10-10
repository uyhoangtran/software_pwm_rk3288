/*
 * gpio.h
 *
 *  Created on: Oct 10, 2017
 *      Author: Hoang Tran
 */

#ifndef GPIO_RK3288_LINUX_H_
#define GPIO_RK3288_LINUX_H_

#include <stdio.h>
#include <stdint.h>
#include <fcntl.h>
#include <unistd.h>

#define GPIO7A0 224
#define GPIO7A2 226

#define BUZZER_PIN GPIO7A0
#define PWM_PIN GPIO7A2

#define GPIO_OUT 1
#define GPIO_IN  0

int init_gpio(uint16_t gpio_pin);
int gpio_set_direction(uint16_t gpio_pin, uint8_t direction);
int gpio_set_value(uint16_t gpio_pin, uint8_t value);
int close_gpio(uint16_t gpio_pin);
#endif /* GPIO_RK3288_LINUX_H_ */
