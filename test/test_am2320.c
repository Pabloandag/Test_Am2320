#include "unity.h"
#include "am2320.h"
#include "mock_sapi_i2c.h"

#define CRC_LENGTH  2
#define FULL_DATA_LENGTH  6
#define FULL_DATA_CRC_LENGTH (FULL_DATA_LENGTH + CRC_LENGTH)
#define ONE_DATA_LENGTH 4
#define ONE_DATA_CRC_LENGTH (ONE_DATA_LENGTH + CRC_LENGTH)
#define TEST_I2C 1

uint8_t emptyBuffer [8]= {0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00};
AM2320_config_t myConfig;
float temp,hum;

// Condiciones iniciales de test
void setUp(void){
    am2320_setConfig(&myConfig,TEST_I2C, AM2320_MODE_I2C, AM2320_CLKRATE_100KHZ);
    temp = 0;
    hum = 0;
}

// Test correcta devolucion de temperatura positiva
void test_temperature(void){
    uint16_t temp = 0x0080;
    TEST_ASSERT_EQUAL_FLOAT(12.8,am2320_formatTemp(temp));
}

// Test correcta devolución de temperatura negativa
void test_negative_temperature(void){
    uint16_t temp = 0x8080;
    TEST_ASSERT_EQUAL_FLOAT(-12.8,am2320_formatTemp(temp));
}

// Test checkCRC con paquete con un CRC válido
void test_checkCRC_true(void){
    uint8_t dataBuffer [FULL_DATA_CRC_LENGTH] = {0x03,0x04,0x03,0x39,0x01,0x15,0xE1,0xFE};
    TEST_ASSERT_TRUE(am2320_checkCRC(&dataBuffer[FULL_DATA_LENGTH],dataBuffer,FULL_DATA_LENGTH));
}

// Test checkCRC con paquete con un CRC inválido
void test_checkCRC_false(void){
    uint8_t dataBuffer [FULL_DATA_CRC_LENGTH] = {0x05,0x04,0x03,0x39,0x01,0x15,0xE1,0xFE};
    TEST_ASSERT_FALSE(am2320_checkCRC(&dataBuffer[FULL_DATA_LENGTH],dataBuffer,FULL_DATA_LENGTH));
}

// Test configuración de un AM2320_config_t
void test_config(void){
    AM2320_config_t config;
    i2cMap_t I2C = TEST_I2C;
    AM2320_mode_t mode = AM2320_MODE_I2C;
    uint32_t clockRateHz = AM2320_CLKRATE_100KHZ;

    am2320_setConfig(&config,I2C, mode, clockRateHz );
    TEST_ASSERT_EQUAL(I2C,config.I2C);
    TEST_ASSERT_EQUAL(mode,config.mode);
    TEST_ASSERT_EQUAL(clockRateHz,config.clockRateHz);
}


// Tests de lecturas de humedad y temperatura juntas
void test_readTempHum_true(void){
    uint8_t expectedReceive [FULL_DATA_CRC_LENGTH]= {0x03,0x04,0x03,0x39,0x01,0x15,0xE1,0xFE};

    i2cWrite_IgnoreAndReturn(1);

    i2cRead_ExpectAndReturn(TEST_I2C, AM2320_ADD, emptyBuffer, FULL_DATA_CRC_LENGTH, TRUE, TRUE);
    i2cRead_ReturnArrayThruPtr_receiveDataBuffer(expectedReceive,FULL_DATA_CRC_LENGTH);

    TEST_ASSERT_TRUE(am2320_readTempHum(&myConfig, &temp, &hum));
    TEST_ASSERT_EQUAL_FLOAT(82.5,hum);
    TEST_ASSERT_EQUAL_FLOAT(27.7,temp);
}

void test_readTempHum_false(void){
    uint8_t expectedReceive [FULL_DATA_CRC_LENGTH]= {0x04,0x04,0x03,0x39,0x01,0x15,0xE1,0xFE};

    i2cWrite_IgnoreAndReturn(TRUE);
    i2cRead_ExpectAndReturn(TEST_I2C, AM2320_ADD, emptyBuffer, FULL_DATA_CRC_LENGTH, TRUE, TRUE);
    i2cRead_ReturnArrayThruPtr_receiveDataBuffer(expectedReceive,FULL_DATA_CRC_LENGTH);

    TEST_ASSERT_FALSE(am2320_readTempHum(&myConfig, &temp, &hum));
}


// Tests de lecturas de temperatura
void test_readTemp_true(void){
    uint8_t expectedReceive [ONE_DATA_CRC_LENGTH]= {0x03,0x02,0x01,0x15,0x61,0xFF};

    i2cWrite_IgnoreAndReturn(TRUE);

    i2cRead_ExpectAndReturn(TEST_I2C, AM2320_ADD, emptyBuffer, ONE_DATA_CRC_LENGTH, TRUE, TRUE);
    i2cRead_ReturnArrayThruPtr_receiveDataBuffer(expectedReceive,ONE_DATA_CRC_LENGTH);

    TEST_ASSERT_TRUE(am2320_readTemp(&myConfig, &temp));
    TEST_ASSERT_EQUAL_FLOAT(27.7,temp);
}

void test_readTemp_false(void){
    uint8_t expectedReceive [ONE_DATA_CRC_LENGTH]= {0x04,0x02,0x01,0x15,0x61,0xFF};

    i2cWrite_IgnoreAndReturn(TRUE);
    i2cRead_ExpectAndReturn(TEST_I2C, AM2320_ADD, emptyBuffer, ONE_DATA_CRC_LENGTH, TRUE, TRUE);
    i2cRead_ReturnArrayThruPtr_receiveDataBuffer(expectedReceive,ONE_DATA_CRC_LENGTH);

    TEST_ASSERT_FALSE(am2320_readTemp(&myConfig, &temp));
}


// Tests de lecturas de humedad
void test_readHum_true(void){
    uint8_t expectedReceive [ONE_DATA_CRC_LENGTH]= {0x03,0x02,0x03,0x39,0x61,0x42};

    i2cWrite_IgnoreAndReturn(TRUE);

    i2cRead_ExpectAndReturn(TEST_I2C, AM2320_ADD, emptyBuffer, ONE_DATA_CRC_LENGTH, TRUE, TRUE);
    i2cRead_ReturnArrayThruPtr_receiveDataBuffer(expectedReceive,ONE_DATA_CRC_LENGTH);

    TEST_ASSERT_TRUE(am2320_readHum(&myConfig, &hum));
    TEST_ASSERT_EQUAL_FLOAT(82.5,hum);
}

void test_readHum_false(void){
    uint8_t expectedReceive [ONE_DATA_CRC_LENGTH]= {0x04,0x02,0x01,0x15,0x61,0xFF};

    i2cWrite_IgnoreAndReturn(TRUE);
    i2cRead_ExpectAndReturn(TEST_I2C, AM2320_ADD, emptyBuffer, ONE_DATA_CRC_LENGTH, TRUE, TRUE);
    i2cRead_ReturnArrayThruPtr_receiveDataBuffer(expectedReceive,ONE_DATA_CRC_LENGTH);

    TEST_ASSERT_FALSE(am2320_readHum(&myConfig, &hum));
}