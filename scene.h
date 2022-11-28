#ifndef SCENE_H
#define SCENE_H

#include "stdafx.h"
#include "camera.h"
#include "application.h"
#include "shader.h"
#include "startui.h"
#include "terrain.h"

// 场景类
class Scene : public Application
{
public:
	bool Init();							// 初始化
	void Uninit();							// 释放内存
	void TickLogic();						// 每一帧逻辑
	void TickRender();						// 每一帧渲染

	void StartMainUI();						// 显示界面UI

private:
	friend class Application;				// 父类作为友元类
	Scene(const char* className);			// 构造函数

	//时间
	float m_DeltaTime;						// 时间差
	float m_LastFrame;						// 上次时间

	// 自定义变量
	Shader* m_CurrentShader;				// 当前着色器指针
	Camera  m_CameraStartUI;				// 开始的摄像机
	Camera  m_SceneCamera;					// 场景相机
	StartUI m_StartUI;						// 开始界面
	Terrain m_Terrain;						// 地面
};

#endif SCENE_H
