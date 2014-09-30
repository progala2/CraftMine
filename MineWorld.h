#ifndef _XKS_MWORLD_H_
#define _XKS_MWORLD_H_
#include "stdafx.h"
#include "CubeShader.h"
#include "Chunk.h"
#include "Utility.h"
#include "Camera.h"
#include "Application.h"
#include "ResourceManager.h"
#include "WQueue.h"
#include "Player.h"
#include "World.h"

namespace XKS {

class MineWorld : public World {
 public:
    typedef std::map<glm::ivec3, std::shared_ptr<Chunk> > ChunksMap;
    typedef std::pair<const glm::ivec3, std::shared_ptr<Chunk> > ChunksMapPair;

    ~MineWorld();

    void Load();
    void Unload();
    void Update(double dt);
    void Draw();

    void GenerateWorld(ChunksMapPair& chunk);
    glm::ivec3 transformPositionToChunk(const glm::vec3& pos) const;
    glm::ivec3 transformPositionToBlock(const glm::vec3& pos) const;

    float getGravityAcceleration() const {
        return m_gravityAcceleration;
    }

    void onCreatureMove(const DelegateCreatureOnMoveData& info);

    void UpdateProjectionMatrix();

 private:

    void MoveToGraphic();

    std::shared_ptr<CubeShader> m_program;

    GLuint m_texturesID;
    ChunksMap m_chunks;
    std::unique_ptr<Player> m_player;
    glm::mat4 m_viewMatrix, m_projectionMatrix;
    GLuint m_seed;
    GLint m_distance;
    std::unique_ptr<std::thread> m_threadBuilding[3], m_threadUpdate;
    WQueue<std::shared_ptr<Chunk> > m_chunkUpdateQueue, m_chunkTransferQueue;
    WQueue<ChunksMapPair> m_chunksBuildingQueue;

    float m_gravityAcceleration;
};

}
#endif
