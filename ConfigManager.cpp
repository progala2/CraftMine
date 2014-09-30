#include "stdafx.h"
#include "ConfigManager.h"

ConfigManager* ConfigManager::ms_instance = nullptr;

ConfigManager* ConfigManager::getInstance() {
    if (ms_instance == nullptr)
        ms_instance = new ConfigManager();

    return ms_instance;
}

ConfigManager::ConfigManager()
        : m_genMipMap(false),
          m_clippingDistance(200),
          m_compressedTex(1),
          m_FoV(45),
          m_mouseSpeed(100) {
    m_texParameters.TextureMinFilter = GL_NEAREST;
    m_texParameters.TextureMagFilter = GL_NEAREST;
    m_texParameters.TextureWrapS = GL_CLAMP_TO_EDGE;
    m_texParameters.TextureWrapT = GL_CLAMP_TO_EDGE;
}
