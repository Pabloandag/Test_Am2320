#include "src/sapi_peripheral_map.h"
#include "src/sapi_datatypes.h"
bool_t i2cInit( i2cMap_t i2cNumber, uint32_t clockRateHz );



bool_t i2cRead( i2cMap_t i2cNumber,

                uint8_t i2cSlaveAddress,

                uint8_t* receiveDataBuffer,

                uint16_t receiveDataBufferSize,

                bool_t sendReadStop );



bool_t i2cWrite( i2cMap_t i2cNumber,

                 uint8_t i2cSlaveAddress,

                 uint8_t* transmitDataBuffer,

                 uint16_t transmitDataBufferSize,

                 bool_t sendWriteStop );



bool_t i2cWriteRead( i2cMap_t i2cNumber,

                     uint8_t i2cSlaveAddress,

                     uint8_t* dataToReadBuffer,

                     uint16_t dataToReadBufferSize,

                     bool_t sendWriteStop,

                     uint8_t* receiveDataBuffer,

                     uint16_t receiveDataBufferSize,

                     bool_t sendReadStop );
