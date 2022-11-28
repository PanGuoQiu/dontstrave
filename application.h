#ifndef APPLICATION_H
#define APPLICATION_H

#include "stdafx.h"
#include "input.h"
#include "screen.h"

// Ӧ����
class Application
{
public:
	// �̳�ʵ��
	static Application* CreateApplication(const char* className);

	void ApplicationMain();							// �������
	virtual ~Application() {}						// ����������

protected:
	// ���캯��
	Application(const char* className);

	// �̳�ʵ��
	virtual bool Init() = 0;						// ��ʼ��
	virtual void Uninit() = 0;						// �ͷ��ڴ�
	virtual void TickLogic() = 0;					// ÿһ֡�߼�
	virtual void TickRender() = 0;					// ÿһ֡��Ⱦ

	// ͨ�ú���
	void UpdateScreenSize();						// ���´��ڴ�С
	void CallbackFunction();						// ����ص�����
	GLFWwindow* GetWindow();						// ��ô���

private:
	bool ApplicationWindow();						// ��������
	void Run();										// ѭ����Ⱦ

	GLFWwindow* m_Window;							// ���ڽṹ��
	const char* m_ClassName;						// ������
	bool		m_IsFullscreen;						// �Ƿ�ȫ��
};

#endif //APPLICATION_H
