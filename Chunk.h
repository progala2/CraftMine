#ifndef _XKS_CHUNK_H_
#define _XKS_CHUNK_H_
#include "stdafx.h"
#include "VertexPositionColor.h"
#include "Utility.h"
#include "CubeShader.h"
#include <bitset>

enum DIRECTIONS {
    DIR_FRONT = 0,
    DIR_BACK,
    DIR_UP,
    DIR_DOWN,
    DIR_LEFT,
    DIR_RIGHT = 5,
    DIR_LAST = DIR_RIGHT
};

enum VISIBLE_FACES {
    VSBL_FCS_NOTHING = 0,
    VSBL_FCS_FRONT = 1,
    VSBL_FCS_BACK = 2,
    VSBL_FCS_UP = 4,
    VSBL_FCS_DOWN = 8,
    VSBL_FCS_LEFT = 16,
    VSBL_FCS_RIGHT = 32,
    VSBL_FCS_ALL = 63
};

class Chunk {
 public:
    Chunk(std::shared_ptr<CubeShader> shader);
    ~Chunk();

    //glm::vec3 GetPosition() const {return m_position;}

    void MoveToGraphic();
    void UpdateVertexes();
    void LoadChunkData(ChunkData& data);

    void SetNeighbour(DIRECTIONS flag, std::shared_ptr<Chunk> obj);
    void Set(GLubyte x, GLubyte y, GLubyte z, BlockTypeNumber tex);
    void Delete(GLubyte x, GLubyte y, GLubyte z);
    void ChunkModified() {
        m_modified = true;
    }
    void SetFacesToDraw(GLubyte flag) {
        m_facesToDraw = flag;
    }

    bool isModified() const {
        return m_modified;
    }
    bool isSolid(GLubyte x, GLubyte y, GLubyte z) const;

    void Draw();

    void AddFace(GLubyte x, GLubyte y, GLubyte z, GLubyte flag, BlockTypeNumber tex[6]);

    static const glm::ivec3 ms_chunkSize;

 private:
    //glm::vec3 m_position;
    std::vector<VertPosTexCompressed> m_vertex[DIR_LAST + 1];
    int m_lastSize[DIR_LAST + 1];

    GLuint m_vertexVBO[DIR_LAST + 1];
    GLuint m_vertexVAO[DIR_LAST + 1];
    std::shared_ptr<Chunk> m_Neighbours[DIR_LAST + 1];

    ChunkData m_chunkData;
    GLubyte m_facesToDraw;
    static const glm::vec3 m_Normals[DIR_LAST + 1];
    std::shared_ptr<CubeShader> m_program;
    bool m_canDrawing;
    bool m_modified;
};

#endif

