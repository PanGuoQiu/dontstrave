#ifndef TERRAIN_H
#define TERRAIN_H

#include "stdafx.h"
#include "Image.h"
#include "shader.h"
#include <vector>

// 地形类
class Terrain
{
public:
	// 构造函数和析构函数
	Terrain();
	~Terrain();

	// 初始化地形
	bool InitTerrain();

	// 设置地形大小
	void SetSize(unsigned int width, unsigned int cell);

	// 渲染地形
	void RenderTerrain();

private:
	void CreateVertex();				// 创建顶点

private:
	unsigned int m_nWidth;				// 地形的总宽度
	unsigned int m_nCellWidth;			// 每一格宽度
	unsigned int m_VertexCount;			// 记录点的个数
	unsigned int m_VAO, m_VBO;			// 顶点缓存句柄
	unsigned int m_Texture;				// 纹理编号
	Shader* m_CurrentShader;			// 着色器句柄
};

#endif //TERRAIN_H
