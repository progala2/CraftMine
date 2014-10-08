#include "stdafx.h"
#include "Application.h"

using namespace std;
namespace XKS {

ApplicationWindow *ApplicationWindow::m_instance = NULL;

void ApplicationWindow::WindowResizeCallback(GLFWwindow* window, int width, int height) {
    if (m_instance)
        m_instance->OnResize(width, height);
}

void ApplicationWindow::WindowKeyCallback(GLFWwindow* window, int key, int scancode, int action, int mods) {
    if (m_instance) {
        m_instance->KeyAction(key, scancode, action, mods);
    }
}

void ApplicationWindow::WindowCloseCallback(GLFWwindow* window) {
    if (m_instance)
        m_instance->Destroy();
}

void ApplicationWindow::WindowFocusCallback(GLFWwindow* window, int focus) {
    if (m_instance) {
        m_instance->Focus(focus);
    }
}

void ApplicationWindow::WindowMouseButtonCallback(GLFWwindow* window, int button, int action, int mods) {
    if (m_instance) {
        m_instance->MouseAction(button, action, mods);
    }
}

void ApplicationWindow::Initialize() {
    if (!glfwInit())
        throw exception("Blad glfwInit!");
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 2);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
    m_window = glfwCreateWindow(m_windowWidth, m_windowHeight, "OpenGL Tutorial", NULL, NULL);
    if (m_window == nullptr)
        throw exception("Blad glfwOpenWindow!");
    glfwMakeContextCurrent(m_window);
    glfwSetWindowSizeCallback(m_window, ApplicationWindow::WindowResizeCallback);
    glfwSetWindowCloseCallback(m_window, ApplicationWindow::WindowCloseCallback);
    glfwSetWindowFocusCallback(m_window, ApplicationWindow::WindowFocusCallback);
    glfwSetKeyCallback(m_window, ApplicationWindow::WindowKeyCallback);
    glfwSetMouseButtonCallback(m_window, ApplicationWindow::WindowMouseButtonCallback);
    glewExperimental = GL_TRUE;
    if (glewInit() != GLEW_OK)
        throw exception("Blad glewInit!");
    glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
    Load();
}

void ApplicationWindow::Draw() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
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

void ApplicationWindow::Run() {
    try {
        Initialize();
        MainLoop();
    } catch (exception& e) {
        cerr << e.what() << endl;
        system("pause");
    }
}

ApplicationWindow::ApplicationWindow()
        : m_windowWidth(800),
          m_windowHeight(600),
          m_window(nullptr) {
    if (m_instance)
        throw exception("It can't be made another instance of this class!");
    m_instance = this;
}

ApplicationWindow::~ApplicationWindow() {
    m_instance = NULL;
    Unload();
    glfwDestroyWindow(m_window);
    glfwTerminate();
}

void ApplicationWindow::OnResize(int width, int height) {
    m_windowWidth = width;
    m_windowHeight = height;
    glViewport(0, 0, m_windowWidth, m_windowHeight);
    Resize(width, height);
}
