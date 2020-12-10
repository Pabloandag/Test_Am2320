/*
 * am2320.c
 *
 *  Created on: 10 oct. 2020
 *      Author: pablo
 */

#include "am2320.h"

uint8_t wakeByte = 0x00;

bool_t am2320_getDataString(AM2320_config_t* config, AM2320_data_t datatype,  char *buf){
	float temp,hum;

	switch(datatype){
	case AM2320_DATA_T:
		if(am2320_readTemp(config, &temp)){
			snprintf(buf,AM2320_DATA_SIZE,"T: %.1f C\r\n",temp);
			return TRUE;
		}
		break;
	case AM2320_DATA_H:
		if(am2320_readHum(config, &hum)){
			snprintf(buf,AM2320_DATA_SIZE,"H: %.1f \%%\r\n",hum);
			return TRUE;
		}
		break;
	case AM2320_DATA_ALL:
		if(am2320_readTempHum(config,&temp,&hum)){
			snprintf(buf,AM2320_DATA_SIZE,"T: %.1f C H: %.1f\%%\r\n",temp,hum);
			return TRUE;
		}
		break;
	}

	return FALSE;
}

bool_t am2320_readTemp(AM2320_config_t* config,float *temp){

	uint8_t transmitDataBuffer[3]={AM2320_CMD_READREG,AM2320_REG_TEMP_H,AM2320_REG_TEMP_SIZE};
	uint8_t receiveDataBuffer[6]={0x00,0x00,0x00,0x00,0x00,0x00};
	uint16_t crc,holder;

	am2320_wakeUp(config);

	i2cWrite( config->I2C,
			AM2320_ADD,
			transmitDataBuffer,
			3,
			TRUE);

	i2cRead( config->I2C,
			AM2320_ADD,
			receiveDataBuffer,
			6,
			TRUE);

	if (am2320_checkCRC(&receiveDataBuffer[4],receiveDataBuffer,4)){
		holder = (receiveDataBuffer[2] << 8) | receiveDataBuffer[3];
		*temp = am2320_formatTemp(holder);
		return TRUE;
	}
	else{
		return FALSE;
	}

}

bool_t am2320_readHum(AM2320_config_t* config, float *hum){
	uint8_t transmitDataBuffer[3]={AM2320_CMD_READREG,AM2320_REG_HUM_H,AM2320_REG_HUM_SIZE};
	uint8_t receiveDataBuffer[6]={0x00,0x00,0x00,0x00,0x00,0x00};
	uint16_t holder;

	am2320_wakeUp(config);

	i2cWrite( config->I2C,
			AM2320_ADD,
			transmitDataBuffer,
			3,
			TRUE);

	i2cRead( config->I2C,
			AM2320_ADD,
			receiveDataBuffer,
			6,
			TRUE);

	if (am2320_checkCRC(&receiveDataBuffer[4],receiveDataBuffer,4)){
		holder = (receiveDataBuffer[2] << 8) | receiveDataBuffer[3];
		*hum = am2320_formatHum(holder);
		return TRUE;
	}
	
	return FALSE;
}

bool_t am2320_readTempHum(AM2320_config_t* config, float* temp, float *hum){
	uint8_t transmitDataBuffer[3]={AM2320_CMD_READREG,AM2320_REG_HUM_H,AM2320_REG_HUM_SIZE+AM2320_REG_TEMP_SIZE};
	uint8_t receiveDataBuffer[8]={0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00};
	uint16_t crc,holder;

	am2320_wakeUp(config);
	i2cWrite( config->I2C,
	          AM2320_ADD,
			  transmitDataBuffer,
	          3,
	          TRUE);

	i2cRead( config->I2C,
			 AM2320_ADD,
	         receiveDataBuffer,
	         8,
	         TRUE);

	if (am2320_checkCRC(&receiveDataBuffer[6],receiveDataBuffer,6)){
		holder  = (receiveDataBuffer[2] << 8) | receiveDataBuffer[3];
		*hum = am2320_formatHum(holder);
		holder = (receiveDataBuffer[4] << 8) | receiveDataBuffer[5];
		*temp = am2320_formatTemp(holder);
		return TRUE;
	}
	else{
		return FALSE;
	}
}

bool_t am2320_checkCRC(uint8_t* crcPtr,uint8_t * dataBuffer,uint8_t dataLength){
	uint16_t crc;

	crc = (crcPtr[1] << 8) | (crcPtr[0]);
	if(crc == am2320_CRC(dataBuffer,dataLength)){
		return TRUE;
	}else{
		return FALSE;
	}
}


void am2320_setConfig(AM2320_config_t* config,i2cMap_t I2C, AM2320_mode_t mode, uint32_t clockRateHz ){
	config->I2C=I2C;
	config->mode=mode;
	if(clockRateHz > AM2320_CLKRATE_MAX){
	config->clockRateHz=AM2320_CLKRATE_100KHZ;
	}else{
		config->clockRateHz = clockRateHz;
	}
}

void am2320_init(AM2320_config_t* config){
	i2cInit( config->I2C, config->clockRateHz );
}

void am2320_wakeUp(AM2320_config_t* config){
	i2cWrite(config->I2C,AM2320_ADD,&wakeByte,1,TRUE);
}

float am2320_formatTemp(uint16_t temp){
	float number;

	//Si el primer byte es 1, la temperatura es negativa.
	if ((temp & 0x8000) != 0x00){
		number = - ((float)(temp & 0x7FFF))/10;
	} else
	{
		number = ((float) temp) /10;
	}

	return number;
}

float am2320_formatHum(uint16_t hum){
	return ((float) hum )/10;
}

uint16_t am2320_CRC(uint8_t *ptr,uint8_t len){
	uint16_t crc = 0xFFFF;
	uint8_t i;

	while(len--){
		crc ^=*ptr++;
		for(i=0;i<8;i++){
			if(crc & 0x01)
			{
				crc >>=1;
				crc^=0xA001;
			}
			else{
				crc >>=1;
			}
		}
	}
	return crc;
}
