#ifndef IMAGE_H
#define IMAGE_H

#include "stdafx.h"

// 图片类
class Image
{
public:

	// 载入图片
	static unsigned int LoadTexture(const char* filename);

	// 载入部分图片：左上角x和y，宽度和高度
	static unsigned int LoadSubTexture(const char* filename, int x, int y, int w, int h);

};

#endif //IMAGE_H