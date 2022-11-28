#ifndef MATERIAL_H
#define MATERIAL_H

#include "stdafx.h"
#include "Image.h"
#include "shader.h"

// ������
class Material
{
public:
	// ���캯������������
	Material();
	~Material();

	// �ͷŶ��㻺��
	void DeleteMaterial();

	// �������
	bool InitMaterial(const std::string shaderName, const std::string shaderValueName,
					  const char* imageName, 
					  int x = 0, int y = 0, int w = 0, int h = 0, bool t = true);

	// ���ƣ�λ�á���С
	void DrawMaterial(glm::vec3 scale = glm::vec3(1.0f, 1.0f, 1.0f),
					  glm::vec3 tans = glm::vec3(0.0f, 0.0f, 0.0f));

private:
	void CreateVertex();							// ��������

private:
	unsigned int m_textureId;						// ������
	unsigned int m_VAO, m_VBO;						// ���ö��㻺��
	Shader* m_Shader;								// ��ɫ��ָ��
};

#endif //MATERIAL_H