/*
 * hal_bma250.c
 *
 *  Created on: 2014?12?25?
 *      Author: Jason.Lau
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
#include "test.h"

sleep_mode_t sleep_mode;
extern bool sys_startup_flag;

int k,b;
/*gpio intrupt */// addby leiyq 20150910
void gpio01_init(void)
{
  SetWord16(GPIO_IRQ0_IN_SEL_REG,0x0012);//0x0006是GPIO05
	SetWord16(GPIO_IRQ1_IN_SEL_REG,0x0007);
	SetWord16(GPIO_DEBOUNCE_REG,0x0310);
	SetWord16(GPIO_INT_LEVEL_CTRL_REG,0x0303);
	NVIC_SetPriority(GPIO0_IRQn,1);
	NVIC_SetPriority(GPIO1_IRQn,2);
	NVIC_EnableIRQ(GPIO0_IRQn);
	NVIC_EnableIRQ(GPIO1_IRQn);
}
void testslp(void)
{
//	k=1;
//	sleep_mode=mode_sleeping;
}
void test(void)
{
	
		k=GPIO_GetPinStatus(GPIO_PORT_0,GPIO_PIN_7);
		wkupct_enable_irq(0x80,0x80,1,0); 

	if(GetBits16(SYS_STAT_REG, PER_IS_DOWN))
				periph_init();
	
		b=app_ble_ext_wakeup_get();
		if (app_ble_ext_wakeup_get())
		{
				//Wakeup BLE here
//				SetBits32(BLE_DEEPSLCNTL_REG, SOFT_WAKEUP_REQ, 1);
				SetBits32(GP_CONTROL_REG, BLE_WAKEUP_REQ, 1); 
				app_ble_ext_wakeup_off();
				app_adv_start();
		}
	
		/*
		if(k==1)
		{
			//sleep_mode=mode_sleeping;
									
			if (sleep_env.slp_state == ARCH_EXT_SLEEP_ON) {
				sleep_mode = mode_ext_sleep;
			} else {
				sleep_mode = mode_deep_sleep;
			}
		
		}
		else if(k==0)
		{
			sleep_mode=rwip_sleep();
		}
	 */
//	NVIC_ClearPendingIRQ(WKUP_QUADEC_IRQn);
	return;
}

void GPIO_0_IRQINIT()
{
		
		GPIO_EnableIRQ( GPIO_PORT_1, GPIO_PIN_3, GPIO0_IRQn, 0 , 1 , 0 );

//  	GPIO_ConfigurePin( GPIO_PORT_0, GPIO_PIN_7, INPUT_PULLUP , PID_GPIO ,false );//引脚状态为高电平
//		GPIO_ConfigurePin( GPIO_PORT_0, GPIO_PIN_7, INPUT_PULLUP , PID_GPIO ,true  );//引脚状态为高电平
//		GPIO_ConfigurePin( GPIO_PORT_0, GPIO_PIN_7, INPUT_PULLDOWN , PID_GPIO ,true  );//引脚状态为低电平
		GPIO_ConfigurePin( GPIO_PORT_0, GPIO_PIN_7, INPUT_PULLDOWN , PID_GPIO ,false  );//引脚状态为低电平	
//	GPIO_ConfigurePin( GPIO_PORT_0, GPIO_PIN_5, INPUT_PULLUP , PID_GPIO ,true );
//	GPIO_ConfigurePin( GPIO_PORT_0, GPIO_PIN_5, INPUT , PID_GPIO    ,false );
//	GPIO_RegisterCallback(WKUP_QUADEC_IRQn,test);
//	GPIO_RegisterCallback(BLE_WAKEUP_LP_IRQn,test);
		GPIO_RegisterCallback(GPIO0_IRQn,testslp);
//		wkupct_register_callback(test); 
}

