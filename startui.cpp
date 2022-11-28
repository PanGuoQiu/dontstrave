#include "startui.h"

// 构造函数
StartUI::StartUI() : m_IsVisible(true)
{
}

// 析构函数
StartUI::~StartUI()
{
}

// 初始化
bool StartUI::Init()
{
	// 背景图
	if (!m_Bg.InitMaterial("normal", "bgTexture","textures/bg_redux_dark_right.png", 0, 0, 1792, 1024, false))
	{
		std::cout << "载入背景材质失败" << std::endl;
		return false;
	}

	// 标题图
	if (!m_Title.InitMaterial("title", "bgTexture", "textures/frontscreen.png", 0, 0, 860, 860, false))
	{
		std::cout << "载入标题材质失败" << std::endl;
		return false;
	}

	// 退出按钮
	if (!m_Quit.InitButton("quit", "bgTexture",
		"textures/frontend.png", 1940, 0, 100, 100,
		"textures/frontend.png", 850, 720, 100, 100))
	{
		std::cout << "载入退出按钮失败" << std::endl;
		return false;
	}
	if (!m_QuitFont.InitFont("quit", "bgTexture", "Quit"))
	{
		std::cout << "初始化退出按钮的字体失败" << std::endl;
		return false;
	}

	// 开始按钮
	if (!m_Start.InitButton("quit", "bgTexture",
		"textures/frontscreen.png", 1464, 254, 394, 124,
		"textures/frontscreen.png", 1464, 130, 394, 124))
	{
		std::cout << "载入开始按钮失败" << std::endl;
		return false;
	}

	// 开始按钮的字体
	if (!m_StartFont.InitFont("quit", "bgTexture", "Play"))
	{
		std::cout << "初始化开始按钮的字体失败" << std::endl;
		return false;
	}

	return true;
}

// 释放顶点缓存
void StartUI::DeleteMaterial()
{
	m_Bg.DeleteMaterial();
	m_Title.DeleteMaterial();
	m_Quit.DestoryButton();
	m_Start.DestoryButton();
}

// 绘制界面
void StartUI::DrawBackGround()
{
	// 背景图
	glm::vec3 scale = glm::vec3((float)Screen::Width(), (float)Screen::Height(), 0.0f);
	m_Bg.DrawMaterial(scale);
}

// 绘制标题
void StartUI::DrawTitle()
{
	// 标题图
	int length;
	if (Screen::Width() > Screen::Height())
		length = Screen::Height();
	else
		length = Screen::Width();

	float width = 0.9f * length;
	float posX = (Screen::Width() - width) / 2;
	float posY = (Screen::Height() - width) / 2;
	glm::vec3 trans(posX + 120.0f, posY + 20.0f, 0.0f);
	glm::vec3 scale(width, width, 0.0f);
	m_Title.DrawMaterial(scale, trans);
}

// 绘制退出按钮
void StartUI::DrawQuit()
{
	// 逻辑
	m_Quit.TriggerButton();

	// 绘制按钮
	glm::vec3 trans(0.10f * Screen::Width(), 0.86f * Screen::Height(), 0.0f);
	glm::vec3 scale(100.0f, 100.0f, 0.0f);
	m_Quit.DrawButton(scale, trans);
}

// 绘制开始按钮
void StartUI::DrawStart()
{
	// 触发器
	m_Start.TriggerButton();

	// 绘制开始按钮
	glm::vec3 trans(0.15f * Screen::Width(), 0.48f * Screen::Height(), 0.0f);
	glm::vec3 scale(320.0f, 80.0f, 0.0f);
	m_Start.DrawAddBack(scale, trans);
}

// 绘制开始按钮的字体
void StartUI::DrawStartFont()
{
	// 绘制字体
	float width = (float)m_StartFont.Width();
	float height = (float)m_StartFont.Height();
	glm::vec3 trans(0.15f * Screen::Width(), 0.48f * Screen::Height(), 0.0f);
	glm::vec3 scale(width, height, 0.0f);
	m_StartFont.ShowPosition(scale, trans);
}

// 绘制开始按钮的字体
void StartUI::DrawQuitFont()
{
	// 绘制字体
	float width = (float)m_StartFont.Width();
	float height = (float)m_StartFont.Height();
	glm::vec3 trans(0.10f * Screen::Width() + 70, 0.86f * Screen::Height(), 0.0f);
	glm::vec3 scale(width, height, 0.0f);
	m_QuitFont.ShowPosition(scale, trans);
}