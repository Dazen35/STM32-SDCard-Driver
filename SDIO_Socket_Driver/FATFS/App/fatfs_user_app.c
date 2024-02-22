#include "fatfs_user_app.h"


static FATFS fs;					// 文件系统

/*********************************************************************************
  *
  * @brief 常用文件操作函数
  *
  *********************************************************************************/

/**
  * @brief  通过文件系统存储一次数据到TF卡TXT文件
  * @note   无
  * @param  path: 路径
  * @param  data: 数据缓冲区
  * @param  len: 数据缓冲区长度
  * @retval 实际写入文件数据长度
  */
uint32_t FATFS_Save_Data_To_File(const char *path, uint8_t *data, uint32_t len)
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

  /* 查看文件大小 */
  file_size = file.obj.objsize;
  if (file_size > 0)
  {
#ifdef FATFS_DEBUG_OPEN
    printf("file size: %d Byte\r\n", file_size);
#endif
  }

  /* 移动文件读写指针 */
  fs_res = f_lseek(&file, file_size);
  if (fs_res != FR_OK)
  {
#ifdef FATFS_DEBUG_OPEN
    printf("f_lseek error, error code: %d\r\n", fs_res);
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
  fs_res = f_mount(NULL, "0:", 0);
  if (fs_res != FR_OK)
  {
#ifdef FATFS_DEBUG_OPEN
    printf("f_mount error, error code: %d\r\n", fs_res);
#endif
    return fs_res;
  }

  return file_size;
}


/**
  * @brief  通过地址写文件数据
  * @note   无
  * @param  path: 路径
  * @param  addr: 数据地址
  * @param  data: 数据缓冲区
  * @param  len: 数据缓冲区长度
  * @retval 实际写入的数据长度
  */
uint32_t FATFS_Write_File_Addr(const char *path, uint32_t addr, uint8_t *data, uint32_t len)
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

  /* 移动文件读写指针 */
  fs_res = f_lseek(&file, addr);
  if (fs_res != FR_OK)
  {
#ifdef FATFS_DEBUG_OPEN
    printf("f_lseek error, error code: %d\r\n", fs_res);
#endif
    return fs_res;
  }

  /* 写文件 */
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
  fs_res = f_mount(NULL, "0:", 0);
  if (fs_res != FR_OK)
  {
#ifdef FATFS_DEBUG_OPEN
    printf("f_mount error, error code: %d\r\n", fs_res);
#endif
    return fs_res;
  }

  return file_size;
}


/**
  * @brief  通过包编号写文件数据
  * @note   无
  * @param  path: 路径
  * @param  pack: 数据包编号
  * @param  data: 数据缓冲区
  * @param  len: 数据缓冲区长度
  * @retval 实际写入的数据长度
  */
uint32_t FATFS_Write_File_Pack(const char *path, uint32_t pack, uint8_t *data, uint32_t len)
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

  /* 移动文件读写指针 */
  fs_res = f_lseek(&file, pack * len);
  if (fs_res != FR_OK)
  {
#ifdef FATFS_DEBUG_OPEN
    printf("f_lseek error, error code: %d\r\n", fs_res);
#endif
    return fs_res;
  }

  /* 写文件 */
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
  fs_res = f_mount(NULL, "0:", 0);
  if (fs_res != FR_OK)
  {
#ifdef FATFS_DEBUG_OPEN
    printf("f_mount error, error code: %d\r\n", fs_res);
#endif
    return fs_res;
  }

  return file_size;
}


/**
  * @brief  通过地址读文件数据
  * @note   无
  * @param  path: 路径
  * @param  addr: 数据地址
  * @param  data: 数据缓冲区
  * @param  len: 数据缓冲区长度
  * @retval 实际读取到的数据长度
  */
uint32_t FATFS_Read_File_Addr(const char *path, uint32_t addr, uint8_t *data, uint32_t len)
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
  fs_res = f_open(&file, path, FA_READ);
  if (fs_res != FR_OK)
  {
#ifdef FATFS_DEBUG_OPEN
    printf("f_open error, error code: %d\r\n", fs_res);
#endif
    return fs_res;
  }

  /* 移动文件读写指针 */
  fs_res = f_lseek(&file, addr);
  if (fs_res != FR_OK)
  {
#ifdef FATFS_DEBUG_OPEN
    printf("f_lseek error, error code: %d\r\n", fs_res);
#endif
    return fs_res;
  }

  /* 读文件 */
  fs_res = f_read(&file, data, len, &file_size);
  if (fs_res != FR_OK)
  {
#ifdef FATFS_DEBUG_OPEN
    printf("f_read error, error code: %d\r\n", fs_res);
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
  fs_res = f_mount(NULL, "0:", 0);
  if (fs_res != FR_OK)
  {
#ifdef FATFS_DEBUG_OPEN
    printf("f_mount error, error code: %d\r\n", fs_res);
#endif
    return fs_res;
  }

  return file_size;
}


/**
  * @brief  通过包编号读文件数据
  * @note   无
  * @param  path: 路径
  * @param  pack: 数据包编号
  * @param  data: 数据缓冲区
  * @param  len: 数据缓冲区长度
  * @retval 实际读取到的数据长度
  */
uint32_t FATFS_Read_File_Pack(const char *path, uint32_t pack, uint8_t *data, uint32_t len)
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
  fs_res = f_open(&file, path, FA_READ);
  if (fs_res != FR_OK)
  {
#ifdef FATFS_DEBUG_OPEN
    printf("f_open error, error code: %d\r\n", fs_res);
#endif
    return fs_res;
  }

  /* 移动文件读写指针 */
  fs_res = f_lseek(&file, pack * len);
  if (fs_res != FR_OK)
  {
#ifdef FATFS_DEBUG_OPEN
    printf("f_lseek error, error code: %d\r\n", fs_res);
#endif
    return fs_res;
  }

  /* 读文件 */
  fs_res = f_read(&file, data, len, &file_size);
  if (fs_res != FR_OK)
  {
#ifdef FATFS_DEBUG_OPEN
    printf("f_read error, error code: %d\r\n", fs_res);
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
  fs_res = f_mount(NULL, "0:", 0);
  if (fs_res != FR_OK)
  {
#ifdef FATFS_DEBUG_OPEN
    printf("f_mount error, error code: %d\r\n", fs_res);
#endif
    return fs_res;
  }

  return file_size;
}


/**
  * @brief  获取文件大小
  * @note   无
  * @param  path: 路径
  * @retval 文件大小，单位Byte
  */
uint32_t FATFS_Get_File_Size(const char *path)
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
  fs_res = f_open(&file, path, FA_READ);
  if (fs_res != FR_OK)
  {
#ifdef FATFS_DEBUG_OPEN
    printf("f_open error, error code: %d\r\n", fs_res);
#endif
    return fs_res;
  }

  /* 查看文件大小 */
  file_size = file.obj.objsize;
  if (file_size > 0)
  {
#ifdef FATFS_DEBUG_OPEN
    printf("file size: %d Byte\r\n", file_size);
#endif
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
  fs_res = f_mount(NULL, "0:", 0);
  if (fs_res != FR_OK)
  {
#ifdef FATFS_DEBUG_OPEN
    printf("f_mount error, error code: %d\r\n", fs_res);
#endif
    return fs_res;
  }

  return file_size;
}



/**
  * @brief  删除文件
  * @note   无
  * @param  path: 路径
  * @retval 0-成功，其他失败
  */
int32_t FATFS_Delete_File(const char *path)
{
  FRESULT fs_res;		// API函数返回结果

  /* 挂载文件系统 */
  fs_res = f_mount(&fs, "0:", 1);
  if (fs_res != FR_OK)
  {
#ifdef FATFS_DEBUG_OPEN
    printf("f_mount error, error code: %d\r\n", fs_res);
#endif
    return fs_res;
  }

  /* 删除文件 */
  fs_res = f_unlink(path);
  if (fs_res != FR_OK)
  {
#ifdef FATFS_DEBUG_OPEN
    printf("f_unlink error, error code: %d\r\n", fs_res);
#endif
    return fs_res;
  }

  /* 卸载文件系统 */
  fs_res = f_mount(NULL, "0:", 0);
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
  * @brief  创建FAT文件系统
  * @note   无
  * @param  path: 路径
  * @retval 0-成功，其他失败
  */
int32_t FATFS_Create_FileSystem(const char *path)
{
  FRESULT fs_res;		// API函数返回结果
  static uint8_t fatfs_mkfs_buffer[1024];

  /* 创建FAT文件系统 */
  fs_res = f_mkfs(path, FM_FAT32, 1024 * 32, fatfs_mkfs_buffer, 1024);
  if (fs_res != FR_OK)
  {
#ifdef FATFS_DEBUG_OPEN
    printf("f_mkfs error, error code: %d\r\n", fs_res);
#endif
    return fs_res;
  }

  return FR_OK;
}


/**
  * @brief  读取磁盘剩余容量
  * @note   单位，1扇区
  * @param  path: 无
  * @retval 剩余容量
  */
uint32_t FATFS_Get_FreeSpace(const char *path)
{
  FRESULT fs_res;		// API函数返回结果
  FATFS *p_fs = &fs;
  DWORD fre_clust, fre_sect, tot_sect;

  /* 挂载文件系统 */
  fs_res = f_mount(&fs, "0:", 1);
  if (fs_res != FR_OK)
  {
#ifdef FATFS_DEBUG_OPEN
    printf("f_mount error, error code: %d\r\n", fs_res);
#endif
    return fs_res;
  }

  /* 读取磁盘剩余容量 */
  fs_res = f_getfree(path, &fre_clust, &p_fs);
  if (fs_res != FR_OK)
  {
#ifdef FATFS_DEBUG_OPEN
    printf("f_getfree error, error code: %d\r\n", fs_res);
#endif
    return fs_res;
  }
  /* Get total sectors and free sectors */
  tot_sect = (p_fs->n_fatent - 2) * p_fs->csize;
  tot_sect = tot_sect;
  fre_sect = fre_clust * p_fs->csize;

  /* 卸载文件系统 */
  fs_res = f_mount(NULL, "0:", 0);
  if (fs_res != FR_OK)
  {
#ifdef FATFS_DEBUG_OPEN
    printf("f_mount error, error code: %d\r\n", fs_res);
#endif
    return fs_res;
  }

  return fre_sect;
}



