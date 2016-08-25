/**
 ****************************************************************************************
 *
 * @file LED.c
 *
 * @brief LED control and initialization. 
 *
 *  add by Clement at 2015.12.21
 *
 ****************************************************************************************
 */
/*
 * INCLUDE FILES
 ****************************************************************************************
 */
#include "rwip_config.h"             // SW configuration
#include "periph_setup.h"            // peripheral configuration
#include "pwm.h"	
#include "LED.h"
#include "adc.h"

unsigned char ledflag=0;
//intrupt time
#define INTR_TIME 65530     //65530:333ms
#define TIME_FRM  1000      //500us
/**
 ****************************************************************************************
 * @brief  pwm2 initialization
 *
 * @return void																						
 * add at 2015.12.18  by Clement
 ****************************************************************************************
 */
 
void pwm2_int(void)
{
//		timer2_enable(TRIPLE_PWM_ENABLED);  
	  timer2_set_hw_pause(HW_CAN_PAUSE_PWM_2_3_4);
    timer2_set_sw_pause(PWM_2_3_4_SW_PAUSE_DISABLED);
    timer2_set_pwm_frequency(TIME_FRM);
//		timer2_set_pwm2_duty_cycle(200);
}
/**
 ****************************************************************************************
 * @brief  Timer initialization
 *
 * @return void																						
 * add at 2015.12.18  by Clement
 ****************************************************************************************
 */
void timer_init(void)
{
  set_tmr_enable(CLK_PER_REG_TMR_ENABLED);
	set_tmr_div(CLK_PER_REG_TMR_DIV_8);
	timer0_init(TIM0_CLK_FAST, PWM_MODE_ONE, TIM0_CLK_DIV_BY_10);
	timer0_set(65530, 0, 0);
	timer0_enable_irq();
}
/**
 ****************************************************************************************
 * @brief  Timer register callback function
 *
 * @return void																						
 * add at 2015.12.18  by Clement
 ****************************************************************************************
 */
void timer_callback(void)
{
	 int i;
	 static unsigned int duty =0;
	 static unsigned char dir = 0;
	 static unsigned char ctime =0;
	 static unsigned char count =0;
#if 0
	static unsigned char flag = 0;
	if(flag)
	{
		GPIO_SetActive(GPIO_PORT_1,GPIO_PIN_0);
		flag = 0;
	}
	else
	{
		GPIO_SetInactive(GPIO_PORT_1,GPIO_PIN_0);
		flag = 1;	
	}
#endif
	
//			adc_data = adc_get_sample();
//			batt_level=batt_cal_LIR1025(adc_data);
#if 1
	switch(ledflag)
		{
			case 0:
				timer2_stop();
				duty =0;
				dir =0;
				timer2_set_pwm2_duty_cycle(0);
				break;
			case 1:
				timer2_enable(TRIPLE_PWM_ENABLED); 
				timer2_set_pwm2_duty_cycle(duty);
				if(0==dir)
				{
					duty+=50;
				}
				else 
				{
						
					duty-=50;
				}
				if(duty<50)
				{
					dir =0;
				}
				if(duty>200)
				{
					dir =1;

				}
				break;
			case 2:
								timer2_enable(TRIPLE_PWM_ENABLED); 
				timer2_set_pwm2_duty_cycle(duty);
				if(0==dir)
				{
					duty+=50;
				}
				else 
				{
						
					duty-=50;
				}
				if(duty<50)
				{
					dir =0;
				}
				if(duty>200)
				{
					dir =1;

				}
				break;
//				ctime++;			
//				timer2_enable(TRIPLE_PWM_ENABLED); 
//				timer2_set_pwm2_duty_cycle(200);
//			  if(ctime>3)
//				{
//					ledflag =0;
//					ctime =0;
//				}
//				break;
			case 3:
				ctime++;			
				timer2_enable(TRIPLE_PWM_ENABLED); 
				timer2_set_pwm2_duty_cycle(200);
			  if(ctime>3)
				{
					ledflag =0;
					ctime =0;
				}
				break;
		  default:
				ledflag =0;
			  ctime=0;
				break;
		}
#endif
}
void delay(unsigned int num)
{
  unsigned int i,j;
	for(i = 0;i < num;i++)
	{
    j = 1000;		
	  while(j--);
	}
}

void LED(void)
{
	pwm2_int();
//	timer2_init(HW_CAN_PAUSE_PWM_2_3_4,PWM_2_3_4_SW_PAUSE_DISABLED,TIME_FRM);
//			timer2_enable(TRIPLE_PWM_ENABLED);
	timer_init();
	timer0_register_callback(timer_callback);
	timer0_start();
}
