#ifndef APPLICATION_H
#define APPLICATION_H

#include "stdafx.h"
#include "input.h"
#include "screen.h"

// 应用类
class Application
{
public:
	// 继承实现
	static Application* CreateApplication(const char* className);

	void ApplicationMain();							// 框架主体
	virtual ~Application() {}						// 虚析构函数

protected:
	// 构造函数
	Application(const char* className);

	// 继承实现
	virtual bool Init() = 0;						// 初始化
	virtual void Uninit() = 0;						// 释放内存
	virtual void TickLogic() = 0;					// 每一帧逻辑
	virtual void TickRender() = 0;					// 每一帧渲染

	// 通用函数
	void UpdateScreenSize();						// 更新窗口大小
	void CallbackFunction();						// 程序回调函数
	GLFWwindow* GetWindow();						// 获得窗口

private:
	bool ApplicationWindow();						// 创建窗口
	void Run();										// 循环渲染

	GLFWwindow* m_Window;							// 窗口结构体
	const char* m_ClassName;						// 程序名
	bool		m_IsFullscreen;						// 是否全屏
};

#endif //APPLICATION_H
