#ifndef TERRAIN_H
#define TERRAIN_H

#include "stdafx.h"
#include "Image.h"
#include "shader.h"
#include <vector>

// ������
class Terrain
{
public:
	// ���캯������������
	Terrain();
	~Terrain();

	// ��ʼ������
	bool InitTerrain();

	// ���õ��δ�С
	void SetSize(unsigned int width, unsigned int cell);

	// ��Ⱦ����
	void RenderTerrain();

private:
	void CreateVertex();				// ��������

private:
	unsigned int m_nWidth;				// ���ε��ܿ��
	unsigned int m_nCellWidth;			// ÿһ����
	unsigned int m_VertexCount;			// ��¼��ĸ���
	unsigned int m_VAO, m_VBO;			// ���㻺����
	unsigned int m_Texture;				// ������
	Shader* m_CurrentShader;			// ��ɫ�����
};

#endif //TERRAIN_H
