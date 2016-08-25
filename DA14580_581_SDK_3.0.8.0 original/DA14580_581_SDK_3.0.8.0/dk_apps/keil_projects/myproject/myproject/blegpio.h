/**
 ****************************************************************************************
 *
 * @file ble_gpio.h
 *
 * @brief RW stack configuration file.
 *
 * Copyright (C) 2014. Dialog Semiconductor Ltd, unpublished work. This computer 
 * program includes Confidential, Proprietary Information and is a Trade Secret of 
 * Dialog Semiconductor Ltd.  All use, disclosure, and/or reproduction is prohibited 
 * unless authorized in writing. All Rights Reserved.
 *
 * <bluetooth.support@diasemi.com> and contributors.
 *
 ****************************************************************************************
 */

#ifndef BLE_GPIO_H
#define BLE_GPIO_H

/////////////////////////////////////////////////////////////
/*Do not alter*/
#define SPI_BMA250_SCLK_PORT           GPIO_PORT_1
#define SPI_BMA250_SCLK_PIN            GPIO_PIN_1

#define SPI_BMA250_MOSI_PORT           GPIO_PORT_0
#define SPI_BMA250_MOSI_PIN            GPIO_PIN_3

#define SPI_BMA250_MISO_PORT           GPIO_PORT_1
#define SPI_BMA250_MISO_PIN            GPIO_PIN_5

#define SPI_BMA250_CS_PORT             GPIO_PORT_0
#define SPI_BMA250_CS_PIN              GPIO_PIN_6

//#define SPI_BMA250_INT1_PORT           GPIO_PORT_0
//#define SPI_BMA250_INT1_PIN            GPIO_PIN_5

//#define EXTERNAL_WAKEUP_GPIO_PORT           GPIO_PORT_0
//#define EXTERNAL_WAKEUP_GPIO_PIN            GPIO_PIN_5

//#define SPI_BMA250_INT2_PORT           GPIO_PORT_0
//#define SPI_BMA250_INT2_PIN            GPIO_PIN_6
//pwm port define
#define PWM_PORT                       GPIO_PORT_0
#define PWM_PIN                        GPIO_PIN_4

#define LED_PORT                       GPIO_PORT_0 //addby leiyq 20150910	
#define LED_PIN                        GPIO_PIN_4 //addby leiyq 20150910


#define ADC_PORT             GPIO_PORT_0
#define ADC_PIN              GPIO_PIN_0                    

#endif // DA14580_STACK_CONFIG_H_
