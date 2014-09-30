#ifndef _XKS_CONFIGMANAGER_H_
#define _XKS_CONFIGMANAGER_H_
#include "stdafx.h"

struct TextureParameters {
    GLint TextureMinFilter, TextureMagFilter, TextureWrapS/*GL_CLAMP_TO_EDGE*/, TextureWrapT;
};

class ConfigManager {
 public:

    static ConfigManager* getInstance();

    GLubyte getTextureComprType() {
        return m_compressedTex;
    }
    GLboolean isGeneratingMipMap() {
        return m_genMipMap;
    }
    const TextureParameters& getTextureParameters() {
        return m_texParameters;
    }

 private:
    ConfigManager();

    GLubyte m_compressedTex;
    GLboolean m_genMipMap;
    TextureParameters m_texParameters;

    float m_mouseSpeed;
    float m_clippingDistance, m_FoV;

    static ConfigManager* ms_instance;
};

#endif
