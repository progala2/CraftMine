#include "stdafx.h"
#include "Application.h"
#include "GameScreen.h"

int main() {
	std::shared_ptr<XKS::ApplicationWindow> appl = XKS::ApplicationWindow::GetInstance();
	appl->Run(std::unique_ptr<XKS::Screen>(new XKS::GameScreen));
    return 0;
}
