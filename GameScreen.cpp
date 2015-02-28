#include "stdafx.h"
#include "GameScreen.h"
#include "MineWorld.h"
#include "OpenGl.h"

namespace XKS {

GameScreen::GameScreen()
    : m_world(nullptr) {
    
}

GameScreen::~GameScreen() {
}

void GameScreen::Load() {
    

    m_world = std::make_shared<MineWorld>();
    m_world->Load();
    printf("World Loaded \n");
}

void GameScreen::Unload() {
    m_world->Unload();
}

void GameScreen::Draw() {
    m_world->Draw();
}

void GameScreen::Update(double dt) {
    if (ApplicationWindow::GetInstance()->IsFocused()) {
        m_world->Update(dt);
    }
}

void GameScreen::Resize(int width, int height) {
    if (m_world == nullptr)
        return;

    // parametry bry³y obcinania - rzutowanie perspektywiczne
    auto app = ApplicationWindow::GetInstance();
    OpenGL::GetInstance()->UpdateProjectionMatrix(app->GetFoV(), app->GetAspect(), 0.1f, app->GetClippingDistance());
}

void GameScreen::KeyAction(int key, int scancode, int action, int mods) {
    if ((key == GLFW_KEY_ESCAPE) && action == GLFW_RELEASE) {
        ApplicationWindow::GetInstance()->CloseApplication();
    }
}

void GameScreen::MouseAction(int button, int action, int mods) {

}

}