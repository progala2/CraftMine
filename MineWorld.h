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

class MineWorld : public World, public std::enable_shared_from_this<MineWorld> {
 public:
    typedef std::map<glm::ivec3, std::shared_ptr<Chunk> > ChunksMap;
    typedef std::pair<const glm::ivec3, std::shared_ptr<Chunk> > ChunksMapPair;

    MineWorld(): m_drawingDistance(15) {
    }

    ~MineWorld();

    void Load();
    void Unload();
    void Update(GLdouble dt);
    void Draw();

    void GenerateWorld(const ChunksMapPair& chunk);
    glm::ivec3 transformPositionToChunk(const glm::vec3& pos) const;
    glm::ivec3 transformPositionToBlock(const glm::vec3& pos) const;

    float getGravityAcceleration() const {
        return m_gravityAcceleration;
    }

    void setDrawingDistance(GLint dist) {
        m_drawingDistance = dist;
    }

    GLint getDrawingDistance() {
        return m_drawingDistance;
    }

    void onCreatureMove(const DelegateCreatureOnMoveData& info);

 private:

    void MoveToGraphic();
    static void BuildingChunksThreadFunction(std::weak_ptr<MineWorld>);
    static void UpdatingChunksThreadFunction(std::weak_ptr<MineWorld>);

    std::shared_ptr<CubeShader> m_program;

    GLuint m_texturesID;
    ChunksMap m_chunks;
    std::shared_ptr<Player> m_player;
    glm::mat4 m_viewMatrix;
    GLuint m_seed;
    GLint m_distance;
    std::unique_ptr<std::thread> m_threadBuilding, m_threadUpdate;
    WQueue<std::shared_ptr<Chunk> > m_chunkUpdateQueue, m_chunkTransferQueue;
    WQueue<ChunksMapPair> m_chunksBuildingQueue;

    GLfloat m_gravityAcceleration;
    GLint m_drawingDistance;
};

}
#endif
