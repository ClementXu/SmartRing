/*
 * hal_bma250.h
 *
 *  Created on: 2014年12月25日
 *      Author: Jason.Lau
 */

#ifndef HAL_BMA250_H_
#define HAL_BMA250_H_

#include <stdint.h>
#include <stdbool.h>



uint8_t BMA250_ReadReg(uint8_t Reg, uint8_t* Data);
uint8_t BMA250_WriteReg(uint8_t WriteAddr, uint8_t Data);
uint8_t BMA250_ReadRegBurst(uint8_t Reg, uint16_t size ,uint8_t* Data);
int hal_bma250_init();
bool hal_bma250_get_int1_status(void);
bool hal_bma250_get_int2_status(void);

#endif /* HAL_BMA250_H_ */
