#include "stdafx.h"
#include "Application.h"

int main()
{
	Application* appl = Application::getInstance();
	appl->Run();
	delete appl;
	return 0;
}