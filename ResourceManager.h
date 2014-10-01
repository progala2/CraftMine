#ifndef _XKS_RESOURCEMANAGER_H_
#define _XKS_RESOURCEMANAGER_H_

#include "stdafx.h"

namespace XKS {

enum BLOCKS {
    BLCK_FIRST = 0,
    BLCK_DIRT = BLCK_FIRST,
    BLCK_COBBLESTONE,
    BLCK_STONE,
    BLCK_SAND,
    BLCK_LAST = BLCK_SAND
};

class ResourceManager {
 public:

    static std::shared_ptr<ResourceManager> GetInstance();
    ~ResourceManager();

    void Load();
    void Unload();
    GLuint getTextureArrID() {
        return m_texArrBlockID;
    }

 private:

    ResourceManager();

    GLuint m_texArrBlockID;

    GLuint loadTexture(const std::string& imagepath);
    GLuint loadTextureArray(const std::vector<std::string>& imagepath);

    std::shared_ptr<ResourceManager> m_instance;
};

}
#endif
