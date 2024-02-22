#ifndef __SPI_SOCKET_H__
#define __SPI_SOCKET_H__	 

#include "stm32_spi.h"


#define TFCARD_SPI_HANDLE       hspi2
#define TFCARD_SPI_PERIPHERAL   SPI2
#define TFCARD_SPI_TIMEOUT      10        // 单位ms

#define TFCARD_SPI_CS_PIN   SPI2_CS_Pin          // SPI_CS引脚编号
#define	TFCARD_SPI_CS_PORT  SPI2_CS_GPIO_Port    // SPI_CS引脚端口

#define TFCARD_SPI_CS_LOW()   HAL_GPIO_WritePin(TFCARD_SPI_CS_PORT, TFCARD_SPI_CS_PIN, GPIO_PIN_RESET)
#define TFCARD_SPI_CS_HIGH()  HAL_GPIO_WritePin(TFCARD_SPI_CS_PORT, TFCARD_SPI_CS_PIN, GPIO_PIN_SET)

#define TFCARD_SPI_Init                STM32_SPI_Init
#define TFCARD_SPI_TransferData	       STM32_SPI_TransferData
#define TFCARD_SPI_TransferData_DMA    STM32_SPI_TransferData_DMA
#define TFCARD_SPI_SetSpeed            STM32_SPI_SetSpeed

#define USE_SPI_DMA_READ_SECTOR    // 定义SPI使用DMA进行数据读扇区
#define USE_SPI_DMA_WRITE_SECTOR   // 定义SPI使用DMA进行数据写扇区
#define USE_SPI_DMA_SEND_CMD       // 定义SPI使用DMA进行数据发送CMD


uint8_t SD_ReadWriteByte(uint8_t Byte);
int8_t SD_ReadBuffer_DMA(uint8_t *RxData, uint16_t Size);
int8_t SD_WriteBuffer_DMA(uint8_t *TxData, uint16_t Size);
int8_t SD_SPI_SetSpeed(uint8_t SPI_BaudRatePrescaler);
int8_t SD_SPI_Init(void);

#endif


