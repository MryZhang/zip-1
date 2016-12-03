/**************************************************************
 * @brief	zlib 提供的例子
 * 
 * 
 * @file	zdemo.h
 * @path	prj/zip
 * @author	fstone.zh@foxmail.com
 * @date	2016-11-30
 * @version	0.1.0
*************************************************************/
#include <stdio.h>

namespace FZip{

#define FZIP_EXT	".fz"

// 压缩文件
int CompressFile(const char * srcFile, const char * zipFile=0, int level=-1);

// 解压文件
int DecompressFile(const char * zipFile, const char * dstFile = 0);

// 报错信息
void zerr(int ret);

}; // namespace Fzip
