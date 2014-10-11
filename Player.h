#ifndef _XKS_PLAYER_
#define _XKS_PLAYER_
#include "stdafx.h"
#include "Creature.h"
#include "Camera.h"
#include "Delegate.h"

namespace XKS {

class Player : public Creature {
 public:
    Player(glm::vec3 pos = glm::vec3(0, 0, 0));
    ~Player();

    void Draw();
    void Update(double dt);

    void SetPosition(const glm::vec3& pos) {
        Creature::setPosition(pos);
        m_camera->SetPosition(glm::vec3(pos.x, pos.y + 2, pos.z));
    }

    std::shared_ptr<Camera> GetCamera() const {
        return m_camera;
    }

 private:
     std::shared_ptr<Camera> m_camera;
};

}
#endif
