#include "button.h"

// 构造函数
Button::Button() : m_IsTrigger(false)
{
}

// 析构函数
Button::~Button()
{
}

// 初始化
bool Button::InitButton(
	const std::string shaderName, const std::string shaderValueName,
	const char* current, int x1, int y1, int w1, int h1,
	const char* back, int x2, int y2, int w2, int h2, bool t)
{
	// 载入着色器
	m_CurrentShader = Shader::Find(shaderName);

	// 创建顶点缓存区
	CreateVertex();

	// 载入纹理
	if (t)
	{
		m_Texture1 = Image::LoadSubTexture(current, x1, y1, w1, h1);
		m_Texture2 = Image::LoadSubTexture(back, x2, y2, w2, h2);
	}
	else
	{
		m_Texture1 = Image::LoadSubTexture(current, x1, y1, w1, h1);
	}

	// 激活着色器
	m_CurrentShader->Active();
	m_CurrentShader->setInt(shaderValueName, 0);

	return true;
}

// 绘制按钮
void Button::DrawButton(glm::vec3 scale, glm::vec3 trans)
{
	// 获得位置和大小
	m_Position = trans;
	m_Size = scale;

	// 选择显示
	if (m_IsTrigger)
	{
		// 触发绘制
		DrawTrigger();
	}
	else
	{
		// 正常绘制
		DrawNormal();
	}
}

// 两张一起绘制
void Button::DrawAddBack(glm::vec3 scale, glm::vec3 trans)
{
	// 获得位置和大小
	m_Position = trans;
	m_Size = scale;

	// 选择显示
	if (m_IsTrigger)
	{
		// 触发绘制
		DrawNormal();
		DrawTrigger();
	}
	else
	{
		// 正常绘制
		DrawNormal();
	}
}

// 正常绘制
void Button::DrawNormal()
{
	glBindVertexArray(m_VAO);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, m_Texture1);

	// 绘制位置和大小
	glm::mat4 model = glm::mat4(1.0f);
	model = glm::translate(model, glm::vec3(m_Position.x, m_Position.y, 0.0f));
	model = glm::scale(model, glm::vec3(m_Size.x, m_Size.y, 1.0f));

	// 设置着色器矩阵
	m_CurrentShader->setMat4("model", model);

	// 绘制
	glDrawArrays(GL_TRIANGLES, 0, 6);
}

// 触发绘制
void Button::DrawTrigger()
{
	glBindVertexArray(m_VAO);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, m_Texture2);

	// 绘制位置和大小
	glm::mat4 model = glm::mat4(1.0f);
	model = glm::translate(model, glm::vec3(m_Position.x, m_Position.y, 0.0f));
	model = glm::scale(model, glm::vec3(m_Size.x, m_Size.y, 1.0f));

	// 设置着色器矩阵
	m_CurrentShader->setMat4("model", model);

	// 绘制
	glDrawArrays(GL_TRIANGLES, 0, 6);
}

// 触发器
void Button::TriggerButton()
{
	// 判断鼠标是否在按键中
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

// 创建顶点
void Button::CreateVertex()
{
	// 顶点数据：中心点在图片中心
	float Vertices[] = {
		// 位置				// 纹理
		-0.5f,  0.5f,		0.0f, 1.0f,
		 0.5f, -0.5f,		1.0f, 0.0f,
		-0.5f, -0.5f,		0.0f, 0.0f,

		-0.5f,  0.5f,		0.0f, 1.0f,
		 0.5f,  0.5f,		1.0f, 1.0f,
		 0.5f, -0.5f,		1.0f, 0.0f
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

// 释放句柄
void Button::DestoryButton()
{
	glDeleteVertexArrays(1, &m_VAO);
	glDeleteBuffers(1, &m_VBO);
}