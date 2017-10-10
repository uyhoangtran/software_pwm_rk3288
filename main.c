/*
 * main.c
 *
 *  Created on: Oct 9, 2017
 *      Author: root
 */
#include <stdlib.h>
#include <stddef.h>
#include <stdio.h>
#include <stdint.h>
#include <fcntl.h>
#include <unistd.h>
#include <time.h>
#include <signal.h>
       #define SIG SIGRTMIN
#define GPIO_PIN 226
#define errExit(msg)    do { perror(msg); exit(EXIT_FAILURE); \
                               } while (0)
uint8_t gpio_value;
int init_gpio(void);
uint8_t loop;
int gpio_set_direction(uint8_t direction);
int gpio_set_value(uint8_t value);

struct sigevent sev;
struct itimerspec timer_value;
int fd;
char buf[100];
int gpio = GPIO_PIN;
//int close_gpio(void);
timer_t timer0id;
static void
       handler(int sig, siginfo_t *si, void *uc)
       {
           gpio_set_value(gpio_value);
           gpio_value^=1;
       }
void signalHandler(int signum)
{
	printf("\n\rctrl-c\n");
	loop = 0;
	timer_value.it_interval.tv_sec = 0;
		timer_value.it_interval.tv_nsec = 0;
		timer_value.it_value.tv_sec = timer_value.it_interval.tv_sec;
		timer_value.it_value.tv_nsec = timer_value.it_interval.tv_nsec;
		timer_settime(timer0id,0,&timer_value,NULL);

}


int main(void)
{	struct sigaction sa;
	long usec;
	signal(SIGINT, signalHandler);
	loop=1;

	scanf("%ld",&usec);
	timer_value.it_interval.tv_sec = 0;
	timer_value.it_interval.tv_nsec = usec*1000;
	timer_value.it_value.tv_sec = timer_value.it_interval.tv_sec;
	timer_value.it_value.tv_nsec = timer_value.it_interval.tv_nsec;
	init_gpio();
	gpio_set_direction(1);
	gpio_value=1;
	sa.sa_flags = SA_SIGINFO;
	sa.sa_sigaction = handler;
	sigemptyset(&sa.sa_mask);
	if (sigaction(SIG, &sa, NULL) == -1)
	               errExit("sigaction");
	 printf("Blocking signal %d\n", SIG);
	 sev.sigev_notify = SIGEV_SIGNAL;
	 sev.sigev_signo = SIG;
	 sev.sigev_value.sival_ptr = &timer0id;
	timer_create(CLOCK_MONOTONIC,&sev,&timer0id);
	timer_settime(timer0id,0,&timer_value,NULL);
	while(loop==1)
	{
		sleep(1);
	}
	return 1;
}

int init_gpio(void)
{
		fd = open("/sys/class/gpio/export", O_WRONLY);

	sprintf(buf, "%d", gpio);

	write(fd, buf, strlen(buf));

	close(fd);
	return 1;
}

int gpio_set_direction(uint8_t direction)
{
	int gpio = GPIO_PIN;
	sprintf(buf, "/sys/class/gpio/gpio%d/direction", gpio);

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
int gpio_set_value(uint8_t value)
{
	sprintf(buf, "/sys/class/gpio/gpio%d/value", gpio);

	fd = open(buf, O_WRONLY);
	if (value)
	// Set GPIO high status
	write(fd, "1", 1);
	else
		write(fd, "0",1);
	close(fd);
	return 1;
}


