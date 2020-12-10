/*
 * am2320.h
 *
 *  Created on: 10 oct. 2020
 *      Author: pablo
 */

#ifndef MYPROGRAMS_AM2320_INC_AM2320_H_
#define MYPROGRAMS_AM2320_INC_AM2320_H_

#include "sapi_i2c.h"
#include <stdio.h>

#define AM2320_ADD              0X5C
#define AM2320_CMD_READREG		0x03
#define AM2320_REG_HUM_H        0x00
#define AM2320_REG_HUM_SIZE     0x02
#define AM2320_REG_TEMP_H       0x02
#define AM2320_REG_TEMP_SIZE    0x02

#define AM2320_CLKRATE_MAX	  100000
#define AM2320_CLKRATE_100KHZ 100000
#define AM2320_CLKRATE_50KHZ   50000
#define AM2320_CLKRATE_20KHZ   20000
#define AM2320_CLKRATE_10KHZ   10000
#define AM2320_CLKRATE_1KHZ     1000

#define AM2320_DATA_SIZE	20

typedef enum{
	AM2320_DATA_T,
	AM2320_DATA_H,
	AM2320_DATA_ALL
}AM2320_data_t;

typedef enum{
	AM2320_MODE_I2C,
	AM2320_MODE_ONEWIRE
}AM2320_mode_t;

typedef struct{
	i2cMap_t I2C;
	AM2320_mode_t mode;
	uint32_t clockRateHz;
}AM2320_config_t;


void am2320_init(AM2320_config_t* config);
void am2320_setConfig(AM2320_config_t* config,i2cMap_t I2C, AM2320_mode_t mode, uint32_t clockRateHz );

bool_t am2320_readTemp(AM2320_config_t* config, float* temp);
bool_t am2320_readHum(AM2320_config_t* config, float* hum);
bool_t am2320_readTempHum(AM2320_config_t* config, float* temp, float* hum);

float am2320_formatTemp(uint16_t temp);
float am2320_formatHum(uint16_t hum);
bool_t am2320_getDataString(AM2320_config_t* config, AM2320_data_t datatype,  char *buf);

void am2320_wakeUp(AM2320_config_t* config);
uint16_t am2320_CRC(uint8_t *ptr,uint8_t len);
bool_t am2320_checkCRC(uint8_t* crcPtr,uint8_t * dataBuffer,uint8_t dataLength);
#endif /* MYPROGRAMS_AM2320_INC_AM2320_H_ */
