#include "stdafx.h"
#include "application.h"

int main()
{
	// ��������ʵ��
	Application* app = Application::CreateApplication("Don't Starve");
	app->ApplicationMain();

	delete app;
	return 0;
}