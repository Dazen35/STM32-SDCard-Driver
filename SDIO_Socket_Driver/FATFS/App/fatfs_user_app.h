#ifndef __FATFS_USER_APP_H__
#define __FATFS_USER_APP_H__

#include "ff.h"
#include "ff_gen_drv.h"


/* 常用文件操作函数定义 */
uint32_t FATFS_Save_Data_To_File(const char *path, uint8_t *data, uint32_t len);
uint32_t FATFS_Write_File_Addr(const char *path, uint32_t addr, uint8_t *data, uint32_t len);
uint32_t FATFS_Write_File_Pack(const char *path, uint32_t pack, uint8_t *data, uint32_t len);
uint32_t FATFS_Read_File_Addr(const char *path, uint32_t addr, uint8_t *data, uint32_t len);
uint32_t FATFS_Read_File_Pack(const char *path, uint32_t pack, uint8_t *data, uint32_t len);
uint32_t FATFS_Get_File_Size(const char *path);
int32_t FATFS_Delete_File(const char *path);
int32_t FATFS_Create_FileSystem(const char *path);
uint32_t FATFS_Get_FreeSpace(const char *path);

#endif





