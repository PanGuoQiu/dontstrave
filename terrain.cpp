#include "terrain.h"

// 构造函数
Terrain::Terrain() : m_nWidth(64), m_nCellWidth(16)
{
}

// 析构函数
Terrain::~Terrain()
{
	glDeleteVertexArrays(1, &m_VAO);
	glDeleteBuffers(1, &m_VBO);

	glDeleteTextures(1, &m_Texture);
}

// 初始化地形
bool Terrain::InitTerrain()
{
	// 载入着色器
	m_CurrentShader = Shader::Find("terrain");

	// 创建顶点缓存区
	CreateVertex();

	// 载入纹理
	m_Texture = Image::LoadTexture("textures/Ground_noise_deciduous.jpg");

	// 激活着色器
	m_CurrentShader->Active();
	m_CurrentShader->setInt("bgTexture", 0);

	return true;
}

// 设置地形大小
void Terrain::SetSize(unsigned int width, unsigned int cell)
{
	m_nWidth = width;
	m_nCellWidth = cell;
}

// 渲染地形
void Terrain::RenderTerrain()
{
	glBindVertexArray(m_VAO);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, m_Texture);

	// 绘制位置和大小
	glm::mat4 model = glm::mat4(1.0f);
	// 设置着色器矩阵
	m_CurrentShader->setMat4("model", model);

	// 绘制
	glDrawArrays(GL_TRIANGLES, 0, 6);
}

// 创建顶点
void Terrain::CreateVertex()
{
	// 创建位置和纹理坐标
	//int X = 0, Y = 0;
	//float x, z, u, v;
	//bool bSwitchSides = false;
	//std::vector<float> Vertices;

	//// 从行X开始循环
	//int count = 0;
	//for (X = 0; X <= m_nWidth; X += m_nCellWidth)
	//{
	//	// 检查该列释放需要从相反顺序绘制
	//	if (bSwitchSides)
	//	{
	//		// 绘制地形的一列
	//		for (Y = m_nWidth; Y >= 0; Y -= m_nCellWidth)
	//		{
	//			// 顶点数据(X, Y, Z)
	//			x = X;
	//			z = Y;
	//			Vertices.push_back(x);
	//			Vertices.push_back(0.0f);
	//			Vertices.push_back(z);

	//			// 纹理数据
	//			u = x / m_nWidth;
	//			v = -z / m_nWidth;
	//			Vertices.push_back(u);
	//			Vertices.push_back(v);

	//			// 顶点(X + m_nCellWidth, Y, Z)
	//			x = X + m_nCellWidth;
	//			z = Y;
	//			Vertices.push_back(x);
	//			Vertices.push_back(0.0f);
	//			Vertices.push_back(z);

	//			// 纹理数据
	//			u = x / m_nWidth;
	//			v = -z / m_nWidth;
	//			Vertices.push_back(u);
	//			Vertices.push_back(v);

	//			count += 2;
	//		}
	//	}
	//	else
	//	{
	//		// 绘制地形的一列
	//		for (Y = 0; Y <= m_nWidth; Y += m_nCellWidth)
	//		{
	//			// 顶点(X + m_nCellWidth, Y, Z)
	//			x = X + m_nCellWidth;
	//			z = Y;
	//			Vertices.push_back(x);
	//			Vertices.push_back(0.0f);
	//			Vertices.push_back(z);

	//			// 纹理坐标
	//			u = x / m_nWidth;
	//			v = -z / m_nWidth;
	//			Vertices.push_back(u);
	//			Vertices.push_back(v);

	//			// 顶点(X, Y, Z)
	//			x = X;
	//			z = Y;
	//			Vertices.push_back(x);
	//			Vertices.push_back(0.0f);
	//			Vertices.push_back(z);

	//			// 纹理坐标
	//			u = x / m_nWidth;
	//			v = -z / m_nWidth;
	//			Vertices.push_back(u);
	//			Vertices.push_back(v);

	//			count += 2;
	//		}
	//	}

	//	// 变换开关
	//	bSwitchSides = !bSwitchSides;

	//	// 记录顶点数目
	//	m_VertexCount = count;
	//}

	//// 顶点缓存
	//unsigned int size = Vertices.size();
	//float *VertexBuffers = new float[size];
	//std::vector<float>::iterator iter = Vertices.begin();
	//int i = 0;
	//for (; iter != Vertices.end(); ++iter)
	//{
	//	VertexBuffers[i] = (*iter);
	//}

	// 顶点数据：中心点在图片中心
	float Vertices[] = {
		// 位置				// 纹理
		10.0f, -0.5f,  10.0f,  2.0f, 0.0f,
		-10.0f, -0.5f,  10.0f,  0.0f, 0.0f,
		-10.0f, -0.5f, -10.0f,  0.0f, 2.0f,

		10.0f, -0.5f,  10.0f,  2.0f, 0.0f,
		-10.0f, -0.5f, -10.0f,  0.0f, 2.0f,
		10.0f, -0.5f, -10.0f,  2.0f, 2.0f
	};

	// 设置VAO和VBO
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