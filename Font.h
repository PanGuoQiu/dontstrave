#ifndef FONT_H
#define FONT_H

#include "stdafx.h"
#include "./fonts/xml.h"
#include <gl/GL.h>
#include "shader.h"
#include "input.h"

// ������
class Font
{
public:
	// ���캯������������
	Font();
	~Font();

	// ��ʼ��
	bool InitFont(const char* shaderName, const std::string shaderValueName, const char* fontString);

	// ������ʾ���ַ���
	void ShowFont(const char* fontString);

	// �����Ҫ��ʾͼ��Ŀ�Ⱥ͸߶�
	int Width() { return m_Width; }
	int Height() { return m_Height; }

	// ��ʾ��С��λ��
	void ShowPosition(glm::vec3 scale = glm::vec3(1.0f, 1.0f, 1.0f),
					  glm::vec3 trans = glm::vec3(0.0f, 0.0f, 0.0f));

	// ������
	void Trigger();
	bool GetTrigger() { return m_IsTrigger; }

private:
	void CreateVertex();							// �������㻺��
	void TextureId();								// ����������

	void DrawNormal();								// ��������
	void DrawTrigger();								// ��������

private:
	Xml m_FontXml;									// ����xml�ļ�

	unsigned int m_VAO, m_VBO;						// ���㻺����
	unsigned int m_ImageId;							// ����ID

	int m_Width, m_Height, m_Format;				// ͼ������
	unsigned char* m_ImageData;						// ͼ������

	bool m_IsTrigger;								// �ж�����Ƿ��ڰ�����
	glm::vec2 m_Position;							// λ��
	glm::vec2 m_Size;								// ��С

	Shader* m_CurrentShader;						// ��ɫ�����
};

#endif //FONT_H