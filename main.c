/*
 * main.c
 *
 *  Created on: Oct 9, 2017
 *  Author: Hoang Tran
 *  Discription: This program is to output PWM signal to GPIO pin on RK3288 use POSIX timer on Linux 3.10
 *
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
struct itimerspec timer_zero_value;
uint8_t count=0;
uint8_t duty_cycle=101;
//int close_gpio(void);
timer_t timer0id;
static void timer_handler(int sig, siginfo_t *si, void *uc)
       {
			count ++;
			if(count<=duty_cycle)
				gpio_set_value(PWM_PIN,1);
			else
				gpio_set_value(PWM_PIN,0);
			if(count == 100)
				count = 0;
       }
void signal_handler(int signum)
{
	printf("\n\rctrl-c\n");
	loop = 0;
}
int set_timer_value(struct itimerspec* p_timer_value,__time_t sec_value,__syscall_slong_t nsec_value)
{
	p_timer_value->it_interval.tv_sec = sec_value;
	p_timer_value->it_interval.tv_nsec = nsec_value;
	p_timer_value->it_value.tv_sec = sec_value;
	p_timer_value->it_value.tv_nsec = nsec_value;
	return 1;
}
int start_timer(timer_t timer_id, struct itimerspec* p_timer_value,__time_t sec_value,__syscall_slong_t nsec_value)
{
	set_timer_value(p_timer_value,sec_value,nsec_value);
	timer_settime(timer_id,0,p_timer_value,NULL);
	return 1;
}
int stop_timer(timer_t timer_id)
{
	timer_settime(timer_id,0,&timer_zero_value,NULL);
	return 1;
}
int main(void)
{	struct sigaction sa;
	long msec;
	signal(SIGINT, signal_handler);
	loop=1;
	printf("Enter timer interval:");
	scanf("%ld",&msec);
	while (duty_cycle>100)
	{
		printf("\nEnter duty cycle (must be <= 100): ");
		scanf("%d",&duty_cycle);
	}
	set_timer_value(&timer_zero_value,0,0);
	init_gpio(PWM_PIN);
	gpio_set_direction(PWM_PIN,GPIO_OUT);
	gpio_set_value(PWM_PIN,1);
	sa.sa_flags = SA_SIGINFO;
	sa.sa_sigaction = timer_handler;
	sigemptyset(&sa.sa_mask);
	if (sigaction(SIG, &sa, NULL) == -1)
	               errExit("sigaction");
	printf("Blocking signal %d\n", SIG);
	sev.sigev_notify = SIGEV_SIGNAL;
	sev.sigev_signo = SIG;
	sev.sigev_value.sival_ptr = &timer0id;
	timer_create(CLOCK_MONOTONIC,&sev,&timer0id);
	start_timer(timer0id,&timer_value,0,msec*1000);
	while(loop==1)
	{
	}
	start_timer(timer0id,&timer_value,0,0);
	close_gpio(PWM_PIN);
	return 1;
}
