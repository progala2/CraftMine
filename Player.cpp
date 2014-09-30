#include "stdafx.h"
#include "Player.h"
#include "Application.h"

namespace XKS {

Player::Player(glm::vec3 pos)
        : Creature(pos) {
    m_camera = new Camera(pos, 45.0f);
    m_camera->m_pos.y += 2;
}

Player::~Player() {
    delete m_camera;
}

void Player::Update(double dt) {
    Application* application = Application::getInstance();
    double xpos, ypos;
    int width, height;
    float mouseSpeed = application->GetMouseSpeed();
    application->GetCursorPos(xpos, ypos);
    application->GetWindowWidthHeight(width, height);
    application->SetCursorPos(width / 2, height / 2);

    m_velocity += m_force * float(dt);
    m_force = glm::vec3(0, 0, 0);

    // Compute new orientation
    m_camera->m_hAngle += mouseSpeed * float(dt * (width / 2 - xpos));
    float y = m_camera->m_vAngle + mouseSpeed * float(dt * (height / 2 - ypos));
    if (y > -1.52 && y < 1.52)
        m_camera->m_vAngle = y;

    glm::vec3 direction = glm::normalize(m_camera->GetDirection());
    m_camera->m_dir = direction;
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
        printf("\n%f %f %f \n", m_camera->m_dir.x, m_camera->m_dir.y, m_camera->m_dir.z);
    }
    if (application->GetKey(GLFW_KEY_2) == GLFW_PRESS) {
        printf("\n%f %f %f \n", m_camera->m_pos.x, m_camera->m_pos.y, m_camera->m_pos.z);
    }

    newPos += m_velocity * float(dt);
    m_position += newPos;

    m_delegateOnMove(DelegateCreatureOnMoveData(this, dt, m_position - newPos));
}

void Player::Draw() {

}

}  //nmspc XKS
