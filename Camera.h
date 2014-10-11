#ifndef _XKS_CAMERA_H_
#define _XKS_CAMERA_H_

#include "stdafx.h"

namespace XKS {
class Camera {
    // position
    glm::vec3 m_pos, m_dir, m_right, m_up;
    float m_vAngle;
    float m_hAngle;
    glm::mat4 m_lookAt;
 public:
    Camera(const glm::vec3& pos, float FoV)
            : m_pos(pos) {
        UpdateAngles(-1, -1);
        UpdateViewMatrix(FoV);
    }

    void UpdateAngles(float addHAngle, float addVAngle);
    void UpdateViewMatrix(float fov);

    const glm::vec3& GetDirection() const {
        return m_dir;
    }

    const glm::vec3& GetRightVec() const {
        return m_right;
    }

    const glm::vec3& GetUpVec() const {
        return m_up;
    }

    const glm::mat4& LookAt() const {
        return m_lookAt;
    }

    const glm::vec3& GetPosition() const {
        return m_pos;
    }

    float GetVAngle() const {
        return m_vAngle;
    }

    float GetHAngle() const {
        return m_hAngle;
    }

    void SetPosition(const glm::vec3& pos) {
        m_pos = pos;
    }
};
}

#endif
