/**
 * @file RFremote_deocder.h
 * @author Danial Koosha 
 * @brief 
 * @version 0.1
 * @date 2025-06-11
 * 
 * @copyright Copyright (c) 2025
 * 
 */

#ifndef __RF_REMOTE_DECODER_H__
#define __RF_REMOTE_DECODER_H__

typedef struct
{
    uint32_t remoteID;  // 24 bit remote ID
    uint8_t remoteCMD;  // 2 bit command
} RFremotePacket_t;



bool RFremote_getPacket(RFremotePacket_t *packet);
void RFremote_callBack(uint32_t cnt, bool edge);

#endif //__RF_REMOTE_H__