#include "spi_tfcard.h"		


/* SD卡信息 */
SD_CardInfo_t SDCard_Info;

uint8_t spi_temp[32];

/**
  * @brief  取消选择, 释放SPI总线
  * @note   无
  * @param  无
  * @retval 0
  */
int32_t SD_Release(void)
{
  TFCARD_SPI_CS_RELEASE();
  /* 延时 */
  bsp_spi_read(&BSP_SPI2, &spi_temp[0], 1);
  
  return 0;
}


/**
  * @brief  选中SD卡, 等待SD卡准备
  * @note   SD卡返回0x00时表示忙，返回0xFF表示准备就绪
  * @param  无
  * @retval 0: 成功, 其他: 失败
  */
int32_t SD_Select(void)
{
  TFCARD_SPI_CS_SELECT();
  /* 延时 */
  bsp_spi_read(&BSP_SPI2, &spi_temp[0], 1);

  if (SD_WaitReady() == 0)
  {
    return 0; // 等待成功
  }
  else
  {
    SD_Release();
    return 1; // 等待失败
  }
}


/**
  * @brief  等待SD卡准备
  * @note   SD卡返回0x00时表示忙，返回0xFF表示准备就绪
  * @param  无
  * @retval 0: 成功, 其他: 失败
  */
int32_t SD_WaitReady(void)
{
  uint16_t count = 0;

  do
  {
    /* 检查SD卡返回的数据 */
    bsp_spi_read(&BSP_SPI2, &spi_temp[0], 1);
    
    if (spi_temp[0] == 0xFF)
    {
      return 0;
    }
    count++;
  }
  while (count < 0x8000); 

  return 1;
}


/**
  * @brief  等待SD卡回应
  * @note   无
  * @param  Response: 回应值
  * @retval 0: 成功, 其他: 失败
  */
int32_t SD_GetResponse(uint8_t Response)
{
  /* 等待次数 */
  uint16_t count = 0x1FFF;

  do
  {
    /* 等待得到准确的回应 */
    bsp_spi_read(&BSP_SPI2, &spi_temp[0], 1);
    count--;
  }
  while ((spi_temp[0] != Response) && count);

  if (count == 0)		// 超时退出
  {
    return MSD_RESPONSE_FAILURE;  // 回应失败  
  }		
  else		// 正常退出
  {
    return MSD_RESPONSE_NO_ERROR; // 回应成功
  }
}


/**
  * @brief  按扇区从SD卡读取数据
  * @note   SD卡的1个扇区固定为512字节
  * @param  buf: 数据缓存区
  * @param  len: 数据缓存区长度
  * @retval 0: 成功, 其他: 失败
  */
int32_t SD_RecvData(uint8_t *buff, uint32_t len)
{			  	  
  if (SD_GetResponse(0xFE) != MSD_RESPONSE_NO_ERROR)    // 等待SD卡发回数据起始指令0xFE
  {
    return 1;   // 读取失败
  }

  /* 接收数据和CRC */
  bsp_spi_read(&BSP_SPI2, buff, len);
  bsp_spi_read(&BSP_SPI2, spi_temp, 2);   

  return 0;   // 读取成功
}

	
/**
  * @brief  按扇区向SD卡发送数据
  * @note   SD卡的1个扇区固定为512字节
  * @param  buff: 数据缓存区
  * @param  cmd: 指令
  * @retval 0: 成功, 其他: 失败
  */
int32_t SD_SendBlock(uint8_t *buff, uint8_t cmd)
{	
  int32_t retval;

  if (SD_WaitReady() == 1)
  {
    return 1;  // 等待准备失效
  }

  bsp_spi_write(&BSP_SPI2, &cmd, 1);
  if (cmd != 0xFD)   // 不是结束指令
  {
    /* 发送数据并接收CRC */
    bsp_spi_write(&BSP_SPI2, buff, 512);
    bsp_spi_read(&BSP_SPI2, spi_temp, 2);  
    
    // 接收响应
    bsp_spi_read(&BSP_SPI2, &retval, 1);   
    if ((retval & 0x1F) != MSD_DATA_OK)			 // 正常响应为xxx00101
    {
      return 2;    // 响应错误		
    }
    
    /* 延时等待SD卡内部数据写入完成，上面的操作只是把数据发送到SD卡控制器缓存中，还没有写入到SD卡闪存内 */
    if (SD_WaitReady() == 1)
    {
      return 1;  // 等待准备失效
    }

  }	    
  return 0;   // 写入成功
}


/**
  * @brief  按扇区从SD卡读取数据
  * @note   SD卡的1个扇区固定为512字节
  * @param  buf: 数据缓存区
  * @param  len: 数据缓存区长度
  * @retval 0: 成功, 其他: 失败
  */
int32_t SD_RecvData_DMA(uint8_t *buff, uint32_t len)
{			  	  
  if (SD_GetResponse(0xFE) != MSD_RESPONSE_NO_ERROR)    // 等待SD卡发回数据起始指令0xFE
  {
    return 1;   // 读取失败
  }

  /* 接收数据和CRC */
  bsp_spi_read_dma(&BSP_SPI2, buff, len);
  bsp_spi_read_dma(&BSP_SPI2, spi_temp, 2);  

  return 0;   // 读取成功
}

	
/**
  * @brief  按扇区向SD卡发送数据
  * @note   SD卡的1个扇区固定为512字节
  * @param  buff: 数据缓存区
  * @param  cmd: 指令
  * @retval 0: 成功, 其他: 失败
  */
int32_t SD_SendBlock_DMA(uint8_t *buff, uint8_t cmd)
{	
  int32_t retval;

  bsp_spi_write(&BSP_SPI2, &cmd, 1);
  if (cmd != 0xFD)   // 不是结束指令
  {
    /* 发送数据并接收CRC */
    bsp_spi_write_dma(&BSP_SPI2, buff, 512);
    bsp_spi_read_dma(&BSP_SPI2, spi_temp, 2); 
    
    // 接收响应
    bsp_spi_read(&BSP_SPI2, &retval, 1);   
    if ((retval & 0x1F) != MSD_DATA_OK)			 // 正常响应为xxx00101
    {
      return 2;    // 响应错误		
    }
    
    /* 延时等待SD卡内部数据写入完成，上面的操作只是把数据发送到SD卡控制器缓存中，还没有写入到SD卡闪存内 */
    if (SD_WaitReady() == 1)
    {
      return 1;  // 等待准备失效
    }

  }
  return 0;   // 写入成功
}


/**
  * @brief  向SD卡发送命令
  * @note   无
  * @param  cmd: 命令
  * @param  arg: 命令参数
  * @param  crc: CRC
  * @retval SD卡返回的响应
  */
int32_t SD_SendCmd(uint8_t cmd, uint32_t arg, uint8_t crc)
{
  int32_t retval;	
  uint8_t count = 0xFF; 

  SD_Release();  // 取消上次片选
  if (SD_Select() == 1)
  {
    return 0xFF;  // 片选失效 
  }

  spi_temp[0] = (uint8_t)(cmd | 0x40);
  spi_temp[1] = (uint8_t)(arg >> 24);
  spi_temp[2] = (uint8_t)(arg >> 16);
  spi_temp[3] = (uint8_t)(arg >> 8);
  spi_temp[4] = (uint8_t)(arg);
  spi_temp[5] = (uint8_t)(crc);

  /* 写入命令 */
  #ifdef  SPI_DMA_SEND_CMD
  bsp_spi_write(&BSP_SPI2, spi_temp, 6);
  #else
  bsp_spi_write_dma(&BSP_SPI2, spi_temp, 6);
  #endif

  if (cmd == TF_CMD12)
  {
    bsp_spi_read(&BSP_SPI2, &spi_temp[0], 1);  // Skip a stuff byte when stop reading
  }

  /* 等待响应, 或超时退出 */
  do
  {
    bsp_spi_read(&BSP_SPI2, &retval, 1);
  }
  while ((retval & 0x80) && count--);	 

  /* 返回状态值 */
  return retval;
}		


/**
  * @brief  获取SD卡的CID信息
  * @note   包括制造商信息
  * @param  cid: 存放CID的缓冲区，至少16Byte
  * @retval 0: 成功, 其他: 失败
  */
int32_t SD_GetCID(uint8_t *cid)
{
  int32_t retval;	

  retval = SD_SendCmd(TF_CMD10, 0, 0x39);  // 发CMD10命令, 读CID
  if (retval == 0x00)
  {
    retval = SD_RecvData(cid, 16);  // 接收16个字节的数据	 
  }

  SD_Release();  // 取消片选
  if (retval == 1)
  {
    return 1;
  }
  else 
  {
    return 0;
  }
}		

	
/**
  * @brief  获取SD卡的CSD信息
  * @note   包括容量和速度信息
  * @param  csd: 存放CSD的缓冲区，至少16Byte
  * @retval 0: 成功, 其他: 失败
  */
int32_t SD_GetCSD(uint8_t *csd)
{
  int32_t retval;	 
  retval = SD_SendCmd(TF_CMD9, 0, 0xAF);  // 发CMD9命令, 读CSD
  if (retval == 0)
  {
    retval = SD_RecvData(csd, 16);   // 接收16个字节的数据 
  }

  SD_Release();  // 取消片选
  if (retval == 1)
  {
    return 1;
  }
  else 
  {
    return 0;
  }
}


/**
  * @brief  获取SD卡的总扇区数
  * @note   1扇区等于512字节
  * @param  无
  * @retval SD卡的总扇区数
  */
uint32_t SD_GetSectorCount(void)
{
  uint32_t capacity;

  capacity = SD_GetCapacity() * 2;

  return capacity;
}


/**
  * @brief  获取SD卡的扇区大小
  * @note   1扇区等于512字节
  * @param  无
  * @retval 512字节
  */
uint32_t SD_GetSectorSize(void)
{
  return 512;
}


/**
  * @brief  获取SD卡的容量，单位KiByte
  * @note   单位KiByte
  * @param  无
  * @retval SD卡的容量
  */
uint32_t SD_GetCapacity(void)
{
  uint8_t csd[16];
  uint32_t capacity;
  uint16_t n;
  uint32_t csize; 

  // 获取CSD信息
  if (SD_GetCSD(csd) != 0)
  {
    return 0;
  }	 

  if ((csd[0] & 0xC0) == 0x40)  // 判断bit126是否为1
  { 
    csize = csd[9] + ((uint32_t)csd[8] << 8) + ((uint32_t)(csd[7] & 63) << 16) + 1;
    capacity = csize << 9; 
  }
  else
  { 
    n = (csd[5] & 0x0F) + ((csd[10] & 0x80) >> 7) + ((csd[9] & 0x03) << 1) + 2;
    csize = (csd[8] >> 6) + ((uint16_t)csd[7] << 2) + ((uint16_t)(csd[6] & 0x03) << 10) + 1;
    capacity = (uint32_t)csize << (n - 10);
  }

  return capacity;
} 


/**
  * @brief  初始化SD卡
  * @note   SD卡的1个扇区固定为512字节
  * @param  无
  * @retval 0: 成功, 其他: 失败
  */
int32_t SD_Card_Init(void)
{
  int32_t retval;
  uint16_t count = 20;
  uint8_t rxbuff[4];
  SDCard_Info.Type = TF_TYPE_ERROR;

  /* 初始化SPI总线，设置到低速模式400KHz以下 */
  bsp_spi_init(&BSP_SPI2, SPI_MODE_0, 400 * 1000);

  /* 延时(8 * 10)个时钟周期 */
  bsp_spi_read(&BSP_SPI2, spi_temp, 10);

  TFCARD_SPI_CS_SELECT();

  /* 进入空闲状态 */
  retval = SD_SendCmd(TF_CMD0, 0, 0x95); 
  while ((retval != 0x01) && (count--))
  {
    retval = SD_SendCmd(TF_CMD0, 0, 0x95);
  }

  if (retval == 0x01)
  {
    /* SD卡V2.0 */
    if (SD_SendCmd(TF_CMD8, 0x000001AA, 0x87) == 0x01)
    {
      /* 接收SD卡返回数据 */
      bsp_spi_read(&BSP_SPI2, rxbuff, 4);
      
      /* SD卡是否支持2.7~3.6V */
      if ((rxbuff[0] == 0x00) && (rxbuff[1] == 0x00) && (rxbuff[2] == 0x01) && (rxbuff[3] == 0xAA))
      {
        count = 100;
        SD_SendCmd(TF_CMD55, 0, 0x01);
        retval = SD_SendCmd(TF_CMD41, 0x40000000, 0x01);
        while (retval && (count--))
        {
          SD_SendCmd(TF_CMD55, 0, 0x01);
          retval = SD_SendCmd(TF_CMD41, 0x40000000, 0x01);
        }
        
        /* 判断SD卡2.0版本 */
        if (count && SD_SendCmd(TF_CMD58, 0, 0x01) == 0)   
        {
          /* 接收OCR值，检查CCS */
          bsp_spi_read(&BSP_SPI2, rxbuff, 4);
          if (rxbuff[0] & 0x40)
          {
            SDCard_Info.Type = TF_TYPE_SDHC;
          }
          else 
          {
            SDCard_Info.Type = TF_TYPE_SDV2;
          }
        }
      }
    }
    
    /* SD卡V1.0 */
    else
    {
      SD_SendCmd(TF_CMD55, 0, 0x01);
      retval = SD_SendCmd(TF_CMD41, 0, 0x01);
      if (retval <= 1)
      {		
        SDCard_Info.Type = TF_TYPE_SDV1;
        count = 0x1FFF;
        
        /* 等待退出空闲模式 */
        SD_SendCmd(TF_CMD55, 0, 0x01);
        retval = SD_SendCmd(TF_CMD41, 0, 0x01);
        while (retval && (count--))
        {
          SD_SendCmd(TF_CMD55, 0, 0x01);
          retval = SD_SendCmd(TF_CMD41, 0, 0x01);
        }
      }
      
      else
      {
        /* 错误的卡 */
        SDCard_Info.Type = TF_TYPE_ERROR;
      }
      
      if(count == 0 || SD_SendCmd(TF_CMD16, 512, 0x01) != 0)
      {
        /* 错误的卡 */
        SDCard_Info.Type = TF_TYPE_ERROR;
      }
    }
  }

  /* 取消片选 */
  SD_Release();

  /* 设置到25MHz */
  bsp_spi_set_max_clk_freq(&BSP_SPI2, 25 * 1000 * 1000);

  /* 获取TF卡大小 */
  SDCard_Info.Capacity = SD_GetCapacity();
  SDCard_Info.BlockSize = SD_GetSectorSize();
  SDCard_Info.BlocksCount = SD_GetSectorCount();

  /* 判断是否为SD_XC的卡 */
  if ((SDCard_Info.Capacity / 1024 > 1024 * 32)
        && (SDCard_Info.Type == TF_TYPE_SDHC))
  {
    SDCard_Info.Type = TF_TYPE_SDXC;
  }

  if (SDCard_Info.Type >= 1)
  {
    return 0;
  }
  else
  {
    /* 其他错误 */
    return 0xEE;
  }
}



/**
  * @brief  打印SD卡的类型和容量信息
  * @note   其中调用了printf函数，注意包含头文件stdio.h
  * @param  无
	* @retval 0: 成功，其他: 失败
  */
int32_t SD_Information_Printf(void)
{
  /* 打印TF卡类型 */
  if (SDCard_Info.Type == TF_TYPE_SDXC)
  {
    printf("\r\nSD_XC,");
  }
  else if (SDCard_Info.Type == TF_TYPE_SDHC)
  {
    printf("\r\nSD_HC,");
  }
  else if (SDCard_Info.Type == TF_TYPE_SDV2)
  {
    printf("\r\nSD_V2,");
  }
  else if (SDCard_Info.Type == TF_TYPE_SDV1)
  {
    printf("\r\nSD_V1,");
  }
  else
  {
    printf("\r\nSD ERROR,");
  }

  /* 打印TF卡容量 */
  if (SDCard_Info.Capacity / 1024 > 1024)
  {
    printf("Capacity:%4.1lfGB\r\n", (float)SDCard_Info.Capacity / 1024 / 1024);
  }
  else
  {
    printf("Capacity:%4.1lfMB\r\n", (float)SDCard_Info.Capacity / 1024);
  }

  return 0;
}


BSP_SDCard_t SPI_SDCard = 
{
  0,
  SPI_SD_Card_Init,
  SPI_SD_Card_DeInit,
  SPI_SD_Card_GetState,
  SPI_SD_Card_GetInfo,
  SPI_SD_Card_WriteSector,
  SPI_SD_Card_ReadSector,
  SPI_SD_Card_WriteSector_DMA,
  SPI_SD_Card_ReadSector_DMA,
};



int32_t SPI_SD_Card_Init(void *Handle)
{
  return SD_Card_Init();
}


int32_t SPI_SD_Card_DeInit(void *Handle)
{
  int32_t retval;
  uint8_t count = 20;

  TFCARD_SPI_CS_RELEASE();
  /* 延时 */
  bsp_spi_read(&BSP_SPI2, spi_temp, 16);

  TFCARD_SPI_CS_SELECT();

  /* 进入空闲状态 */
  retval = SD_SendCmd(TF_CMD0, 0, 0x95); 
  while ((retval != 0x01) && (count--))
  {
    retval = SD_SendCmd(TF_CMD0, 0, 0x95);
  }

  return 0;
}


int32_t SPI_SD_Card_GetState(void *Handle)
{
  if (SD_WaitReady() == 0)
  {
    return 0;
  }
  else
  {
    return 1;
  }
}


int32_t SPI_SD_Card_GetInfo(void *Handle, SD_CardInfo_t *Card_Info)
{
  Card_Info->Type = SDCard_Info.Type;
  Card_Info->Capacity = SDCard_Info.Capacity;
  Card_Info->BlocksCount = SDCard_Info.BlocksCount;
  Card_Info->BlockSize = SDCard_Info.BlockSize;
  return 0;
}


/**
  * @brief  按扇区读取SD卡数据
  * @note   SD卡的1个扇区固定为512字节
  * @param  buff: 数据缓冲区
  * @param  sector: 起始扇区
  * @param  cnt: 扇区数
  * @retval 0: 成功, 其他: 失败
  */
int32_t SPI_SD_Card_ReadSector(void *Handle, uint8_t *buff, uint32_t sector, uint32_t cnt)
{
  int32_t retval;

  if (SDCard_Info.Type != TF_TYPE_SDHC)
  {
    sector *= 512;   // 转换为字节地址
  }

  if (cnt == 1)
  {
    retval = SD_SendCmd(TF_CMD17, sector, 0x01);  // 读命令
    if(retval == 0)  // 指令发送成功
    {
      retval = SD_RecvData(buff, 512);   // 接收512个字节	   
    }
  }

  else
  {
    retval = SD_SendCmd(TF_CMD18, sector, 0x01);   // 连续读命令
    do
    {
      retval = SD_RecvData(buff, 512);   // 接收512个字节	 
      buff += 512;
    }
    while (--cnt && retval == 0); 
    
    SD_SendCmd(TF_CMD12, 0, 0x01);	  // 发送停止命令
  }

  SD_Release();  // 取消片选
  return retval;
}


/**
  * @brief  按扇区写入SD卡数据
  * @note   SD卡的1个扇区固定为512字节
  * @param  buff: 数据缓冲区
  * @param  sector: 起始扇区
  * @param  cnt: 扇区数
  * @retval 0: 成功, 其他: 失败
  */
int32_t SPI_SD_Card_WriteSector(void *Handle, uint8_t *buff, uint32_t sector, uint32_t cnt)
{
  int32_t retval;

  if (SDCard_Info.Type != TF_TYPE_SDHC)
  {
    sector *= 512;  // 转换为字节地址
  }

  if (cnt == 1)
  {
    retval = SD_SendCmd(TF_CMD24, sector, 0x01);  // 写命令
    if (retval == 0x00)  // 指令发送成功
    {
      retval = SD_SendBlock(buff, 0xFE);  // 写512个字节
    }
    
    else
    {
      SD_Card_Init();
    }
  }

  else
  {
    SD_SendCmd(TF_CMD55, 0, 0x01);	
    SD_SendCmd(TF_CMD23, cnt, 0x01);
    
    retval = SD_SendCmd(TF_CMD25, sector, 0x01);  // 连续写命令
    if (retval == 0)
    {
      do
      {
        retval = SD_SendBlock(buff, 0xFC);  // 发送512个字节	 
        buff += 512;
      }
      while (--cnt && retval == 0);
      retval = SD_SendBlock(0, 0xFD);  // 接收512个字节 
    }
  }

  SD_Release();  // 取消片选
  return retval;
}


int32_t SPI_SD_Card_ReadSector_DMA(void *Handle, uint8_t *buff, uint32_t sector, uint32_t cnt)
{
  int32_t retval;

  if (SDCard_Info.Type != TF_TYPE_SDHC)
  {
    sector *= 512;   // 转换为字节地址
  }

  if (cnt == 1)
  {
    retval = SD_SendCmd(TF_CMD17, sector, 0x01);  // 读命令
    if(retval == 0)  // 指令发送成功
    {
      retval = SD_RecvData_DMA(buff, 512);   // 接收512个字节	   
    }
  }

  else
  {
    retval = SD_SendCmd(TF_CMD18, sector, 0x01);   // 连续读命令
    do
    {
      retval = SD_RecvData_DMA(buff, 512);   // 接收512个字节	 
      buff += 512;
    }
    while (--cnt && retval == 0); 
    
    SD_SendCmd(TF_CMD12, 0, 0x01);	  // 发送停止命令
  }

  SD_Release();  // 取消片选
  return retval;
}


int32_t SPI_SD_Card_WriteSector_DMA(void *Handle, uint8_t *buff, uint32_t sector, uint32_t cnt)
{
  int32_t retval;

  if (SDCard_Info.Type != TF_TYPE_SDHC)
  {
    sector *= 512;  // 转换为字节地址
  }

  if (cnt == 1)
  {
    retval = SD_SendCmd(TF_CMD24, sector, 0x01);  // 写命令
    if (retval == 0x00)  // 指令发送成功
    {
      retval = SD_SendBlock_DMA(buff, 0xFE);  // 写512个字节
    }
    
    else
    {
      SD_Card_Init();
    }
  }

  else
  {
    SD_SendCmd(TF_CMD55, 0, 0x01);	
    SD_SendCmd(TF_CMD23, cnt, 0x01);
    
    retval = SD_SendCmd(TF_CMD25, sector, 0x01);  // 连续写命令
    if (retval == 0)
    {
      do
      {
        retval = SD_SendBlock_DMA(buff, 0xFC);  // 发送512个字节	 
        buff += 512;
      }
      while (--cnt && retval == 0);
      retval = SD_SendBlock_DMA(0, 0xFD);  // 接收512个字节 
    }
  }

  SD_Release();  // 取消片选
  return retval;
}




