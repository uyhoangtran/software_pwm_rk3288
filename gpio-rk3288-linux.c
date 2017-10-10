/*
 * gpio.c
 *
 *  Created on: Oct 10, 2017
 *      Author: Hoang Tran
 */
#include "gpio-rk3288-linux.h"
#define GPIO_PIN 226

int fd;
char buf[100];

int init_gpio(uint16_t gpio_pin)
{
	fd = open("/sys/class/gpio/export", O_WRONLY);
	sprintf(buf, "%d", gpio_pin);
	write(fd, buf, strlen(buf));
	close(fd);
	return 1;
}

int gpio_set_direction(uint16_t gpio_pin, uint8_t direction)
{
	int gpio = GPIO_PIN;
	sprintf(buf, "/sys/class/gpio/gpio%d/direction", gpio_pin);
	fd = open(buf, O_WRONLY);
	if (direction)
	// Set out direction
	write(fd, "out", 3);
	else
	// Set in direction
	write(fd, "in", 2);
	close(fd);
	return 1;
}

int gpio_set_value(uint16_t gpio_pin, uint8_t value)
{
	sprintf(buf, "/sys/class/gpio/gpio%d/value", gpio_pin);
	fd = open(buf, O_WRONLY);
	if (value)
	// Set GPIO high status
	write(fd, "1", 1);
	else
		write(fd, "0",1);
	close(fd);
	return 1;
}

int close_gpio(uint16_t gpio_pin)
{
	gpio_set_direction(gpio_pin,GPIO_IN); // Reset to GPIO's defauls direction.
	fd = open("/sys/class/gpio/unexport", O_WRONLY);
	sprintf(buf, "%d", gpio_pin);
	write(fd, buf, strlen(buf));
	close(fd);
	return 1;
}
