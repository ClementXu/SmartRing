/*
 * bma250_driver.c
 *
 *  Created on: 2014-12-25
 *      Author: Jason.Lau
 */
#include <bma250_driver.h>
#include <hal_bma250.h>
#include <stdbool.h>
#include <stdint.h>

uint8_t BMA250_GetChipID(uint8_t* id)
{
    BMA250_ReadReg(BMA250_CHIP_ID_REG,id);

    return 0;
}

uint8_t BMA250_GetTemprature(int8_t* tmp)
{
    BMA250_ReadReg(BMA250_CHIP_ID_REG,(uint8_t*)tmp);
    *tmp += 23;

    return 0;
}

uint8_t BMA250_GetIntStatus(uint32_t* int_status)
{
    uint8_t tmp[4];

    BMA250_ReadReg(BMA250_INT_STATUS_0_REG,&tmp[0]);
    BMA250_ReadReg(BMA250_INT_STATUS_1_REG,&tmp[1]);
    BMA250_ReadReg(BMA250_INT_STATUS_2_REG,&tmp[2]);
    BMA250_ReadReg(BMA250_INT_STATUS_3_REG,&tmp[3]);

    *int_status = 0;
    *int_status =
              ((uint32_t)tmp[3]<<24)
            | ((uint32_t)tmp[2]<<16)
            | ((uint32_t)tmp[1]<<8)
            |  (uint32_t)tmp[0];

    return 0;
}

/**
 * reset BMA250
 * @return
 */
uint8_t BMA250_SoftwareReset( void  )
{
    BMA250_WriteReg(BMA250_BGW_SOFTRESET_REG,0xB6);

    return 0;
}



/**
 * @example BMA250_SoftwareReset(BMA250_PMU_RANGE_2G);
 *          BMA250_SoftwareReset(BMA250_PMU_RANGE_4G);
 *          BMA250_SoftwareReset(BMA250_PMU_RANGE_8G);
 *          BMA250_SoftwareReset(BMA250_PMU_RANGE_16G);
 * @return
 */
uint8_t BMA250_SetRange(uint8_t range  )
{
    uint8_t range_r;

    BMA250_WriteReg(BMA250_PMU_RANGE_REG, range);
    BMA250_ReadReg(BMA250_PMU_RANGE_REG, &range_r);

    return range_r;
//    return 0;
}


/**
 * @example
 *          BMA250_SetFilterBandwidth(BMA250_PMU_BW_7_81HZ  );
 *          BMA250_SetFilterBandwidth(BMA250_PMU_BW_15_63HZ );
 *          BMA250_SetFilterBandwidth(BMA250_PMU_BW_31_25HZ );
 *          BMA250_SetFilterBandwidth(BMA250_PMU_BW_62_50HZ );
 *          BMA250_SetFilterBandwidth(BMA250_PMU_BW_125HZ   );
 *          BMA250_SetFilterBandwidth(BMA250_PMU_BW_250HZ   );
 *          BMA250_SetFilterBandwidth(BMA250_PMU_BW_500HZ   );
 *          BMA250_SetFilterBandwidth(BMA250_PMU_BW_1000HZ  );
 * @return
 */
uint8_t BMA250_SetFilterBandwidth( uint8_t bw  )
{
    BMA250_WriteReg(BMA250_PMU_BW_REG, bw);

    return 0;
}



/**
 * @example
 *          BMA250_SetPowerMode(BMA250_POWER_MODE_NORMAL      );
 *          BMA250_SetPowerMode(BMA250_POWER_MODE_DEEP_SUSOEND);
 *          BMA250_SetPowerMode(BMA250_POWER_MODE_LOW_POWER   );
 *          BMA250_SetPowerMode(BMA250_POWER_MODE_SUSPEND     );
 * @return
 */
//uint8_t BMA250_SetPowerMode( uint8_t mode  )
//{
//    uint8_t old_val;
////    uint8_t new_val;

//    BMA250_ReadReg(BMA250_PMU_LPW_REG,&old_val);

////    new_val = old_val & (~BMA250_PMU_LPW_POWER_MODE_MASK);
////    new_val |= (mode << BMA250_PMU_LPW_POWER_MODE_POS);

////    BMA250_WriteReg(BMA250_PMU_LPW_REG, new_val);
//	  BMA250_WriteReg(BMA250_PMU_LPW_REG, mode);

//    return 0;
//}
uint8_t BMA250_SetPowerMode( bma250_mode mode  )
{
    uint8_t old_val;
//    uint8_t new_val;

    BMA250_ReadReg(BMA250_PMU_LPW_REG,&old_val);

//    new_val = old_val & (~BMA250_PMU_LPW_POWER_MODE_MASK);  
//    new_val |= (mode << BMA250_PMU_LPW_POWER_MODE_POS);

//    BMA250_WriteReg(BMA250_PMU_LPW_REG, new_val);
	  BMA250_WriteReg(BMA250_PMU_LPW_REG, mode);

    return 0;
}

uint8_t BMA250_GetRawXYZ(int16_t* x,int16_t* y,int16_t* z)
{
    uint8_t temp_data[6];

    BMA250_ReadReg(BMA250_X_AXIS_LSB_REG  , &temp_data[0]   );
    BMA250_ReadReg(BMA250_X_AXIS_MSB_REG  , &temp_data[1]   );
    BMA250_ReadReg(BMA250_Y_AXIS_LSB_REG  , &temp_data[2]   );
    BMA250_ReadReg(BMA250_Y_AXIS_MSB_REG  , &temp_data[3]   );
    BMA250_ReadReg(BMA250_Z_AXIS_LSB_REG  , &temp_data[4]   );
    BMA250_ReadReg(BMA250_Z_AXIS_MSB_REG  , &temp_data[5]   );

    *x = (int16_t)((((int32_t)((int8_t)temp_data[1])) << 8) | (temp_data[0] & 0xc0));
    *y = (int16_t)((((int32_t)((int8_t)temp_data[3])) << 8) | (temp_data[2] & 0xc0));
    *z = (int16_t)((((int32_t)((int8_t)temp_data[5])) << 8) | (temp_data[4] & 0xc0));

    *x >>= 6;
    *y >>= 6;
    *z >>= 6;

    return 0;
}


void BMA250_TansferFifoDataXYZ(uint8_t* fifo_data,int16_t* x,int16_t* y,int16_t* z)
{
    *x = (int16_t)((((int32_t)((int8_t)fifo_data[1])) << 8) | (fifo_data[0] & 0xc0));
    *y = (int16_t)((((int32_t)((int8_t)fifo_data[3])) << 8) | (fifo_data[2] & 0xc0));
    *z = (int16_t)((((int32_t)((int8_t)fifo_data[5])) << 8) | (fifo_data[4] & 0xc0));

    *x >>= 6;
    *y >>= 6;
    *z >>= 6;
}

 



/**
 *
 * @param low_g_delay_time  unit is ms ,  range from 2 ms to 512 ms
 * @param low_g_threshold   unit is mg ,  range from 0 g to 1.992 g
 * @param low_g_hysteresis  unit is mg
 * @param high_g_delay_time unit is ms
 * @param high_g_threshold  unit is mg
 * @param high_g_hysteresis unit is mg
 * @param intrrupt_mode     ‘0’ single-axis mode, or ‘1’ axis-summing mode
 * @return
 */
uint8_t BMA250_CfgLowHighGInterrupt(
        uint16_t low_g_delay_time,
        uint16_t low_g_threshold,
        uint16_t low_g_hysteresis,

        uint16_t high_g_delay_time,
        uint16_t high_g_threshold,
        uint16_t high_g_hysteresis,

        uint8_t intrrupt_mode)
{
    uint8_t range;
    //get current range
    BMA250_ReadReg( BMA250_PMU_RANGE_REG,&range);

    /**
     * check input low g parameter
     */
    if((low_g_delay_time<2) || (low_g_delay_time>512))
    {
        return 1;
    }
    if(  (low_g_threshold>1992))
    {
        return 1;
    }
    if(  (low_g_hysteresis>3*125))
    {
        return 1;
    }

    /**
     * check input high g parameter
     */
    if((high_g_delay_time<2) || (high_g_delay_time>512))
    {
        return 1;
    }
    switch(range)
    {
    case BMA250_PMU_RANGE_2G  :
        if(  (high_g_threshold>1992))
        {
            return 1;
        }
        if( (high_g_hysteresis>3*125))
        {
            return 1;
        }
        break;
    case BMA250_PMU_RANGE_4G  :
        if( (high_g_threshold>3986))
        {
            return 1;
        }
        if(  (high_g_hysteresis>3*250))
        {
            return 1;
        }
        break;
    case BMA250_PMU_RANGE_8G  :
        if( (high_g_threshold>7969))
        {
            return 1;
        }
        if(  (high_g_hysteresis>3*500))
        {
            return 1;
        }
        break;
    case BMA250_PMU_RANGE_16G  :
        if( (high_g_threshold>15938))
        {
            return 1;
        }
        if(  (high_g_hysteresis>3*1000))
        {
            return 1;
        }
        break;
    default:break;
    }

    uint8_t low_dur;
    uint8_t low_th;
    uint8_t low_hy;

    uint8_t high_dur;
    uint8_t high_th;
    uint8_t high_hy;

    uint8_t int_2_value;

    low_dur = low_g_delay_time/2 - 1;     //delay_time = [low_dur<7:0>  +  1]  •  2ms
    low_th = (uint8_t)((float)low_g_threshold/7.84);    //low_th<7:0>  •  7.81
    low_hy = (uint8_t)((float)low_g_threshold/125);

    high_dur = high_g_delay_time/2 - 1;     //delay_time = [low_dur<7:0>  +  1]  •  2ms
    switch(range)
    {
    case BMA250_PMU_RANGE_2G  :
        high_th = (uint8_t)((float)high_g_threshold/7.84);    //low_th<7:0>  •  7.81
        high_hy = (uint8_t)((float)high_g_threshold/125);
        break;
    case BMA250_PMU_RANGE_4G  :
        high_th = (uint8_t)((float)high_g_threshold/15.63);    //low_th<7:0>  •  7.81
        high_hy = (uint8_t)((float)high_g_threshold/250);
        break;
    case BMA250_PMU_RANGE_8G  :
        high_th = (uint8_t)((float)high_g_threshold/31.25);    //low_th<7:0>  •  7.81
        high_hy = (uint8_t)((float)high_g_threshold/500);
        break;
    case BMA250_PMU_RANGE_16G  :
        high_th = (uint8_t)((float)high_g_threshold/62.5);    //low_th<7:0>  •  7.81
        high_hy = (uint8_t)((float)high_g_threshold/1000);
        break;
    default:break;
    }

    int_2_value = 0;

    int_2_value |= ((low_hy << BMA250_INT_2_LOW_HY_POS)   & BMA250_INT_2_LOW_HY_MASK  );
    int_2_value |= ((high_hy << BMA250_INT_2_HIGH_HY_POS) & BMA250_INT_2_HIGH_HY_MASK );

    if(intrrupt_mode)
    {
        int_2_value |= (0x01<<BMA250_INT_2_LOW_MODE_POS);
    }
    else
    {
        int_2_value &= ~(0x01<<BMA250_INT_2_LOW_MODE_POS);
    }

    BMA250_WriteReg( BMA250_INT_0_REG,low_dur);
    BMA250_WriteReg( BMA250_INT_1_REG,low_th);
    BMA250_WriteReg( BMA250_INT_2_REG,high_dur);
    BMA250_WriteReg( BMA250_INT_3_REG,high_dur);
    BMA250_WriteReg( BMA250_INT_4_REG,high_th);

    return 0;
}






/**
 *
 * @param enable_cfg
 *
 * @return
 * @example BMA250_SetIntEnable(BMA250_INT_EN_SLO_NO_MOT_EN_Y | BMA250_INT_EN_SLO_NO_MOT_EN_X);
 */
uint8_t BMA250_SetIntEnable( uint32_t enable_cfg )
{
    BMA250_WriteReg( BMA250_INT_EN_0_REG, enable_cfg        & 0xff);
    BMA250_WriteReg( BMA250_INT_EN_1_REG,(enable_cfg >> 8 ) & 0xff);
    BMA250_WriteReg( BMA250_INT_EN_2_REG,(enable_cfg >> 16) & 0xff);

    return 0;
}



uint8_t BMA250_SetIntPinOutMode(
        bool int1_act_lvl_is_high,
        bool int1_is_od_mode,
        bool int2_act_lvl_is_high,
        bool int2_is_od_mode )
{
    uint8_t int_ctrl = 0;

    if(int1_act_lvl_is_high)
    {
    int_ctrl |= 0x01;
    }
    if(int1_is_od_mode)
    {
    int_ctrl |= (0x01<<2);
    }
    if(int2_act_lvl_is_high)
    {
    int_ctrl |= (0x01<<3);
    }
    if(int2_is_od_mode)
    {
    int_ctrl |= (0x01<<4);
    }

    BMA250_WriteReg( BMA250_INT_OUT_CTRL_REG,int_ctrl);

    return 0;
}






/**
 *
 *
 *
 *
 * @param int1func
 * @param int2func
 * @return
 * @example   BMA250_SetIntPinMap( BMA250_INT_MAP_INT1_FWM, BMA250_INT_MAP_INT2_FWM) ;
 */
uint8_t BMA250_SetIntPinMap(uint16_t int1func,  uint16_t int2func)
{
    BMA250_WriteReg( BMA250_INT_MAP_0_REG,(uint8_t)( int1func & 0xff) );
    BMA250_WriteReg( BMA250_INT_MAP_1_REG,(uint8_t)((int1func >> 8  ) & 0x07 )
                                        | (uint8_t)((int2func >> 5  ) & 0xE0) );
    BMA250_WriteReg( BMA250_INT_MAP_2_REG,(uint8_t)( int2func & 0xff) );

    return 0;
}


uint8_t BMA250_SetOffsetCompensation()
{

}


/**
 *
 * @param addr   0x38  to  0x3C
 * @param data
 * @return
 */
uint8_t BMA250_ReadNVM(uint8_t addr,uint8_t* data)
{
    return BMA250_ReadReg(addr,data);
}


#define BMA250_TRIM_NVM_CTRL_NVM_REMAIN            (1<<4)
#define BMA250_TRIM_NVM_CTRL_NVM_LOAD              (1<<3)
#define BMA250_TRIM_NVM_CTRL_NVM_RDY               (1<<2)
#define BMA250_TRIM_NVM_CTRL_NVM_PROG_TRIG         (1<<1)
#define BMA250_TRIM_NVM_CTRL_NVM_PROG_MODE         (1<<0)


uint8_t BMA250_WriteNVM(uint8_t addr,uint8_t data)
{
    //TODO: complete safe check when operate
    BMA250_WriteReg(addr,data);
    BMA250_WriteReg(BMA250_TRIM_NVM_CTRL_REG,BMA250_TRIM_NVM_CTRL_NVM_PROG_MODE);
    BMA250_WriteReg(BMA250_TRIM_NVM_CTRL_REG,
            BMA250_TRIM_NVM_CTRL_NVM_PROG_MODE | BMA250_TRIM_NVM_CTRL_NVM_PROG_TRIG );

    return 0;
}






uint8_t BMA250_SetFifoWatermark(uint8_t water_mark)
{
    BMA250_WriteReg(BMA250_FIFO_CONFIG_0_REG,water_mark);
    return 0;
}





/**
 *
 * @param mode
 * @param uint8_t
 * @return
 * @example BMA250_SetFifoConfig(BMA250_FIFO_STREAM_MODE,BMA250_FIFO_STORE_XYZ);
 */
uint8_t BMA250_SetFifoConfig(uint8_t mode,uint8_t select)
{
    uint8_t tmp = 0;

    tmp |= ((mode<<BMA250_FIFO_CONFIG_MODE_POS) & BMA250_FIFO_CONFIG_MODE_MASK);
    tmp |= ((select<<BMA250_FIFO_CONFIG_DATA_SELECT_POS) & BMA250_FIFO_CONFIG_DATA_SELECT_MASK);
    BMA250_WriteReg(BMA250_FIFO_CONFIG_1_REG,tmp);

    return 0;
}


/**
 *
 * @param isOnlyOneAxis
   BMA250_FIFO_STORE_XYZ
   BMA250_FIFO_STORE_X
   BMA250_FIFO_STORE_Y
   BMA250_FIFO_STORE_Z

 * @param acc
 * @param frame_num
 * @return
 * @example BMA250_GetFifoData(BMA250_FIFO_STORE_XYZ,acc,5);
 */
uint8_t BMA250_GetFifoData( uint8_t size,uint8_t * data )
{

            BMA250_ReadRegBurst(BMA250_FIFO_DATA_REG , size , data);

    return 0;
}



uint8_t BMA250_GetFifoStatus( bool *isOverrun ,uint8_t* count  )
{
    uint8_t data;

    BMA250_ReadReg(BMA250_FIFO_STATUS_REG,&data);
    if(data & 0x80 )
    {
        *isOverrun = true;
    }
    else
    {
        *isOverrun = false;
    }

    *count = data & 0x7f;

    return 0;
}









