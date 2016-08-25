/**
 ****************************************************************************************
 *
 * @file gpio_config.c
 *
 * @brief GPIO configer and initialization. 
 *
 *  add by Clement at 2015.12.21
 *
 ****************************************************************************************
 */

#include "gpio.h"
#include "app_api.h"
#include "periph_setup.h" 
#include "adc.h"
#include "global_io.h"

#include "datasheet.h"

#include "em_map_ble_user.h"
#include "em_map_ble.h"

#include "lld_sleep.h"
#include "rwble.h"
#include "rf_580.h"
#include "gpio.h"
#include "da14580_scatter_config.h"
#include "arch.h"
#include "arch_sleep.h"
#include <stdlib.h>
#include <stddef.h>     // standard definitions
#include <stdint.h>     // standard integer definition
#include <stdbool.h>    // boolean definition
#include "boot.h"       // boot definition
#include "rwip.h"       // BLE initialization
#include "syscntl.h"    // System control initialization
#include "emi.h"        // EMI initialization
#include "intc.h"       // Interrupt initialization
#include "timer.h"      // TIMER initialization
#include "em_map_ble.h"
#include "ke_mem.h"
#include "ke_event.h"
#include "periph_setup.h"
#include "adc.h"
#include "wkupct_quadec.h"
#include "gpio_config.h"
#include "bma250_driver.h"

uint32_t int_status;
int wakeupflag;
int fakeflag;
int doubleflag;
uint8_t addr;
extern int batt_level;
//uint8_t batt_level;
int intflag;
extern int bma_state;
extern sleep_mode_t sleep_mode;
uint8_t id;
extern bool sys_startup_flag;
extern unsigned char ledflag;
extern struct arch_sleep_env_tag sleep_env;
int k,b;
/*gpio intrupt */// 
void gpio01_init(void)
{
  SetWord16(GPIO_IRQ0_IN_SEL_REG,0x0016);//0x0006是GPIO05  0x0012是GPIO13  16是21
	SetWord16(GPIO_IRQ1_IN_SEL_REG,0x0007);
	SetWord16(GPIO_DEBOUNCE_REG,0x0310);
	SetWord16(GPIO_INT_LEVEL_CTRL_REG,0x0303);
	NVIC_SetPriority(GPIO0_IRQn,1);
	NVIC_SetPriority(GPIO1_IRQn,2);
	NVIC_EnableIRQ(GPIO0_IRQn);
	NVIC_EnableIRQ(GPIO1_IRQn);
}
void testslp0(void)
{

	BMA250_GetIntStatus( &int_status);
	if((int_status&0x20) ==0x20)
	{
		if(ledflag == 0)
		{
//			app_sent_cmd(0x3a);
			app_sent_data(0x3a,batt_level);
			ledflag =2;
		}
		else if(ledflag ==1)
		{
			ledflag =0;
			intflag=0;
			delay(200);
//			app_sent_cmd(0x2a);//recv call
			app_sent_data(0x2a,batt_level);
			if(bma_state==0)
			{
				wakeupflag=0;
				hal_bma250_init();
			}
		}
	}

	if((int_status&0x10) ==0x10)
	{
			ledflag = 0;	
//		  app_sent_cmd(0x3b);
			app_sent_data(0x3b,batt_level);
			intflag=0;
			delay(200);
	}		
		SetWord16(GPIO_RESET_IRQ_REG,0x0001);
}

void test(void)
{
	
			k=GPIO_GetPinStatus(GPIO_PORT_0,GPIO_PIN_5);
			intflag=1;
			wkupct_disable_irq();
	if(GetBits16(SYS_STAT_REG, PER_IS_DOWN))
			periph_init();
			GPIO_CONF();
			wakeupflag=1;
			hal_bma250_init();
			return;
}

void GPIO_CONF()
{
		RESERVE_GPIO(GPIO,GPIO_PORT_0 ,GPIO_PIN_5 ,PID_GPIO);
		RESERVE_GPIO(GPIO,GPIO_PORT_0 ,GPIO_PIN_4 ,PID_PWM2);
		RESERVE_GPIO(GPIO,GPIO_PORT_0 ,GPIO_PIN_0 ,PID_ADC);

		GPIO_EnableIRQ( GPIO_PORT_0, GPIO_PIN_5, GPIO0_IRQn, 0 , 1 , 0 );

		GPIO_ConfigurePin( GPIO_PORT_0, GPIO_PIN_5, INPUT_PULLDOWN , PID_GPIO ,false  );//引脚状态为低电平	

		GPIO_ConfigurePin( GPIO_PORT_0, GPIO_PIN_4, OUTPUT , PID_PWM2 ,false );
		GPIO_ConfigurePin( GPIO_PORT_0, GPIO_PIN_0, INPUT , PID_ADC ,false );

		GPIO_RegisterCallback(GPIO0_IRQn,testslp0);


}
