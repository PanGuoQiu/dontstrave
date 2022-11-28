#include "Image.h"

// 载入图片，返回纹理ID
unsigned int Image::LoadTexture(const char* filename)
{
	unsigned int textureId;
	glGenTextures(1, &textureId);

	int width, height, nrComponents;
	unsigned char* data = stbi_load(filename, &width, &height, &nrComponents, 0);
	//std::cout << filename << ":" << width << ":" << height << ":" << nrComponents << std::endl;
	if (data)
	{
		GLenum format;
		if (nrComponents == 1)
			format = GL_RED;
		else if (nrComponents == 3)
			format = GL_RGB;
		else if (nrComponents == 4)
			format = GL_RGBA;

		glBindTexture(GL_TEXTURE_2D, textureId);
		glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, format == GL_RGBA ? GL_CLAMP_TO_EDGE : GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, format == GL_RGBA ? GL_CLAMP_TO_EDGE : GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	}
	else
	{
		std::cout << "载入纹理失败，名称：" << filename << std::endl;
	}

	stbi_image_free(data);
	return textureId;
}

// 载入部分图片
unsigned int Image::LoadSubTexture(const char* filename, int x, int y, int w, int h)
{
	unsigned int textureID;
	glGenTextures(1, &textureID);

	int width, height, nrComponents;
	unsigned char* data = stbi_load(filename, &width, &height, &nrComponents, 0);
	if (!data)
	{
		std::cout << "载入纹理失败，路径：" << filename << std::endl;
		stbi_image_free(data);
	}

	// 处理像素数据()
	int r = 0;
	unsigned char* cData = new unsigned char[w * h * nrComponents];
	for (int i = 0; i < height; ++i)
	{
		if (i < y || i >= (y + h))
			continue;

		for (int j = 0; j < width; ++j)
		{
			if (j < x || j >= (x + w))
				continue;

			cData[r + 0] = data[(i * width + j) * nrComponents + 0];
			cData[r + 1] = data[(i * width + j) * nrComponents + 1];
			cData[r + 2] = data[(i * width + j) * nrComponents + 2];
			cData[r + 3] = data[(i * width + j) * nrComponents + 3];
			r += nrComponents;
		}
	}

	GLenum format = GL_RGBA;
	glBindTexture(GL_TEXTURE_2D, textureID);
	glTexImage2D(GL_TEXTURE_2D, 0, format, w, h, 0, format, GL_UNSIGNED_BYTE, cData);
	glGenerateMipmap(GL_TEXTURE_2D);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	stbi_image_free(data);
	delete[] cData;

	return textureID;
	//// 生成纹理编号
	//unsigned int textureId;
	//glGenTextures(1, &textureId);

	//// 载入图片
	//int imageWidth, imageHeight, nrComponents;
	//unsigned char* imageData = stbi_load(filename, &imageWidth, &imageHeight, &nrComponents, 0);
	//if (!imageData)
	//{
	//	std::cout << "载入纹理失败，名称：" << filename << std::endl;
	//	stbi_image_free(imageData);
	//	return false;
	//}

	//// 处理像素数据
	//int r = 0;
	//unsigned char* subImageData = new unsigned char[width * height * nrComponents];
	//for (int i = 0; i < imageHeight; ++i)
	//{
	//	if (i < y || i >= (y + height))
	//		continue;

	//	for (int j = 0; j < imageWidth; ++j)
	//	{
	//		if (j < x || j >= (x + width))
	//			continue;

	//		if (nrComponents == 1)
	//		{
	//			subImageData[r + 0] = imageData[(i * width + j) * nrComponents + 0];
	//		}
	//		else
	//		{
	//			subImageData[r + 0] = imageData[(i * width + j) * nrComponents + 0];
	//			subImageData[r + 1] = imageData[(i * width + j) * nrComponents + 1];
	//			subImageData[r + 2] = imageData[(i * width + j) * nrComponents + 2];
	//			if (nrComponents == 4)
	//				subImageData[r + 3] = imageData[(i * width + j) * nrComponents + 3];
	//		}
	//		
	//		r += nrComponents;
	//	}
	//}

	//// 图片像素颜色格式
	//GLenum format;
	//if (nrComponents == 1)
	//	format = GL_RED;
	//else if (nrComponents == 3)
	//	format = GL_RGB;
	//else if (nrComponents == 4)
	//	format = GL_RGBA;
	//glBindTexture(GL_TEXTURE_2D, textureId);
	//glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, subImageData);
	//glGenerateMipmap(GL_TEXTURE_2D);

	//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, format == GL_RGBA ? GL_CLAMP_TO_EDGE : GL_REPEAT);
	//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, format == GL_RGBA ? GL_CLAMP_TO_EDGE : GL_REPEAT);
	//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	//stbi_image_free(imageData);
	//delete[] subImageData;
	//return textureId;
}