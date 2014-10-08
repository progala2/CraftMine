#include "stdafx.h"
#include "MyApplication.h"

namespace XKS {
using namespace std;

std::shared_ptr<MyApplication> MyApplication::m_instance(nullptr);

MyApplication::MyApplication()
        : m_mouseSpeed(0.1f),
          m_world(nullptr),
          m_resourceManager(nullptr),
          m_isFocused(GL_TRUE),
          m_clippingDistance(200.f),
          m_FoV(45.f) {
    m_aspect = m_windowWidth / static_cast<GLfloat>(m_windowHeight);
}

std::shared_ptr<MyApplication> MyApplication::getInstance() {
    if (m_instance == nullptr)
        m_instance = std::make_shared<MyApplication>();

    return m_instance;
}

MyApplication::~MyApplication() {
}

void MyApplication::Load() {
    glfwSwapInterval(0);

    auto resourceManager = ResourceManager::GetInstance();
    resourceManager->Load();
    printf("ResourceManager Loaded \n");

    m_world = std::make_shared<MineWorld>();
    m_world->Load();
    printf("World Loaded \n");

    glEnable(GL_DEPTH_TEST);
    glEnable(GL_CULL_FACE);
    glEnable(GL_POINT_SMOOTH);
    glHint(GL_POINT_SMOOTH_HINT, GL_NICEST);
    glEnable(GL_LINE_SMOOTH);
    glHint(GL_LINE_SMOOTH_HINT, GL_NICEST);
}

void MyApplication::Unload() {
    m_world->Unload();
}

void MyApplication::Draw() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    m_world->Draw();
    glfwSwapBuffers(m_window);
}

void MyApplication::Update(double dt) {
    if (m_isFocused) {
        m_world->Update(dt);
    }
}

void MyApplication::Resize(int width, int height) {
    if (m_world == nullptr)
        return;

    // aspekt obrazu
    if (height > 0)
        m_aspect = width / static_cast<GLfloat>(height);

    // parametry bry³y obcinania - rzutowanie perspektywiczne
    m_world->UpdateProjectionMatrix();
}

void MyApplication::KeyAction(int key, int scancode, int action, int mods) {
    if ((key == GLFW_KEY_ESCAPE) && action == GLFW_RELEASE) {
        CloseApplication();
    }
}

void MyApplication::MouseAction(int button, int action, int mods) {

}

}
