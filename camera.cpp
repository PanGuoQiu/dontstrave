#include "camera.h"

//������̬����
std::vector<Camera*> Camera::m_AllCamera;					// �洢���е������
Camera* Camera::m_CurrentCamera;							// ��ǰ�����

// ���캯��
Camera::Camera(glm::vec3 position, glm::vec3 up, float yaw, float pitch) : 
	m_Front(glm::vec3(0.0f, 0.0f, -1.0f)),					// �������
	m_MovementSpeed(SPEED),									// �ٶ�
	m_MouseSensitivity(SENSITIVITY),						// ������ж�
	m_Zoom(ZOOM),											// �ӽ�
	m_ClearColor(0.1f, 0.1f, 0.1f, 1.0f),					// ������ɫ
	m_ClearFlag(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT),	// �������
	m_Depth(0),												// ������
	m_CullingMask(0x01)										// �ڵ�����
{
	// �����������
	m_Position = position;
	m_WorldUp = up;
	m_Yaw = yaw;
	m_Pitch = pitch;

	// Ĭ�ϻ�ȡ������������ȣ����õ�ǰ������+1
	if (m_AllCamera.size() > 0)
	{
		unsigned char maxDepth = m_AllCamera.back()->GetDepth();
		m_Depth = maxDepth + 1;
	}

	// ���������������
	m_AllCamera.push_back(this);

	// �����������
	UpdateCameraVectors();
}

// ���ع��캯��
Camera::Camera(float posX, float posY, float posZ, float upX, float upY, float upZ, float yaw, float pitch) :
	m_Front(glm::vec3(0.0f, 0.0f, -1.0f)),					// �������
	m_MovementSpeed(SPEED),									// �ٶ�
	m_MouseSensitivity(SENSITIVITY),						// ������ж�
	m_Zoom(ZOOM),											// �ӽ�
	m_ClearColor(0.1f, 0.1f, 0.1f, 1.0f),					// ������ɫ
	m_ClearFlag(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT),	// �������
	m_Depth(0),												// ������
	m_CullingMask(0x01)										// �ڵ�����
{
	m_Position = glm::vec3(posX, posY, posZ);
	m_WorldUp = glm::vec3(upX, upY, upZ);
	m_Yaw = yaw;
	m_Pitch = pitch;

	// Ĭ�ϻ�ȡ������������ȣ����õ�ǰ������+1
	if (m_AllCamera.size() > 0)
	{
		unsigned char maxDepth = m_AllCamera.back()->GetDepth();
		m_Depth = maxDepth + 1;
	}

	// ���������������
	m_AllCamera.push_back(this);

	// �����������
	UpdateCameraVectors();
}

// ��������
Camera::~Camera()
{
	// �ͷ��������
	auto iter = std::find(m_AllCamera.begin(), m_AllCamera.end(), this);
	if (iter != m_AllCamera.end())
	{
		m_AllCamera.erase(iter);
	}
}

// ������ͼ����
void Camera::SetView(const glm::vec3& position, const glm::vec3& front, const glm::vec3& up)
{
	m_ViewMat = glm::lookAt(m_Position, m_Position + m_Front, m_Up);
}

// ����ͶӰ����
void Camera::SetPerspective(float fovDegrees, float aspectRatio, float nearClip, float farcClip)
{
	m_ProjectionMat = glm::perspective(glm::radians(fovDegrees), aspectRatio, nearClip, farcClip);
}

// ������������
void Camera::SetOrthographic(float left, float right, float bottom, float top, float zNear, float zFar)
{
	m_ProjectionMat = glm::ortho(left, right, bottom, top, zNear, zFar);
}

// �����Ļ
void Camera::Clear()
{
	glClearColor(m_ClearColor.r, m_ClearColor.g, m_ClearColor.b, m_ClearColor.a);
	glClear(m_ClearFlag);
}

// ����������
void Camera::SetDepth(unsigned char depth)
{
	// �ж�����ֵ�Ƿ��뵱ǰֵ���
	if (m_Depth == depth)
		return;

	// ������ȣ�������
	m_Depth = depth;
	Sort();
}

// �������
void Camera::Sort()
{
	std::sort(m_AllCamera.begin(), m_AllCamera.end(), [](Camera* a, Camera* b) {
		return a->GetDepth() < b->GetDepth();
	});
}

// �����������
void Camera::Foreach(std::function<void()> func)
{
	for (auto iter = m_AllCamera.begin(); iter != m_AllCamera.end(); iter++)
	{
		m_CurrentCamera = *iter;
		m_CurrentCamera->Clear();
		func();
	}
}

// ��������
void Camera::ProcessKeyboard(float deltaTime)
{
	float velocity = m_MovementSpeed * deltaTime;
	if (Input::GetKeyDown(KEY_CODE_W))
		m_Position += m_Front * velocity;
	if (Input::GetKeyDown(KEY_CODE_S))
		m_Position -= m_Front * velocity;
	if (Input::GetKeyDown(KEY_CODE_A))
		m_Position -= m_Right * velocity;
	if (Input::GetKeyDown(KEY_CODE_D))
		m_Position += m_Right * velocity;
}

// ��괦��
void Camera::ProcessMouseMovement(GLboolean constrainPitch)
{
	// ���ƫ����Ļ���ĵľ���
	Vec2 mousePos = Input::MousePosition();
	int centerX = Screen::Width() >> 1;
	int centerY = Screen::Height() >> 1;
	float xoffset = (float)(mousePos.m_x - centerX);
	float yoffset = (float)(mousePos.m_y - centerY);

	xoffset *= m_MouseSensitivity;
	yoffset *= m_MouseSensitivity;

	m_Yaw += xoffset;
	m_Pitch += yoffset;

	// ȷ���Ƕȵķ�Χ
	if (constrainPitch)
	{
		if (m_Pitch > 89.0f)
			m_Pitch = 89.0f;
		if (m_Pitch < -89.0f)
			m_Pitch = -89.0f;
	}

	// ���������ǰ����
	UpdateCameraVectors();
}

// ����������
void Camera::ProcessMouseScroll()
{
	short yoffset = Input::MouseScroll();
	m_Zoom -= (float)yoffset;
	if (m_Zoom < 1.0f)
		m_Zoom = 1.0f;
	if (m_Zoom > 45.0f)
		m_Zoom = 45.0f;
}

//�����������ǰ���������������(����)ŷ����
void Camera::UpdateCameraVectors()
{
	// �����µķ�������
	glm::vec3 front;
	front.x = cos(glm::radians(m_Yaw)) * cos(glm::radians(m_Pitch));
	front.y = sin(glm::radians(m_Pitch));
	front.z = sin(glm::radians(m_Yaw)) * cos(glm::radians(m_Pitch));
	m_Front = glm::normalize(front);

	// ���¼�������������������
	m_Right = glm::normalize(glm::cross(m_Front, m_WorldUp));
	m_Up = glm::normalize(glm::cross(m_Right, m_Front));
}
