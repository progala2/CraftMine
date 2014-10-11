#include "stdafx.h"
#include "ResourceManager.h"
#include "ConfigManager.h"

namespace XKS {

std::shared_ptr<ResourceManager> ResourceManager::ms_instance = nullptr;

std::shared_ptr<ResourceManager> ResourceManager::GetInstance() {
    if (ms_instance == nullptr)
        ms_instance.reset(new ResourceManager());
    return ms_instance;
}

ResourceManager::ResourceManager()
        : m_texArrBlockID(-1) {
    std::vector<std::string> tym;

    tym.push_back("Data\\dirt.png");
    tym.push_back("Data\\cobblestone.png");
    tym.push_back("Data\\stone.png");
    tym.push_back("Data\\sand.png");
    m_texArrBlockID = ResourceManager::loadTextureArray(tym);

    tym.clear();
}

ResourceManager::~ResourceManager() {
    glDeleteTextures(1, &m_texArrBlockID);
    m_texArrBlockID = -1;
    ms_instance = nullptr;
}

GLuint ResourceManager::loadTexture(const std::string& imagepath) {
    // Create one OpenGL texture
    glHint(GL_TEXTURE_COMPRESSION_HINT, GL_FASTEST);
    GLuint textureID;
    glGenTextures(1, &textureID);

    // "Bind" the newly created texture : all future texture functions will modify this texture
    glBindTexture(GL_TEXTURE_2D, textureID);

    int width, height;
    unsigned char* image = SOIL_load_image(imagepath.c_str(), &width, &height, 0, SOIL_LOAD_RGBA);
    glTexImage2D( GL_TEXTURE_2D, 0, GL_COMPRESSED_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, image);
    SOIL_free_image_data(image);

    glGenerateMipmap(GL_TEXTURE_2D);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);

    glBindTexture(GL_TEXTURE_2D, 0);

    return textureID;
}

GLuint ResourceManager::loadTextureArray(const std::vector<std::string>& imagepath) {
    GLuint textureID;
    glGenTextures(1, &textureID);

    glBindTexture(GL_TEXTURE_2D_ARRAY, textureID);

    int width, height, count = imagepath.size();
    unsigned char** image = new unsigned char*[count];
    for (int i = 0; i < count; ++i) {
        image[i] = SOIL_load_image(imagepath[i].c_str(), &width, &height, 0, SOIL_LOAD_RGBA);
    }

    long long* temp = new long long[width * (width >> 2) * count];

    auto cnfMngr = ConfigManager::GetInstance();

    if (cnfMngr->GetTextureComprType() > 0) {
        glHint(GL_TEXTURE_COMPRESSION_HINT, GL_NICEST);
        if (cnfMngr->GetTextureComprType() >= 2)
            glHint(GL_TEXTURE_COMPRESSION_HINT, GL_FASTEST);
        glTexImage3D( GL_TEXTURE_2D_ARRAY, 0, GL_COMPRESSED_RGBA, width, height, count, 0, GL_RGBA, GL_UNSIGNED_BYTE,
                     temp);
    } else
        glTexImage3D( GL_TEXTURE_2D_ARRAY, 0, GL_RGBA, width, height, count, 0, GL_RGBA, GL_UNSIGNED_BYTE, temp);

    delete[] temp;
    for (int i = 0; i < count; i++) {
        glTexSubImage3D(GL_TEXTURE_2D_ARRAY, 0, 0, 0, i, width, height, 1, GL_RGBA, GL_UNSIGNED_BYTE, image[i]);
    }

    for (int i = 0; i < count; i++) {
        SOIL_free_image_data(image[i]);
    }
    delete[] image;

    if (cnfMngr->IsGeneratingMipMap()) {
        glHint(GL_GENERATE_MIPMAP_HINT, GL_NICEST);
        glGenerateMipmap(GL_TEXTURE_2D_ARRAY);
    }

    const TextureParameters txtPar = cnfMngr->GetTextureParameters();

    glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_MIN_FILTER, txtPar.TextureMinFilter);
    glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_MAG_FILTER, txtPar.TextureMagFilter);
    glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_WRAP_S, txtPar.TextureWrapS);  //GL_CLAMP_TO_EDGE
    glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_WRAP_T, txtPar.TextureWrapT);

    glBindTexture(GL_TEXTURE_2D_ARRAY, 0);

    return textureID;
}

}
