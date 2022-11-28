#ifndef CAMERA_H
#define CAMERA_H

#include "stdafx.h"
#include "input.h"
#include "screen.h"

// �����������
enum class CameraType { Perspective, Orthographic };

//Ĭ�ϵ����������
const float YAW = -90.0f;
const float PITCH = 0.0f;
const float SPEED = 2.5f;
const float SENSITIVITY = 0.1f;
const float ZOOM = 45.0f;

// �������
class Camera
{
public:
	// ���캯������������
	Camera(glm::vec3 position = glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f), float yaw = YAW, float pitch = PITCH);
	Camera(float posX, float posY, float posZ, float upX, float upY, float upZ, float yaw, float pitch);
	~Camera();

	// �����ͼ�����ͶӰ����
	glm::mat4& GetViewMat() { return m_ViewMat; }
	glm::mat4& GetProjectionMat() { return m_ProjectionMat; }

	// ������ͼ����
	void SetView(const glm::vec3& position, const glm::vec3& front, const glm::vec3& up);

	// ���������Ұ
	void SetPerspective(float fovDegrees, float aspectRatio, float nearClip, float farClip);

	// ���������������
	void SetOrthographic(float left, float right, float bottom, float top, float z_near, float z_far);

	//������
	void ProcessKeyboard(float deltaTime);

	//��괦��
	void ProcessMouseMovement(GLboolean constrainPitch = true);

	//����������
	void ProcessMouseScroll();

	// ����������ɫ
	void SetClearColor(float r, float g, float b, float a) { m_ClearColor = glm::vec4(r, g, b, a); }
	// ����ˢ֡������������
	void SetClearFlag(unsigned int clearFlag) { m_ClearFlag = clearFlag; }
	// �����Ļ
	void Clear();

	// ���������ȣ���������Ӻ���ǰ���򣬰���ȴ�С������������Խ���Խ�ϲ�
	unsigned char GetDepth() { return m_Depth; }		// ���������
	void SetDepth(unsigned char depth);					// ����������

	// �ü�����
	unsigned char CullingMask() { return m_CullingMask; }
	void SetCullingMask(unsigned char cullingMask) { m_CullingMask = cullingMask; }
	
	void Sort();										// �������
	static void Foreach(std::function<void()> func);	// �������е����

	// ���ص�ǰ���
	static Camera* GetCurrentCamera() { return m_CurrentCamera; }

	// ���úͻ�����������
	void SetCameraType(CameraType cType) { m_cType = cType; }
	CameraType GetCameraType() { return m_cType; }

private:
	//�����������ǰ���������������(����)ŷ����
	void UpdateCameraVectors();

private:
	// �������
	CameraType m_cType;

	// �����������
	glm::vec3 m_Position;								// λ��
	glm::vec3 m_Front;									// ����
	glm::vec3 m_Up;										// ����
	glm::vec3 m_Right;									// ����
	glm::vec3 m_WorldUp;								// ������������������
	//ŷ����
	float m_Yaw;										// ������תֵ
	float m_Pitch;										// ������תֵ
	//�����ѡ��
	float m_MovementSpeed;								// �ƶ��ٶ�
	float m_MouseSensitivity;							// ������ж�
	float m_Zoom;										// �ӽ�

	// �洢���ʱ����ľ���
	glm::mat4 m_ViewMat;								// ��ͼ����
	glm::mat4 m_ProjectionMat;							// ͶӰ����

	glm::vec4 m_ClearColor;								// ������ɫ
	unsigned int m_ClearFlag;							// ˢ�����ݱ�־

	unsigned char m_Depth;								// �������
	unsigned char m_CullingMask;						// ������Ⱦ��Щ�㼶������

	static std::vector<Camera*>m_AllCamera;				// �洢���������ÿһ֡������
	static Camera* m_CurrentCamera;						// ��ǰ�����
};

#endif //CAMERA_H

