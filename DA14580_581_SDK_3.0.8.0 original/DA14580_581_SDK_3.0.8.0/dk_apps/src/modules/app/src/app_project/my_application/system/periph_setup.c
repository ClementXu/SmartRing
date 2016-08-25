/**
 ****************************************************************************************
 *
 * @file periph_setup.c
 *
 * @brief Peripherals setup and initialization. 
 *
 * Copyright (C) 2012. Dialog Semiconductor Ltd, unpublished work. This computer 
 * program includes Confidential, Proprietary Information and is a Trade Secret of 
 * Dialog Semiconductor Ltd.  All use, disclosure, and/or reproduction is prohibited 
 * unless authorized in writing. All Rights Reserved.
 *
 * <bluetooth.support@diasemi.com> and contributors.
 *
 ****************************************************************************************
 */
/*
 * INCLUDE FILES
 ****************************************************************************************
 */
#include "rwip_config.h"             // SW configuration
#include "periph_setup.h"            // peripheral configuration
#include "global_io.h"
#include "gpio.h"
#include "uart.h"                    // UART initialization
#include "LED.h" 
#include "adc.h" 
#include "battery_level.h"
/**
 ****************************************************************************************
 * @brief Each application reserves its own GPIOs here.
 *
 * @return void
 ****************************************************************************************
 */

#if DEVELOPMENT_DEBUG

void GPIO_reservations(void)
{
/*
* Globally reserved GPIOs reservation
*/

/*
* Application specific GPIOs reservation. Used only in Development mode (#if DEVELOPMENT_DEBUG)
    
i.e.  
    RESERVE_GPIO(DESCRIPTIVE_NAME, GPIO_PORT_0, GPIO_PIN_1, PID_GPIO);    //Reserve P_01 as Generic Purpose I/O
*/
#if BLE_INTEGRATED_HOST_GTL
    // UART
      RESERVE_GPIO( UART1_TX, UART1_TX_GPIO_PORT,  UART1_TX_GPIO_PIN, PID_UART1_TX);
      RESERVE_GPIO( UART1_RX, UART1_RX_GPIO_PORT,  UART1_RX_GPIO_PIN, PID_UART1_RX);    

      RESERVE_GPIO( UART1_RTS, UART1_RTSN_GPIO_PORT, UART1_RTSN_GPIO_PIN, PID_UART1_RTSN);
      RESERVE_GPIO( UART1_CTS, UART1_CTSN_GPIO_PORT, UART1_CTSN_GPIO_PIN, PID_UART1_CTSN);    
#endif
#ifdef CFG_PRINTF_UART2
	RESERVE_GPIO( UART2_TX, GPIO_PORT_1,  GPIO_PIN_2, PID_UART2_TX);
	RESERVE_GPIO( UART2_RX, GPIO_PORT_1,  GPIO_PIN_3, PID_UART2_RX);
#endif
//	RESERVE_GPIO(GPIO,GPIO_PORT_1 ,GPIO_PIN_3 ,PID_GPIO);
//	RESERVE_GPIO(GPIO,GPIO_PORT_0 ,GPIO_PIN_7 ,PID_GPIO);
//	RESERVE_GPIO(GPIO,GPIO_PORT_1 ,GPIO_PIN_0 ,PID_PWM2);
}
#endif //DEVELOPMENT_DEBUG

/**
 ****************************************************************************************
 * @brief Map port pins
 *
 * The Uart and SPI port pins and GPIO ports are mapped
 ****************************************************************************************
 */
void set_pad_functions(void)        // set gpio port function mode
{

#ifdef CFG_PRINTF_UART2
	GPIO_ConfigurePin( GPIO_PORT_1, GPIO_PIN_2, OUTPUT, PID_UART2_TX, false );
	GPIO_ConfigurePin( GPIO_PORT_1, GPIO_PIN_3, INPUT, PID_UART2_RX, false );
#endif
/*
* Configure application ports.
i.e.    
    GPIO_ConfigurePin( GPIO_PORT_0, GPIO_PIN_1, OUTPUT, PID_GPIO, false ); // Set P_01 as Generic purpose Output
*/
//	GPIO_ConfigurePin( GPIO_PORT_1, GPIO_PIN_0, OUTPUT , PID_PWM2 ,false );
//	GPIO_ConfigurePin( GPIO_PORT_1, GPIO_PIN_3, INPUT, PID_GPIO, false );
}


/**
 ****************************************************************************************
 * @brief Enable pad's and peripheral clocks assuming that peripherals' power domain is down. The Uart and SPi clocks are set.
 *
 * @return void
 ****************************************************************************************
 */

void periph_init(void) 
{
	// Power up peripherals' power domain
    SetBits16(PMU_CTRL_REG, PERIPH_SLEEP, 0);
    while (!(GetWord16(SYS_STAT_REG) & PER_IS_UP)) ; 
    
    SetBits16(CLK_16M_REG,XTAL16_BIAS_SH_DISABLE, 1);
	
	//rom patch
	patch_func();
	
	//Init pads
	set_pad_functions();
	//Timer initialization
//add by Clement at 2015.12.21
	LED();
	adc_config();
#if (BLE_APP_PRESENT)
/*
* (Re)Initialize peripherals
i.e.    
    uart_init(UART_BAUDRATE_115K2, 3);
*/        
#endif

#ifdef CFG_PRINTF_UART2
		SetBits16(CLK_PER_REG, UART2_ENABLE, 1);
		uart2_init(UART_BAUDRATE_115K2, 3);
#endif
    
   // Enable the pads
	SetBits16(SYS_CTRL_REG, PAD_LATCH_EN, 1);
}
