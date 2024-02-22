
/* Includes ------------------------------------------------------------------*/
#include "fx_sd_socket.h"
#include "fx_stm32_sd_driver.h"




/**
  * @brief  FileX底层的初始化磁盘函数
  * @note   无
  * @param  Instance: 磁盘编号
  * @retval 结果 0-成功，其他-失败
  */
INT fx_stm32_sd_init(UINT Instance)
{
	int32_t res = 0;
	
	if (Instance == FX_STM32_SD_INSTANCE)
	{
		res = SD_Card_Init();
	}
	
	if (res == 0)
	{
	  return 0;
	}
	else
	{
		return 1;
	}
}


/**
  * @brief  FileX底层的复位磁盘函数
  * @note   无
  * @param  Instance: 磁盘编号
  * @retval 结果 0-成功，其他-失败
  */
INT fx_stm32_sd_deinit(UINT Instance)
{
	int32_t res = 0;
	
	if (Instance == FX_STM32_SD_INSTANCE)
	{
		SD_Set_IdleMode();
	}
	
	if (res == 0)
	{
	  return 0;
	}
	else
	{
		return 1;
	}
}


/**
  * @brief  FileX底层的获取磁盘状态函数
  * @note   无
  * @param  Instance: 磁盘编号
  * @retval 结果 0-成功，其他-失败
  */
INT fx_stm32_sd_get_status(UINT Instance)
{
	int32_t res = 0;
	
	if (Instance == FX_STM32_SD_INSTANCE)
	{
		res = SD_GetCardState();
	}
	
	if (res == 0)
	{
	  return 0;
	}
	else
	{
		return 1;
	}
}


/**
  * @brief  FileX底层的读磁盘函数
  * @note   无
  * @param  Instance: 磁盘编号
  * @param  Buffer: 数据缓存区
  * @param  StartSector: 扇区编号
  * @param  NbrOfBlocks: 扇区数
  * @retval 结果 0-成功，其他-失败
  */
INT fx_stm32_sd_read_blocks(UINT Instance, UINT *Buffer, UINT StartSector, UINT NbrOfBlocks)
{
	int32_t res = 0;
	
	if (Instance == FX_STM32_SD_INSTANCE)
	{
		res = SD_ReadSector((uint8_t *) Buffer, StartSector, NbrOfBlocks);
	}
	
	if (res == 0)
	{
	  return 0;
	}
	else
	{
		return 1;
	}
}


/**
  * @brief  FileX底层的写磁盘函数
  * @note   无
  * @param  Instance: 磁盘编号
  * @param  Buffer: 数据缓存区
  * @param  StartSector: 扇区编号
  * @param  NbrOfBlocks: 扇区数
  * @retval 结果 0-成功，其他-失败
  */
INT fx_stm32_sd_write_blocks(UINT Instance, UINT *Buffer, UINT StartSector, UINT NbrOfBlocks)
{
	int32_t res = 0;
	
	if (Instance == FX_STM32_SD_INSTANCE)
	{
		res = SD_WriteSector((uint8_t *) Buffer, StartSector, NbrOfBlocks);
	}
	
	if (res == 0)
	{
	  return 0;
	}
	else
	{
		return 1;
	}
}








