#include "stdafx.h"
#include "OpenGl.h"

namespace XKS {

std::shared_ptr<OpenGL> OpenGL::ms_instance = nullptr;

OpenGL::OpenGL() {
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_CULL_FACE);
    glEnable(GL_LINE_SMOOTH);
    glHint(GL_LINE_SMOOTH_HINT, GL_NICEST);
}

}