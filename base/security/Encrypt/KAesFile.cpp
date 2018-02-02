/* -------------------------------------------------------------------------
//  File Name   :   KAesFile.cpp
//  Author      :   Li Jiahan
//  Create Time :   2012-3-21 10:12:52
//  Description :   
//
// -----------------------------------------------------------------------*/
#include "../FileConfig/FileConfig.h"
#include "KAesFile.h"

#if defined (_MSC_VER)
#include <io.h>
#else
#include <sys/ioctl.h>
#endif /* _MSC_VER */


#include "../Misc/ErrorCheck.h"
#include "../Misc/FileHelper.h"

#include <iostream>
//#include <sys/stat.h>

#define _def_K_AES_HREADER_TOTAL_LEN                        64
#define _def_K_AES_RESERVE_LEN                              23
#define _def_K_AES_PADDING_LEN                              1
#define _def_K_AES_VERSION_LEN                              1

#define  _def_K_AES_OLD_MD5LEN								16

#define _def_K_AES_BLOCK_SIZE                               AES_BLOCK_SIZE
#define _def_READ_FILE_BLOCK_SIZE                           (1024 * 64)

#define _def_K_AES_HEADER_LEN									4
#define _def_K_AES_OLD_HEADER_LEN                               19
const Byte _g_fileHeader[_def_K_AES_HEADER_LEN] =				{'K', 'S', 'F', '0'};
const Byte _g_old_fileHeader[_def_K_AES_OLD_HEADER_LEN] =       {'K', 'i', 'n', 'g', 's', 'o', 'f', 't', '_', 'F', 'i', 'l', 'e', 'H', 'e', 'a', 'd', 'e', 'r'};


//-------------------------------------------------------------------------

namespace KAesFile
{
    bool encryptFile2(
        const CBoostPath& SrcFilePath, 
        const CBoostPath& DestFilePath, 
        bool       compressed,
        UInt32     orgFileSize, 
        const Byte orgFileSha1[def_K_AES_SHA1_LEN], 
        const Byte hint[def_K_AES_HINT_LEN], 
        const Byte fileKey[def_K_AES_FILE_KEY_LEN]
    )
    {
        XLIVE_ASSERT(!SrcFilePath.empty());
        XLIVE_ASSERT(!DestFilePath.empty());

        bool ret = true;

        FILE *pSrcFile      = NULL;
        FILE *pDestFile     = NULL;

        CAesAlg AesAlg;
        AesAlg.SetKey(fileKey, def_K_AES_FILE_KEY_LEN);

        Byte *pBufSrc       = NULL;
        Byte *pBufDest      = NULL;
        {
            Int32 tmpRet = 0;
            ERROR_CHECK_BOOLEX(SrcFilePath.exists(), ret = false);
            pDestFile = ::fopen_x(DestFilePath, "wb");
            ERROR_CHECK_BOOLEX(NULL != pDestFile, ret = false);

            // 写入文件头
            tmpRet = ::fwrite(_g_fileHeader, 1, _def_K_AES_HEADER_LEN, pDestFile);
            ERROR_CHECK_BOOLEX(_def_K_AES_HEADER_LEN == tmpRet, ret = false);

            // 写入文件版本
            const Byte fileVersion = 0x01;
            tmpRet = ::fwrite(&fileVersion, 1, _def_K_AES_VERSION_LEN, pDestFile);
            ERROR_CHECK_BOOLEX(_def_K_AES_VERSION_LEN == tmpRet, ret = false);

            // 写入文件特征
            Byte feature        = 0x00;
            bool encrypted      = true;     // 目前默认都加密
            if (compressed)
                feature |= (0x01 << 0);
            if (encrypted)
                feature |= (0x01 << 1);
            tmpRet = ::fwrite(&feature, 1, 1, pDestFile);
            ERROR_CHECK_BOOLEX(1 == tmpRet, ret = false);

            // 写入特征保留位
            Byte featureReserve[2] = {0};
            tmpRet = ::fwrite(featureReserve, 1, 2, pDestFile);
            ERROR_CHECK_BOOLEX(2 == tmpRet, ret = false);

            // 写入原始文件大小
            tmpRet = ::fwrite(&orgFileSize, 4, 1, pDestFile);
            ERROR_CHECK_BOOLEX(1 == tmpRet, ret = false);

            // 写入原始文件SHA1
            tmpRet = ::fwrite(orgFileSha1, 1, def_K_AES_SHA1_LEN, pDestFile);
            ERROR_CHECK_BOOLEX(def_K_AES_SHA1_LEN == tmpRet, ret = false);

            // 写入Hint
            tmpRet = ::fwrite(hint, 1, def_K_AES_HINT_LEN, pDestFile);
            ERROR_CHECK_BOOLEX(def_K_AES_HINT_LEN == tmpRet, ret = false);

            // 写入Padding
            UInt32 srcFileSize = 0;
            FileHelper::getFileSize(SrcFilePath, srcFileSize);
            XLIVE_ASSERT(srcFileSize);

            Byte mod = srcFileSize % 16;
            Byte padding = mod == 0 ? 0x00 : 16 - mod;
            tmpRet = ::fwrite(&padding, 1, _def_K_AES_PADDING_LEN, pDestFile);
            ERROR_CHECK_BOOLEX(_def_K_AES_PADDING_LEN == tmpRet, ret = false);

            // 写入保留位
            Byte reserve[_def_K_AES_RESERVE_LEN] = {0};
            tmpRet = ::fwrite(reserve, 1, _def_K_AES_RESERVE_LEN, pDestFile);
            ERROR_CHECK_BOOLEX(_def_K_AES_RESERVE_LEN == tmpRet, ret = false);

            // 分块读取原文件、加密、写入目标文件
            pSrcFile = ::fopen_x(SrcFilePath, "rb");
            ERROR_CHECK_BOOLEX(NULL != pSrcFile, ret = false);

            UInt32 readBytes = 0;

            pBufSrc     = (Byte *)new Byte[_def_READ_FILE_BLOCK_SIZE];
            pBufDest    = (Byte *)new Byte[_def_READ_FILE_BLOCK_SIZE];

            do 
            {
                // buffer清零
                ::memset(pBufSrc, 0, _def_READ_FILE_BLOCK_SIZE);
                ::memset(pBufDest, 0, _def_READ_FILE_BLOCK_SIZE);

                // 读取一块文件
                readBytes = ::fread(pBufSrc, 1, _def_READ_FILE_BLOCK_SIZE, pSrcFile);
                tmpRet = ::ferror(pSrcFile);
                ERROR_CHECK_BOOLEX(0 == tmpRet, ret = false);

                if (0 == readBytes)
                    break;

                Byte *pTempSrc  = pBufSrc;
                Byte *pTempDest = pBufDest;

                // 根据当前文件块大小确定需要加密的AES块数
                Int32 aesBlockCount = 0;
                if (_def_READ_FILE_BLOCK_SIZE == readBytes)
                    aesBlockCount = _def_READ_FILE_BLOCK_SIZE / _def_K_AES_BLOCK_SIZE;
                else
                {
                    ERROR_CHECK_BOOLEX(readBytes < _def_READ_FILE_BLOCK_SIZE, ret = false);
                    aesBlockCount = (readBytes / _def_K_AES_BLOCK_SIZE) + (readBytes % _def_K_AES_BLOCK_SIZE > 0 ? 1 : 0);
                    ERROR_CHECK_BOOLEX(aesBlockCount > 0, ret = false);
                }

                // 加密当前文件块
                for (Int32 i = 0; i < aesBlockCount; ++i, pTempSrc += _def_K_AES_BLOCK_SIZE, pTempDest += _def_K_AES_BLOCK_SIZE)
                    AesAlg.Aes_Encode32((UInt32*)pTempDest, (UInt32*)pTempSrc);

                // 写入加密后文件块
                tmpRet = ::fwrite(pBufDest, 1, _def_K_AES_BLOCK_SIZE * aesBlockCount, pDestFile);
                ERROR_CHECK_BOOLEX(_def_K_AES_BLOCK_SIZE * aesBlockCount == tmpRet, ret = false);
            } 
            while (_def_READ_FILE_BLOCK_SIZE == readBytes);
        }

Exit0:
        if (pDestFile)
            ::fclose(pDestFile);
        if (pSrcFile)
            ::fclose(pSrcFile);

        // 释放buffer
        if (pBufSrc)
            delete[] pBufSrc;
        if (pBufDest)
            delete[] pBufDest;

        return ret;
    }


    bool decryptFile2(
        const CBoostPath     &SrcFilePath, 
        const CBoostPath     &DestFilePath, 
        const Byte     fileKey[def_K_AES_FILE_KEY_LEN], 
        bool           &compressed
        )
    {
        XLIVE_ASSERT(!SrcFilePath.empty());
        XLIVE_ASSERT(!DestFilePath.empty());

        bool    ret = true;
        FILE    *pSrcFile   = NULL;
        FILE    *pDestFile  = NULL;

        Byte *pBufSrc   = NULL;
        Byte *pBufDest  = NULL;
        {
            CAesAlg AesAlg;
            AesAlg.SetKey(fileKey, def_K_AES_FILE_KEY_LEN);

            UInt32  tmpRet = 0;

            ERROR_CHECK_BOOLEX(SrcFilePath.exists(), ret = false);

            pSrcFile = ::fopen_x(SrcFilePath, "rb");
            ERROR_CHECK_BOOLEX(NULL != pSrcFile, ret = false);

            // 检查文件大小
            UInt32 srcFileSize;
            FileHelper::getFileSize(SrcFilePath, srcFileSize);

            // 检查文件头
            UInt32 readBytes = 0;
            Byte bufHeader[_def_K_AES_HEADER_LEN] = {0};
            readBytes = ::fread(bufHeader, 1, _def_K_AES_HEADER_LEN, pSrcFile);
            ERROR_CHECK_BOOLEX(_def_K_AES_HEADER_LEN == readBytes, ret = false);

            // 是否是K-AES文件文件
            if (srcFileSize < _def_K_AES_HREADER_TOTAL_LEN                      ||
                0 != ::memcmp(_g_fileHeader, bufHeader, _def_K_AES_HEADER_LEN))
            {
                ret = false;
                goto Exit0;
            }

            // 检查文件版本
            Byte fileVersion = 0x00;
            tmpRet = ::fread(&fileVersion, 1, 1, pSrcFile);
            ERROR_CHECK_BOOLEX(1 == tmpRet, ret = false);

            // 检查文件特征
            Byte feature = 0x00;
            tmpRet = ::fread(&feature, 1, 1, pSrcFile);
            ERROR_CHECK_BOOLEX(1 == tmpRet, ret = false);

            // 跳过特征保留位
            ::fseek(pSrcFile, 2, SEEK_CUR);

            compressed = (feature & (0x01 << 0)) != 0;
            bool encrypted = false;
            encrypted = (feature & (0x01 << 1)) != 0;
            ERROR_CHECK_BOOLEX(encrypted, ret = false);

            // 跳过原始文件大小
            tmpRet = ::fseek(pSrcFile, 4, SEEK_CUR);
            ERROR_CHECK_BOOLEX(0 == tmpRet, ret = false);

            // 跳过原始文件SHA1
            tmpRet = ::fseek(pSrcFile, def_K_AES_SHA1_LEN, SEEK_CUR);
            ERROR_CHECK_BOOLEX(0 == tmpRet, ret = false);

            // 跳过Hint
            tmpRet = ::fseek(pSrcFile, def_K_AES_HINT_LEN, SEEK_CUR);
            ERROR_CHECK_BOOLEX(0 == tmpRet, ret = false);

            // 读取Padding
            Byte padding = 0;
            tmpRet = ::fread(&padding, 1, _def_K_AES_PADDING_LEN, pSrcFile);
            ERROR_CHECK_BOOLEX(_def_K_AES_PADDING_LEN == tmpRet, ret = false);

            // 跳过保留位
            tmpRet = ::fseek(pSrcFile, _def_K_AES_RESERVE_LEN, SEEK_CUR);
            ERROR_CHECK_BOOLEX(0 == tmpRet, ret = false);


            // 打开文件
            pDestFile = ::fopen_x(DestFilePath, "wb");

            // 申请buffer
            pBufSrc     = (Byte *)new Byte[_def_READ_FILE_BLOCK_SIZE];
            pBufDest    = (Byte *)new Byte[_def_READ_FILE_BLOCK_SIZE];

            // 分块读取、解密文件
            do 
            {
                // buffer清零
                ::memset(pBufSrc, 0, _def_READ_FILE_BLOCK_SIZE);
                ::memset(pBufDest, 0, _def_READ_FILE_BLOCK_SIZE);

                readBytes = ::fread(pBufSrc, 1, _def_READ_FILE_BLOCK_SIZE, pSrcFile);
                if (0 == readBytes)
                    break;
                ERROR_CHECK_BOOLEX(0 == (readBytes % _def_K_AES_BLOCK_SIZE), ret = false);

                Byte *pTempSrc  = pBufSrc;
                Byte *pTempDest = pBufDest;

                // 根据当前文件块大小确定需要加密的AES块数
                UInt32  aesBlockCount = 0;
                if (_def_READ_FILE_BLOCK_SIZE == readBytes)
                    aesBlockCount = _def_READ_FILE_BLOCK_SIZE / _def_K_AES_BLOCK_SIZE;
                else
                {
                    ERROR_CHECK_BOOLEX(readBytes < _def_READ_FILE_BLOCK_SIZE, ret = false);
                    aesBlockCount = (readBytes / _def_K_AES_BLOCK_SIZE);
                    ERROR_CHECK_BOOLEX(aesBlockCount > 0, ret = false);
                }

                // 解密当前文件块
                for (UInt32 i = 0; i < aesBlockCount; ++i, pTempSrc += _def_K_AES_BLOCK_SIZE, pTempDest += _def_K_AES_BLOCK_SIZE)
                    AesAlg.Aes_Decode32((UInt32*)pTempDest, (UInt32*)pTempSrc);

                // 写入解密后文件块
                UInt32 writeBytes = 0;
                writeBytes = ::fwrite(pBufDest, 1, _def_K_AES_BLOCK_SIZE * aesBlockCount, pDestFile);
                ERROR_CHECK_BOOLEX( (_def_K_AES_BLOCK_SIZE * aesBlockCount) == writeBytes, ret = false);
            } 
            while (_def_READ_FILE_BLOCK_SIZE == readBytes);

            // 丢弃文件末尾的对齐位
            ::fflush(pDestFile);
#if defined (_MSC_VER)
            tmpRet = ::_chsize(_fileno(pDestFile), srcFileSize - padding - _def_K_AES_HREADER_TOTAL_LEN);
#else
            tmpRet = ::ftruncate(fileno(pDestFile), srcFileSize - padding - _def_K_AES_HREADER_TOTAL_LEN);
#endif /* _MSC_VER */

            ERROR_CHECK_BOOLEX(0 == tmpRet, ret = false);

        }


Exit0:
        if (pDestFile)
            ::fclose(pDestFile);
        if (pSrcFile)
            ::fclose(pSrcFile);

        if (pBufSrc)
            delete[] pBufSrc;
        if (pBufDest)
            delete[] pBufDest;

        return ret;
    }

	bool isNewFileFormat(const CBoostPath& blockPath)
	{
		bool isNewFormat = true;
		FILE* pSrcFile = NULL;
		{
			pSrcFile = ::fopen_x(blockPath, "rb");
			ERROR_CHECK_BOOL(NULL != pSrcFile);

			UInt32 readBytes = 0;
			Byte bufHeader[_def_K_AES_HEADER_LEN] = {0};
			readBytes = ::fread(bufHeader, 1, _def_K_AES_HEADER_LEN, pSrcFile);
			ERROR_CHECK_BOOL(_def_K_AES_HEADER_LEN == readBytes);

			if (0 != ::memcmp(_g_fileHeader, bufHeader, _def_K_AES_HEADER_LEN))
			{
				::fseek(pSrcFile, 0L, SEEK_SET);
				Byte bufHeader[_def_K_AES_OLD_HEADER_LEN] = {0};
				readBytes = ::fread(bufHeader, 1, _def_K_AES_OLD_HEADER_LEN, pSrcFile);
				ERROR_CHECK_BOOL(_def_K_AES_OLD_HEADER_LEN == readBytes);
				if (0 == ::memcmp(_g_old_fileHeader, bufHeader, _def_K_AES_OLD_HEADER_LEN))
					isNewFormat = false;
			}
		}
Exit0:
		if (pSrcFile)
			::fclose(pSrcFile);
		return isNewFormat;
	}

#ifdef _MSC_VER
	bool decryptOldFormatFile(
		const CBoostPath  &SrcFilePath, 
		const CBoostPath  &DestFilePath,
		const Byte  fileKey[def_K_AES_FILE_KEY_LEN],
		bool        &compressed
		)
	{
		XLIVE_ASSERT(!SrcFilePath.empty());
		XLIVE_ASSERT(!DestFilePath.empty());

		bool    ret = true;
		FILE    *pSrcFile   = NULL;
		FILE    *pDestFile  = NULL;

		Byte *pBufSrc   = NULL;
		Byte *pBufDest  = NULL;
		{
			CAesAlg AesAlg;
			AesAlg.SetKey(fileKey, def_K_AES_FILE_KEY_LEN);

			UInt32  tmpRet = 0;

			ERROR_CHECK_BOOLEX(SrcFilePath.exists(), ret = false);

			pSrcFile = ::fopen_x(SrcFilePath, "rb");
			ERROR_CHECK_BOOLEX(NULL != pSrcFile, ret = false);

			// 检查文件大小
			UInt32 srcFileSize;
			FileHelper::getFileSize(SrcFilePath, srcFileSize);

			// 检查文件头
			UInt32 readBytes = 0;
			Byte bufHeader[_def_K_AES_OLD_HEADER_LEN] = {0};
			readBytes = ::fread(bufHeader, 1, _def_K_AES_OLD_HEADER_LEN, pSrcFile);
			ERROR_CHECK_BOOLEX(_def_K_AES_OLD_HEADER_LEN == readBytes, ret = false);

			// 是否是K-AES文件文件
			if (0 != ::memcmp(_g_old_fileHeader, bufHeader, _def_K_AES_OLD_HEADER_LEN))
			{
				ret = false;
				goto Exit0;
			}

			// 检查文件版本
			Byte fileVersion[4] = {0};
			tmpRet = ::fread(&fileVersion, 1, 4, pSrcFile);
			ERROR_CHECK_BOOLEX(4 == tmpRet, ret = false);

			// 读取文件md5
			Byte fileMd5[_def_K_AES_OLD_HEADER_LEN] = {0};
			tmpRet = ::fread(&fileMd5, 1, _def_K_AES_OLD_MD5LEN, pSrcFile);
			ERROR_CHECK_BOOLEX(16 == tmpRet, ret = false);

			// 读取原始文件大小
			DWORD fileOrgSize = 0;
			tmpRet = ::fread(&fileOrgSize, 1, 4, pSrcFile);
			ERROR_CHECK_BOOLEX(4 == tmpRet, ret = false);

			// 读取加密文件大小
			Byte fileEncrptedSize[4] = {0};
			tmpRet = ::fread(&fileEncrptedSize, 1, 4, pSrcFile);
			ERROR_CHECK_BOOLEX(4 == tmpRet, ret = false);

			// 打开文件
			pDestFile = ::fopen_x(DestFilePath, "wb");

			// 申请buffer
			pBufSrc     = (Byte *)new Byte[_def_READ_FILE_BLOCK_SIZE];
			pBufDest    = (Byte *)new Byte[_def_READ_FILE_BLOCK_SIZE];

			// 分块读取、解密文件
			do 
			{
				// buffer清零
				::memset(pBufSrc, 0, _def_READ_FILE_BLOCK_SIZE);
				::memset(pBufDest, 0, _def_READ_FILE_BLOCK_SIZE);

				readBytes = ::fread(pBufSrc, 1, _def_READ_FILE_BLOCK_SIZE, pSrcFile);
				if (0 == readBytes)
					break;
				ERROR_CHECK_BOOLEX(0 == (readBytes % _def_K_AES_BLOCK_SIZE), ret = false);

				Byte *pTempSrc  = pBufSrc;
				Byte *pTempDest = pBufDest;

				// 根据当前文件块大小确定需要加密的AES块数
				UInt32  aesBlockCount = 0;
				if (_def_READ_FILE_BLOCK_SIZE == readBytes)
					aesBlockCount = _def_READ_FILE_BLOCK_SIZE / _def_K_AES_BLOCK_SIZE;
				else
				{
					ERROR_CHECK_BOOLEX(readBytes < _def_READ_FILE_BLOCK_SIZE, ret = false);
					aesBlockCount = (readBytes / _def_K_AES_BLOCK_SIZE);
					ERROR_CHECK_BOOLEX(aesBlockCount > 0, ret = false);
				}

				// 解密当前文件块
				for (UInt32 i = 0; i < aesBlockCount; ++i, pTempSrc += _def_K_AES_BLOCK_SIZE, pTempDest += _def_K_AES_BLOCK_SIZE)
					AesAlg.Aes_Decode32((UInt32*)pTempDest, (UInt32*)pTempSrc);

				// 写入解密后文件块
				UInt32 writeBytes = 0;
				writeBytes = ::fwrite(pBufDest, 1, _def_K_AES_BLOCK_SIZE * aesBlockCount, pDestFile);
				ERROR_CHECK_BOOLEX( (_def_K_AES_BLOCK_SIZE * aesBlockCount) == writeBytes, ret = false);
			} 
			while (_def_READ_FILE_BLOCK_SIZE == readBytes);

			// 丢弃文件末尾的对齐位
			::fflush(pDestFile);
			tmpRet = ::_chsize(_fileno(pDestFile), fileOrgSize);
			ERROR_CHECK_BOOLEX(0 == tmpRet, ret = false);
		}


Exit0:
		if (pDestFile)
			::fclose(pDestFile);
		if (pSrcFile)
			::fclose(pSrcFile);

		if (pBufSrc)
			delete[] pBufSrc;
		if (pBufDest)
			delete[] pBufDest;

		return ret;
	}
#endif

}

//--------------------------------------------------------------------------