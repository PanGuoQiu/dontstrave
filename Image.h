#ifndef IMAGE_H
#define IMAGE_H

#include "stdafx.h"

// ͼƬ��
class Image
{
public:

	// ����ͼƬ
	static unsigned int LoadTexture(const char* filename);

	// ���벿��ͼƬ�����Ͻ�x��y����Ⱥ͸߶�
	static unsigned int LoadSubTexture(const char* filename, int x, int y, int w, int h);

};

#endif //IMAGE_H