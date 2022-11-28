#include "camera.h"

//声明静态变量
std::vector<Camera*> Camera::m_AllCamera;					// 存储所有的摄像机
Camera* Camera::m_CurrentCamera;							// 当前摄像机

// 构造函数
Camera::Camera(glm::vec3 position, glm::vec3 up, float yaw, float pitch) : 
	m_Front(glm::vec3(0.0f, 0.0f, -1.0f)),					// 相机朝向
	m_MovementSpeed(SPEED),									// 速度
	m_MouseSensitivity(SENSITIVITY),						// 鼠标敏感度
	m_Zoom(ZOOM),											// 视角
	m_ClearColor(0.1f, 0.1f, 0.1f, 1.0f),					// 清屏颜色
	m_ClearFlag(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT),	// 清除类型
	m_Depth(0),												// 相机深度
	m_CullingMask(0x01)										// 遮挡掩码
{
	// 设置相机属性
	m_Position = position;
	m_WorldUp = up;
	m_Yaw = yaw;
	m_Pitch = pitch;

	// 默认获取现有相机最大深度，设置当前相机深度+1
	if (m_AllCamera.size() > 0)
	{
		unsigned char maxDepth = m_AllCamera.back()->GetDepth();
		m_Depth = maxDepth + 1;
	}

	// 把相机存入数组中
	m_AllCamera.push_back(this);

	// 更新相机属性
	UpdateCameraVectors();
}

// 重载构造函数
Camera::Camera(float posX, float posY, float posZ, float upX, float upY, float upZ, float yaw, float pitch) :
	m_Front(glm::vec3(0.0f, 0.0f, -1.0f)),					// 相机朝向
	m_MovementSpeed(SPEED),									// 速度
	m_MouseSensitivity(SENSITIVITY),						// 鼠标敏感度
	m_Zoom(ZOOM),											// 视角
	m_ClearColor(0.1f, 0.1f, 0.1f, 1.0f),					// 清屏颜色
	m_ClearFlag(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT),	// 清除类型
	m_Depth(0),												// 相机深度
	m_CullingMask(0x01)										// 遮挡掩码
{
	m_Position = glm::vec3(posX, posY, posZ);
	m_WorldUp = glm::vec3(upX, upY, upZ);
	m_Yaw = yaw;
	m_Pitch = pitch;

	// 默认获取现有相机最大深度，设置当前相机深度+1
	if (m_AllCamera.size() > 0)
	{
		unsigned char maxDepth = m_AllCamera.back()->GetDepth();
		m_Depth = maxDepth + 1;
	}

	// 把相机存入数组中
	m_AllCamera.push_back(this);

	// 更新相机属性
	UpdateCameraVectors();
}

// 析构函数
Camera::~Camera()
{
	// 释放所有相机
	auto iter = std::find(m_AllCamera.begin(), m_AllCamera.end(), this);
	if (iter != m_AllCamera.end())
	{
		m_AllCamera.erase(iter);
	}
}

// 设置视图矩阵
void Camera::SetView(const glm::vec3& position, const glm::vec3& front, const glm::vec3& up)
{
	m_ViewMat = glm::lookAt(m_Position, m_Position + m_Front, m_Up);
}

// 设置投影矩阵
void Camera::SetPerspective(float fovDegrees, float aspectRatio, float nearClip, float farcClip)
{
	m_ProjectionMat = glm::perspective(glm::radians(fovDegrees), aspectRatio, nearClip, farcClip);
}

// 设置正交矩阵
void Camera::SetOrthographic(float left, float right, float bottom, float top, float zNear, float zFar)
{
	m_ProjectionMat = glm::ortho(left, right, bottom, top, zNear, zFar);
}

// 清空屏幕
void Camera::Clear()
{
	glClearColor(m_ClearColor.r, m_ClearColor.g, m_ClearColor.b, m_ClearColor.a);
	glClear(m_ClearFlag);
}

// 设置相机深度
void Camera::SetDepth(unsigned char depth)
{
	// 判断设置值是否与当前值相等
	if (m_Depth == depth)
		return;

	// 设置深度，并排序
	m_Depth = depth;
	Sort();
}

// 相机排序
void Camera::Sort()
{
	std::sort(m_AllCamera.begin(), m_AllCamera.end(), [](Camera* a, Camera* b) {
		return a->GetDepth() < b->GetDepth();
	});
}

// 遍历所有相机
void Camera::Foreach(std::function<void()> func)
{
	for (auto iter = m_AllCamera.begin(); iter != m_AllCamera.end(); iter++)
	{
		m_CurrentCamera = *iter;
		m_CurrentCamera->Clear();
		func();
	}
}

// 按键处理
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

// 鼠标处理
void Camera::ProcessMouseMovement(GLboolean constrainPitch)
{
	// 鼠标偏移屏幕中心的距离
	Vec2 mousePos = Input::MousePosition();
	int centerX = Screen::Width() >> 1;
	int centerY = Screen::Height() >> 1;
	float xoffset = (float)(mousePos.m_x - centerX);
	float yoffset = (float)(mousePos.m_y - centerY);

	xoffset *= m_MouseSensitivity;
	yoffset *= m_MouseSensitivity;

	m_Yaw += xoffset;
	m_Pitch += yoffset;

	// 确保角度的范围
	if (constrainPitch)
	{
		if (m_Pitch > 89.0f)
			m_Pitch = 89.0f;
		if (m_Pitch < -89.0f)
			m_Pitch = -89.0f;
	}

	// 更新相机向前向量
	UpdateCameraVectors();
}

// 处理鼠标滚轴
void Camera::ProcessMouseScroll()
{
	short yoffset = Input::MouseScroll();
	m_Zoom -= (float)yoffset;
	if (m_Zoom < 1.0f)
		m_Zoom = 1.0f;
	if (m_Zoom > 45.0f)
		m_Zoom = 45.0f;
}

//计算摄像机向前的向量来自摄像机(更新)欧拉角
void Camera::UpdateCameraVectors()
{
	// 计算新的方向向量
	glm::vec3 front;
	front.x = cos(glm::radians(m_Yaw)) * cos(glm::radians(m_Pitch));
	front.y = sin(glm::radians(m_Pitch));
	front.z = sin(glm::radians(m_Yaw)) * cos(glm::radians(m_Pitch));
	m_Front = glm::normalize(front);

	// 重新计算右向量和向上向量
	m_Right = glm::normalize(glm::cross(m_Front, m_WorldUp));
	m_Up = glm::normalize(glm::cross(m_Right, m_Front));
}
