#include "terrain.h"

// ���캯��
Terrain::Terrain() : m_nWidth(64), m_nCellWidth(16)
{
}

// ��������
Terrain::~Terrain()
{
	glDeleteVertexArrays(1, &m_VAO);
	glDeleteBuffers(1, &m_VBO);

	glDeleteTextures(1, &m_Texture);
}

// ��ʼ������
bool Terrain::InitTerrain()
{
	// ������ɫ��
	m_CurrentShader = Shader::Find("terrain");

	// �������㻺����
	CreateVertex();

	// ��������
	m_Texture = Image::LoadTexture("textures/Ground_noise_deciduous.jpg");

	// ������ɫ��
	m_CurrentShader->Active();
	m_CurrentShader->setInt("bgTexture", 0);

	return true;
}

// ���õ��δ�С
void Terrain::SetSize(unsigned int width, unsigned int cell)
{
	m_nWidth = width;
	m_nCellWidth = cell;
}

// ��Ⱦ����
void Terrain::RenderTerrain()
{
	glBindVertexArray(m_VAO);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, m_Texture);

	// ����λ�úʹ�С
	glm::mat4 model = glm::mat4(1.0f);
	// ������ɫ������
	m_CurrentShader->setMat4("model", model);

	// ����
	glDrawArrays(GL_TRIANGLES, 0, 6);
}

// ��������
void Terrain::CreateVertex()
{
	// ����λ�ú���������
	//int X = 0, Y = 0;
	//float x, z, u, v;
	//bool bSwitchSides = false;
	//std::vector<float> Vertices;

	//// ����X��ʼѭ��
	//int count = 0;
	//for (X = 0; X <= m_nWidth; X += m_nCellWidth)
	//{
	//	// �������ͷ���Ҫ���෴˳�����
	//	if (bSwitchSides)
	//	{
	//		// ���Ƶ��ε�һ��
	//		for (Y = m_nWidth; Y >= 0; Y -= m_nCellWidth)
	//		{
	//			// ��������(X, Y, Z)
	//			x = X;
	//			z = Y;
	//			Vertices.push_back(x);
	//			Vertices.push_back(0.0f);
	//			Vertices.push_back(z);

	//			// ��������
	//			u = x / m_nWidth;
	//			v = -z / m_nWidth;
	//			Vertices.push_back(u);
	//			Vertices.push_back(v);

	//			// ����(X + m_nCellWidth, Y, Z)
	//			x = X + m_nCellWidth;
	//			z = Y;
	//			Vertices.push_back(x);
	//			Vertices.push_back(0.0f);
	//			Vertices.push_back(z);

	//			// ��������
	//			u = x / m_nWidth;
	//			v = -z / m_nWidth;
	//			Vertices.push_back(u);
	//			Vertices.push_back(v);

	//			count += 2;
	//		}
	//	}
	//	else
	//	{
	//		// ���Ƶ��ε�һ��
	//		for (Y = 0; Y <= m_nWidth; Y += m_nCellWidth)
	//		{
	//			// ����(X + m_nCellWidth, Y, Z)
	//			x = X + m_nCellWidth;
	//			z = Y;
	//			Vertices.push_back(x);
	//			Vertices.push_back(0.0f);
	//			Vertices.push_back(z);

	//			// ��������
	//			u = x / m_nWidth;
	//			v = -z / m_nWidth;
	//			Vertices.push_back(u);
	//			Vertices.push_back(v);

	//			// ����(X, Y, Z)
	//			x = X;
	//			z = Y;
	//			Vertices.push_back(x);
	//			Vertices.push_back(0.0f);
	//			Vertices.push_back(z);

	//			// ��������
	//			u = x / m_nWidth;
	//			v = -z / m_nWidth;
	//			Vertices.push_back(u);
	//			Vertices.push_back(v);

	//			count += 2;
	//		}
	//	}

	//	// �任����
	//	bSwitchSides = !bSwitchSides;

	//	// ��¼������Ŀ
	//	m_VertexCount = count;
	//}

	//// ���㻺��
	//unsigned int size = Vertices.size();
	//float *VertexBuffers = new float[size];
	//std::vector<float>::iterator iter = Vertices.begin();
	//int i = 0;
	//for (; iter != Vertices.end(); ++iter)
	//{
	//	VertexBuffers[i] = (*iter);
	//}

	// �������ݣ����ĵ���ͼƬ����
	float Vertices[] = {
		// λ��				// ����
		10.0f, -0.5f,  10.0f,  2.0f, 0.0f,
		-10.0f, -0.5f,  10.0f,  0.0f, 0.0f,
		-10.0f, -0.5f, -10.0f,  0.0f, 2.0f,

		10.0f, -0.5f,  10.0f,  2.0f, 0.0f,
		-10.0f, -0.5f, -10.0f,  0.0f, 2.0f,
		10.0f, -0.5f, -10.0f,  2.0f, 2.0f
	};

	// ����VAO��VBO
	glGenVertexArrays(1, &m_VAO);
	glGenBuffers(1, &m_VBO);

	glBindVertexArray(m_VAO);
	glBindBuffer(GL_ARRAY_BUFFER, m_VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(Vertices), Vertices, GL_STATIC_DRAW);

	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));

	glBindVertexArray(0);
	//delete[] VertexBuffers;
}