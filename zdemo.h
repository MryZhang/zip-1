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

struct FILE;

// 压缩
int def(FILE *source, FILE *dest, int level = 6);

// 解压
int inf(FILE *source, FILE *dest);

// 报错
void zerr(int ret);

