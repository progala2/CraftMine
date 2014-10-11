#include "stdafx.h"
#include "Player.h"
#include "Application.h"

namespace XKS {

Player::Player(glm::vec3 pos)
        : Creature(pos) {
    m_camera.reset(new Camera(glm::vec3(pos.x, pos.y + 2, pos.z), 45.0f));
}

Player::~Player() {
}

void Player::Update(double dt) {
    auto application = ApplicationWindow::GetInstance();
    double xpos, ypos;
    int width, height;
    float mouseSpeed = application->GetMouseSpeed();
    float hAngle = m_camera->GetHAngle();
    float vAngle = m_camera->GetVAngle();
    application->GetCursorPos(xpos, ypos);
    application->GetWindowWidthHeight(width, height);
    application->SetCursorPos(width / 2, height / 2);

    m_velocity += m_force * float(dt);
    m_force = glm::vec3(0, 0, 0);

    // Compute new orientation
    hAngle += mouseSpeed * float(dt * (width / 2 - xpos));
    float y = vAngle + mouseSpeed * float(dt * (height / 2 - ypos));
    if (y > -1.52 && y < 1.52)
        vAngle = y;
    m_camera->UpdateAngles(hAngle, vAngle);

    glm::vec3 direction = m_camera->GetDirection();
    glm::vec3 right = m_camera->GetRightVec();

    glm::vec3 newPos(0, 0, 0);
    if (application->GetKey(GLFW_KEY_SPACE) == GLFW_PRESS && m_velocity.y == 0) {
        m_velocity.y = 15.0f;
    }

    direction.y = 0;
    direction = glm::normalize(direction);
    if (application->GetKey(GLFW_KEY_UP) == GLFW_PRESS) {
        m_force += direction * m_speed;
    }
    // Move backward
    else if (application->GetKey(GLFW_KEY_DOWN) == GLFW_PRESS) {
        m_force -= direction * m_speed;
    }
    // Strafe right
    if (application->GetKey(GLFW_KEY_RIGHT) == GLFW_PRESS) {
        m_force += right * m_speed;
    } else if (application->GetKey(GLFW_KEY_LEFT) == GLFW_PRESS) {
        m_force -= right * m_speed;
    }
    if (application->GetKey(GLFW_KEY_1) == GLFW_PRESS) {
        const glm::vec3& dir = m_camera->GetDirection();
        printf("\n%f %f %f \n", dir.x, dir.y, dir.z);
    }
    if (application->GetKey(GLFW_KEY_2) == GLFW_PRESS) {
        const glm::vec3& pos = m_camera->GetPosition();
        printf("\n%f %f %f \n", pos.x, pos.y, pos.z);
    }

    newPos += m_velocity * float(dt);
    SetPosition(m_position + newPos);

    m_delegateOnMove(DelegateCreatureOnMoveData(this->shared_from_this(), dt, m_position - newPos));
    m_camera->UpdateViewMatrix(0);
}

void Player::Draw() {

}

}  //nmspc XKS
