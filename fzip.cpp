/**************************************************************
 * @brief	解压zip文件
 * 
 * 
 * @file	fzip.cpp
 * @path	prj/zip
 * @author	fstone.zh@foxmail.com
 * @date	2016-11-29
 * @version	0.1.0
*************************************************************/
#include "zdemo.h"

void fzip(int argc, char * argv[],int level = -1)
{
	int err = 0;
	switch( argc )
	{
		case 1:
			fputs("give source file name & dest zip file name to compress; source zip file name to uncompress!\n", stderr);
			break;
		// 压缩 源文件 ( fzip.exe srcFileName.ext ) 
		// 生成 srcFileName.zip
		case 2:
			err = CompressFile( argv[1] );
			if( err )
				zerr(err);
			else
				fprintf(stderr, "compress %s success.\n", argv[1]);
			break;
		// 解压 压缩文件.zip 目标文件 ( fzip.exe srcFileName.zip dstFileName )
		case 3:
			err = DecompressFile( argv[1], argv[2]);
			if( err )
				zerr(err);
			else
				fprintf(stderr, "uncompress to %s from %s success.\n", argv[2], argv[1]);
			break;
		default:
			fputs("too many parameters given!\n", stderr);
			break;
	}
}

int main(int argc, char * argv[])
{
	fzip(argc, argv);
	return 0;
}

