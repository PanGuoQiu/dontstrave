#ifndef MATERIAL_H
#define MATERIAL_H

#include "stdafx.h"
#include "Image.h"
#include "shader.h"

// 材质类
class Material
{
public:
	// 构造函数和析构函数
	Material();
	~Material();

	// 释放顶点缓存
	void DeleteMaterial();

	// 载入材质
	bool InitMaterial(const std::string shaderName, const std::string shaderValueName,
					  const char* imageName, 
					  int x = 0, int y = 0, int w = 0, int h = 0, bool t = true);

	// 绘制：位置、大小
	void DrawMaterial(glm::vec3 scale = glm::vec3(1.0f, 1.0f, 1.0f),
					  glm::vec3 tans = glm::vec3(0.0f, 0.0f, 0.0f));

private:
	void CreateVertex();							// 创建顶点

private:
	unsigned int m_textureId;						// 纹理编号
	unsigned int m_VAO, m_VBO;						// 设置顶点缓存
	Shader* m_Shader;								// 着色器指针
};

#endif //MATERIAL_H