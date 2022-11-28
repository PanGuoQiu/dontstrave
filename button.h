#ifndef BUTTON_H
#define BUTTON_H

#include "stdafx.h"
#include "input.h"
#include "screen.h"
#include "Image.h"
#include "shader.h"

// ������trigger
class Button
{
public:
	// ���캯������������
	Button();
	~Button();

	// �ͷž��
	void DestoryButton();

	// �ж��Ƿ񴥷�
	bool GetTrigger() { return m_IsTrigger; }

	// ��ʼ��
	bool InitButton(const std::string shaderName, const std::string shaderValueName,
					const char* current, int x1, int y1, int w1, int h1,
					const char* back, int x2, int y2, int w2, int h2, bool t = true);

	// ���ƣ�λ�á���С
	void DrawButton(glm::vec3 scale = glm::vec3(1.0f, 1.0f, 1.0f),
					glm::vec3 tans = glm::vec3(0.0f, 0.0f, 0.0f));

	// ����һ�����
	void DrawAddBack(glm::vec3 scale = glm::vec3(1.0f, 1.0f, 1.0f),
					 glm::vec3 tans = glm::vec3(0.0f, 0.0f, 0.0f));

	// ������
	void TriggerButton();

private:
	void CreateVertex();							// ��������

	void DrawNormal();								// ��������
	void DrawTrigger();								// ��������

private:
	bool m_IsTrigger;								// �ж�����Ƿ��ڰ�����
	glm::vec2 m_Position;							// λ��
	glm::vec2 m_Size;								// ��С
	unsigned int m_VAO, m_VBO;						// ���㻺����
	unsigned int m_Texture1, m_Texture2;			// ͼƬ���
	Shader* m_CurrentShader;						// ��ɫ�����
};

#endif //BUTTON_H