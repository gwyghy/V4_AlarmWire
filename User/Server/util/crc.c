#include "crc.h"

void CRCCal(uint8_t * dat,  uint16_t len, uint8_t *crc)
{
    uint16_t	i; 
	uint8_t		j;
	uint8_t		tmp;
    uint16_t	crcVal = 0xffff;
	
    for(i = 0; i < len; i++)
	{
        crcVal ^= dat[i];
        for(j = 0; j < 8; j++)
		{
            tmp = crcVal & 0x01;
            crcVal >>= 1;
            if(tmp)
			{
                crcVal ^= 0xa001;
            }
        }
    }	
	
    crc[0] = (uint8_t)(crcVal &  0xff);          //little-endian
    crc[1] = (uint8_t)(crcVal >> 0x08);
}

void CRCCal_continue(uint8_t * dat,  uint32_t len ,uint8_t *crc ,uint16_t crcValue)
{
    uint32_t	i; 
	uint8_t		j;
	uint8_t		tmp;
    uint16_t	crcVal = crcValue;
	
    for(i = 0; i < len; i++)
	{
        crcVal ^= dat[i];
        for(j = 0; j < 8; j++)
		{
            tmp = crcVal & 0x01;
            crcVal >>= 1;
            if(tmp)
			{
                crcVal ^= 0xa001;
            }
        }
    }	
	
    crc[0] = (uint8_t)(crcVal &  0xff);          //little-endian
    crc[1] = (uint8_t)(crcVal >> 0x08);
}

uint16_t CRCCal_break(uint8_t * dat,  uint16_t len,uint16_t crcValue)
{
    uint16_t	i; 
	uint8_t		j;
	uint8_t		tmp;
    uint16_t	crcVal = crcValue;
	
    for(i = 0; i < len; i++)
	{
        crcVal ^= dat[i];
        for(j = 0; j < 8; j++)
		{
            tmp = crcVal & 0x01;
            crcVal >>= 1;
            if(tmp)
			{
                crcVal ^= 0xa001;
            }
        }
    }	
	
    return crcVal;
}
