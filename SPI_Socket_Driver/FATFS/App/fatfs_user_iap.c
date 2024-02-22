#include "fatfs_user_iap.h"
#include "stm32_flash.h"


#define FLASH_PAGE_SIZE (128 * 1024)

static FATFS fs;					// 文件系统

uint8_t firmware_buffer[FLASH_PAGE_SIZE];  /* 固件写入时使用的临时缓存区，FLASH_PAGE_SIZE为HAL中定义 */

/*********************************************************************************
  *
  * @brief 固件升级相关函数
  *
  *********************************************************************************/

/**
  * @brief  通过文件系统检查固件是否存在
  * @note   固件名建议为fw_crc.bin
  * @param  path: 路径
  * @retval 0-存在，其他-不存在
  */
int32_t FATFS_Check_Update_Firmware(const char *path)
{
	FRESULT fs_res;		// API函数返回结果
  FIL file;		      // 文件
	UINT fw_size;
	
  /* 挂载文件系统 */
	fs_res = f_mount(&fs, "0:", 1);
	if (fs_res != FR_OK)
	{
#ifdef FATFS_DEBUG_OPEN
		printf("f_mount error, error code: %d\r\n", fs_res);
#endif
		return fs_res;
	}
	
	/* 打开文件 */
	fs_res = f_open(&file, path, FA_READ);
	if (fs_res != FR_OK)
	{
#ifdef FATFS_DEBUG_OPEN
		printf("f_open error, error code: %d\r\n", fs_res);
#endif
		return fs_res;
	}
	
	fw_size = file.obj.objsize;
	/* 固件小于10KB或大于1MB */
	if ((fw_size < 1024 * 10) || (fw_size > 1024 * 1024))
	{
#ifdef FATFS_DEBUG_OPEN
		printf("f_lseek error, error code: %d\r\n", fs_res);
#endif
		return fs_res;
	}
	
	/* 关闭文件 */
	fs_res = f_close(&file);
	if (fs_res != FR_OK)
	{
#ifdef FATFS_DEBUG_OPEN
		printf("f_close error, error code: %d\r\n", fs_res);
#endif
		return fs_res;
	}
	
	/* 卸载文件系统 */
	fs_res = f_mount(NULL, "", 0);
	if (fs_res != FR_OK)
	{
#ifdef FATFS_DEBUG_OPEN
		printf("f_mount error, error code: %d\r\n", fs_res);
#endif
		return fs_res;
	}
	
	return FR_OK;
}


/**
  * @brief  通过文件系统读文件数据升级固件
  * @note   固件名建议为fw_crc.bin
  * @param  path: 路径
  * @param  addr: 固件要写入的flash地址
  * @param  data: 数据缓冲区
  * @param  len: 数据缓冲区长度，一般为2048或4096
  * @retval 固件大小
  */
int32_t FATFS_Read_File_Update_Firmware(const char *path, uint32_t addr)
{
	FRESULT fs_res;		// API函数返回结果
  FIL file;		      // 文件
  UINT file_size;   // 文件大小
	UINT index, fw_size, fw_page_count;
	
  /* 挂载文件系统 */
	fs_res = f_mount(&fs, "0:", 1);
	if (fs_res != FR_OK)
	{
#ifdef FATFS_DEBUG_OPEN
		printf("f_mount error, error code: %d\r\n", fs_res);
#endif
		return fs_res;
	}
	
	/* 打开文件 */
	fs_res = f_open(&file, path, FA_READ);
	if (fs_res != FR_OK)
	{
#ifdef FATFS_DEBUG_OPEN
		printf("f_open error, error code: %d\r\n", fs_res);
#endif
		return fs_res;
	}
	
	fw_size = file.obj.objsize;
	/* 固件小于10KB或大于1MB */
	if ((fw_size < 1024 * 10) || (fw_size > 1024 * 1024))
	{
#ifdef FATFS_DEBUG_OPEN
		printf("f_lseek error, error code: %d\r\n", fs_res);
#endif
		return fs_res;
	}
	
	if ((fw_size % FLASH_PAGE_SIZE) > 0)
	{
		fw_page_count = fw_size / FLASH_PAGE_SIZE + 1;
	}
	else
	{
		fw_page_count = fw_size / FLASH_PAGE_SIZE;
	}
	
	for (index = 0; index < fw_page_count; index++)
	{
		/* 读文件 */
		fs_res = f_read(&file, firmware_buffer, FLASH_PAGE_SIZE, &file_size);
		if (fs_res != FR_OK)
		{
#ifdef FATFS_DEBUG_OPEN
			printf("f_read error, error code: %d\r\n", fs_res);
#endif
			return fs_res;
		}
		
		STM32_FLASH_Unlock();
		STM32_FLASH_Erase_Page((addr - 0x08000000) / FLASH_PAGE_SIZE + index, 1);
		STM32_FLASH_Write_Data(addr + index * FLASH_PAGE_SIZE, firmware_buffer, FLASH_PAGE_SIZE);
		STM32_FLASH_Lock();
  }
	
	/* 关闭文件 */
	fs_res = f_close(&file);
	if (fs_res != FR_OK)
	{
#ifdef FATFS_DEBUG_OPEN
		printf("f_close error, error code: %d\r\n", fs_res);
#endif
		return fs_res;
	}
	
	/* 卸载文件系统 */
	fs_res = f_mount(NULL, "", 0);
	if (fs_res != FR_OK)
	{
#ifdef FATFS_DEBUG_OPEN
		printf("f_mount error, error code: %d\r\n", fs_res);
#endif
		return fs_res;
	}
	
	return fw_size;
}


/**
  * @brief  在磁盘中写入对应错误类型
  * @note   固件升级发生错误时使用此函数
  * @param  path: 路径
  * @param  data: 数据缓冲区
  * @param  len: 数据缓冲区长度
  * @retval 实际写入文件数据长度
  */
int32_t FATFS_Update_Firmware_Error(const char *path, uint8_t *data, uint32_t len)
{
	FRESULT fs_res;		// API函数返回结果
  FIL file;		      // 文件
  UINT file_size;   // 文件大小
	
	/* 挂载文件系统 */
	fs_res = f_mount(&fs, "0:", 1);
	if (fs_res != FR_OK)
	{
#ifdef FATFS_DEBUG_OPEN
		printf("f_mount error, error code: %d\r\n", fs_res);
#endif
		return fs_res;
	}
	
	/* 打开文件 */
	fs_res = f_open(&file, path, FA_OPEN_ALWAYS | FA_WRITE);
	if (fs_res != FR_OK)
	{
#ifdef FATFS_DEBUG_OPEN
		printf("f_open error, error code: %d\r\n", fs_res);
#endif
		return fs_res;
	}
	
	/* 写入数据到文件 */
	fs_res = f_write(&file, data, len, &file_size);
	if (fs_res != FR_OK)
	{
#ifdef FATFS_DEBUG_OPEN
		printf("f_write error, error code: %d\r\n", fs_res);
#endif
		return fs_res;
	}

	/* 关闭文件 */
	fs_res = f_close(&file);
	if (fs_res != FR_OK)
	{
#ifdef FATFS_DEBUG_OPEN
		printf("f_close error, error code: %d\r\n", fs_res);
#endif
		return fs_res;
	}
	
	/* 卸载文件系统 */
	fs_res = f_mount(NULL, "", 0);
	if (fs_res != FR_OK)
	{
#ifdef FATFS_DEBUG_OPEN
		printf("f_mount error, error code: %d\r\n", fs_res);
#endif
		return fs_res;
	}
	
	return file_size;
}














