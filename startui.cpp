#include "startui.h"

// ���캯��
StartUI::StartUI() : m_IsVisible(true)
{
}

// ��������
StartUI::~StartUI()
{
}

// ��ʼ��
bool StartUI::Init()
{
	// ����ͼ
	if (!m_Bg.InitMaterial("normal", "bgTexture","textures/bg_redux_dark_right.png", 0, 0, 1792, 1024, false))
	{
		std::cout << "���뱳������ʧ��" << std::endl;
		return false;
	}

	// ����ͼ
	if (!m_Title.InitMaterial("title", "bgTexture", "textures/frontscreen.png", 0, 0, 860, 860, false))
	{
		std::cout << "����������ʧ��" << std::endl;
		return false;
	}

	// �˳���ť
	if (!m_Quit.InitButton("quit", "bgTexture",
		"textures/frontend.png", 1940, 0, 100, 100,
		"textures/frontend.png", 850, 720, 100, 100))
	{
		std::cout << "�����˳���ťʧ��" << std::endl;
		return false;
	}
	if (!m_QuitFont.InitFont("quit", "bgTexture", "Quit"))
	{
		std::cout << "��ʼ���˳���ť������ʧ��" << std::endl;
		return false;
	}

	// ��ʼ��ť
	if (!m_Start.InitButton("quit", "bgTexture",
		"textures/frontscreen.png", 1464, 254, 394, 124,
		"textures/frontscreen.png", 1464, 130, 394, 124))
	{
		std::cout << "���뿪ʼ��ťʧ��" << std::endl;
		return false;
	}

	// ��ʼ��ť������
	if (!m_StartFont.InitFont("quit", "bgTexture", "Play"))
	{
		std::cout << "��ʼ����ʼ��ť������ʧ��" << std::endl;
		return false;
	}

	return true;
}

// �ͷŶ��㻺��
void StartUI::DeleteMaterial()
{
	m_Bg.DeleteMaterial();
	m_Title.DeleteMaterial();
	m_Quit.DestoryButton();
	m_Start.DestoryButton();
}

// ���ƽ���
void StartUI::DrawBackGround()
{
	// ����ͼ
	glm::vec3 scale = glm::vec3((float)Screen::Width(), (float)Screen::Height(), 0.0f);
	m_Bg.DrawMaterial(scale);
}

// ���Ʊ���
void StartUI::DrawTitle()
{
	// ����ͼ
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

// �����˳���ť
void StartUI::DrawQuit()
{
	// �߼�
	m_Quit.TriggerButton();

	// ���ư�ť
	glm::vec3 trans(0.10f * Screen::Width(), 0.86f * Screen::Height(), 0.0f);
	glm::vec3 scale(100.0f, 100.0f, 0.0f);
	m_Quit.DrawButton(scale, trans);
}

// ���ƿ�ʼ��ť
void StartUI::DrawStart()
{
	// ������
	m_Start.TriggerButton();

	// ���ƿ�ʼ��ť
	glm::vec3 trans(0.15f * Screen::Width(), 0.48f * Screen::Height(), 0.0f);
	glm::vec3 scale(320.0f, 80.0f, 0.0f);
	m_Start.DrawAddBack(scale, trans);
}

// ���ƿ�ʼ��ť������
void StartUI::DrawStartFont()
{
	// ��������
	float width = (float)m_StartFont.Width();
	float height = (float)m_StartFont.Height();
	glm::vec3 trans(0.15f * Screen::Width(), 0.48f * Screen::Height(), 0.0f);
	glm::vec3 scale(width, height, 0.0f);
	m_StartFont.ShowPosition(scale, trans);
}

// ���ƿ�ʼ��ť������
void StartUI::DrawQuitFont()
{
	// ��������
	float width = (float)m_StartFont.Width();
	float height = (float)m_StartFont.Height();
	glm::vec3 trans(0.10f * Screen::Width() + 70, 0.86f * Screen::Height(), 0.0f);
	glm::vec3 scale(width, height, 0.0f);
	m_QuitFont.ShowPosition(scale, trans);
}