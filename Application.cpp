#include "stdafx.h"
#include "Application.h"

namespace XKS {
using namespace std;

std::shared_ptr<Application> Application::m_instance(nullptr);

Application::Application()
        : m_mouseSpeed(0.1f),
          m_world(nullptr),
          m_resourceManager(nullptr),
          m_isFocused(GL_TRUE),
          m_clippingDistance(200.f),
          m_FoV(45.f) {
    m_aspect = m_windowWidth / static_cast<GLfloat>(m_windowHeight);
}

std::shared_ptr<Application> Application::getInstance() {
    if (m_instance == nullptr)
        m_instance = std::make_shared<Application>();

    return m_instance;
}

Application::~Application() {
    Unload();
}

void Application::Load() {
    glfwSwapInterval(0);

    auto resourceManager = ResourceManager::GetInstance();
    resourceManager->Load();
    printf("ResourceManager Loaded");

    m_world = std::make_shared<MineWorld>();
    m_world->Load();
    printf("World Loaded");

    glEnable(GL_DEPTH_TEST);
    glEnable(GL_CULL_FACE);
    glEnable(GL_POINT_SMOOTH);
    glHint(GL_POINT_SMOOTH_HINT, GL_NICEST);
    glEnable(GL_LINE_SMOOTH);
    glHint(GL_LINE_SMOOTH_HINT, GL_NICEST);
}

void Application::Unload() {
}

void Application::Draw() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    m_world->Draw();
    glfwSwapBuffers(m_window);
}

void Application::Update(double dt) {
    if (m_isFocused) {
        m_world->Update(dt);
    }
}

void Application::Resize(int width, int height) {
    if (m_world == nullptr)
        return;

    // aspekt obrazu
    if (height > 0)
        m_aspect = width / static_cast<GLfloat>(height);

    // parametry bry³y obcinania - rzutowanie perspektywiczne
    m_world->UpdateProjectionMatrix();
}

void Application::KeyAction(int key, int scancode, int action, int mods) {
    if ((key == GLFW_KEY_ESCAPE) && action == GLFW_RELEASE) {
        glfwSetWindowShouldClose(m_window, GL_TRUE);
    }
}

void Application::MouseAction(int button, int action, int mods) {

}

}
