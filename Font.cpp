#include "Font.h"

// 构造函数
Font::Font() : m_Width(0), m_Height(0), m_Format(0), m_IsTrigger(false)
{

}

// 析构函数
Font::~Font()
{
	// 释放顶点缓存
	glDeleteVertexArrays(1, &m_VAO);
	glDeleteBuffers(1, &m_VBO);
	// 纹理编号
	glDeleteTextures(1, &m_ImageId);

	// 释放内存
	m_FontXml.clear();
	stbi_image_free(m_ImageData);
}

// 初始化
bool Font::InitFont(const char* shaderName, const std::string shaderValueName, const char* fontString)
{
	// 载入xml字体文件
	if (!m_FontXml.load("fonts/font.fnt"))
	{
		std::cout << "载入XML字体文件失败" << std::endl;
		return false;
	}

	// 载入字体图片
	m_ImageData = stbi_load("fonts/buttonfont_0.png", &m_Width, &m_Height, &m_Format, 0);
	if (!m_ImageData)
	{
		std::cout << "载入纹理失败，路径：" << std::endl;
		stbi_image_free(m_ImageData);
		return false;
	}

	// 载入着色器
	m_CurrentShader = Shader::Find(shaderName);

	// 创建顶点缓存
	CreateVertex();

	// 生成纹理编号
	ShowFont(fontString);

	// 激活着色器
	m_CurrentShader->Active();
	m_CurrentShader->setInt(shaderValueName, 0);

	return true;
}

// 显示字体字符串
void Font::ShowFont(const char* fontString)
{
	// xml属性
	int x, y, w, h;
	std::string xmlNodeName;

	// 图片属性
	int width = m_Width;
	int height = m_Height;
	int r = 0;
	m_Width = 0;
	m_Height = 0;

	// 第几个字符
	int num = 0;
	int charWidth = 0;

	// 计算字符的个数，以及所需缓存的大小
	while (true)
	{
		// 获得字符串的一个字符
		char c = fontString[num];
		if (c == '\0')
			break;
		// 下一个字符
		num++;

		// 遍历所有节点，获得字图的位置和大小
		for (auto it = m_FontXml[3].begin(); it != m_FontXml[3].end(); ++it)
		{
			// 获得节点名称
			xmlNodeName = it->name();
			if (xmlNodeName.compare("char") == 0)
			{
				// 判断id的值
				int val = it->attr("id");
				if (val == int(c))
				{
					x = it->attr("x");
					y = it->attr("y");
					w = it->attr("width");
					h = it->attr("height");

					// 内存的宽度和高度
					m_Width += w;
					m_Height = (m_Height > h) ? m_Height : h;

					break;
				}
			}
		}
	}
	// 分配字符所需内存
	unsigned char* cFont = new unsigned char[m_Width * m_Height * m_Format]();

	// 处理每一个字符
	num = 0;
	while (true)
	{
		// 获得字符串的一个字符
		char c = fontString[num];
		if (c == '\0')
			break;
		// 下一个字符
		num++;

		// 遍历所有节点，获得字图的位置和大小
		for (auto it = m_FontXml[3].begin(); it != m_FontXml[3].end(); ++it)
		{
			// 获得节点名称
			xmlNodeName = it->name();
			if (xmlNodeName.compare("char") == 0)
			{
				// 判断id的值
				int val = it->attr("id");
				if (val == int(c))
				{
					x = it->attr("x");
					y = it->attr("y");
					w = it->attr("width");
					h = it->attr("height");

					break;
				}
			}
		}

		// 获得图片的字图数据()
		r = 0;
		unsigned char* cData = new unsigned char[w * h * m_Format];
		for (int i = 0; i < height; ++i)
		{
			if (i < y || i >= (y + h))
				continue;

			for (int j = 0; j < width; ++j)
			{
				if (j < x || j >= (x + w))
					continue;

				cData[r + 0] = m_ImageData[(i * width + j) * m_Format + 0];
				cData[r + 1] = m_ImageData[(i * width + j) * m_Format + 1];
				cData[r + 2] = m_ImageData[(i * width + j) * m_Format + 2];
				cData[r + 3] = m_ImageData[(i * width + j) * m_Format + 3];
				r += m_Format;
			}
		}

		// 合并图片
		charWidth += w;
		r = 0;
		for (int i = 0; i < m_Height; ++i)
		{
			if (i < (m_Height - h) || i >= m_Height)
				continue;

			for (int j = 0; j < m_Width; ++j)
			{
				if (j < (charWidth - w) || j >= charWidth)
					continue;

				cFont[(i * m_Width + j) * m_Format + 0] = cData[r + 0];
				cFont[(i * m_Width + j) * m_Format + 1] = cData[r + 1];
				cFont[(i * m_Width + j) * m_Format + 2] = cData[r + 2];
				cFont[(i * m_Width + j) * m_Format + 3] = cData[r + 3];
				r += m_Format;
			}
		}

		//销毁临时内存
		delete[] cData;
	}

	// 生成纹理编号
	glGenTextures(1, &m_ImageId);

	glBindTexture(GL_TEXTURE_2D, m_ImageId);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, m_Width, m_Height, 0, GL_RGBA, GL_UNSIGNED_BYTE, cFont);
	glGenerateMipmap(GL_TEXTURE_2D);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	// 销毁内存
	delete[] cFont;
}

// 创建顶点缓存
void Font::CreateVertex()
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

// 显示字体位置
void Font::ShowPosition(glm::vec3 scale, glm::vec3 trans)
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

// 正常绘制
void Font::DrawNormal()
{
	glBindVertexArray(m_VAO);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, m_ImageId);

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
void Font::DrawTrigger()
{
	glBindVertexArray(m_VAO);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, m_ImageId);

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
void Font::Trigger()
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