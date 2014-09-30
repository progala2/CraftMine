/*
 * Camera.cpp
 *
 *  Created on: Sep 30, 2014
 *      Author: p.rogala
 */
#include "Camera.h"

namespace XKS {
void Camera::Update(const glm::vec3& pos, float hAngle, float vAngle, float fov) {
    m_dir = glm::vec3(cos(vAngle) * sin(hAngle), sin(vAngle), cos(vAngle) * cos(hAngle));
    m_right = glm::vec3(sin(hAngle - 3.14f * 0.5f), 0, cos(hAngle - 3.14f * 0.5f));
    m_up = glm::cross(m_right, m_dir);
    m_lookAt = glm::lookAt(m_pos, m_pos + m_dir, m_up);
}
}

