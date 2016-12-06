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
#include <string>
#include <sys/file.h>

namespace FZip{

#define FZIP_EXT	".fz"

// 压缩文件
int CompressFile(const char * srcFile, const char * zipFile=0, bool overwrite = false, int level=-1);

// 解压文件
int DecompressFile(const char * zipFile, const char * dstFile = 0, bool overwrite = false);

// 错误号意义
void zerr(int errorCode);

// 协同锁
class AdvisoryLock
{
protected:
	int		_fd;	// 文件描述符
	std::string	_name;	// 文件名
public:
	AdvisoryLock(const char * filename);
	virtual ~ AdvisoryLock();

	// 加锁
	virtual int Lock(int operation);
	// 解锁（读/写者以相同方式解锁）
	virtual void Unlock();
};

// 写者
class Writer : public AdvisoryLock
{
public:
	Writer(const char * filename);
	// 写文件时加互斥锁/非阻塞，解锁前加互斥锁、共享锁均会失败
	int Lock(int operation = LOCK_EX | LOCK_NB);
};

// 读者
class Reader : public AdvisoryLock
{
public:
	Reader(const char * filename);
	// 读文件时加共享锁/阻塞等待，解锁前加互斥锁失败，可成功加共享锁
	int Lock(int operation = LOCK_SH);
};

}; // namespace FZip
