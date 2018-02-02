/* -------------------------------------------------------------------------
//  File Name   :   Sha1.h
//  Author      :   Li Jiahan
//  Create Time :   2012-3-22 10:03:29
//  Description :
//
// -----------------------------------------------------------------------*/

#ifndef __SHA1_H__
#define __SHA1_H__

#include "libxlive/CrossPlatformConfig.h"
typedef unsigned char Byte;
typedef unsigned int UInt32;
typedef int Int32;

//-------------------------------------------------------------------------

class SHA1
{
public:
    SHA1();
    ~SHA1();

    void reset();
    void update (const Byte *message_array, UInt32 length);
    bool final (Byte message_digest_array[20]);
    
private:

        /*
         *  Process the next 512 bits of the message
         */
        void _processMessageBlock();

        /*
         *  Pads the current message block to 512 bits
         */
        void _padMessage();

        /*
         *  Performs a circular left shift operation
         */
        inline UInt32 _circularShift(int bits, UInt32 word);

        UInt32  m_h[5];                     // Message digest buffers

        UInt32  m_length_Low;               // Message length in bits
        UInt32  m_length_High;              // Message length in bits

        Byte    m_message_Block[64];        // 512-bit message blocks
        Int32   m_message_Block_Index;      // Index into message block array

        bool    m_computed;                   // Is the digest computed?
        bool    m_corrupted;                  // Is the message digest corruped?
private:
        static __inline unsigned short bswap_16(unsigned short x) 
        {
            return (x>>8) | (x<<8);
        }

        static __inline unsigned int bswap_32(unsigned int x) 
        {
            return (bswap_16(x&0xffff)<<16) | (bswap_16(x>>16));
        }
 
};

//--------------------------------------------------------------------------
#endif /* __SHA1_H__ */