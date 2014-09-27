#include "stdafx.h"
#include "Application.h"

int main()
{
	XKS::Application* appl = XKS::Application::getInstance();
	appl->Run();
	delete appl;
	return 0;
}