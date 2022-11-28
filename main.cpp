#include "stdafx.h"
#include "application.h"

int main()
{
	// 创建窗口实例
	Application* app = Application::CreateApplication("Don't Starve");
	app->ApplicationMain();

	delete app;
	return 0;
}