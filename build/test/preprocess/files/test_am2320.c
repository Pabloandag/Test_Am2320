#include "build/temp/_test_am2320.c"
#include "build/test/mocks/mock_sapi_i2c.h"
#include "src/am2320.h"
#include "C:/Ruby27-x64/lib/ruby/gems/2.7.0/gems/ceedling-0.30.0/vendor/unity/src/unity.h"
uint8_t emptyBuffer [8]= {0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00};

AM2320_config_t myConfig;

float temp,hum;





void setUp(void){

    am2320_setConfig(&myConfig,1, AM2320_MODE_I2C, 100000);

    temp = 0;

    hum = 0;

}





void test_temperature(void){

    uint16_t temp = 0x0080;

    UnityAssertFloatsWithin((UNITY_FLOAT)((UNITY_FLOAT)((12.8)) * (UNITY_FLOAT)(0.00001f)), (UNITY_FLOAT)((UNITY_FLOAT)((12.8))), (UNITY_FLOAT)((UNITY_FLOAT)((am2320_formatTemp(temp)))), ((

   ((void *)0)

   )), (UNITY_UINT)((UNITY_UINT)(26)));

}





void test_negative_temperature(void){

    uint16_t temp = 0x8080;

    UnityAssertFloatsWithin((UNITY_FLOAT)((UNITY_FLOAT)((-12.8)) * (UNITY_FLOAT)(0.00001f)), (UNITY_FLOAT)((UNITY_FLOAT)((-12.8))), (UNITY_FLOAT)((UNITY_FLOAT)((am2320_formatTemp(temp)))), ((

   ((void *)0)

   )), (UNITY_UINT)((UNITY_UINT)(32)));

}





void test_checkCRC_true(void){

    uint8_t dataBuffer [(6 + 2)] = {0x03,0x04,0x03,0x39,0x01,0x15,0xE1,0xFE};

    do {if ((am2320_checkCRC(&dataBuffer[6],dataBuffer,6))) {} else {UnityFail( ((" Expected TRUE Was FALSE")), (UNITY_UINT)((UNITY_UINT)(38)));}} while(0);

}





void test_checkCRC_false(void){

    uint8_t dataBuffer [(6 + 2)] = {0x05,0x04,0x03,0x39,0x01,0x15,0xE1,0xFE};

    do {if (!(am2320_checkCRC(&dataBuffer[6],dataBuffer,6))) {} else {UnityFail( ((" Expected FALSE Was TRUE")), (UNITY_UINT)((UNITY_UINT)(44)));}} while(0);

}





void test_config(void){

    AM2320_config_t config;

    i2cMap_t I2C = 1;

    AM2320_mode_t mode = AM2320_MODE_I2C;

    uint32_t clockRateHz = 100000;



    am2320_setConfig(&config,I2C, mode, clockRateHz );

    UnityAssertEqualNumber((UNITY_INT)((I2C)), (UNITY_INT)((config.I2C)), (

   ((void *)0)

   ), (UNITY_UINT)(55), UNITY_DISPLAY_STYLE_INT);

    UnityAssertEqualNumber((UNITY_INT)((mode)), (UNITY_INT)((config.mode)), (

   ((void *)0)

   ), (UNITY_UINT)(56), UNITY_DISPLAY_STYLE_INT);

    UnityAssertEqualNumber((UNITY_INT)((clockRateHz)), (UNITY_INT)((config.clockRateHz)), (

   ((void *)0)

   ), (UNITY_UINT)(57), UNITY_DISPLAY_STYLE_INT);

}







void test_readTempHum_true(void){

    uint8_t expectedReceive [(6 + 2)]= {0x03,0x04,0x03,0x39,0x01,0x15,0xE1,0xFE};



    i2cWrite_CMockIgnoreAndReturn(65, 1);



    i2cRead_CMockExpectAndReturn(67, 1, 0X5C, emptyBuffer, (6 + 2), (!0), (!0));

    i2cRead_CMockReturnMemThruPtr_receiveDataBuffer(68, expectedReceive, (int)((6 + 2) * (int)sizeof(*expectedReceive)));



    do {if ((am2320_readTempHum(&myConfig, &temp, &hum))) {} else {UnityFail( ((" Expected TRUE Was FALSE")), (UNITY_UINT)((UNITY_UINT)(70)));}} while(0);

    UnityAssertFloatsWithin((UNITY_FLOAT)((UNITY_FLOAT)((82.5)) * (UNITY_FLOAT)(0.00001f)), (UNITY_FLOAT)((UNITY_FLOAT)((82.5))), (UNITY_FLOAT)((UNITY_FLOAT)((hum))), ((

   ((void *)0)

   )), (UNITY_UINT)((UNITY_UINT)(71)));

    UnityAssertFloatsWithin((UNITY_FLOAT)((UNITY_FLOAT)((27.7)) * (UNITY_FLOAT)(0.00001f)), (UNITY_FLOAT)((UNITY_FLOAT)((27.7))), (UNITY_FLOAT)((UNITY_FLOAT)((temp))), ((

   ((void *)0)

   )), (UNITY_UINT)((UNITY_UINT)(72)));

}



void test_readTempHum_false(void){

    uint8_t expectedReceive [(6 + 2)]= {0x04,0x04,0x03,0x39,0x01,0x15,0xE1,0xFE};



    i2cWrite_CMockIgnoreAndReturn(78, (!0));

    i2cRead_CMockExpectAndReturn(79, 1, 0X5C, emptyBuffer, (6 + 2), (!0), (!0));

    i2cRead_CMockReturnMemThruPtr_receiveDataBuffer(80, expectedReceive, (int)((6 + 2) * (int)sizeof(*expectedReceive)));



    do {if (!(am2320_readTempHum(&myConfig, &temp, &hum))) {} else {UnityFail( ((" Expected FALSE Was TRUE")), (UNITY_UINT)((UNITY_UINT)(82)));}} while(0);

}







void test_readTemp_true(void){

    uint8_t expectedReceive [(4 + 2)]= {0x03,0x02,0x01,0x15,0x61,0xFF};



    i2cWrite_CMockIgnoreAndReturn(90, (!0));



    i2cRead_CMockExpectAndReturn(92, 1, 0X5C, emptyBuffer, (4 + 2), (!0), (!0));

    i2cRead_CMockReturnMemThruPtr_receiveDataBuffer(93, expectedReceive, (int)((4 + 2) * (int)sizeof(*expectedReceive)));



    do {if ((am2320_readTemp(&myConfig, &temp))) {} else {UnityFail( ((" Expected TRUE Was FALSE")), (UNITY_UINT)((UNITY_UINT)(95)));}} while(0);

    UnityAssertFloatsWithin((UNITY_FLOAT)((UNITY_FLOAT)((27.7)) * (UNITY_FLOAT)(0.00001f)), (UNITY_FLOAT)((UNITY_FLOAT)((27.7))), (UNITY_FLOAT)((UNITY_FLOAT)((temp))), ((

   ((void *)0)

   )), (UNITY_UINT)((UNITY_UINT)(96)));

}



void test_readTemp_false(void){

    uint8_t expectedReceive [(4 + 2)]= {0x04,0x02,0x01,0x15,0x61,0xFF};



    i2cWrite_CMockIgnoreAndReturn(102, (!0));

    i2cRead_CMockExpectAndReturn(103, 1, 0X5C, emptyBuffer, (4 + 2), (!0), (!0));

    i2cRead_CMockReturnMemThruPtr_receiveDataBuffer(104, expectedReceive, (int)((4 + 2) * (int)sizeof(*expectedReceive)));



    do {if (!(am2320_readTemp(&myConfig, &temp))) {} else {UnityFail( ((" Expected FALSE Was TRUE")), (UNITY_UINT)((UNITY_UINT)(106)));}} while(0);

}







void test_readHum_true(void){

    uint8_t expectedReceive [(4 + 2)]= {0x03,0x02,0x03,0x39,0x61,0x42};



    i2cWrite_CMockIgnoreAndReturn(114, (!0));



    i2cRead_CMockExpectAndReturn(116, 1, 0X5C, emptyBuffer, (4 + 2), (!0), (!0));

    i2cRead_CMockReturnMemThruPtr_receiveDataBuffer(117, expectedReceive, (int)((4 + 2) * (int)sizeof(*expectedReceive)));



    do {if ((am2320_readHum(&myConfig, &hum))) {} else {UnityFail( ((" Expected TRUE Was FALSE")), (UNITY_UINT)((UNITY_UINT)(119)));}} while(0);

    UnityAssertFloatsWithin((UNITY_FLOAT)((UNITY_FLOAT)((82.5)) * (UNITY_FLOAT)(0.00001f)), (UNITY_FLOAT)((UNITY_FLOAT)((82.5))), (UNITY_FLOAT)((UNITY_FLOAT)((hum))), ((

   ((void *)0)

   )), (UNITY_UINT)((UNITY_UINT)(120)));

}



void test_readHum_false(void){

    uint8_t expectedReceive [(4 + 2)]= {0x04,0x02,0x01,0x15,0x61,0xFF};



    i2cWrite_CMockIgnoreAndReturn(126, (!0));

    i2cRead_CMockExpectAndReturn(127, 1, 0X5C, emptyBuffer, (4 + 2), (!0), (!0));

    i2cRead_CMockReturnMemThruPtr_receiveDataBuffer(128, expectedReceive, (int)((4 + 2) * (int)sizeof(*expectedReceive)));



    do {if (!(am2320_readHum(&myConfig, &hum))) {} else {UnityFail( ((" Expected FALSE Was TRUE")), (UNITY_UINT)((UNITY_UINT)(130)));}} while(0);

}
