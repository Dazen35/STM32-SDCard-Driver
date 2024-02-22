/* Includes ------------------------------------------------------------------*/
#include "sd_device.h"
#include <string.h>


__IO uint8_t RxCplt = 0;
__IO uint8_t TxCplt = 0;

/******** SD Transmission Buffer definition *******/
#if defined ( __ICCARM__ )
#pragma location = 0x24100000
#elif defined ( __CC_ARM )
__attribute__((section (".RAM_D1")))
#elif defined ( __GNUC__ )
__attribute__((section (".RAM_D1")))
#endif
uint8_t aTxBuffer[512];

/******** SD Receive Buffer definition *******/
#if defined ( __ICCARM__ )
#pragma location = 0x24100200
#elif defined ( __CC_ARM )
__attribute__((section (".RAM_D1")))
#elif defined ( __GNUC__ )
__attribute__((section (".RAM_D1")))
#endif
uint8_t aRxBuffer[512];

/**
  * @brief  Initializes the SD card device.
  * @param  Instance      SD Instance
  * @retval BSP status
  */
int32_t BSP_SD_Init(uint32_t Instance)
{
  int32_t retval = BSP_ERROR_NONE;
	
  hsd1.Instance = SDMMC1;
  hsd1.Init.ClockEdge = SDMMC_CLOCK_EDGE_RISING;
  hsd1.Init.ClockPowerSave = SDMMC_CLOCK_POWER_SAVE_DISABLE;
  hsd1.Init.BusWide = SDMMC_BUS_WIDE_4B;
  hsd1.Init.HardwareFlowControl = SDMMC_HARDWARE_FLOW_CONTROL_DISABLE;
  hsd1.Init.ClockDiv = 2;				// 80MHz / (2 * 2) = 20MHz
  hsd1.Init.TranceiverPresent = SDMMC_TRANSCEIVER_NOT_PRESENT;
  if (HAL_SD_Init(&hsd1) != HAL_OK)
  {
    Error_Handler();
  }
	
  return retval;
}

/**
  * @brief  Gets the current SD card data status.
  * @param  Instance  SD Instance
  * @retval Data transfer state.
  *          This value can be one of the following values:
  *            @arg  BSP_ERROR_NONE: No data transfer is acting
  *            @arg  BSP_ERROR_BUSY: Data transfer is acting
  */
int32_t BSP_SD_GetCardState(uint32_t Instance)
{
  int32_t retval = HAL_SD_GetCardState(&hsd1);
	
  if (retval == HAL_SD_CARD_TRANSFER)
  {
    retval = BSP_ERROR_NONE;
  }
  else
  {
    retval = BSP_ERROR_BUSY;
  }

  return retval;
}


/**
  * @brief  Get SD information about specific SD card.
  * @param  Instance  SD Instance
  * @param  CardInfo  Pointer to HAL_SD_CardInfoTypedef structure
  * @retval BSP status
  */
int32_t BSP_SD_GetCardInfo(uint32_t Instance, SD_CardInfoTypeDef *CardInfo)
{
  int32_t ret = BSP_ERROR_NONE;

  if (HAL_SD_GetCardInfo(&hsd1, CardInfo) != HAL_OK)
  {
    ret = BSP_ERROR_BUSY;
  }
	
  /* Return BSP status */
  return ret;
}


/**
  * @brief  Reads block(s) from a specified address in an SD card, in polling mode.
  * @param  Instance   SD Instance
  * @param  pData      Pointer to the buffer that will contain the data to transmit
  * @param  BlockIdx   Block index from where data is to be read
  * @param  BlocksNbr  Number of SD blocks to read
  * @retval BSP status
  */
int32_t BSP_SD_ReadBlocks(uint32_t Instance, uint32_t *pData, uint32_t BlockIdx, uint32_t BlocksNbr)
{
  int32_t ret = BSP_ERROR_NONE;
  uint32_t timeout = 100 * BlocksNbr;

  if (HAL_SD_ReadBlocks(&hsd1, (uint8_t *)pData, BlockIdx, BlocksNbr, timeout) != HAL_OK)
  {
    ret = BSP_ERROR_BUSY;
  }

  /* Return BSP status   */
  return ret;
}


/**
  * @brief  Writes block(s) to a specified address in an SD card, in polling mode.
  * @param  Instance   SD Instance
  * @param  pData      Pointer to the buffer that will contain the data to transmit
  * @param  BlockIdx   Block index from where data is to be written
  * @param  BlocksNbr  Number of SD blocks to write
  * @retval BSP status
  */
int32_t BSP_SD_WriteBlocks(uint32_t Instance, uint32_t *pData, uint32_t BlockIdx, uint32_t BlocksNbr)
{
  int32_t ret = BSP_ERROR_NONE;
  uint32_t timeout = 100 * BlocksNbr;

  if (HAL_SD_WriteBlocks(&hsd1, (uint8_t *)pData, BlockIdx, BlocksNbr, timeout) != HAL_OK)
  {
    ret = BSP_ERROR_BUSY;
  }

  /* Return BSP status   */
  return ret;
}


/**
  * @brief  Reads block(s) to a specified address in an SD card, in DMA mode.
  * @param  Instance   SD Instance
  * @param  pData      Pointer to the buffer that will contain the data to transmit
  * @param  BlockIdx   Block index from where data is to be written
  * @param  BlocksNbr  Number of SD blocks to write
  * @retval BSP status
  */
int32_t BSP_SD_ReadBlocks_DMA(uint32_t Instance, uint32_t *pData, uint32_t BlockIdx, uint32_t BlocksNbr)
{
  int32_t retval = BSP_ERROR_NONE;

  if (HAL_SD_ReadBlocks_DMA(&hsd1, (uint8_t *)aRxBuffer, BlockIdx, BlocksNbr) != HAL_OK)
  {
    retval = BSP_ERROR_BUSY;
  }
	
  while (RxCplt != 1){}
  RxCplt = 0;
	
  memcpy((uint8_t *)pData, (uint8_t *)aRxBuffer, BlocksNbr * 512);

  return retval;
}


/**
  * @brief  Writes block(s) to a specified address in an SD card, in DMA mode.
  * @param  Instance   SD Instance
  * @param  pData      Pointer to the buffer that will contain the data to transmit
  * @param  BlockIdx   Block index from where data is to be written
  * @param  BlocksNbr  Number of SD blocks to write
  * @retval BSP status
  */
int32_t BSP_SD_WriteBlocks_DMA(uint32_t Instance, uint32_t *pData, uint32_t BlockIdx, uint32_t BlocksNbr)
{
  int32_t retval = BSP_ERROR_NONE;
	
  memcpy((uint8_t *)aTxBuffer, (uint8_t *)pData, BlocksNbr * 512);

  if (HAL_SD_WriteBlocks_DMA(&hsd1, (uint8_t *)aTxBuffer, BlockIdx, BlocksNbr) != HAL_OK)
  {
    retval = BSP_ERROR_BUSY;
  }
	
  while (TxCplt != 1){}
  TxCplt = 0;

  return retval;
}


/**
  * @brief Rx Transfer completed callbacks
  * @param hsd: SD handle
  * @retval None
  */
void HAL_SD_RxCpltCallback(SD_HandleTypeDef *hsd)
{
  SCB_InvalidateDCache_by_Addr((uint32_t*)aRxBuffer, 512);
  RxCplt = 1;
}


/**
  * @brief Tx Transfer completed callbacks
  * @param hsd: SD handle
  * @retval None
  */
void HAL_SD_TxCpltCallback(SD_HandleTypeDef *hsd)
{
  TxCplt = 1;
}

