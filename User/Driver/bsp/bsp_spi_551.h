
#ifndef __BSP_SPI_551_H
#define __BSP_SPI_551_H

#include "includes.h"

#define SPI_WAIT_TIMEDLY        800000
#define BCM_DUMMY_BYTE          0xA5

#define BCM_SPI                       SPI4
#define BCM_SPI_CLK                   SPI_BaudRatePrescaler_4  //21mhz  //SPI_BaudRatePrescaler_16     //5MHz 2.5mhz
#define ENABLE_BCMSPI_RCC() 	      RCC_APB2PeriphClockCmd(RCC_APB2Periph_SPI4, ENABLE)
	
#define BCM_SPI_SCK_PIN               GPIO_Pin_2                 
#define BCM_SPI_SCK_GPIO_PORT         GPIOE                       
#define BCM_SPI_SCK_GPIO_CLK          RCC_APB1Periph_GPIOE

#define BCM_SPI_MISO_PIN              GPIO_Pin_5                  
#define BCM_SPI_MISO_GPIO_PORT        GPIOE                       
#define BCM_SPI_MISO_GPIO_CLK         RCC_APB1Periph_GPIOE

#define BCM_SPI_MOSI_PIN              GPIO_Pin_6                 
#define BCM_SPI_MOSI_GPIO_PORT        GPIOE                       
#define BCM_SPI_MOSI_GPIO_CLK         RCC_APB1Periph_GPIOE

#define BCM_CS_PIN                    GPIO_Pin_9                  
#define BCM_CS_GPIO_PORT              GPIOI
#define BCM_CS_GPIO_CLK               RCC_APB1Periph_GPIOI

#define BCM_CS_LOW()                  GPIO_ResetBits(BCM_CS_GPIO_PORT, BCM_CS_PIN)
#define BCM_CS_HIGH()                 GPIO_SetBits(BCM_CS_GPIO_PORT, BCM_CS_PIN) 

#define BCM_551CS_PIN                 GPIO_Pin_9  //GPIO_Pin_3                  
#define BCM_551CS_GPIO_PORT           GPIOH //GPIOB
#define BCM_551CS_GPIO_CLK            RCC_APB1Periph_GPIOH //RCC_APB1Periph_GPIOB

#define BCM_551CS_LOW()               GPIO_ResetBits(BCM_551CS_GPIO_PORT, BCM_551CS_PIN)
#define BCM_551CS_HIGH()              GPIO_SetBits(BCM_551CS_GPIO_PORT, BCM_551CS_PIN)


int8_t BSP_BCMInit(void);
u8 BCM_ReadByte(u8 *byte,u32 len);
u8 BCM_WriteByte(u8 *byte,u32 wr_len);
int32_t aps_spi_read(int32_t d,uint8_t *wr_buf,int32_t wr_len,uint8_t *rd_buf,int32_t rd_len);
int32_t aps_spi_write(int32_t d,uint8_t *wr_buf,int32_t wr_len);

#endif /* __STM32_SPI_551_H */
