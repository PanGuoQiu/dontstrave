#ifndef STARTUI_H
#define STARTUI_H

#include "screen.h"
#include "material.h"
#include "button.h"
#include "Font.h"

// ��ʼ������
class StartUI
{
public:
	// ���캯������������
	StartUI();
	~StartUI();

	// ��ʼ��
	bool Init();

	// ���Ʊ���
	void DrawBackGround();

	// ���Ʊ���
	void DrawTitle();

	// �����˳�����
	void DrawQuit();
	// �ж��Ƿ��˳�
	bool QuitWindow() { return (m_Quit.GetTrigger() && Input::GetMouseButtonDown(0)); }

	// ���ƿ�ʼ����
	void DrawStart();
	// �ж��Ƿ�ʼ
	bool StartGame() { return (m_Start.GetTrigger() && Input::GetMouseButtonDown(0)); }

	// ���ƿ�ʼ����
	void DrawStartFont();

	// �����˳�����
	void DrawQuitFont();

	// �ͷŶ��㻺��
	void DeleteMaterial();

	// �Ƿ���ʾ����
	void SetStartUI(){m_IsVisible = false; }
	bool ShowStartUI() { return m_IsVisible; }

private:
	bool m_IsVisible;						// �Ƿ���ʾ����
	Material m_Bg;							// ����ͼ
	Material m_Title;						// ����ͼ
	Button m_Quit;							// �˳���ť
	Font m_QuitFont;						// �˳�����
	Button m_Start;							// ��ʼ��ť
	Font m_StartFont;						// ��ʼ����
};

#endif //STARTUI_H