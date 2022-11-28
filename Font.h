#ifndef FONT_H
#define FONT_H

#include "stdafx.h"
#include "./fonts/xml.h"
#include <gl/GL.h>
#include "shader.h"
#include "input.h"

// 字体类
class Font
{
public:
	// 构造函数和析构函数
	Font();
	~Font();

	// 初始化
	bool InitFont(const char* shaderName, const std::string shaderValueName, const char* fontString);

	// 设置显示的字符串
	void ShowFont(const char* fontString);

	// 获得需要显示图像的宽度和高度
	int Width() { return m_Width; }
	int Height() { return m_Height; }

	// 显示大小、位置
	void ShowPosition(glm::vec3 scale = glm::vec3(1.0f, 1.0f, 1.0f),
					  glm::vec3 trans = glm::vec3(0.0f, 0.0f, 0.0f));

	// 触发器
	void Trigger();
	bool GetTrigger() { return m_IsTrigger; }

private:
	void CreateVertex();							// 创建顶点缓存
	void TextureId();								// 生成纹理编号

	void DrawNormal();								// 正常绘制
	void DrawTrigger();								// 触发绘制

private:
	Xml m_FontXml;									// 字体xml文件

	unsigned int m_VAO, m_VBO;						// 顶点缓存句柄
	unsigned int m_ImageId;							// 纹理ID

	int m_Width, m_Height, m_Format;				// 图像属性
	unsigned char* m_ImageData;						// 图像数据

	bool m_IsTrigger;								// 判断鼠标是否在按键上
	glm::vec2 m_Position;							// 位置
	glm::vec2 m_Size;								// 大小

	Shader* m_CurrentShader;						// 着色器句柄
};

#endif //FONT_H