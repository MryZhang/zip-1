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

bool Zip(const char * src, const char * dst, ZipFunc zip)
{
	bool ret = false;
	FILE	* source, * dest;
	source = fopen( src, "rb" );
	if( source )
	{
		dest =fopen( dst, "wb" );
		if( dest )
		{
			ret = (0 == zip( source, dest ));
			fclose( dest );
		}
		fclose( source );

	}
	return ret;
}

void fzip(int argc, char * argv[])
{
	int err = 0;
	char filename[FILENAME_MAX]={0}, *pExt=0;
	switch( argc )
	{
		case 1:
			fputs("give source file name & dest zip file name to compress; source zip file name to uncompress!\n", stderr);
			break;
		case 2:
			fputs("1 parameter", stderr);
			strcpy(filename, argv[1]);
			pExt = ( strchr(filename, '.') );
			strcpy(pExt, ".zip");
			printf("filename=%s.\n", filename);
			err = Zip( argv[1], filename, [](FILE * src, FILE * dst){ return def(src, dst); } );
			if( err )
				zerr(err);
			break;
		case 3:
			err = Zip( argv[1], argv[2], inf );
			if( err )
				zerr(err);
			break;
		default:
			fputs("too many parameters given!\n", stderr);
			break;
	}
}

int main(int argc, char * argv[])
{
	fputs("hello", stderr);
	fzip(argc, argv);
	return 0;
}

