/**************************************************************
 * @brief	解压zip文件
 * 
 * 
 * @file	main.cpp
 * @author	fstone.zh@foxmail.com
 * @date	2016-12-03
 * @version	0.1.0
*************************************************************/
#include "fzip.h"
#include <unistd.h>
#include <getopt.h>

// 版本号 更新日期
#define VERSION		"1.0"
#define LASTUPDATE	"2016-12-03"

// 帮助信息
#define HELP_MSG 	"use fzip:\n\
\t\t\tdecompress file.fz to file by command:\n\
\t\t\t\t$./fzip file.fz [file]\n\
options:\n\
\t-r\t\tcompress file to file.fz by command:\n\
\t\t\t\t$./fzip -r file [file.fz]\n\
\t-f\t\tforced fzip, overwrite the existed file\n\
\t-h, --help\tshow help message\n\
\t-v, --version\tshow version informations"

// 版本信息
#define VERSION_MSG	"fzip version:\t%s\n\
last update:\t%s\n", VERSION, LASTUPDATE

// fzip 命令行工具
void fzip(int argc, char * argv[],int level = -1)
{
	enum {	DECOMPRESS, COMPRESS };
	int	optCnt = 0, todo = DECOMPRESS;	// 缺省执行解压
	bool	overwrite = false;		// 缺省不覆盖现存文件
	char	opt = -1;
	const char *	short_opts = "hvrf";
	struct option	long_opts[] = {
		"help",		no_argument,	0,	'h',
		"version",	no_argument,	0,	'v'
	};
	while( -1 != (opt = getopt_long(argc, argv, short_opts, long_opts, 0 )) )
	{
		optCnt++;
		switch( opt )
		{
			case 'r': // 压缩标志
				todo = COMPRESS;
				break;
			case 'f': // 强制执行，覆盖已存在文件
				overwrite = true;
				break;
			case 'h':
				printf("%s\n", HELP_MSG);
				return;
			case 'v':
				printf(VERSION_MSG);
				return;
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
		// 解压 -r 压缩文件 目标文件 ( fzip srcFileName.zip dstFileName )
		case 3: // src dst
			src = argv[ optCnt + 1 ];
			dst = argv[ optCnt + 2 ];
			break;
		default:
			fputs("Try './fzip --help' for more information.\n", stderr);
			return;
	}

	int err = 0;
	// 压缩
	if( COMPRESS == todo )
	{
		err = FZip::CompressFile( src, dst, overwrite);
		if( err )
			FZip::zerr(err);
		else
			fprintf(stderr, "compress %s from %s success.\n", dst?dst:"", src); 
	}
	// 解压
	else if( DECOMPRESS == todo )
	{
		err = FZip::DecompressFile( src, dst, overwrite );
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

