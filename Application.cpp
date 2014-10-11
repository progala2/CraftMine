#include "stdafx.h"
#include "Application.h"

using namespace std;
namespace XKS {

std::shared_ptr<ApplicationWindow> ApplicationWindow::ms_instance = nullptr;

void ApplicationWindow::WindowResizeCallback(GLFWwindow* window, int width, int height) {
    if (ms_instance) {
        ms_instance->OnResize(width, height);
		ms_instance->m_screen->Resize(width, height);
	}
}

void ApplicationWindow::WindowKeyCallback(GLFWwindow* window, int key, int scancode, int action, int mods) {
    if (ms_instance) {
        ms_instance->KeyAction(key, scancode, action, mods);
    }
}

void ApplicationWindow::WindowCloseCallback(GLFWwindow* window) {
    if (ms_instance) {
        ms_instance->Destroy();
	}
}

void ApplicationWindow::WindowFocusCallback(GLFWwindow* window, int focus) {
    if (ms_instance) {
        ms_instance->Focus(focus);
    }
}

void ApplicationWindow::WindowMouseButtonCallback(GLFWwindow* window, int button, int action, int mods) {
    if (ms_instance) {
        ms_instance->MouseAction(button, action, mods);
    }
}

void ApplicationWindow::Initialize(std::unique_ptr<Screen> startScreen) {
    if (!glfwInit())
        throw exception("Error in glfwInit!");
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 2);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
    m_window = glfwCreateWindow(m_windowWidth, m_windowHeight, "OpenGL Tutorial", NULL, NULL);
    if (m_window == nullptr)
        throw exception("Error in glfwOpenWindow!");
    glfwMakeContextCurrent(m_window);
    glfwSetWindowSizeCallback(m_window, ApplicationWindow::WindowResizeCallback);
    glfwSetWindowCloseCallback(m_window, ApplicationWindow::WindowCloseCallback);
    glfwSetWindowFocusCallback(m_window, ApplicationWindow::WindowFocusCallback);
    glfwSetKeyCallback(m_window, ApplicationWindow::WindowKeyCallback);
    glfwSetMouseButtonCallback(m_window, ApplicationWindow::WindowMouseButtonCallback);
    glewExperimental = GL_TRUE;
    if (glewInit() != GLEW_OK)
        throw exception("Error in glewInit!");
    glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
    StartNewScreen(std::move(startScreen));
}

void ApplicationWindow::Update(double dt) {
    m_screen->Update(dt);
}

void ApplicationWindow::Draw() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    m_screen->Draw();
    glfwSwapBuffers(m_window);
}

void ApplicationWindow::MainLoop() {
    double lastTime = glfwGetTime();
    int nbFrames = 0;
    double time = glfwGetTime();
    do {
        Draw();
        double newTime = glfwGetTime();
        Update(newTime - time);
        time = newTime;
        // Measure speed

        glfwPollEvents();
        double currentTime = glfwGetTime();
        nbFrames++;
        if (currentTime - lastTime >= 1.0) {  // If last prinf() was more than 1 sec ago

            // printf and reset timer
            char str[30];
            sprintf_s(str, 30, "%.1f frames/sec\n", nbFrames / (currentTime - lastTime));
            glfwSetWindowTitle(m_window, str);
            lastTime = currentTime;
            nbFrames = 0;
        }
    } while (!glfwWindowShouldClose(m_window));
}

void ApplicationWindow::Run(std::unique_ptr<Screen> startScreen) {
    try {
        Initialize(std::move(startScreen));
        MainLoop();
    } catch (exception& e) {
        cerr << e.what() << endl;
        system("pause");
    }
}

ApplicationWindow::ApplicationWindow()
        : m_windowWidth(800),
          m_windowHeight(600),
          m_window(nullptr),
          m_mouseSpeed(0.1f),
          m_isFocused(GL_TRUE),
          m_clippingDistance(200.f),
          m_FoV(45.f) {
    m_aspect = m_windowWidth / static_cast<GLfloat>(m_windowHeight);
}

ApplicationWindow::~ApplicationWindow() {
    ms_instance = NULL;
    glfwDestroyWindow(m_window);
    glfwTerminate();
}

void ApplicationWindow::OnResize(int width, int height) {
    m_windowWidth = width;
    m_windowHeight = height;
    glViewport(0, 0, m_windowWidth, m_windowHeight);
    if (height > 0)
        m_aspect = width / static_cast<GLfloat>(height);
    m_screen->Resize(width, height);
}

void ApplicationWindow::Focus(int a) {
    m_isFocused = a != 0;
}

void ApplicationWindow::Destroy() {
}

void ApplicationWindow::KeyAction(int key, int scancode, int action, int mods) {
    m_screen->KeyAction(key, scancode, action, mods);
}

void ApplicationWindow::MouseAction(int button, int action, int mods) {
    m_screen->MouseAction(button, action, mods);
}

}
