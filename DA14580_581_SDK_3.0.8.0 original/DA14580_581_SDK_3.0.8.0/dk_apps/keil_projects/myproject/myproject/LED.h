/**
 ****************************************************************************************
 *
 * @file LED.h
 *
 * @brief LED control and initialization. 
 *
 *  add by Clement at 2015.12.21
 *
 ****************************************************************************************
 */

#include <stdint.h>
#include <stdbool.h>

void pwm2_int(void);
void timer_init(void);
void timer_callback(void);
void delay(unsigned int num);
void LED(void);