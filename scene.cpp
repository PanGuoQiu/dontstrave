#include "scene.h"

unsigned int m_VAO, m_VBO;
unsigned int bg;

// 创建一个程序实例
Application* Application::CreateApplication(const char* className)
{
	static Application* app = new Scene(className);
	return app;
}

// 构造函数
Scene::Scene(const char* className) : Application(className)
{
	// 初始用户自定义的变量
	m_DeltaTime = 0.0f;
	m_LastFrame = 0.0f;
}

// 初始化
bool Scene::Init()
{
	//循环播放背景音乐
	PlaySound(L"sounds/Theme.wav", NULL, SND_FILENAME | SND_ASYNC | SND_LOOP);

	// 开始界面的相机
	m_CameraStartUI.SetCameraType(CameraType::Orthographic);
	m_CameraStartUI.SetClearFlag(GL_COLOR_BUFFER_BIT);

	// 场景相机
	m_SceneCamera.SetCameraType(CameraType::Perspective);
	m_SceneCamera.SetClearColor(0.1f, 0.1f, 0.1f, 1.0f);
	m_SceneCamera.SetClearFlag(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	// 初始开始界面
	if (!m_StartUI.Init())
	{
		std::cout << "初始化开始界面失败" << std::endl;
		return false;
	}

	// 初始化地面
	if (!m_Terrain.InitTerrain())
	{
		std::cout << "初始化地面失败" << std::endl;
		return false;
	}

	return true;
}

// 内存释放
void Scene::Uninit()
{
	// 释放顶点缓存
	m_StartUI.DeleteMaterial();
}

// 每一帧的逻辑，即更新
void Scene::TickLogic()
{
	//每一帧的时间逻辑
	float currentFrame = glfwGetTime();
	m_DeltaTime = currentFrame - m_LastFrame;
	m_LastFrame = currentFrame;

	// 关闭窗口
	if (Input::GetKeyDown(KEY_CODE_ESCAPE) || m_StartUI.QuitWindow())
		glfwSetWindowShouldClose(GetWindow(), true);

	// 关闭界面，进入游戏场景
	if (m_StartUI.StartGame())
		m_StartUI.SetStartUI();
}

// 每一帧的渲染，即绘制
void Scene::TickRender()
{
	// 3D场景
	if (!m_StartUI.ShowStartUI())
	{
		// 清除缓存和设置相机
		m_SceneCamera.Clear();
		m_SceneCamera.SetPerspective(45.0f, Screen::AspectRatio(), 0.1f, 100.0f);

		// 地面
		// 激活着色器
		m_CurrentShader = Shader::Find("terrain");
		m_CurrentShader->Active();
		// 设置正交矩阵
		m_CurrentShader->setMat4("projection", m_SceneCamera.GetProjectionMat());
		glm::mat4 viewMat = glm::lookAt(glm::vec3(0.0f, 6.0f, 6.0f), glm::vec3(0.0f, 5.0f, 5.0f), glm::vec3(0.0f, 7.0f, 5.0f));
		m_CurrentShader->setMat4("view", viewMat);
		m_Terrain.RenderTerrain();
	}

	// 是否显示界面UI
	if (m_StartUI.ShowStartUI())
	{
		// 清除缓存
		m_CameraStartUI.Clear();
		m_CameraStartUI.SetOrthographic(0.0f, (float)Screen::Width(), (float)Screen::Height(), 0.0f, -1.0f, 1.0f);

		// 开始界面UI
		StartMainUI();
	}
	else
	{
		// 显示场景界面
	}
}

// 显示界面UI
void Scene::StartMainUI()
{
	// 激活着色器
	m_CurrentShader = Shader::Find("normal");
	m_CurrentShader->Active();
	// 设置正交矩阵
	m_CurrentShader->setMat4("projection", m_CameraStartUI.GetProjectionMat());
	// 绘制背景
	m_StartUI.DrawBackGround();

	// 绘制标题
	m_CurrentShader = Shader::Find("title");
	m_CurrentShader->Active();
	// 设置正交矩阵
	m_CurrentShader->setMat4("projection", m_CameraStartUI.GetProjectionMat());
	// 绘制标题
	m_StartUI.DrawTitle();

	// 绘制按钮
	m_CurrentShader = Shader::Find("title");
	m_CurrentShader->Active();
	// 设置正交矩阵
	m_CurrentShader->setMat4("projection", m_CameraStartUI.GetProjectionMat());
	// 绘制退出按钮
	m_StartUI.DrawQuit();

	// 绘制按钮
	m_CurrentShader = Shader::Find("title");
	m_CurrentShader->Active();
	// 设置正交矩阵
	m_CurrentShader->setMat4("projection", m_CameraStartUI.GetProjectionMat());
	// 绘制开始按钮
	m_StartUI.DrawStart();

	// 绘制开始字体
	m_CurrentShader = Shader::Find("title");
	m_CurrentShader->Active();
	// 设置正交矩阵
	m_CurrentShader->setMat4("projection", m_CameraStartUI.GetProjectionMat());
	// 绘制开始按钮
	m_StartUI.DrawStartFont();

	// 绘制开始字体
	m_CurrentShader = Shader::Find("title");
	m_CurrentShader->Active();
	// 设置正交矩阵
	m_CurrentShader->setMat4("projection", m_CameraStartUI.GetProjectionMat());
	// 绘制开始按钮
	m_StartUI.DrawQuitFont();
}