/*
 * main.c
 *
 *  Created on: Oct 9, 2017
 *  Author: Hoang Tran
 */
#include <stdlib.h>
#include <stddef.h>
#include <time.h>
#include <signal.h>
#include "gpio-rk3288-linux.h"
#define SIG SIGRTMIN
#define errExit(msg)    do { perror(msg); exit(EXIT_FAILURE); \
                               } while (0)
uint8_t gpio_value;
uint8_t loop;

struct sigevent sev;
struct itimerspec timer_value;

//int close_gpio(void);
timer_t timer0id;
static void
       handler(int sig, siginfo_t *si, void *uc)
       {
           gpio_set_value(PWM_PIN,gpio_value);
           gpio_value^=1;
       }
void signalHandler(int signum)
{
	printf("\n\rctrl-c\n");
	loop = 0;
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
	init_gpio(PWM_PIN);
	gpio_set_direction(PWM_PIN,GPIO_OUT);
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
	}
	timer_value.it_interval.tv_sec = 0;
	timer_value.it_interval.tv_nsec = 0;
	timer_value.it_value.tv_sec = timer_value.it_interval.tv_sec;
	timer_value.it_value.tv_nsec = timer_value.it_interval.tv_nsec;
	timer_settime(timer0id,0,&timer_value,NULL);
	close_gpio(PWM_PIN);
	return 1;
}
