#include "stdafx.h"
#include "Application.h"

int main() {
    XKS::MyApplication* appl = XKS::MyApplication::getInstance();
    appl->Run();
    delete appl;
    return 0;
}
