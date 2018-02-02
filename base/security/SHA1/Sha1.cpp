/* -------------------------------------------------------------------------
//  File Name   :   Sha1.cpp
//  Author      :   Li Jiahan
//  Create Time :   2012-3-22 10:03:39
//  Description :   
//
// -----------------------------------------------------------------------*/

/*
 *  sha1.cpp
 *
 *  Copyright (C) 1998, 2009
 *  Paul E. Jones <paulej@packetizer.com>
 *  All Rights Reserved.
 *
 *****************************************************************************
 *  $Id: sha1.cpp 12 2009-06-22 19:34:25Z paulej $
 *****************************************************************************
 *
 *  Description:
 *      This class implements the Secure Hashing Standard as defined
 *      in FIPS PUB 180-1 published April 17, 1995.
 *
 *      The Secure Hashing Standard, which uses the Secure Hashing
 *      Algorithm (SHA), produces a 160-bit message digest for a
 *      given data stream.  In theory, it is highly improbable that
 *      two messages will produce the same message digest.  Therefore,
 *      this algorithm can serve as a means of providing a "fingerprint"
 *      for a message.
 *
 *  Portability Issues:
 *      SHA-1 is defined in terms of 32-bit "words".  This code was
 *      written with the expectation that the processor has at least
 *      a 32-bit machine word size.  If the machine word size is larger,
 *      the code should still function properly.  One caveat to that
 *      is that the input functions taking characters and character arrays
 *      assume that only 8 bits of information are stored in each character.
 *
 *  Caveats:
 *      SHA-1 is designed to work with messages less than 2^64 bits long.
 *      Although SHA-1 allows a message digest to be generated for
 *      messages of any number of bits less than 2^64, this implementation
 *      only works with messages with a length that is a multiple of 8
 *      bits.
 *
 */


#include "Sha1.h"

//-------------------------------------------------------------------------

SHA1::SHA1()
{
    reset();
}

SHA1::~SHA1()
{
    // The destructor does nothing
}



//This function will initialize the sha1 class member variables in preparation for computing a new message digest.
void SHA1::reset()
{
    m_length_Low          = 0;
    m_length_High         = 0;
    m_message_Block_Index = 0;

    m_h[0]        = 0x67452301;
    m_h[1]        = 0xEFCDAB89;
    m_h[2]        = 0x98BADCFE;
    m_h[3]        = 0x10325476;
    m_h[4]        = 0xC3D2E1F0;

    m_computed    = false;
    m_corrupted   = false;
}



/*  
 *  Description:
 *      This function will return the 160-bit message digest into the
 *      array provided.
 *
 *  Parameters:
 *      message_digest_array: [out]
 *          This is an array of five unsigned integers which will be filled
 *          with the message digest that has been computed.
 *
 *  Returns:
 *      True if successful, false if it failed.
 */
bool SHA1::final(Byte *message_digest_array)
{
    
    if (m_corrupted)
    {
        return false;
    }

    if (!m_computed)
    {
        _padMessage();
        m_computed = true;
    }

    for( Int32 i = 0; i < 5; i++)
    {
        m_h[i] = bswap_32(m_h[i]);
        ((UInt32 *)message_digest_array)[i] = m_h[i];
    }

    return true;
}



/*  
 *  _padMessage
 *
 *  Description:
 *      According to the standard, the message must be padded to an even
 *      512 bits.  The first padding bit must be a '1'.  The last 64 bits
 *      represent the length of the original message.  All bits in between
 *      should be 0.  This function will pad the message according to those
 *      rules by filling the message_block array accordingly.  It will also
 *      call _processMessageBlock() appropriately.  When it returns, it
 *      can be assumed that the message digest has been computed.
 */
void SHA1::_padMessage()
{
    /*
     *  Check to see if the current message block is too small to hold
     *  the initial padding bits and length.  If so, we will pad the
     *  block, process it, and then continue padding into a second block.
     */
    if (m_message_Block_Index > 55)
    {
        m_message_Block[m_message_Block_Index++] = 0x80;
        while(m_message_Block_Index < 64)
        {
            m_message_Block[m_message_Block_Index++] = 0;
        }

        _processMessageBlock();

        while(m_message_Block_Index < 56)
        {
            m_message_Block[m_message_Block_Index++] = 0;
        }
    }
    else
    {
        m_message_Block[m_message_Block_Index++] = 0x80;
        while(m_message_Block_Index < 56)
        {
            m_message_Block[m_message_Block_Index++] = 0;
        }

    }

    
    // Store the message length as the last 8 octets
    m_message_Block[56] = (m_length_High >> 24) & 0xFF;
    m_message_Block[57] = (m_length_High >> 16) & 0xFF;
    m_message_Block[58] = (m_length_High >> 8) & 0xFF;
    m_message_Block[59] = (m_length_High) & 0xFF;
    m_message_Block[60] = (m_length_Low >> 24) & 0xFF;
    m_message_Block[61] = (m_length_Low >> 16) & 0xFF;
    m_message_Block[62] = (m_length_Low >> 8) & 0xFF;
    m_message_Block[63] = (m_length_Low) & 0xFF;

    _processMessageBlock();
}



/*  
 *  update
 *
 *  Description:
 *      This function accepts an array of octets as the next portion of
 *      the message.
 *
 *  Parameters:
 *      message_array: [in]
 *          An array of characters representing the next portion of the
 *          message.
 */
void SHA1::update(const Byte *message_array, UInt32 length)
{
    if (!length)
    {
        return;
    }

    if (m_computed || m_corrupted)
    {
        m_corrupted = true;
        return;
    }

    while(length-- && !m_corrupted)
    {
        m_message_Block[m_message_Block_Index++] = (*message_array & 0xFF);

        m_length_Low += 8;
        m_length_Low &= 0xFFFFFFFF;               // Force it to 32 bits
        if (m_length_Low == 0)
        {
            m_length_High++;
            m_length_High &= 0xFFFFFFFF;          // Force it to 32 bits
            if (m_length_High == 0)
            {
                m_corrupted = true;               // Message is too long
            }
        }

        if (m_message_Block_Index == 64)
        {
            _processMessageBlock();
        }

        message_array++;
    }
}



/*  
 *  _processMessageBlock
 *
 *  Description:
 *      This function will process the next 512 bits of the message
 *      stored in the Message_Block array.
 *
 *  Comments:
 *      Many of the variable names in this function, especially the single
 *      character names, were used because those were the names used
 *      in the publication.
 *
 */
void SHA1::_processMessageBlock()
{
    const UInt32 K[] =    {               // Constants defined for SHA-1
                                0x5A827999,
                                0x6ED9EBA1,
                                0x8F1BBCDC,
                                0xCA62C1D6
                            };
    Int32     t = 0;                      // Loop counter
    UInt32    temp;                       // Temporary word value
    UInt32    W[80];                      // Word sequence
    UInt32    A, B, C, D, E;              // Word buffers

    /*
     *  Initialize the first 16 words in the array W
     */
    for(t = 0; t < 16; t++)
    {
        W[t] = ((UInt32) m_message_Block[t * 4]) << 24;
        W[t] |= ((UInt32) m_message_Block[t * 4 + 1]) << 16;
        W[t] |= ((UInt32) m_message_Block[t * 4 + 2]) << 8;
        W[t] |= ((UInt32) m_message_Block[t * 4 + 3]);
    }

    for(t = 16; t < 80; t++)
    {
       W[t] = _circularShift(1,W[t-3] ^ W[t-8] ^ W[t-14] ^ W[t-16]);
    }

    A = m_h[0];
    B = m_h[1];
    C = m_h[2];
    D = m_h[3];
    E = m_h[4];

    for(t = 0; t < 20; t++)
    {
        temp = _circularShift(5,A) + ((B & C) | ((~B) & D)) + E + W[t] + K[0];
        temp &= 0xFFFFFFFF;
        E = D;
        D = C;
        C = _circularShift(30,B);
        B = A;
        A = temp;
    }

    for(t = 20; t < 40; t++)
    {
        temp = _circularShift(5,A) + (B ^ C ^ D) + E + W[t] + K[1];
        temp &= 0xFFFFFFFF;
        E = D;
        D = C;
        C = _circularShift(30,B);
        B = A;
        A = temp;
    }

    for(t = 40; t < 60; t++)
    {
        temp = _circularShift(5,A) +
               ((B & C) | (B & D) | (C & D)) + E + W[t] + K[2];
        temp &= 0xFFFFFFFF;
        E = D;
        D = C;
        C = _circularShift(30,B);
        B = A;
        A = temp;
    }

    for(t = 60; t < 80; t++)
    {
        temp = _circularShift(5,A) + (B ^ C ^ D) + E + W[t] + K[3];
        temp &= 0xFFFFFFFF;
        E = D;
        D = C;
        C = _circularShift(30,B);
        B = A;
        A = temp;
    }

    m_h[0] = (m_h[0] + A) & 0xFFFFFFFF;
    m_h[1] = (m_h[1] + B) & 0xFFFFFFFF;
    m_h[2] = (m_h[2] + C) & 0xFFFFFFFF;
    m_h[3] = (m_h[3] + D) & 0xFFFFFFFF;
    m_h[4] = (m_h[4] + E) & 0xFFFFFFFF;

    m_message_Block_Index = 0;
}



/*  
 *  _circularShift
 *
 *  Description:
 *      This member function will perform a circular shifting operation.
 *
 *  Parameters:
 *      bits: [in]
 *          The number of bits to shift (1-31)
 *      word: [in]
 *          The value to shift (assumes a 32-bit integer)
 *
 *  Returns:
 *      The shifted value.
 */
UInt32 SHA1::_circularShift(Int32 bits, UInt32 word)
{
    return ((word << bits) & 0xFFFFFFFF) | ((word & 0xFFFFFFFF) >> (32-bits));
}


//--------------------------------------------------------------------------

