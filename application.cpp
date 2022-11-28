#include "application.h"

// ���̻ص�����¼���µļ�
static void KeyCallback(GLFWwindow* window, int key, int scancode, int action, int modes)
{
	Input::RecordKey(key, action);
}

// ���ص�����¼���µļ�
static void MouseButtonCallback(GLFWwindow* window, int button, int action, int mods)
{
	Input::RecordKey(button, action);
}

// ���ص�����������λ��
static void MouseMoveCallback(GLFWwindow* window, double x, double y)
{
	Input::SetMousePosition(x, y);
}

// ���ص�����¼�����ֵ
static void MouseScrollCallback(GLFWwindow* window, double x, double y)
{
	Input::RecordScroll(y);
}

// ���캯��
Application::Application(const char* className)
{
	m_ClassName = className;
	m_IsFullscreen = false;
}

// ��������
void Application::ApplicationMain()
{
	// ���������Ƿ�ɹ�
	if (!ApplicationWindow())
	{
		std::cout << "��������ʧ��" << std::endl;
		exit(EXIT_FAILURE);
	}

	// ����ص�����
	CallbackFunction();

	// ��ʼ��
	if (Init() == false)
	{
		std::cout << "��ʼ��ʧ��" << std::endl;
		exit(EXIT_FAILURE);
	}

	// ѭ����Ⱦ
	Run();
}

// ������Ļ��С
void Application::UpdateScreenSize()
{
	// ���֡�����С���������ӿڴ�С
	int vWidth, vHeight;
	glfwGetFramebufferSize(m_Window, &vWidth, &vHeight);
	glViewport(0, 0, vWidth, vHeight);

	// ��ô��ڴ�С����������Ļ
	int wWidth, wHeight;
	glfwGetWindowSize(m_Window, &wWidth, &wHeight);
	Screen::SetWidthHeight(wWidth, wHeight);
}

// ����ص�����
void Application::CallbackFunction()
{
	glfwSetKeyCallback(m_Window, KeyCallback);					// ���ü��̻ص�����
	glfwSetMouseButtonCallback(m_Window, MouseButtonCallback);	// ������갴���ص�����
	glfwSetScrollCallback(m_Window, MouseScrollCallback);		// ����������ص�����
	glfwSetCursorPosCallback(m_Window, MouseMoveCallback);		// ���������λ�ûص�����
}

// ��ô���
GLFWwindow* Application::GetWindow()
{
	return m_Window;
}

// ��������
bool Application::ApplicationWindow()
{
	// ��ʼ��glfw
	if (!glfwInit())
	{
		std::cout << "��ʼ��glfwʧ�ܣ�" << std::endl;
		return false;
	}

	// OpenGLʹ�ú��İ汾��
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
#ifdef __APPLE__
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif //__APPLE__

	// �жϴ��ڵ����ͣ��������ͺ�ȫ������
	if (!m_IsFullscreen)
	{
		// �������д���
		int cx = GetSystemMetrics(SM_CXSCREEN);
		int cy = GetSystemMetrics(SM_CYSCREEN);
		int wid = (cx - 1440) >> 1;
		int hei = (cy - 900) >> 1;
		glfwWindowHint(GLFW_VISIBLE, GLFW_FALSE);
		m_Window = glfwCreateWindow(1440, 900, m_ClassName, NULL, NULL);
		if (!m_Window)
		{
			std::cout << "�������ڴ���ʧ��" << std::endl;
			glfwTerminate();
			return false;
		}
		// ���ô��ڵ�λ�ò���ʾ����
		glfwSetWindowPos(m_Window, wid, hei);

		// ���ô���ͼ��
		GLFWimage icon;
		icon.width = 32;
		icon.height = 32;
		int channels = 4;
		icon.pixels = stbi_load("textures/icon.png", &icon.width, &icon.height, &channels, 4);
		glfwSetWindowIcon(m_Window, 1, &icon);

		glfwShowWindow(m_Window);
	}
	else
	{
		// ��ȡ������
		GLFWmonitor* primary = glfwGetPrimaryMonitor();
		// �õ���������ǰ��Ƶģʽ
		const GLFWvidmode* mode = glfwGetVideoMode(primary);
		// �������� :��ȡ��߶ȡ����⡢����ȫ����һ������Դ�Ĵ���
		m_Window = glfwCreateWindow(mode->width, mode->height, m_ClassName, primary, NULL);
		if (!m_Window) 
		{
			std::cout << "����ȫ������ʧ�ܣ�" << std::endl;
			glfwTerminate();
			return false;
		}
	}
	glfwMakeContextCurrent(m_Window);
	
	// glad: ��������OpenGL����ָ��
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cout << "��ʼ��GLADʧ��" << std::endl;
		return false;
	}
	
	// ������Ļ����������
	UpdateScreenSize();
	glfwSwapInterval(1);

	return true;
}

void Application::Run()
{
	// ѭ����Ⱦ
	while (!glfwWindowShouldClose(m_Window))
	{
		// ������Ļ
		UpdateScreenSize();

		TickLogic();								// ÿһʱ��Ƭ���߼�

		// ���°���
		Input::Update();

		TickRender();								// ÿһʱ��Ƭ����Ⱦ

		glfwSwapBuffers(m_Window);
		glfwPollEvents();
	}

	// �ͷ��ڴ������ٴ���
	Uninit();
	glfwDestroyWindow(m_Window);
	glfwTerminate();
}
