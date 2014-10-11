/*
 * Camera.cpp
 *
 *  Created on: Sep 30, 2014
 *      Author: p.rogala
 */
#include "stdafx.h"
#include "Camera.h"

namespace XKS {
void Camera::UpdateAngles(float hAngle, float vAngle) {
    m_vAngle = vAngle;
    m_hAngle = hAngle;
    m_dir = glm::normalize(glm::vec3(cos(m_vAngle) * sin(m_hAngle), sin(m_vAngle), cos(m_vAngle) * cos(m_hAngle)));
    m_right = glm::vec3(sin(m_hAngle - 3.14f * 0.5f), 0, cos(m_hAngle - 3.14f * 0.5f));
    m_up = glm::cross(m_right, m_dir);
}

void Camera::UpdateViewMatrix(float fov) {
    m_lookAt = glm::lookAt(m_pos, m_pos + m_dir, m_up);
}

}

