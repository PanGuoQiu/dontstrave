#include "application.h"

// 键盘回调：记录按下的键
static void KeyCallback(GLFWwindow* window, int key, int scancode, int action, int modes)
{
	Input::RecordKey(key, action);
}

// 鼠标回调：记录按下的键
static void MouseButtonCallback(GLFWwindow* window, int button, int action, int mods)
{
	Input::RecordKey(button, action);
}

// 鼠标回调：设置鼠标的位置
static void MouseMoveCallback(GLFWwindow* window, double x, double y)
{
	Input::SetMousePosition(x, y);
}

// 鼠标回调：记录滚轴的值
static void MouseScrollCallback(GLFWwindow* window, double x, double y)
{
	Input::RecordScroll(y);
}

// 构造函数
Application::Application(const char* className)
{
	m_ClassName = className;
	m_IsFullscreen = false;
}

// 程序主体
void Application::ApplicationMain()
{
	// 创建窗口是否成功
	if (!ApplicationWindow())
	{
		std::cout << "创建窗口失败" << std::endl;
		exit(EXIT_FAILURE);
	}

	// 程序回调函数
	CallbackFunction();

	// 初始化
	if (Init() == false)
	{
		std::cout << "初始化失败" << std::endl;
		exit(EXIT_FAILURE);
	}

	// 循环渲染
	Run();
}

// 更新屏幕大小
void Application::UpdateScreenSize()
{
	// 获得帧缓存大小，并设置视口大小
	int vWidth, vHeight;
	glfwGetFramebufferSize(m_Window, &vWidth, &vHeight);
	glViewport(0, 0, vWidth, vHeight);

	// 获得窗口大小，并设置屏幕
	int wWidth, wHeight;
	glfwGetWindowSize(m_Window, &wWidth, &wHeight);
	Screen::SetWidthHeight(wWidth, wHeight);
}

// 程序回调函数
void Application::CallbackFunction()
{
	glfwSetKeyCallback(m_Window, KeyCallback);					// 设置键盘回调函数
	glfwSetMouseButtonCallback(m_Window, MouseButtonCallback);	// 设置鼠标按键回调函数
	glfwSetScrollCallback(m_Window, MouseScrollCallback);		// 设置鼠标滚轴回调函数
	glfwSetCursorPosCallback(m_Window, MouseMoveCallback);		// 设置鼠标光标位置回调函数
}

// 获得窗口
GLFWwindow* Application::GetWindow()
{
	return m_Window;
}

// 创建窗口
bool Application::ApplicationWindow()
{
	// 初始化glfw
	if (!glfwInit())
	{
		std::cout << "初始化glfw失败！" << std::endl;
		return false;
	}

	// OpenGL使用核心版本号
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
#ifdef __APPLE__
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif //__APPLE__

	// 判断窗口的类型：窗口类型和全屏类型
	if (!m_IsFullscreen)
	{
		// 创建居中窗口
		int cx = GetSystemMetrics(SM_CXSCREEN);
		int cy = GetSystemMetrics(SM_CYSCREEN);
		int wid = (cx - 1440) >> 1;
		int hei = (cy - 900) >> 1;
		glfwWindowHint(GLFW_VISIBLE, GLFW_FALSE);
		m_Window = glfwCreateWindow(1440, 900, m_ClassName, NULL, NULL);
		if (!m_Window)
		{
			std::cout << "创建窗口窗体失败" << std::endl;
			glfwTerminate();
			return false;
		}
		// 设置窗口的位置并显示窗体
		glfwSetWindowPos(m_Window, wid, hei);

		// 设置窗口图标
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
		// 获取监视器
		GLFWmonitor* primary = glfwGetPrimaryMonitor();
		// 得到监视器当前视频模式
		const GLFWvidmode* mode = glfwGetVideoMode(primary);
		// 创建窗体 :宽度、高度、标题、控制全屏、一起共享资源的窗体
		m_Window = glfwCreateWindow(mode->width, mode->height, m_ClassName, primary, NULL);
		if (!m_Window) 
		{
			std::cout << "创建全屏窗体失败！" << std::endl;
			glfwTerminate();
			return false;
		}
	}
	glfwMakeContextCurrent(m_Window);
	
	// glad: 载入所有OpenGL函数指针
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cout << "初始化GLAD失败" << std::endl;
		return false;
	}
	
	// 更新屏幕：交换缓存
	UpdateScreenSize();
	glfwSwapInterval(1);

	return true;
}

void Application::Run()
{
	// 循环渲染
	while (!glfwWindowShouldClose(m_Window))
	{
		// 更新屏幕
		UpdateScreenSize();

		TickLogic();								// 每一时间片的逻辑

		// 更新按键
		Input::Update();

		TickRender();								// 每一时间片的渲染

		glfwSwapBuffers(m_Window);
		glfwPollEvents();
	}

	// 释放内存且销毁窗口
	Uninit();
	glfwDestroyWindow(m_Window);
	glfwTerminate();
}
