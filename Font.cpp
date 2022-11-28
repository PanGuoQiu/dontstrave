#include "Font.h"

// ���캯��
Font::Font() : m_Width(0), m_Height(0), m_Format(0), m_IsTrigger(false)
{

}

// ��������
Font::~Font()
{
	// �ͷŶ��㻺��
	glDeleteVertexArrays(1, &m_VAO);
	glDeleteBuffers(1, &m_VBO);
	// ������
	glDeleteTextures(1, &m_ImageId);

	// �ͷ��ڴ�
	m_FontXml.clear();
	stbi_image_free(m_ImageData);
}

// ��ʼ��
bool Font::InitFont(const char* shaderName, const std::string shaderValueName, const char* fontString)
{
	// ����xml�����ļ�
	if (!m_FontXml.load("fonts/font.fnt"))
	{
		std::cout << "����XML�����ļ�ʧ��" << std::endl;
		return false;
	}

	// ��������ͼƬ
	m_ImageData = stbi_load("fonts/buttonfont_0.png", &m_Width, &m_Height, &m_Format, 0);
	if (!m_ImageData)
	{
		std::cout << "��������ʧ�ܣ�·����" << std::endl;
		stbi_image_free(m_ImageData);
		return false;
	}

	// ������ɫ��
	m_CurrentShader = Shader::Find(shaderName);

	// �������㻺��
	CreateVertex();

	// ����������
	ShowFont(fontString);

	// ������ɫ��
	m_CurrentShader->Active();
	m_CurrentShader->setInt(shaderValueName, 0);

	return true;
}

// ��ʾ�����ַ���
void Font::ShowFont(const char* fontString)
{
	// xml����
	int x, y, w, h;
	std::string xmlNodeName;

	// ͼƬ����
	int width = m_Width;
	int height = m_Height;
	int r = 0;
	m_Width = 0;
	m_Height = 0;

	// �ڼ����ַ�
	int num = 0;
	int charWidth = 0;

	// �����ַ��ĸ������Լ����軺��Ĵ�С
	while (true)
	{
		// ����ַ�����һ���ַ�
		char c = fontString[num];
		if (c == '\0')
			break;
		// ��һ���ַ�
		num++;

		// �������нڵ㣬�����ͼ��λ�úʹ�С
		for (auto it = m_FontXml[3].begin(); it != m_FontXml[3].end(); ++it)
		{
			// ��ýڵ�����
			xmlNodeName = it->name();
			if (xmlNodeName.compare("char") == 0)
			{
				// �ж�id��ֵ
				int val = it->attr("id");
				if (val == int(c))
				{
					x = it->attr("x");
					y = it->attr("y");
					w = it->attr("width");
					h = it->attr("height");

					// �ڴ�Ŀ�Ⱥ͸߶�
					m_Width += w;
					m_Height = (m_Height > h) ? m_Height : h;

					break;
				}
			}
		}
	}
	// �����ַ������ڴ�
	unsigned char* cFont = new unsigned char[m_Width * m_Height * m_Format]();

	// ����ÿһ���ַ�
	num = 0;
	while (true)
	{
		// ����ַ�����һ���ַ�
		char c = fontString[num];
		if (c == '\0')
			break;
		// ��һ���ַ�
		num++;

		// �������нڵ㣬�����ͼ��λ�úʹ�С
		for (auto it = m_FontXml[3].begin(); it != m_FontXml[3].end(); ++it)
		{
			// ��ýڵ�����
			xmlNodeName = it->name();
			if (xmlNodeName.compare("char") == 0)
			{
				// �ж�id��ֵ
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

		// ���ͼƬ����ͼ����()
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

		// �ϲ�ͼƬ
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

		//������ʱ�ڴ�
		delete[] cData;
	}

	// ����������
	glGenTextures(1, &m_ImageId);

	glBindTexture(GL_TEXTURE_2D, m_ImageId);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, m_Width, m_Height, 0, GL_RGBA, GL_UNSIGNED_BYTE, cFont);
	glGenerateMipmap(GL_TEXTURE_2D);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	// �����ڴ�
	delete[] cFont;
}

// �������㻺��
void Font::CreateVertex()
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

// ��ʾ����λ��
void Font::ShowPosition(glm::vec3 scale, glm::vec3 trans)
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

// ��������
void Font::DrawNormal()
{
	glBindVertexArray(m_VAO);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, m_ImageId);

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
void Font::DrawTrigger()
{
	glBindVertexArray(m_VAO);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, m_ImageId);

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
void Font::Trigger()
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