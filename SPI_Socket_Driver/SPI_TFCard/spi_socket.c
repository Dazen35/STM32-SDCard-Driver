#include "spi_socket.h"			



/**
  * @brief  通过SPI总线读写一个字节数据
  * @note   无
  * @param  txdata: 写入的数据
  * @retval 读取的数据
  */
uint8_t SD_ReadWriteByte(uint8_t Byte)
{
  uint8_t txdata, rxdata;
  txdata = Byte;
  TFCARD_SPI_TransferData(&TFCARD_SPI_HANDLE, &txdata, &rxdata, 1);       
  return rxdata;	
}


/**
  * @brief  通过SPI总线读数据
  * @note   无
  * @param  RxData: 读取的数据
  * @param  Size: 数据长度，不能超过512字节
  * @retval 结果 0-成功，其他-失败
  */
int8_t SD_ReadBuffer_DMA(uint8_t *RxData, uint16_t Size)
{
  uint8_t txdata[512];		// 发送缓存

  memset(txdata, 0xFF, 512);

  TFCARD_SPI_TransferData_DMA(&TFCARD_SPI_HANDLE, txdata, RxData, Size); 

  return 0;
}


/**
  * @brief  通过SPI总线写数据
  * @note   无
  * @param  TxData: 写入的数据
  * @param  Size: 数据长度，不能超过512字节
  * @retval 结果 0-成功，其他-失败
  */
int8_t SD_WriteBuffer_DMA(uint8_t *TxData, uint16_t Size)
{
  uint8_t rxdata[512];		// 接收缓存
	
  TFCARD_SPI_TransferData_DMA(&TFCARD_SPI_HANDLE, TxData, rxdata, Size); 

  return 0;
}


/**
  * @brief  设置SPI总线速度
  * @note   移植时用户需要修改的接口函数
  * @param  SPI_BaudRate_Prescaler: SPI总线分频系数, SPI_BAUDRATEPRESCALER_2 ~ SPI_BAUDRATEPRESCALER_256
  * @retval 无
  */
int8_t SD_SPI_SetSpeed(uint8_t SPI_BaudRatePrescaler)
{
  TFCARD_SPI_SetSpeed(&TFCARD_SPI_HANDLE, SPI_BaudRatePrescaler);
	
  return 0;
}


/**
  * @brief  初始化SPI总线
  * @note   移植时用户需要修改的接口函数
  * @param  无
  * @retval 0
  */
int8_t SD_SPI_Init(void)
{
  TFCARD_SPI_Init(TFCARD_SPI_PERIPHERAL, &TFCARD_SPI_HANDLE, SPI_MODE_0);
  
  return 0;
}






