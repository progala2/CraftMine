#ifndef _XKS_CREATURE_
#define _XKS_CREATURE_
#include "stdafx.h"
#include "Delegate.h"

namespace XKS {

class Creature;
struct DelegateCreatureOnMoveData {
    Creature* creature;
    GLdouble dt;
    glm::vec3 oldPos;
    DelegateCreatureOnMoveData(Creature* cre, GLdouble _dt, glm::vec3 old)
            : creature(cre),
              dt(_dt),
              oldPos(old) {
    }
};
typedef Delegate<DelegateCreatureOnMoveData> DelegateCreatureOnMove;

class Creature {
 protected:
    glm::vec3 m_position, m_force, m_velocity;
    GLfloat m_speed;
    DelegateCreatureOnMove m_delegateOnMove;
 public:
    explicit Creature(glm::vec3 pos = glm::vec3(0, 0, 0))
            : m_position(pos),
              m_speed(2.0f),
              m_force(0, 0, 0),
              m_velocity(0, 0, 0) {
    }
    virtual ~Creature() {
    }
    virtual void Draw() = 0;
    virtual void Update(double dt) = 0;

    virtual void setPosition(const glm::vec3& pos) {
        m_position = pos;
    }
    virtual glm::vec3 getPosition() const {
        return m_position;
    }

    virtual void setForce(const glm::vec3& force) {
        m_force = force;
    }
    virtual glm::vec3 getForce() const {
        return m_force;
    }

    virtual void setVelocity(const glm::vec3& velocity) {
        m_velocity = velocity;
    }
    virtual glm::vec3 getVelocity() const {
        return m_velocity;
    }

    virtual void setSpeed(GLfloat speed) {
        m_speed = speed;
    }
    virtual GLfloat getSpeed() const {
        return m_speed;
    }

    void setDelOnMove(DelegateCreatureOnMove& dlgPlr) {
        m_delegateOnMove = dlgPlr;
    }
};

}
#endif
