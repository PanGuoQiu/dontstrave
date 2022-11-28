#include "button.h"

// ���캯��
Button::Button() : m_IsTrigger(false)
{
}

// ��������
Button::~Button()
{
}

// ��ʼ��
bool Button::InitButton(
	const std::string shaderName, const std::string shaderValueName,
	const char* current, int x1, int y1, int w1, int h1,
	const char* back, int x2, int y2, int w2, int h2, bool t)
{
	// ������ɫ��
	m_CurrentShader = Shader::Find(shaderName);

	// �������㻺����
	CreateVertex();

	// ��������
	if (t)
	{
		m_Texture1 = Image::LoadSubTexture(current, x1, y1, w1, h1);
		m_Texture2 = Image::LoadSubTexture(back, x2, y2, w2, h2);
	}
	else
	{
		m_Texture1 = Image::LoadSubTexture(current, x1, y1, w1, h1);
	}

	// ������ɫ��
	m_CurrentShader->Active();
	m_CurrentShader->setInt(shaderValueName, 0);

	return true;
}

// ���ư�ť
void Button::DrawButton(glm::vec3 scale, glm::vec3 trans)
{
	// ���λ�úʹ�С
	m_Position = trans;
	m_Size = scale;

	// ѡ����ʾ
	if (m_IsTrigger)
	{
		// ��������
		DrawTrigger();
	}
	else
	{
		// ��������
		DrawNormal();
	}
}

// ����һ�����
void Button::DrawAddBack(glm::vec3 scale, glm::vec3 trans)
{
	// ���λ�úʹ�С
	m_Position = trans;
	m_Size = scale;

	// ѡ����ʾ
	if (m_IsTrigger)
	{
		// ��������
		DrawNormal();
		DrawTrigger();
	}
	else
	{
		// ��������
		DrawNormal();
	}
}

// ��������
void Button::DrawNormal()
{
	glBindVertexArray(m_VAO);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, m_Texture1);

	// ����λ�úʹ�С
	glm::mat4 model = glm::mat4(1.0f);
	model = glm::translate(model, glm::vec3(m_Position.x, m_Position.y, 0.0f));
	model = glm::scale(model, glm::vec3(m_Size.x, m_Size.y, 1.0f));

	// ������ɫ������
	m_CurrentShader->setMat4("model", model);

	// ����
	glDrawArrays(GL_TRIANGLES, 0, 6);
}

// ��������
void Button::DrawTrigger()
{
	glBindVertexArray(m_VAO);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, m_Texture2);

	// ����λ�úʹ�С
	glm::mat4 model = glm::mat4(1.0f);
	model = glm::translate(model, glm::vec3(m_Position.x, m_Position.y, 0.0f));
	model = glm::scale(model, glm::vec3(m_Size.x, m_Size.y, 1.0f));

	// ������ɫ������
	m_CurrentShader->setMat4("model", model);

	// ����
	glDrawArrays(GL_TRIANGLES, 0, 6);
}

// ������
void Button::TriggerButton()
{
	// �ж�����Ƿ��ڰ�����
	Vec2 mousePos = Input::MousePosition();
	glm::vec2 halfSize = glm::vec2(m_Size.x / 2, m_Size.y / 2);
	glm::vec2 buttonStart = m_Position - halfSize;
	glm::vec2 buttonEnd = m_Position + halfSize;
	if ((buttonStart.x < (float)mousePos.m_x) && ((float)mousePos.m_x < buttonEnd.x) &&
		(buttonStart.y < (float)mousePos.m_y) && ((float)mousePos.m_y < buttonEnd.y))
	{
		m_IsTrigger = true;
	}
	else
	{
		m_IsTrigger = false;
	}
}

// ��������
void Button::CreateVertex()
{
	// �������ݣ����ĵ���ͼƬ����
	float Vertices[] = {
		// λ��				// ����
		-0.5f,  0.5f,		0.0f, 1.0f,
		 0.5f, -0.5f,		1.0f, 0.0f,
		-0.5f, -0.5f,		0.0f, 0.0f,

		-0.5f,  0.5f,		0.0f, 1.0f,
		 0.5f,  0.5f,		1.0f, 1.0f,
		 0.5f, -0.5f,		1.0f, 0.0f
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

// �ͷž��
void Button::DestoryButton()
{
	glDeleteVertexArrays(1, &m_VAO);
	glDeleteBuffers(1, &m_VBO);
}