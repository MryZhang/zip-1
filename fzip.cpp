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
#include <unistd.h>


void fzip(int argc, char * argv[],int level = -1)
{
	enum {	DECOMPRESS, COMPRESS };
	int	optCnt=0, todo = DECOMPRESS;
	char	ch;
	while( -1 != (ch = getopt(argc, argv, "r")) )
	{
		optCnt++;
		switch( ch )
		{
			case 'r': // 压缩标志
				todo = COMPRESS;
				break;
			case '?': // 非法参数
				break;
		}
	}

	const char * src=0, * dst=0;
	switch( argc - optCnt )
	{
		case 2: // src
			src = argv[ optCnt + 1 ];
			break;
		// 解压 -r 压缩文件 目标文件 ( fzip.exe srcFileName.zip dstFileName )
		case 3: // src dst
			src = argv[ optCnt + 1 ];
			dst = argv[ optCnt + 2 ];
			break;
		default:
			fputs("Useage: [-r] sourceFile [destFile]\n", stderr);
			return;
	}

	int err = 0;
	// 压缩
	if( COMPRESS == todo )
	{
		err = FZip::CompressFile( src, dst );
		if( err )
			FZip::zerr(err);
		else
			fprintf(stderr, "compress %s from %s success.\n", dst?dst:"", src); 
	}
	// 解压
	else if( DECOMPRESS == todo )
	{
		err = FZip::DecompressFile( argv[1], argv[2]);
		if( err )
			FZip::zerr(err);
		else
			fprintf(stderr, "decompress %s from %s success.\n", dst?dst:"", src);
	}
}

int main(int argc, char * argv[])
{
	fzip(argc, argv);
	return 0;
}

