#ifndef __SD_DEVICE_H__
#define __SD_DEVICE_H__


/* SDMMC总线初始化由STM32CubeMX完成，使用时需包含头文件sdmmc.h */
#include "sdmmc.h"

#define  SD_CardInfoTypeDef  HAL_SD_CardInfoTypeDef

/* SD transfer state definition */
#define  SD_TRANSFER_OK       0U
#define  SD_TRANSFER_BUSY     1U

/* SD-detect signal */
#define  SD_PRESENT           1UL
#define  SD_NOT_PRESENT       0UL

/* BSP error codes */
#define  BSP_ERROR_NONE                    0
#define  BSP_ERROR_NO_INIT                -1
#define  BSP_ERROR_WRONG_PARAM            -2
#define  BSP_ERROR_BUSY                   -3
#define  BSP_ERROR_PERIPH_FAILURE         -4
#define  BSP_ERROR_COMPONENT_FAILURE      -5
#define  BSP_ERROR_UNKNOWN_FAILURE        -6
#define  BSP_ERROR_UNKNOWN_COMPONENT      -7
#define  BSP_ERROR_BUS_FAILURE            -8
#define  BSP_ERROR_CLOCK_FAILURE          -9
#define  BSP_ERROR_MSP_FAILURE            -10
#define  BSP_ERROR_FEATURE_NOT_SUPPORTED  -11

/* BSP BUS error codes */
#define  BSP_ERROR_BUS_TRANSACTION_FAILURE    -100
#define  BSP_ERROR_BUS_ARBITRATION_LOSS       -101
#define  BSP_ERROR_BUS_ACKNOWLEDGE_FAILURE    -102
#define  BSP_ERROR_BUS_PROTOCOL_FAILURE       -103

#define  BSP_ERROR_BUS_MODE_FAULT             -104
#define  BSP_ERROR_BUS_FRAME_ERROR            -105
#define  BSP_ERROR_BUS_CRC_ERROR              -106
#define  BSP_ERROR_BUS_DMA_FAILURE            -107


int32_t  BSP_SD_Init(uint32_t Instance);
int32_t  BSP_SD_GetCardState(uint32_t Instance);
int32_t  BSP_SD_GetCardInfo(uint32_t Instance, SD_CardInfoTypeDef *CardInfo);
int32_t  BSP_SD_WriteBlocks(uint32_t Instance, uint32_t *pData, uint32_t BlockIdx, uint32_t BlocksNbr);
int32_t  BSP_SD_ReadBlocks(uint32_t Instance, uint32_t *pData, uint32_t BlockIdx, uint32_t BlocksNbr);
int32_t  BSP_SD_WriteBlocks_DMA(uint32_t Instance, uint32_t *pData, uint32_t BlockIdx, uint32_t BlocksNbr);
int32_t  BSP_SD_ReadBlocks_DMA(uint32_t Instance, uint32_t *pData, uint32_t BlockIdx, uint32_t BlocksNbr);



#endif

