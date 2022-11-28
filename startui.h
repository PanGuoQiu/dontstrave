#ifndef STARTUI_H
#define STARTUI_H

#include "screen.h"
#include "material.h"
#include "button.h"
#include "Font.h"

// 开始界面类
class StartUI
{
public:
	// 构造函数和析构函数
	StartUI();
	~StartUI();

	// 初始化
	bool Init();

	// 绘制背景
	void DrawBackGround();

	// 绘制标题
	void DrawTitle();

	// 绘制退出按键
	void DrawQuit();
	// 判断是否退出
	bool QuitWindow() { return (m_Quit.GetTrigger() && Input::GetMouseButtonDown(0)); }

	// 绘制开始按键
	void DrawStart();
	// 判断是否开始
	bool StartGame() { return (m_Start.GetTrigger() && Input::GetMouseButtonDown(0)); }

	// 绘制开始字体
	void DrawStartFont();

	// 绘制退出字体
	void DrawQuitFont();

	// 释放顶点缓存
	void DeleteMaterial();

	// 是否显示界面
	void SetStartUI(){m_IsVisible = false; }
	bool ShowStartUI() { return m_IsVisible; }

private:
	bool m_IsVisible;						// 是否显示界面
	Material m_Bg;							// 背景图
	Material m_Title;						// 标题图
	Button m_Quit;							// 退出按钮
	Font m_QuitFont;						// 退出字体
	Button m_Start;							// 开始按钮
	Font m_StartFont;						// 开始字体
};

#endif //STARTUI_H