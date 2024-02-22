#ifndef __SPI_TFCARD_H__
#define __SPI_TFCARD_H__	 

#include "stm32_spi.h"
#include "bsp_sdcard.h"


#define TFCARD_SPI_CS_PIN  SPI2_CS_Pin
#define	TFCARD_SPI_CS_PORT SPI2_CS_GPIO_Port

#define TFCARD_SPI_CS_SELECT()   HAL_GPIO_WritePin(TFCARD_SPI_CS_PORT, TFCARD_SPI_CS_PIN, GPIO_PIN_RESET)
#define TFCARD_SPI_CS_RELEASE()  HAL_GPIO_WritePin(TFCARD_SPI_CS_PORT, TFCARD_SPI_CS_PIN, GPIO_PIN_SET)


// TF卡类型定义  
#define  TF_TYPE_ERROR     0x00
#define  TF_TYPE_MMC       0x01
#define  TF_TYPE_SDV1      0x02
#define  TF_TYPE_SDV2      0x04
#define  TF_TYPE_SDHC      0x06
#define  TF_TYPE_SDXC      0x08

// TF卡指令表  	   
#define  TF_CMD0    0       // 命令0 ，SD卡进入空闲状态
#define  TF_CMD1    1       // 命令1 ，SD卡进入工作状态
#define  TF_CMD6    6       // 命令6 ，SD卡进入高速模式50MHz
#define  TF_CMD8    8       // 命令8 ，SEND_IF_COND
#define  TF_CMD9    9       // 命令9 ，读CSD数据
#define  TF_CMD10   10      // 命令10，读CID数据
#define  TF_CMD12   12      // 命令12，停止数据传输
#define  TF_CMD13   13      // 命令13，读状态
#define  TF_CMD16   16      // 命令16，设置块大小 应返回0x00
#define  TF_CMD17   17      // 命令17，读单个扇区
#define  TF_CMD18   18      // 命令18，读多个扇区
#define  TF_CMD23   23      // 命令23，设置多sector写入前预先擦除N个block
#define  TF_CMD24   24      // 命令24，写单个扇区
#define  TF_CMD25   25      // 命令25，写多个扇区
#define  TF_CMD32   32      // 命令32，设置要擦除的起始地址
#define  TF_CMD33   33      // 命令33，设置要擦除的结束地址
#define  TF_CMD38   38      // 命令38，擦除指定区间的内容
#define  TF_CMD41   41      // 命令41，应返回0x00
#define  TF_CMD55   55      // 命令55，应返回0x01
#define  TF_CMD58   58      // 命令58，读OCR信息
#define  TF_CMD59   59      // 命令59，使能/禁止CRC，应返回0x00

// 数据写入回应字意义
#define MSD_DATA_OK                0x05
#define MSD_DATA_CRC_ERROR         0x0B
#define MSD_DATA_WRITE_ERROR       0x0D
#define MSD_DATA_OTHER_ERROR       0xFF

// TF卡回应标记字
#define MSD_RESPONSE_NO_ERROR      0x00
#define MSD_IN_IDLE_STATE          0x01
#define MSD_ERASE_RESET            0x02
#define MSD_ILLEGAL_COMMAND        0x04
#define MSD_COM_CRC_ERROR          0x08
#define MSD_ERASE_SEQUENCE_ERROR   0x10
#define MSD_ADDRESS_ERROR          0x20
#define MSD_PARAMETER_ERROR        0x40
#define MSD_RESPONSE_FAILURE       0xFF


// 数据写入回应字意义
#define MSD_DATA_OK                0x05
#define MSD_DATA_CRC_ERROR         0x0B
#define MSD_DATA_WRITE_ERROR       0x0D
#define MSD_DATA_OTHER_ERROR       0xFF

// TF卡回应标记字
#define MSD_RESPONSE_NO_ERROR      0x00
#define MSD_IN_IDLE_STATE          0x01
#define MSD_ERASE_RESET            0x02
#define MSD_ILLEGAL_COMMAND        0x04
#define MSD_COM_CRC_ERROR          0x08
#define MSD_ERASE_SEQUENCE_ERROR   0x10
#define MSD_ADDRESS_ERROR          0x20
#define MSD_PARAMETER_ERROR        0x40
#define MSD_RESPONSE_FAILURE       0xFF

	
#define SD_CardInfo_t BSP_SD_CardInfo_t


/* SD卡API */
int32_t  SD_Release(void);
int32_t  SD_Select(void);
int32_t  SD_WaitReady(void);
int32_t  SD_GetResponse(uint8_t Response);
int32_t  SD_SendCmd(uint8_t cmd, uint32_t arg, uint8_t crc);
int32_t  SD_RecvData(uint8_t *buff, uint32_t len);
int32_t  SD_SendBlock(uint8_t *buff, uint8_t cmd);
int32_t  SD_RecvData_DMA(uint8_t *buff, uint32_t len);
int32_t  SD_SendBlock_DMA(uint8_t *buff, uint8_t cmd);
int32_t  SD_GetCID(uint8_t *cid_data);
int32_t  SD_GetCSD(uint8_t *csd_data);
uint32_t SD_GetSectorCount(void);
uint32_t SD_GetSectorSize(void);
uint32_t SD_GetCapacity(void);
int32_t  SD_Information_Printf(void);
int32_t  SD_Card_Init(void);


extern BSP_SDCard_t SPI_SDCard;
int32_t SPI_SD_Card_Init(void *Handle);
int32_t SPI_SD_Card_DeInit(void *Handle);
int32_t SPI_SD_Card_GetState(void *Handle);
int32_t SPI_SD_Card_GetInfo(void *Handle, SD_CardInfo_t *Card_Info);
int32_t SPI_SD_Card_ReadSector(void *Handle, uint8_t *buff, uint32_t sector, uint32_t cnt);
int32_t SPI_SD_Card_WriteSector(void *Handle, uint8_t *buff, uint32_t sector, uint32_t cnt);
int32_t SPI_SD_Card_ReadSector_DMA(void *Handle, uint8_t *buff, uint32_t sector, uint32_t cnt);
int32_t SPI_SD_Card_WriteSector_DMA(void *Handle, uint8_t *buff, uint32_t sector, uint32_t cnt);

#endif


