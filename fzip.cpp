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
#include <string.h>
#include <functional>

typedef std::function<int(FILE*,FILE*)> ZipFunc;

int Zip(const char * src, const char * dst, ZipFunc zip)
{
	bool ret = false;
	FILE	* source, * dest;
	source = fopen( src, "rb" );
	if( source )
	{
		dest =fopen( dst, "wb" );
		if( dest )
		{
			ret = zip( source, dest );
			fclose( dest );
		}
		fclose( source );

	}
	return ret;
}

void fzip(int argc, char * argv[],int level = -1)
{
	int err = 0;
	char filename[FILENAME_MAX]={0}, *pExt=0;
	switch( argc )
	{
		case 1:
			fputs("give source file name & dest zip file name to compress; source zip file name to uncompress!\n", stderr);
			break;
		// 压缩 源文件 ( fzip.exe srcFileName.ext ) 
		// 生成 srcFileName.zip
		case 2:
			strcpy(filename, argv[1]);
			pExt = ( strchr(filename, '.') );
			pExt = pExt ? pExt : filename + strlen(filename);
			strcpy(pExt, ".zip");
			err = Zip( argv[1], filename, [ level ](FILE * src, FILE * dst){ return def(src, dst, level); } );
			if( err )
				zerr(err);
			else
				fprintf(stderr, "compress %s to %s success.\n", argv[1], filename);
			break;
		// 解压 压缩文件.zip 目标文件 ( fzip.exe srcFileName.zip dstFileName )
		case 3:
			err = Zip( argv[1], argv[2], inf );
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

