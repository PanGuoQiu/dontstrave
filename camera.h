#ifndef CAMERA_H
#define CAMERA_H

#include "stdafx.h"
#include "input.h"
#include "screen.h"

// 设置相机类型
enum class CameraType { Perspective, Orthographic };

//默认的摄像机变量
const float YAW = -90.0f;
const float PITCH = 0.0f;
const float SPEED = 2.5f;
const float SENSITIVITY = 0.1f;
const float ZOOM = 45.0f;

// 摄像机类
class Camera
{
public:
	// 构造函数和析构函数
	Camera(glm::vec3 position = glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f), float yaw = YAW, float pitch = PITCH);
	Camera(float posX, float posY, float posZ, float upX, float upY, float upZ, float yaw, float pitch);
	~Camera();

	// 获得视图矩阵和投影矩阵
	glm::mat4& GetViewMat() { return m_ViewMat; }
	glm::mat4& GetProjectionMat() { return m_ProjectionMat; }

	// 设置视图矩阵
	void SetView(const glm::vec3& position, const glm::vec3& front, const glm::vec3& up);

	// 设置相机视野
	void SetPerspective(float fovDegrees, float aspectRatio, float nearClip, float farClip);

	// 设置正交相机矩阵
	void SetOrthographic(float left, float right, float bottom, float top, float z_near, float z_far);

	//处理按键
	void ProcessKeyboard(float deltaTime);

	//鼠标处理
	void ProcessMouseMovement(GLboolean constrainPitch = true);

	//处理鼠标滚轴
	void ProcessMouseScroll();

	// 设置清屏颜色
	void SetClearColor(float r, float g, float b, float a) { m_ClearColor = glm::vec4(r, g, b, a); }
	// 设置刷帧清屏内容种类
	void SetClearFlag(unsigned int clearFlag) { m_ClearFlag = clearFlag; }
	// 清空屏幕
	void Clear();

	// 设置相机深度，多相机按从后往前排序，按深度从小往大遍历，深度越大的越上层
	unsigned char GetDepth() { return m_Depth; }		// 获得相机深度
	void SetDepth(unsigned char depth);					// 设置相机深度

	// 裁剪掩码
	unsigned char CullingMask() { return m_CullingMask; }
	void SetCullingMask(unsigned char cullingMask) { m_CullingMask = cullingMask; }
	
	void Sort();										// 相机排序
	static void Foreach(std::function<void()> func);	// 遍历所有的相机

	// 返回当前相机
	static Camera* GetCurrentCamera() { return m_CurrentCamera; }

	// 设置和获得相机的类型
	void SetCameraType(CameraType cType) { m_cType = cType; }
	CameraType GetCameraType() { return m_cType; }

private:
	//计算摄像机向前的向量来自摄像机(更新)欧拉角
	void UpdateCameraVectors();

private:
	// 相机类型
	CameraType m_cType;

	// 摄像机的属性
	glm::vec3 m_Position;								// 位置
	glm::vec3 m_Front;									// 朝向
	glm::vec3 m_Up;										// 向上
	glm::vec3 m_Right;									// 向右
	glm::vec3 m_WorldUp;								// 世界坐标中向上向量
	//欧拉角
	float m_Yaw;										// 上下旋转值
	float m_Pitch;										// 左右旋转值
	//摄像机选项
	float m_MovementSpeed;								// 移动速度
	float m_MouseSensitivity;							// 鼠标敏感度
	float m_Zoom;										// 视角

	// 存储相机时所需的矩阵
	glm::mat4 m_ViewMat;								// 视图矩阵
	glm::mat4 m_ProjectionMat;							// 投影矩阵

	glm::vec4 m_ClearColor;								// 清屏颜色
	unsigned int m_ClearFlag;							// 刷新数据标志

	unsigned char m_Depth;								// 排序深度
	unsigned char m_CullingMask;						// 控制渲染哪些层级的物体

	static std::vector<Camera*>m_AllCamera;				// 存储所有相机，每一帧都遍历
	static Camera* m_CurrentCamera;						// 当前摄像机
};

#endif //CAMERA_H

