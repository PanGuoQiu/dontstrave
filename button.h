#ifndef BUTTON_H
#define BUTTON_H

#include "stdafx.h"
#include "input.h"
#include "screen.h"
#include "Image.h"
#include "shader.h"

// 按键类trigger
class Button
{
public:
	// 构造函数和析构函数
	Button();
	~Button();

	// 释放句柄
	void DestoryButton();

	// 判断是否触发
	bool GetTrigger() { return m_IsTrigger; }

	// 初始化
	bool InitButton(const std::string shaderName, const std::string shaderValueName,
					const char* current, int x1, int y1, int w1, int h1,
					const char* back, int x2, int y2, int w2, int h2, bool t = true);

	// 绘制：位置、大小
	void DrawButton(glm::vec3 scale = glm::vec3(1.0f, 1.0f, 1.0f),
					glm::vec3 tans = glm::vec3(0.0f, 0.0f, 0.0f));

	// 两张一起绘制
	void DrawAddBack(glm::vec3 scale = glm::vec3(1.0f, 1.0f, 1.0f),
					 glm::vec3 tans = glm::vec3(0.0f, 0.0f, 0.0f));

	// 触发器
	void TriggerButton();

private:
	void CreateVertex();							// 创建顶点

	void DrawNormal();								// 正常绘制
	void DrawTrigger();								// 触发绘制

private:
	bool m_IsTrigger;								// 判断鼠标是否在按键上
	glm::vec2 m_Position;							// 位置
	glm::vec2 m_Size;								// 大小
	unsigned int m_VAO, m_VBO;						// 顶点缓存句柄
	unsigned int m_Texture1, m_Texture2;			// 图片句柄
	Shader* m_CurrentShader;						// 着色器句柄
};

#endif //BUTTON_H