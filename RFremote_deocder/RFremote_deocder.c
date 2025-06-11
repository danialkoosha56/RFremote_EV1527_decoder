/**
 * @file RFremote_deocder.c
 * @author Danial Koosha 
 * @brief 
 * @version 0.1
 * @date 2025-06-11
 * 
 * @copyright Copyright (c) 2025
 * 
 */

#include "stdbool.h"

#include "stm32f103xb.h"
#include "RFremote_deocder.h"
#include "timer.h"
#include "gpio.h"

#define PACKET_SIZE 24
#define ONE_BIT_RATIO 3
#define ZERO_BIT_RATIO 3
#define SYNC_BIT_RATIO 31

enum
{
    FALLING_EDGE,
    RISING_EDGE
};

enum
{
    BAD_BIT = -1,
    ZERO_BIT = 0,
    ONE_BIT = 1,
    SYNC_BIT = 2
};

enum
{
    NOT_SYNC,
    IS_SYNC
};

enum
{
    PACKET_NOT_READY,
    PACKET_READY
};

static bool readyFlag = PACKET_NOT_READY;
static uint32_t remotePacket = 0; //only 24 bit needed

int bitDetect(uint32_t high, uint32_t low);

void RFremote_callBack(uint32_t cnt, bool edge)
{
    static bool firstEnter = 0;
    static uint32_t cnt_H = 1; // because of devide cant be 0
    static uint32_t cnt_L = 1; // because of devide cant be 0
    static bool syncFlag = NOT_SYNC;
    static uint8_t bitCnt = 0;
    int bitVal = BAD_BIT; //not supported val

    
    if (firstEnter == 0)
    {
        firstEnter = 1;
        if (edge == RISING_EDGE)
        {
            return;
        }
        
    }
    

    if (edge == RISING_EDGE)
    {
        cnt_L = cnt;
    }
    else
    {
        cnt_H = cnt;
        return;
    }
    bitVal = bitDetect(cnt_H, cnt_L);


    if (syncFlag == NOT_SYNC)
    {
        if (bitVal == SYNC_BIT)
        {
            syncFlag = IS_SYNC;
            return;
        }
        else
        {
            return;
        }
    }
    
    if ( !( (bitVal == ONE_BIT) || (bitVal == ZERO_BIT))) //except 0 or 1 not accepted
    {
        syncFlag = NOT_SYNC;
        bitCnt = 0;
        return;
    }

    bitCnt++;
    remotePacket = (remotePacket << 1) | bitVal; //shift left and add new bit
    if (bitCnt == PACKET_SIZE)
    {
        __NOP();
        bitCnt = 0;
        readyFlag = PACKET_READY;
        remotePacket = remotePacket & 0x00FFFFFF; //mask to 24 bit, i dont know why but its neded
    }
    
    /*FUNCTION SUMMARY:
        // TIMER COUNTER LOW PULSE
        // TIMER COUNTER HIGH PULSE
        // CALC PULSE RATIO

        // IS SYNC PULSE
            // NO: LOOK FOR SYNC PULSE
            // YES: STORE NEXT 24bit
                // IF 24bit HAS CORRECT RATIO
                    // OK: CALL RFremote_receiveDataCallback
                    // ELSE: LOOK FOR SYNC PULSE AGAIN    
    */
}


int bitDetect(uint32_t high, uint32_t low)
{
    if ( (ONE_BIT_RATIO-1 <= (float)high/low)  && ((float)high/low <= ONE_BIT_RATIO+1) ) // 1 detect (exact ratio 3)
    {
        return ONE_BIT;
    }
    else if ( (ZERO_BIT_RATIO-1 <= (float)low/high)  && ((float)low/high <= ZERO_BIT_RATIO+1) ) // 0 detect (exact ratio 3)
    {
        return ZERO_BIT;
    }
    if ( (SYNC_BIT_RATIO-5 <= (float)low/high)  && ((float)low/high <= SYNC_BIT_RATIO+5) ) //sync detect (exact ratio 31)
    {
        return SYNC_BIT;
    }
    else //bad ratio
        return BAD_BIT;
}

bool RFremote_getPacket(RFremotePacket_t *packet)
{
    if (readyFlag == PACKET_NOT_READY)
        return false;
    
    readyFlag = PACKET_NOT_READY; //reset flag
    packet->remoteID = (remotePacket & (0x00FFFFFF - 0b11)) >> 2; //mask last 2 bits and shift right 2 bits
    packet->remoteCMD = remotePacket & 0b11; //mask last 2 bits
    return true; //packet ready
    
}