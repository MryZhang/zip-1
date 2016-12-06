/**************************************************************
 * @brief	解压zip文件
 * 
 * 
 * @file	main.cpp
 * @author	fstone.zh@foxmail.com
 * @date	2016-12-06
 * @version	0.1.1
*************************************************************/
#include "fzip.h"
#include <unistd.h>
#include <getopt.h>

// 版本号 更新日期
#define VERSION		"1.1"
#define LASTUPDATE	"2016-12-06"

// 帮助信息
#define HELP_MSG 	"use fzip:\n\
\t\t\tdecompress file.fz to file by command:\n\
\t\t\t\t$./fzip file.fz [file]\n\
options:\n\
\t-r\t\tcompress file to file.fz by command:\n\
\t\t\t\t$./fzip -r file [file.fz]\n\
\t-f\t\tforced fzip, overwrite the existed file\n\
\t-d\t\tlock [file] for [N] seconds.it's excluded lock when [-r] given as below, otherwise shared lock\n\
\t\t\t\t$./fzip -r file -d N\n\
\t-h, --help\tshow help message\n\
\t-v, --version\tshow version informations"

// 版本信息
#define VERSION_MSG	"fzip version:\t%s\n\
last update:\t%s\n", VERSION, LASTUPDATE

// more
#define MORE_INFO	printf("\tMore infor at http://confluence.oa.zulong.com/pages/viewpage.action?pageId=6526298\n")


// fzip 命令行工具
void fzip(int argc, char * argv[],int level = -1)
{
	enum {	DECOMPRESS, COMPRESS };
	int	optCnt = 0, todo = DECOMPRESS;	// 缺省执行解压
	bool	overwrite = false;		// 缺省不覆盖现存文件
	char	opt = -1;
	const char *	short_opts = "hvrfd:";
	struct option	long_opts[] = {
		"help",		no_argument,	0,	'h',
		"version",	no_argument,	0,	'v'
	};
	int optIdx = overwrite, delay = 0;
	while( -1 != (opt = getopt_long(argc, argv, short_opts, long_opts, &optIdx )) )
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
				MORE_INFO;
				return;
			case 'v':
				printf(VERSION_MSG);
				MORE_INFO;
				return;
			case 'd': // time delay
				delay = atoi(optarg);
				optCnt++;
				break;
			case '?': // 非法参数
				break;
		}
	}

	const char * file1=0, * file2=0;
	switch( argc - optCnt )
	{
		case 2: // fzip [options] file1
			file1 = argv[ optCnt + 1 ];
			break;
		case 3: // fzip [options] file1 file2
			file1 = argv[ optCnt + 1 ];
			file2 = argv[ optCnt + 2 ];
			break;
		default:
			fputs("Try './fzip --help' for more information.\n", stderr);
			return;
	}

	int err = 0;
	// 压缩
	if( COMPRESS == todo )
	{
		if(0 < delay)
		{
			FZip::Writer wr(file1);
			if( -1 == wr.Lock())
				printf("write lock error\n");
			else
				printf("write lock ok\n");
			sleep(delay);
			wr.Unlock();
		}
		else
		{
			err = FZip::CompressFile( file1, file2, overwrite);
			if( err )
				FZip::zerr(err);
			else
				fprintf(stderr, "compress %s from %s success.\n", file2 ? file2 : "", file1); 
		}
	}
	// 解压
	else if( DECOMPRESS == todo )
	{
		if( 0 < delay )
		{
			FZip::Reader rd(file1);
			if( -1 == rd.Lock() )
				printf("read lock error\n");
			else
				printf("read lock ok\n");
			sleep(delay);
			rd.Unlock();
		}
		else
		{
			err = FZip::DecompressFile( file1, file2, overwrite );
			if( err )
				FZip::zerr(err);
			else
				fprintf(stderr, "decompress %s from %s success.\n", file2 ? file2:"", file1);
		}
	}
}

int main(int argc, char * argv[])
{
	fzip(argc, argv);
	return 0;
}

