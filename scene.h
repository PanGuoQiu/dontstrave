#ifndef SCENE_H
#define SCENE_H

#include "stdafx.h"
#include "camera.h"
#include "application.h"
#include "shader.h"
#include "startui.h"
#include "terrain.h"

// ������
class Scene : public Application
{
public:
	bool Init();							// ��ʼ��
	void Uninit();							// �ͷ��ڴ�
	void TickLogic();						// ÿһ֡�߼�
	void TickRender();						// ÿһ֡��Ⱦ

	void StartMainUI();						// ��ʾ����UI

private:
	friend class Application;				// ������Ϊ��Ԫ��
	Scene(const char* className);			// ���캯��

	//ʱ��
	float m_DeltaTime;						// ʱ���
	float m_LastFrame;						// �ϴ�ʱ��

	// �Զ������
	Shader* m_CurrentShader;				// ��ǰ��ɫ��ָ��
	Camera  m_CameraStartUI;				// ��ʼ�������
	Camera  m_SceneCamera;					// �������
	StartUI m_StartUI;						// ��ʼ����
	Terrain m_Terrain;						// ����
};

#endif SCENE_H
