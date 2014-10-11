#ifndef _XKS_CONFIGMANAGER_H_
#define _XKS_CONFIGMANAGER_H_
#include "stdafx.h"

namespace XKS {

struct TextureParameters {
    GLint TextureMinFilter, TextureMagFilter, TextureWrapS/*GL_CLAMP_TO_EDGE*/, TextureWrapT;
};

class ConfigManager {
 public:

    static std::shared_ptr<ConfigManager> GetInstance();

    GLubyte GetTextureComprType() {
        return m_compressedTex;
    }
    GLboolean IsGeneratingMipMap() {
        return m_genMipMap;
    }
    const TextureParameters& GetTextureParameters() {
        return m_texParameters;
    }

 private:

    ConfigManager();

    GLubyte m_compressedTex;
    GLboolean m_genMipMap;
    TextureParameters m_texParameters;

    GLfloat m_mouseSpeed;
    GLfloat m_clippingDistance, m_FoV;

    static std::shared_ptr<ConfigManager> ms_instance;
};

}

#endif
