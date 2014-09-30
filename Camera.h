#ifndef _XKS_CAMERA_H_
#define _XKS_CAMERA_H_

#include "stdafx.h"

namespace XKS {
class Camera {
    // position
    glm::vec3 m_pos, m_dir, m_right, m_up;
    glm::mat4 m_lookAt;
 public:
    Camera(const glm::vec3& pos, float FoV)
            : m_pos(pos) {
        Update(m_pos, -1, -1, FoV);
    }

    void Update(const glm::vec3& pos, float hAngle, float vAngle, float fov);

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
};
}

#endif
