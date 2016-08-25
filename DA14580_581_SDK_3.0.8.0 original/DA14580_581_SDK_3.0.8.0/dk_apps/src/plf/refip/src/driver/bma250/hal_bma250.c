/*
 * hal_bma250.c
 *
 *  Created on: 2014-12-25
 *      Author: Jason.Lau
 */

#include <bma250_driver.h>
#include <hal_bma250.h>
#include "gpio.h"

#include "spi.h"





/**
 * macro for DA14580DEVKT-S BMA250
 */
#define SPI_BMA250_SCLK_PORT           GPIO_PORT_1
#define SPI_BMA250_SCLK_PIN            GPIO_PIN_1

#define SPI_BMA250_MOSI_PORT           GPIO_PORT_0
#define SPI_BMA250_MOSI_PIN            GPIO_PIN_3

#define SPI_BMA250_MISO_PORT           GPIO_PORT_1
#define SPI_BMA250_MISO_PIN            GPIO_PIN_5

#define SPI_BMA250_CS_PORT             GPIO_PORT_0
#define SPI_BMA250_CS_PIN              GPIO_PIN_6



#define SPI_BMA250_INT1_PORT           GPIO_PORT_0
#define SPI_BMA250_INT1_PIN            GPIO_PIN_5

//#define SPI_BMA250_INT2_PORT           GPIO_PORT_0
//#define SPI_BMA250_INT2_PIN            GPIO_PIN_5


extern int wakeupflag;
#if 0
bool hal_bma250_get_int1_status(void)
{
    return GPIO_GetPinStatus( SPI_BMA250_INT1_PORT, SPI_BMA250_INT1_PIN );
}

bool hal_bma250_get_int2_status(void)
{
    return GPIO_GetPinStatus( SPI_BMA250_INT2_PORT, SPI_BMA250_INT2_PIN );
}

#endif

int hal_bma250_init()
{
    RESERVE_GPIO( DUMMY ,  SPI_BMA250_SCLK_PORT, SPI_BMA250_SCLK_PIN, PID_SPI_CLK);
    RESERVE_GPIO( DUMMY ,  SPI_BMA250_MOSI_PORT, SPI_BMA250_MOSI_PIN, PID_SPI_DO );
    RESERVE_GPIO( DUMMY ,  SPI_BMA250_MISO_PORT, SPI_BMA250_MISO_PIN, PID_SPI_DI );
    RESERVE_GPIO( DUMMY ,  SPI_BMA250_CS_PORT  , SPI_BMA250_CS_PIN  , PID_GPIO   );
    RESERVE_GPIO( DUMMY ,  SPI_BMA250_INT1_PORT, SPI_BMA250_INT1_PIN, PID_GPIO   );
//    RESERVE_GPIO( DUMMY ,  SPI_BMA250_INT2_PORT, SPI_BMA250_INT2_PIN, PID_GPIO   );

    // IO configuration
    GPIO_ConfigurePin( SPI_BMA250_SCLK_PORT, SPI_BMA250_SCLK_PIN, OUTPUT, PID_SPI_CLK ,false );
    GPIO_ConfigurePin( SPI_BMA250_MOSI_PORT, SPI_BMA250_MOSI_PIN, OUTPUT, PID_SPI_DO  ,false );
    GPIO_ConfigurePin( SPI_BMA250_MISO_PORT, SPI_BMA250_MISO_PIN, INPUT , PID_SPI_DI  ,false );
    GPIO_ConfigurePin( SPI_BMA250_CS_PORT  , SPI_BMA250_CS_PIN,   OUTPUT, PID_GPIO    ,true  );

//    GPIO_ConfigurePin( SPI_BMA250_INT1_PORT, SPI_BMA250_INT1_PIN, INPUT , PID_GPIO    ,false );
//    GPIO_ConfigurePin( SPI_BMA250_INT2_PORT, SPI_BMA250_INT2_PIN, INPUT , PID_GPIO    ,false );

    SPI_Pad_t adxl_spi_cs_pad = {
            SPI_BMA250_CS_PORT,
            SPI_BMA250_CS_PIN
    };

    SetBits16(CLK_PER_REG, SPI_ENABLE, 1);


    spi_init(&adxl_spi_cs_pad,
            SPI_MODE_8BIT,
            SPI_ROLE_MASTER,
            SPI_CLK_IDLE_POL_LOW,
            SPI_PHA_MODE_0,
            SPI_MINT_DISABLE,       // Disable SPI interrupt (SPI_INT_BIT) to ICU.
            SPI_XTAL_DIV_8);
    spi_cs_high ();


    uint8_t id;

    BMA250_GetChipID(  &id);
    BMA250_SoftwareReset();
		delay(500);
		
    BMA250_SetRange(BMA250_PMU_RANGE_8G  );
    BMA250_SetFilterBandwidth( BMA250_PMU_BW_62_50HZ );
		

//    BMA250_SetPowerMode(   BMA250_POWER_MODE_NORMAL);//设置bma250的工作模式

		
		if(wakeupflag==1)
		{
			BMA250_SetPowerMode(   BMA250_POWER_MODE_NORMAL);//设置bma250的工作模式
		
		}else
		{
			BMA250_SetPowerMode(   BMA250_POWER_MODE_DEEP_SUSPEND);
		}
		

    BMA250_SetIntEnable( BMA250_INT_EN_S_TAP_EN |BMA250_INT_EN_D_TAP_EN);
		BMA250_SetIntPinMap(BMA250_INT_MAP_INT1_S_TAP|BMA250_INT_MAP_INT1_D_TAP,BMA250_INT_MAP_INT2_D_TAP);

		
    BMA250_WriteReg(  BMA250_INT_RST_LATCH_REG, 0 );
    BMA250_ReadReg(  BMA250_ACCD_HBW_REG, &id  );


    BMA250_SetFifoConfig(BMA250_FIFO_STREAM_MODE,BMA250_FIFO_STORE_XYZ);
    BMA250_SetFifoWatermark(16);
		

    return id;
}





/**
 * @brief  Generic Reading function. It must be fullfilled with either
 *         I2C or SPI reading functions
 * @param  Reg       Register Address
 * @param  Data      Data Read
 * @return Reserve
 */
uint8_t BMA250_ReadReg(uint8_t Reg, uint8_t* Data)
{
    spi_cs_low();

    spi_access(Reg | (0x01<<7));
    *Data = spi_access(0x00);

    spi_cs_high();

    return 0;
}

/**
 * @brief  Generic Writing function. It must be fullfilled with either
*          I2C or SPI writing function
 * @param  Reg       Register Address
 * @param  Data      Data to be written
 * @return Reserve
 */
uint8_t BMA250_WriteReg(uint8_t WriteAddr, uint8_t Data)
{
    spi_cs_low();

    spi_access( WriteAddr & (~(0x01<<7)) );

    spi_access( Data);

    spi_cs_high();

    return 0;
}




/**
 * @brief  Generic Reading function. It must be fullfilled with either
 *         I2C or SPI reading functions
 * @param  Reg       Register Address
 * @param  size
 * @param  Data      Data Read
 * @return Reserve
 */
uint8_t BMA250_ReadRegBurst(uint8_t Reg, uint16_t size ,uint8_t* Data)
{
    uint16_t i;

    spi_cs_low();

    spi_access(Reg | (0x01<<7));

    for(i=0;i<size;i++)
    {
        *Data++ = spi_access(0x00);
    }

    spi_cs_high();

    return 0;
}




















