#include "scene.h"

unsigned int m_VAO, m_VBO;
unsigned int bg;

// ����һ������ʵ��
Application* Application::CreateApplication(const char* className)
{
	static Application* app = new Scene(className);
	return app;
}

// ���캯��
Scene::Scene(const char* className) : Application(className)
{
	// ��ʼ�û��Զ���ı���
	m_DeltaTime = 0.0f;
	m_LastFrame = 0.0f;
}

// ��ʼ��
bool Scene::Init()
{
	//ѭ�����ű�������
	PlaySound(L"sounds/Theme.wav", NULL, SND_FILENAME | SND_ASYNC | SND_LOOP);

	// ��ʼ��������
	m_CameraStartUI.SetCameraType(CameraType::Orthographic);
	m_CameraStartUI.SetClearFlag(GL_COLOR_BUFFER_BIT);

	// �������
	m_SceneCamera.SetCameraType(CameraType::Perspective);
	m_SceneCamera.SetClearColor(0.1f, 0.1f, 0.1f, 1.0f);
	m_SceneCamera.SetClearFlag(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	// ��ʼ��ʼ����
	if (!m_StartUI.Init())
	{
		std::cout << "��ʼ����ʼ����ʧ��" << std::endl;
		return false;
	}

	// ��ʼ������
	if (!m_Terrain.InitTerrain())
	{
		std::cout << "��ʼ������ʧ��" << std::endl;
		return false;
	}

	return true;
}

// �ڴ��ͷ�
void Scene::Uninit()
{
	// �ͷŶ��㻺��
	m_StartUI.DeleteMaterial();
}

// ÿһ֡���߼���������
void Scene::TickLogic()
{
	//ÿһ֡��ʱ���߼�
	float currentFrame = glfwGetTime();
	m_DeltaTime = currentFrame - m_LastFrame;
	m_LastFrame = currentFrame;

	// �رմ���
	if (Input::GetKeyDown(KEY_CODE_ESCAPE) || m_StartUI.QuitWindow())
		glfwSetWindowShouldClose(GetWindow(), true);

	// �رս��棬������Ϸ����
	if (m_StartUI.StartGame())
		m_StartUI.SetStartUI();
}

// ÿһ֡����Ⱦ��������
void Scene::TickRender()
{
	// 3D����
	if (!m_StartUI.ShowStartUI())
	{
		// ���������������
		m_SceneCamera.Clear();
		m_SceneCamera.SetPerspective(45.0f, Screen::AspectRatio(), 0.1f, 100.0f);

		// ����
		// ������ɫ��
		m_CurrentShader = Shader::Find("terrain");
		m_CurrentShader->Active();
		// ������������
		m_CurrentShader->setMat4("projection", m_SceneCamera.GetProjectionMat());
		glm::mat4 viewMat = glm::lookAt(glm::vec3(0.0f, 6.0f, 6.0f), glm::vec3(0.0f, 5.0f, 5.0f), glm::vec3(0.0f, 7.0f, 5.0f));
		m_CurrentShader->setMat4("view", viewMat);
		m_Terrain.RenderTerrain();
	}

	// �Ƿ���ʾ����UI
	if (m_StartUI.ShowStartUI())
	{
		// �������
		m_CameraStartUI.Clear();
		m_CameraStartUI.SetOrthographic(0.0f, (float)Screen::Width(), (float)Screen::Height(), 0.0f, -1.0f, 1.0f);

		// ��ʼ����UI
		StartMainUI();
	}
	else
	{
		// ��ʾ��������
	}
}

// ��ʾ����UI
void Scene::StartMainUI()
{
	// ������ɫ��
	m_CurrentShader = Shader::Find("normal");
	m_CurrentShader->Active();
	// ������������
	m_CurrentShader->setMat4("projection", m_CameraStartUI.GetProjectionMat());
	// ���Ʊ���
	m_StartUI.DrawBackGround();

	// ���Ʊ���
	m_CurrentShader = Shader::Find("title");
	m_CurrentShader->Active();
	// ������������
	m_CurrentShader->setMat4("projection", m_CameraStartUI.GetProjectionMat());
	// ���Ʊ���
	m_StartUI.DrawTitle();

	// ���ư�ť
	m_CurrentShader = Shader::Find("title");
	m_CurrentShader->Active();
	// ������������
	m_CurrentShader->setMat4("projection", m_CameraStartUI.GetProjectionMat());
	// �����˳���ť
	m_StartUI.DrawQuit();

	// ���ư�ť
	m_CurrentShader = Shader::Find("title");
	m_CurrentShader->Active();
	// ������������
	m_CurrentShader->setMat4("projection", m_CameraStartUI.GetProjectionMat());
	// ���ƿ�ʼ��ť
	m_StartUI.DrawStart();

	// ���ƿ�ʼ����
	m_CurrentShader = Shader::Find("title");
	m_CurrentShader->Active();
	// ������������
	m_CurrentShader->setMat4("projection", m_CameraStartUI.GetProjectionMat());
	// ���ƿ�ʼ��ť
	m_StartUI.DrawStartFont();

	// ���ƿ�ʼ����
	m_CurrentShader = Shader::Find("title");
	m_CurrentShader->Active();
	// ������������
	m_CurrentShader->setMat4("projection", m_CameraStartUI.GetProjectionMat());
	// ���ƿ�ʼ��ť
	m_StartUI.DrawQuitFont();
}