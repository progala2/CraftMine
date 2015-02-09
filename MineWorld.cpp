#include "stdafx.h"
#include "MineWorld.h"
#include "OpenGl.h"

namespace XKS {

MineWorld::~MineWorld() {
}

void MineWorld::GenerateWorld(const ChunksMapPair& chunk) {
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
            if (max < 1)
                max = 1;
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
    auto application = ApplicationWindow::GetInstance();
    auto resourceManager = ResourceManager::GetInstance();

    m_gravityAcceleration = 9.83f;

    OpenGL::GetInstance()->UpdateProjectionMatrix(application->GetFoV(), application->GetAspect(), 0.1f, application->GetClippingDistance());
    m_program = std::make_shared<CubeShader>();
    m_program->Attach();
    m_seed = unsigned(4);
    m_texturesID = resourceManager->getTextureArrID();

    m_player = std::make_shared<Player>(glm::vec3(0, 84, 0));
    m_player->setDelOnMove(
        DelegateCreatureOnMove::from_method<MineWorld, &MineWorld::onCreatureMove>(this->shared_from_this()));

    glm::ivec3 a;
    a.y = 0;
    GLint distance = GLint(application->GetClippingDistance() / Chunk::ms_chunkSize.z) + 4;
    m_distance = distance;

    for (a.z = -distance; a.z < distance; ++a.z) {
        for (a.x = -distance; a.x < distance; ++a.x) {
            m_chunks[a] = std::make_shared<Chunk>(m_program, glm::vec3(a.x * Chunk::ms_chunkSize.x, 0, a.z * Chunk::ms_chunkSize.z));
        }
    }

    for (auto it = m_chunks.begin(); it != m_chunks.end(); ++it) {
        glm::ivec3 pos(it->first);
        auto obj = it->second;

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

    m_threadBuilding.reset(new std::thread(&MineWorld::BuildingChunksThreadFunction, this->shared_from_this()));
    m_threadUpdate.reset(new std::thread(&MineWorld::UpdatingChunksThreadFunction, this->shared_from_this()));
    m_threadBuilding->detach();
    m_threadUpdate->detach();
    std::this_thread::sleep_for(std::chrono::duration<int>(1));
}

void MineWorld::Unload() {
    m_chunks.clear();
}

void MineWorld::Update(double dt) {
    while (m_chunkTransferQueue.Size() > 0) {
        std::shared_ptr<Chunk> obj = m_chunkTransferQueue.Remove();
        obj->MoveToGraphic();
    }
    for (auto it = m_chunks.begin(); it != m_chunks.end(); ++it)
        if (it->second->isModified())
            m_chunkUpdateQueue.Add(it->second);

    m_player->Update(dt);
}

GLuint time = 0;
void MineWorld::Draw() {
    auto application = ApplicationWindow::GetInstance();
    GLenum err = glGetError();
    GLubyte flag = VSBL_FCS_ALL;
    auto camera = m_player->GetCamera();
    glm::vec4 dir = glm::vec4(camera->GetDirection(), 0);
    
    time++;

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

    glm::mat4 mView = camera->LookAt();
    m_program->SetVision(mView);
    auto projection = OpenGL::GetInstance()->GetProjectionMatrix();
    m_program->SetProjection(projection);
    auto mvp = projection * mView;
    m_program->SetLight(glm::vec3(0, 100, 0));

    glm::vec4 cameraPos(camera->GetPosition(), 1);
    // Let's draw all the chunks
    int i = 0;
    auto fov = glm::radians(application->GetFoV());
    for (auto it = m_chunks.begin(); it != m_chunks.end(); ++it) {
        // Making real chunk position
        auto clipDist = application->GetClippingDistance();
        auto chunkPos = it->second->GetPosition();
        glm::vec4 pos[] = {
            glm::vec4(chunkPos.x, 0, chunkPos.z, 1),
            glm::vec4(chunkPos.x + Chunk::ms_chunkSize.x, 0, chunkPos.z + Chunk::ms_chunkSize.z, 1),
            glm::vec4(chunkPos.x, 0, chunkPos.z + Chunk::ms_chunkSize.z, 1),
            glm::vec4(chunkPos.x + Chunk::ms_chunkSize.x, 0, chunkPos.z, 1),
            glm::vec4(chunkPos.x, 255, chunkPos.z, 1),
            glm::vec4(chunkPos.x + Chunk::ms_chunkSize.x, 255, chunkPos.z + Chunk::ms_chunkSize.z, 1),
            glm::vec4(chunkPos.x, 255, chunkPos.z + Chunk::ms_chunkSize.z, 1),
            glm::vec4(chunkPos.x + Chunk::ms_chunkSize.x, 255, chunkPos.z, 1),
        };

        for (int j = 0; j < 8; ++j) {
            pos[j] = mvp * pos[j];
        }

        bool cont = true;
        for (int j = 0; j < 8; ++j) {
            if (pos[j].z >= -pos[j].w)
                cont = false;
        }
        if (cont) 
            continue;

        cont = true;
        for (int j = 0; j < 8; ++j) {
            if (pos[j].z <= pos[j].w)

        cont = true;
        for (int j = 0; j < 8; ++j) {
            cpVec[j] = glm::normalize(cpVec[j]);
            angles[j] = glm::acos(glm::dot(cpVec[j], dir));
        }
        if (cont) 
            continue;
        cont = true;
        for (int j = 0; j < 8; ++j) {
            if (pos[j].x <= pos[j].w)
                cont = false;
        }
        if (cont)
            continue;

        cont = true;
        for (int j = 0; j < 8; ++j) {
            if (pos[j].y >= -pos[j].w)
                cont = false;
        }
        if (cont)
            continue;

        cont = true;
        for (int j = 0; j < 8; ++j) {
            if (pos[j].y <= pos[j].w)
                cont = false;
        }
        if (cont)
            continue;
        GLubyte cflag = VSBL_FCS_ALL;
        if (chunkPos.x + Chunk::ms_chunkSize.x < cameraPos.x)
            cflag ^= VSBL_FCS_LEFT;
        if (chunkPos.x > cameraPos.x)
            cflag ^= VSBL_FCS_RIGHT;
        if (chunkPos.z + Chunk::ms_chunkSize.z < cameraPos.z)
            cflag ^= VSBL_FCS_BACK;
        if (chunkPos.z > cameraPos.z)
            cflag ^= VSBL_FCS_FRONT;
        it->second->SetFacesToDraw(flag & cflag);

        ++i;
        m_program->SetWorldPos(chunkPos);
        it->second->Draw();
    }
    if (time == 100) {
        std::cout << i <<std::endl;
        time = 0;
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
                force.y = 0;
            } else
                force.y = -m_gravityAcceleration;
        } else
            force.y = -m_gravityAcceleration;
    } else
        force.y = -m_gravityAcceleration;

    float len = glm::length(vel);
    if (vel != glm::ZERO<glm::vec3>()) {
        glm::vec3 normForce(glm::normalize(vel));
        force -= normForce * len * len * 0.5f * 0.01f * 1.2f;
        if (!vel.y) {
            force.x -= glm::min(normForce.x * 5.0f, vel.x);
            force.z -= glm::min(normForce.z * 5.0f, vel.z);
        }
    }

    m_player->setVelocity(vel);
    m_player->setPosition(newPos2);
    m_player->setForce(force);
}

void MineWorld::BuildingChunksThreadFunction(std::weak_ptr<MineWorld> _this) {
    for (;;) {
        if (auto p = _this.lock()) {
            while (p->m_chunksBuildingQueue.Size() != 0) {
                auto obj = p->m_chunksBuildingQueue.Remove();
                p->GenerateWorld(obj);
                p->m_chunkUpdateQueue.Add(obj.second);
            }
        }else
            break;
        std::this_thread::sleep_for(std::chrono::microseconds(1));
    }
}

void MineWorld::UpdatingChunksThreadFunction(std::weak_ptr<MineWorld> _this) {
    for (;;) {
        if (auto p = _this.lock()) {
            while (p->m_chunkUpdateQueue.Size() != 0) {
                std::shared_ptr<Chunk> obj = p->m_chunkUpdateQueue.Remove();
                obj->UpdateVertexes();
                p->m_chunkTransferQueue.Add(obj);
            }
        }else
            break;
        std::this_thread::sleep_for(std::chrono::microseconds(1));
    }
}

}