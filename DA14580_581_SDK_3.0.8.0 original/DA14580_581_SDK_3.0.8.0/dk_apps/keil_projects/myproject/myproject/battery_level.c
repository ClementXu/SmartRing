/**
 ****************************************************************************************
 *
 * @file battery_level.c
 *
 * @brief ADC configeration and battery level sample 
 *
 *  add by Clement at 2016.1.22
 *
 ****************************************************************************************
 */
#include "adc.h"
#include "battery_level.h"

int adc_data;
int voltage[10];
int sum=0;
int real_voltage;
void adc_config(){

		adc_init(GP_ADC_SE,0,0);
		adc_enable_channel(ADC_CHANNEL_P00);
};



uint8_t batt_cal_LIR1025(){
    
		int batt_lvl;
	  int adc_data;
	  int i;
    adc_data = adc_get_sample();
		  for(i=0;i<10;i++)
		{
				voltage[i] = adc_get_sample();
			  sum+=voltage[i];
		}
	  real_voltage=sum/0xA;
    //1023=4.2V, 677=2.75V
    if(real_voltage >= 677)
        batt_lvl = (real_voltage - 677)*100/346;
    else
        batt_lvl = 0;
		sum=0;
		return batt_lvl;
};

