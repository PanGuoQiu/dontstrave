#include "material.h"

// 构造函数
Material::Material()
{
}

// 析构函数
Material::~Material()
{
}

// 释放顶点缓存
void Material::DeleteMaterial()
{
	glDeleteVertexArrays(1, &m_VAO);
	glDeleteBuffers(1, &m_VBO);
}

// 载入材质
bool Material::InitMaterial(const std::string shaderName, const std::string shaderValueName,
							const char* imageName, int x, int y, int w, int h, bool t)
{
	// 绑定着色器
	m_Shader = Shader::Find(shaderName);

	// 创建顶点缓存
	CreateVertex();

	// 载入图片
	if (t)
	{
		m_textureId = Image::LoadTexture(imageName);
	}
	else
	{
		m_textureId = Image::LoadSubTexture(imageName, x, y, w, h);
	}

	// 激活着色器
	m_Shader->Active();
	m_Shader->setInt(shaderValueName, 0);

	return true;
}

// 创建顶点缓存
void Material::CreateVertex()
{
	// 顶点数据
	float Vertices[] = {
		// 位置			// 纹理
		0.0f, 1.0f,		0.0f, 1.0f,
		1.0f, 0.0f,		1.0f, 0.0f,
		0.0f, 0.0f,		0.0f, 0.0f,

		0.0f, 1.0f,		0.0f, 1.0f,
		1.0f, 1.0f,		1.0f, 1.0f,
		1.0f, 0.0f,		1.0f, 0.0f
	};

	// 设置VAO和VBO
	glGenVertexArrays(1, &m_VAO);
	glGenBuffers(1, &m_VBO);

	glBindVertexArray(m_VAO);
	glBindBuffer(GL_ARRAY_BUFFER, m_VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(Vertices), Vertices, GL_STATIC_DRAW);

	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)(2 * sizeof(float)));
	
	glBindVertexArray(0);
}

// 绘制材质
void Material::DrawMaterial(glm::vec3 scale, glm::vec3 trans)
{
	glBindVertexArray(m_VAO);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, m_textureId);

	// 绘制位置和大小
	glm::mat4 model = glm::mat4(1.0f);
	model = glm::translate(model, trans);
	model = glm::scale(model, scale);

	// 设置着色器矩阵
	m_Shader->setMat4("model", model);

	// 绘制
	glDrawArrays(GL_TRIANGLES, 0, 6);
}