#ifndef _XKS_WORLD_H_
#define _XKS_WORLD_H_
#include "stdafx.h"
#include "CubeShader.h"
#include "Chunk.h"
#include "Utility.h"
#include "Camera.h"
#include "Application.h"
#include "ResourceManager.h"
#include "WQueue.h"
#include "Player.h"
namespace XKS {

class World {
 public:
    virtual void Load() = 0;
    virtual void Unload() = 0;
    virtual void Update(double dt) = 0;
    virtual void Draw() = 0;

    virtual ~World() {}

    float getGravityAcceleration() const {
        return m_gravityAcceleration;
    }

    virtual void onCreatureMove(const DelegateCreatureOnMoveData& info) {}

 protected:
    float m_gravityAcceleration;
};

}
#endif
