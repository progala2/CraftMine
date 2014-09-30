#include "stdafx.h"
#include "MineWorld.h"

namespace XKS {

MineWorld::~MineWorld() {
    Unload();
}

void MineWorld::UpdateProjectionMatrix() {
    Application* application = Application::getInstance();
    m_projectionMatrix = glm::perspective(application->GetFoV(), application->GetAspect(), 0.1f,
                                          application->GetClippingDistance());
    m_gravityAcceleration = 9.83f;
}

void MineWorld::GenerateWorld(std::pair<const glm::ivec3, Chunk*>& chunk) {
    std::srand(m_seed);
    glm::ivec3 pos(chunk.first.x * Chunk::ms_chunkSize.x, chunk.first.y * Chunk::ms_chunkSize.y,
                   chunk.first.z * Chunk::ms_chunkSize.z);
    float p = std::rand() / float(RAND_MAX), r = std::rand() / float(RAND_MAX), s = std::rand() / float(RAND_MAX), t =
            std::rand() / float(RAND_MAX);

    for (GLubyte x = 0; x < Chunk::ms_chunkSize.x; ++x) {
        for (GLubyte z = 0; z < Chunk::ms_chunkSize.z; ++z) {
            glm::vec3 pos2(pos.x + x, pos.z + z, t);
            pos2 *= (s + r) * (p + t);
            int max = 20 + int(75 * glm::perlin(pos2));
            pos2.z = s;
            BlockTypeNumber block = BlockTypeNumber(4 * glm::perlin(pos2));
            for (GLubyte y = 0; y < max; ++y) {
                chunk.second->Set(x, y, z, block);
            }
        }
    }
}

glm::ivec3 MineWorld::transformPositionToChunk(const glm::vec3& pos) const {

    return glm::ivec3(
            pos.x < 0 ?
                    int((pos.x - Chunk::ms_chunkSize.x) / Chunk::ms_chunkSize.x) : int(pos.x / Chunk::ms_chunkSize.x),
            int(pos.y / Chunk::ms_chunkSize.y),
            pos.z < 0 ?
                    int((pos.z - Chunk::ms_chunkSize.z) / Chunk::ms_chunkSize.z) : int(pos.z / Chunk::ms_chunkSize.z));
}

glm::ivec3 MineWorld::transformPositionToBlock(const glm::vec3& pos) const {
    return glm::ivec3(
            pos.x < 0 ?
                    15 - std::abs((int(pos.x) - Chunk::ms_chunkSize.x) % Chunk::ms_chunkSize.x) :
                    int(pos.x) % Chunk::ms_chunkSize.x,
            int(pos.y) % Chunk::ms_chunkSize.y,
            pos.z < 0 ?
                    15 - std::abs((int(pos.z) - Chunk::ms_chunkSize.z) % Chunk::ms_chunkSize.z) :
                    int(pos.z) % Chunk::ms_chunkSize.z);
}

void MineWorld::Load() {
    Application* application = Application::getInstance();
    ResourceManager* resourceManager = ResourceManager::getInstance();

    double time = glfwGetTime();

    UpdateProjectionMatrix();
    m_program = new CubeShader();
    m_program->Attach();
    m_seed = unsigned(glfwGetTime());
    m_texturesID = resourceManager->getTextureArrID();

    m_player = new Player(glm::vec3(0, 84, 0));
    m_player->setDelOnMove(
            DelegateCreatureOnMove::from_method<MineWorld, &MineWorld::onCreatureMove>(const_cast<MineWorld*>(this)));

    glm::ivec3 a;
    a.y = 0;
    GLint distance = GLint(application->GetClippingDistance() / Chunk::ms_chunkSize.z) + 4;
    m_distance = distance;

    for (a.z = -distance; a.z < distance; ++a.z) {
        for (a.x = -distance; a.x < distance; ++a.x) {
            m_chunks[a] = new Chunk(m_program);
        }
    }

    for (auto it = m_chunks.begin(); it != m_chunks.end(); ++it) {
        glm::ivec3 pos(it->first);
        Chunk* obj = it->second;

        --pos.x;
        auto temp = m_chunks.find(pos);
        if (temp != m_chunks.end())
            obj->SetNeighbour(DIR_LEFT, temp->second);

        pos.x += 2;
        temp = m_chunks.find(pos);
        if (temp != m_chunks.end())
            obj->SetNeighbour(DIR_RIGHT, temp->second);

        pos.x = it->first.x;
        ++pos.z;
        temp = m_chunks.find(pos);
        if (temp != m_chunks.end())
            obj->SetNeighbour(DIR_FRONT, temp->second);

        pos.z -= 2;
        temp = m_chunks.find(pos);
        if (temp != m_chunks.end())
            obj->SetNeighbour(DIR_BACK, temp->second);

        pos.z = it->first.z;
        --pos.y;
        temp = m_chunks.find(pos);
        if (temp != m_chunks.end())
            obj->SetNeighbour(DIR_DOWN, temp->second);

        pos.y += 2;
        temp = m_chunks.find(pos);
        if (temp != m_chunks.end())
            obj->SetNeighbour(DIR_UP, temp->second);
        m_chunksBuildingQueue.Add(*it);
    }
    for (int i = 0; i < 3; ++i)
        m_threadBuilding[i] = new std::thread([this] {
            for(;;)
            {
                while(m_chunksBuildingQueue.Size() != 0) {
                    auto obj = m_chunksBuildingQueue.Remove();
                    GenerateWorld(obj);
                    m_chunkUpdateQueue.Add(obj.second);
                }
                while(m_chunkUpdateQueue.Size() != 0) {
                    Chunk* obj = m_chunkUpdateQueue.Remove();
                    obj->UpdateVertexes();
                    m_chunkTransferQueue.Add(obj);
                }
                std::this_thread::sleep_for(std::chrono::microseconds(1));
            }

        });
    for (int i = 0; i < 3; ++i) {
        m_threadBuilding[i]->detach();
    }
    printf(" %Lf s", glfwGetTime() - time);
    std::this_thread::sleep_for(std::chrono::duration<int>(1));
}

void MineWorld::Unload() {
    auto end = m_chunks.end();
    for (auto it = m_chunks.begin(); it != end; ++it) {
        delete it->second;
    }
    m_chunks.clear();
}

void MineWorld::Update(double dt) {
    while (m_chunkTransferQueue.Size() > 0) {
        Chunk* obj = m_chunkTransferQueue.Remove();
        obj->MoveToGraphic();
    }
    for (auto it = m_chunks.begin(); it != m_chunks.end(); ++it)
        if (it->second->isModified())
            m_chunkUpdateQueue.Add(it->second);

    m_player->Update(dt);
}

void MineWorld::Draw() {
    Application* application = Application::getInstance();
    GLenum err = glGetError();
    GLubyte flag = VSBL_FCS_ALL;
    Camera* camera = m_player->getCamera();
    auto dir = camera->GetDirection();

    GLfloat aspect = application->GetAspect();
    aspect -= (aspect - 1) * 0.5f;
    if (dir.x < -0.5 * aspect)
        flag ^= VSBL_FCS_LEFT;
    if (dir.x > 0.5 * aspect)
        flag ^= VSBL_FCS_RIGHT;
    if (dir.y < -0.5)
        flag ^= VSBL_FCS_DOWN;
    if (dir.y > 0.5)
        flag ^= VSBL_FCS_UP;
    if (dir.z < -0.5 * aspect)
        flag ^= VSBL_FCS_BACK;
    if (dir.z > 0.5 * aspect)
        flag ^= VSBL_FCS_FRONT;
    m_program->Attach();
    glBindTexture(GL_TEXTURE_2D_ARRAY, m_texturesID);

    glm::mat4 mvp = camera->LookAt();
    m_program->SetVision(mvp);
    m_program->SetProjection(m_projectionMatrix);
    m_program->SetLight(camera->m_pos);

    glm::vec3 cameraPos(camera->GetPosition());
    for (auto it = m_chunks.begin(); it != m_chunks.end(); ++it) {
        glm::vec3 pos = glm::vec3((it->first.x) * Chunk::ms_chunkSize.x, 0, (it->first.z) * Chunk::ms_chunkSize.z);
        glm::vec3 cpVec = pos - cameraPos;
        GLfloat length = std::max(cpVec.x < 0 ? -(cpVec.x + 15) : cpVec.x, cpVec.z < 0 ? -(cpVec.z + 15) : cpVec.z);
        if (length > application->GetClippingDistance() + 7)
            continue;

        GLubyte cflag = VSBL_FCS_ALL;
        if (pos.x < cameraPos.x - Chunk::ms_chunkSize.x)
            cflag ^= VSBL_FCS_LEFT;
        if (pos.x > cameraPos.x)
            cflag ^= VSBL_FCS_RIGHT;
        if (pos.z < cameraPos.z - Chunk::ms_chunkSize.z)
            cflag ^= VSBL_FCS_BACK;
        if (pos.z > cameraPos.z)
            cflag ^= VSBL_FCS_FRONT;
        it->second->SetFacesToDraw(flag & cflag);

        m_program->SetWorldPos(pos);
        it->second->Draw();
    }
    glBindTexture(GL_TEXTURE_2D_ARRAY, 0);
    glBindVertexArray(0);
}

void MineWorld::onCreatureMove(const DelegateCreatureOnMoveData& info) {
    glm::vec3 newPos(m_player->getPosition());
    glm::vec3 vel(m_player->getVelocity());
    glm::vec3 force(m_player->getForce());
    GLfloat speed(m_player->getSpeed());

    if (newPos.y < 0) {
        newPos.y = 0;
        vel.y = 0;
    }

    glm::vec3 tmpPos = newPos, newPos2 = newPos;
    glm::vec3 oldPos = info.oldPos;
    glm::vec3 vec(newPos - oldPos);

    if (vec.x < 0 || vec.x > 0) {
        if (vec.x < 0)
            tmpPos.x -= 0.5f;
        else
            tmpPos.x += 0.5f;
        glm::ivec3 chunkPos = transformPositionToChunk(tmpPos);
        glm::ivec3 blockPos = transformPositionToBlock(tmpPos);
        if (m_chunks.find(chunkPos) != m_chunks.end()) {
            if (m_chunks[chunkPos]->isSolid(blockPos.x, blockPos.y, blockPos.z)) {
                vel.x = 0;
                newPos2.x = oldPos.x;
            }
        }
    }
    tmpPos = newPos2;
    if (vec.z < 0 || vec.z > 0) {
        if (vec.z < 0)
            tmpPos.z -= 0.5f;
        else
            tmpPos.z += 0.5f;
        glm::ivec3 chunkPos = transformPositionToChunk(tmpPos);
        glm::ivec3 blockPos = transformPositionToBlock(tmpPos);
        if (m_chunks.find(chunkPos) != m_chunks.end()) {
            if (m_chunks[chunkPos]->isSolid(blockPos.x, blockPos.y, blockPos.z)) {
                vel.z = 0;
                newPos2.z = oldPos.z;
            }
        }
    }
    tmpPos = newPos2;
    if (vec.y < 0 || vec.y > 0) {
        if (vec.y < 0)
            tmpPos.y -= 1;
        else
            tmpPos.y += 0.2f;
        glm::ivec3 chunkPos = transformPositionToChunk(tmpPos);
        glm::ivec3 blockPos = transformPositionToBlock(tmpPos);
        if (m_chunks.find(chunkPos) != m_chunks.end()) {
            if (m_chunks[chunkPos]->isSolid(blockPos.x, blockPos.y, blockPos.z)) {
                vel.y = 0;
                newPos2.y = oldPos.y;
            } else
                force.y = -m_gravityAcceleration;
        } else
            force.y = -m_gravityAcceleration;
    } else
        force.y = -m_gravityAcceleration;

    float len = glm::length(vel);
    if (vel != glm::ZERO<glm::vec3>()) {
        glm::vec3 normForce(glm::normalize(vel));
        force -= normForce * len * len * 0.5f * 0.1f * 1.2f;
        if (!vel.y) {
            force.x -= glm::min(normForce.x * 1.0f, vel.x);
            force.z -= glm::min(normForce.z * 1.0f, vel.z);
        }
    }

    m_player->setVelocity(vel);
    m_player->setPosition(newPos2);
    m_player->setForce(force);
}

}
