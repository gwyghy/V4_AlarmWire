#ifndef CRC_H
#define CRC_H

#include <includes.h>

void CRCCal(uint8_t * dat,  uint16_t len, uint8_t *crc);
void CRCCal_continue(uint8_t * dat,  uint32_t len ,uint8_t *crc ,uint16_t crcValue);
uint16_t CRCCal_break(uint8_t * dat,  uint16_t len,uint16_t crcValue);

#endif

