/**************************************************************
 * @brief	zlib 压缩解压程序
 * 
 * 
 * @file	fzip.cpp
 * @author	fstone.zh@foxmail.com
 * @date	2016-12-03
 * @version	0.1.0
 * @see		http://confluence.oa.zulong.com/pages/viewpage.action?pageId=6526298
*************************************************************/
#include "fzip.h"
#include <string.h>
#include <assert.h>
#include <zlib.h>
#include <functional>
#include <sys/file.h>

namespace FZip {

#define CHUNK 262144 // 256k ( 64K 128K 256K )

/**
 * @brief	压缩source文件至其文件尾标志EOF，输出到压缩文件dest。
 * 
 * 输出压缩文件为zlib格式（不同于gzip/zip）：前2字节是zlib标志和解码信息；后4字节是解压后文件的快速校验和。
 * @param	source	源文件
 * @param	dest	输出文件
 * @param	level	压缩水平 (1-9) ：越小压缩速度越快、压缩率越小；越大，压缩速度越慢，压缩率越大；
 * 			推荐折中值 Z_DEFAULT_COMPRESSION=-1，相当于取6的水平；取 0 时不压缩。
 * @return	Z_OK		成功
 * 		Z_MEM_ERROR	内存申请失败
 * 		Z_STREAM_ERROR	非法的level
 * 		Z_VERSION_ERROR	版本不一致的 zlib.h 和 libz.a
 * 		Z_ERRNO		文件读写错误
*/
int def(FILE *source, FILE *dest, int level)
{
	int ret,			/** zlib返回值 */
	    flush;			/** 当前 deflate() 的 flushing 状态: Z_FINISH 读到文件尾，通知 deflate() 进行 flush 以完成读入工作；Z_NO_FLUSH 不进行 flushing */
	unsigned have;			/** deflate() 返回的值 */
	z_stream strm;			/** zlib 传递数据信息的结构，包含输入/输出信息、压缩引擎状态、flush方法、 */
	unsigned char in[CHUNK];	/** 输入buffer */
	unsigned char out[CHUNK];	/** 输出buffer */

	// 初始化
	strm.zalloc = Z_NULL;     	/** 使用缺省内存申请函数 */
	strm.zfree = Z_NULL;      	/** 使用缺省内存释放函数 */
	strm.opaque = Z_NULL;     	/** TODO */
	ret = deflateInit(&strm, level);/** 初始化压缩过程 */
	if (ret != Z_OK)
		return ret;

	// 循环 [读入、压缩] 到文件结束
	do {
		// 读入源文件
		strm.avail_in = fread(in, 1, CHUNK, source);
		if (ferror(source))
		{
			(void)deflateEnd(&strm);
			return Z_ERRNO;
		}
		flush = feof(source) ? Z_FINISH : Z_NO_FLUSH;
		strm.next_in = in;

		// 压缩
		do {
			strm.avail_out = CHUNK;	/** 可用输出缓存大小 */
			strm.next_out = out;	/** 输出位置 */

			/** 压缩引擎接口
			 * 尽可能多地 (<=avail_in) 从 next_in 取数据并更新 avail_in 和 next_in，
			 * 尽可能多地 (<=avail_out) 写出到 next_out */
			ret = deflate(&strm, flush);
			/** 返回值
			 * Z_BUF_ERROR		保存压缩引擎状态的内存被破坏
			 * Z_STREAM_ERROR	z_stream 没有被正确初始化
			 * Z_STREAM_END		压缩结束，最后一次调用 deflate()
			 * Z_OK 		成功*/
			assert(ret != Z_STREAM_ERROR); /** 断言：没有发生 Z_SREAM_ERROR */
			/** 写出压缩后的数据 */
			have = CHUNK - strm.avail_out;
			if (fwrite(out, 1, have, dest) != have || ferror(dest)) 
			{
				(void)deflateEnd(&strm);
				return Z_ERRNO;
			}

		} while (strm.avail_out == 0);	/** 直到压缩数据不足一个 CHUNK 大小，表示已经压缩完读入的数据 */
		assert(strm.avail_in == 0);	/** 断言：剩余可压缩数据应为 0 */

	} while(flush != Z_FINISH);   		/** 循环读取压缩一个CHUNK的源文件，直到 flush 的值为 Z_FINISH，表示文件结束 */
	assert(ret == Z_STREAM_END);  		/** 断言：文件读取结束 */

	// 清理资源
	(void)deflateEnd(&strm);
	return Z_OK;
}


/**
 * @brief	解压 source 文件至 dest 文件
 * 
 * 
 * @param	source		压缩文件
 * @param	dest		解压后的文件
 * @return	Z_OK		成功
 * 		Z_MEM_ERROR	内存错误
 * 		Z_DATA_ERROR	压缩文件source非法或不完整
 * 		Z_VERSION_ERROR	版本错误(zlib.h和libz.so版本不一致)
 * 		Z_ERRNO		文件读写错误
 * 		Z_NEED_DICT	需要字典(当压缩前调用deflateSetDictionary()则会在压缩文件头(前32K)上记录字典),将报Z_DATA_ERROR
*/
int inf(FILE *source, FILE *dest)
{
	int ret;
	unsigned have;
	z_stream strm;
	unsigned char in[CHUNK];
	unsigned char out[CHUNK];
	
	// 初始化
	strm.zalloc = Z_NULL;
	strm.zfree = Z_NULL;
	strm.opaque = Z_NULL;
	strm.avail_in = 0;
	strm.next_in = Z_NULL;
	ret = inflateInit(&strm);
	if (ret != Z_OK)
		return ret;

	// 解压直到被压缩的源文件结束
	do {
		strm.avail_in = fread(in, 1, CHUNK, source);
		if (ferror(source))
		{
			(void)inflateEnd(&strm);
			return Z_ERRNO;
		}
		// 文件不完整，报错
		if (strm.avail_in == 0)
			break;
		strm.next_in = in;

		// 解压
		do {
			strm.avail_out = CHUNK;
			strm.next_out = out;
			
			// 解压引擎接口
			ret = inflate(&strm, Z_NO_FLUSH);
			assert(ret != Z_STREAM_ERROR);
			switch (ret)
			{
				case Z_NEED_DICT:
					ret = Z_DATA_ERROR;
				case Z_DATA_ERROR:
				case Z_MEM_ERROR:
					(void)inflateEnd(&strm);
					return ret;
			}
			// 写入文件
			have = CHUNK - strm.avail_out;
			if (fwrite(out, 1, have, dest) != have || ferror(dest)) 
			{
				(void)inflateEnd(&strm);
				return Z_ERRNO;
			}

		} while (strm.avail_out == 0); // 当解压数据未能填满一个CHUNK时，证明已经完成本次读入数据的解压
		
	} while (ret != Z_STREAM_END); // 源文件未结束，则继续读入

	// 清理资源
	(void)inflateEnd(&strm);
	return ret == Z_STREAM_END ? Z_OK : Z_DATA_ERROR;
}

// 自定义错误号
enum FZ_ERROR_NUM {
	FZ_DFT_ERROR = 1024,
	FZ_FILE_EXIST
};

// 查询错误信息
void zerr(int errorCode)
{
	switch (errorCode)
	{
		case Z_ERRNO:
			fputs("zlib error: I/O\n", stderr);
			break;
		case Z_STREAM_ERROR:
			fputs("zlib error: invalid compression level\n", stderr);
			break;
		case Z_DATA_ERROR:
			fputs("zlib error: invalid or incomplete deflate data\n", stderr);
			break;
		case Z_MEM_ERROR:
			fputs("zlib error: out of memory\n", stderr);
			break;
		case Z_VERSION_ERROR:
			fputs("zlib error: version mismatch between zlib.h and libz.a\n", stderr);
			break;
		case FZ_FILE_EXIST:
			fputs("fzip error: file already exists\n", stderr);
			break;
		default:
			fprintf(stderr, "zlib unknown error: error code = %d\n", errorCode);
	}
}


// 压缩/解压一致函数
typedef std::function<int(FILE*,FILE*)> ZipFunc;

// 压缩/解压一致流程
int Zip(const char * src, const char * dst, ZipFunc zip, bool overwrite)
{
	int ret = Z_DATA_ERROR;
	FILE	* source, * dest;
	source = fopen( src, "rb" );
	if( source )
	{
		// 文件已经存在
		if( (dest = fopen( dst, "r" )) )
		{
			fclose( dest );
			ret = FZ_FILE_EXIST;
		}
		// 覆盖 或 文件不存在
		if( overwrite || FZ_FILE_EXIST != ret )
		{
			int fd = -1;
			// 如果文件不存在，则创建文件；
			if( -1 == (fd = open(dst, O_RDONLY|O_CREAT|O_EXCL, S_IRUSR|S_IWUSR | S_IRGRP | S_IROTH)) )
				fd = open(dst, O_RDONLY);
			// 请求独占文件锁(阻塞)，压缩/解压过程中，目标文件禁止读/写
			if( -1 != flock(fd, LOCK_EX) )
			{
				dest =fopen( dst, "wb" );
				if( dest )
				{
					ret = zip( source, dest );
					fclose( dest );
				}
			}
			// 释放锁
			close(fd);
		}
		fclose( source );
	}
	return ret;
}


/**
 * @brief	压缩文件
 * 
 * 
 * @param	srcFile	被压缩的源文件名
 * @param	zipFile	压缩后得到的压缩文件名,缺省为源文件名+.fz
 * @param	overwrite 覆盖已存在文件标志, 缺省不覆盖
 * @param	level	压缩级别( 1~9,默认-1表示取zlib推荐值。level越大，压缩比越大，速度越慢 )
 * @return	0 成功； 非零错误号，可调用 zerr() 查询错误信息
*/
int CompressFile(const char * srcFile, const char * zipFile, bool overwrite, int level)
{
	char filename[FILENAME_MAX]={0}, *pName=0;
	if( ! zipFile )
	{
		strcpy(filename, srcFile);
		pName = filename + strlen(filename);
		strcpy(pName, FZIP_EXT);
		zipFile = filename;
	}
	return Zip( srcFile, zipFile, [ level ] (FILE * src, FILE * dst) { return def(src, dst, level); }, overwrite );
}

/**
 * @brief	解压文件
 * 
 * 
 * @param	zipFile	压缩文件名
 * @param	dstFile	解压后的文件名，缺省为去掉压缩文件名的.fz后缀后得到的文件名
 * @param	overwrite 覆盖已存在文件标志, 缺省不覆盖
 * @return	0 成功；非零错误号可调用 zerr() 查询错误信息
*/
int DecompressFile(const char * zipFile, const char * dstFile, bool overwrite)
{
	char filename[FILENAME_MAX]={0}, *pName=0;
	if( ! dstFile )
	{
		strcpy(filename, zipFile);
		pName = ( strstr(filename, FZIP_EXT) );
		if( pName )
			*pName = 0;
		dstFile = filename;
	}
	return Zip( zipFile, dstFile, inf, overwrite );
}

}; // namespace FZip
