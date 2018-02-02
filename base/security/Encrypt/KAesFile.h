/* -------------------------------------------------------------------------
//  File Name   :   KAesFile.h
//  Author      :   Li Jiahan
//  Create Time :   2012-3-21 10:19:31
//  Description :
//
// -----------------------------------------------------------------------*/

#ifndef __KAESFILE_H__
#define __KAESFILE_H__

#include "libxlive/CrossPlatformConfig.h"
#include "AesAlg.h"
#include "../FileSystem/Path/BoostPath.h"

#define def_K_AES_SHA1_LEN                                  20
#define def_K_AES_HINT_LEN                                  8
#define def_K_AES_FILE_KEY_LEN                              32

typedef enum
{
    FCI_COMPRESSED = 0,
    FCI_UNCOMPRESSED,
    FCI_UNKNOWN
} FileCompressInfo;

//-------------------------------------------------------------------------
namespace KAesFile
{
   bool encryptFile2(
        const CBoostPath  &pSrcFilePath, 
        const CBoostPath  &pDestFilePath, 
        bool        compressed, 
        UInt32      orgFileSize,
        const Byte  orgFileSha1[def_K_AES_SHA1_LEN], 
        const Byte  hint[def_K_AES_HINT_LEN], 
        const Byte  fileKey[def_K_AES_FILE_KEY_LEN]
    );
   bool decryptFile2(
        const CBoostPath  &pSrcFilePath, 
        const CBoostPath  &pDestFilePath,
        const Byte  fileKey[def_K_AES_FILE_KEY_LEN],
        bool        &compressed
        );

   bool isNewFileFormat(const CBoostPath& blockPath);

#ifdef _MSC_VER
   bool decryptOldFormatFile(
	   const CBoostPath  &SrcFilePath, 
	   const CBoostPath  &DestFilePath,
	   const Byte  fileKey[def_K_AES_FILE_KEY_LEN],
	   bool        &compressed
	   );
#endif
};

//--------------------------------------------------------------------------
#endif /* __KAESFILE_H__ */