#include "material.h"

// ���캯��
Material::Material()
{
}

// ��������
Material::~Material()
{
}

// �ͷŶ��㻺��
void Material::DeleteMaterial()
{
	glDeleteVertexArrays(1, &m_VAO);
	glDeleteBuffers(1, &m_VBO);
}

// �������
bool Material::InitMaterial(const std::string shaderName, const std::string shaderValueName,
							const char* imageName, int x, int y, int w, int h, bool t)
{
	// ����ɫ��
	m_Shader = Shader::Find(shaderName);

	// �������㻺��
	CreateVertex();

	// ����ͼƬ
	if (t)
	{
		m_textureId = Image::LoadTexture(imageName);
	}
	else
	{
		m_textureId = Image::LoadSubTexture(imageName, x, y, w, h);
	}

	// ������ɫ��
	m_Shader->Active();
	m_Shader->setInt(shaderValueName, 0);

	return true;
}

// �������㻺��
void Material::CreateVertex()
{
	// ��������
	float Vertices[] = {
		// λ��			// ����
		0.0f, 1.0f,		0.0f, 1.0f,
		1.0f, 0.0f,		1.0f, 0.0f,
		0.0f, 0.0f,		0.0f, 0.0f,

		0.0f, 1.0f,		0.0f, 1.0f,
		1.0f, 1.0f,		1.0f, 1.0f,
		1.0f, 0.0f,		1.0f, 0.0f
	};

	// ����VAO��VBO
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

// ���Ʋ���
void Material::DrawMaterial(glm::vec3 scale, glm::vec3 trans)
{
	glBindVertexArray(m_VAO);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, m_textureId);

	// ����λ�úʹ�С
	glm::mat4 model = glm::mat4(1.0f);
	model = glm::translate(model, trans);
	model = glm::scale(model, scale);

	// ������ɫ������
	m_Shader->setMat4("model", model);

	// ����
	glDrawArrays(GL_TRIANGLES, 0, 6);
}