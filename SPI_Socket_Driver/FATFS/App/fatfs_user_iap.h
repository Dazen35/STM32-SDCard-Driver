#ifndef __FATFS_USER_IAP_H__
#define __FATFS_USER_IAP_H__

#include "ff.h"
#include "ff_gen_drv.h"


/* 固件升级相关函数 */
int32_t FATFS_Check_Update_Firmware(const char *path);
int32_t FATFS_Read_File_Update_Firmware(const char *path, uint32_t addr);
int32_t FATFS_Update_Firmware_Error(const char *path, uint8_t *data, uint32_t len);


#endif






