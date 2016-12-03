/**************************************************************
 * @brief	zlib 压缩解压程序
 * 
 * 
 * @file	fzip.h
 * @author	fstone.zh@foxmail.com
 * @date	2016-12-03
 * @version	0.1.0
 * @see		http://confluence.oa.zulong.com/pages/viewpage.action?pageId=6526298
*************************************************************/
#pragma once
#include <stdio.h>

namespace FZip{

#define FZIP_EXT	".fz"

// 压缩文件
int CompressFile(const char * srcFile, const char * zipFile=0, bool overwrite = false, int level=-1);

// 解压文件
int DecompressFile(const char * zipFile, const char * dstFile = 0, bool overwrite = false);

// 错误号意义
void zerr(int errorCode);

}; // namespace FZip
