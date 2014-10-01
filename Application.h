#ifndef _XKS_APPLICATION_H_
#define _XKS_APPLICATION_H_

#include "OpenGLAppBase.h"
#include "CubeShader.h"
#include "World.h"
#include "Camera.h"
#include "ResourceManager.h"

namespace XKS {

class World;
class Application : public OpenGLAppBase {
 public:

    static std::shared_ptr<Application> getInstance();

    void Resize(int width, int height);
    void Draw();
    void Load();
    void Unload();

    void Update(double dt);
    void Focus(int foc) {
        m_isFocused = foc;
    }

    virtual void KeyAction(int key, int scancode, int action, int mods);
    virtual void MouseAction(int button, int action, int mods);

    float GetMouseSpeed() const {
        return m_mouseSpeed;
    }
    float GetClippingDistance() const {
        return m_clippingDistance;
    }
    float GetAspect() const {
        return m_aspect;
    }
    float GetFoV() const {
        return m_FoV;
    }

    ~Application();

 private:

    Application();
    float m_mouseSpeed;
    float m_clippingDistance, m_aspect, m_FoV;

    std::shared_ptr<World> m_world;
    std::shared_ptr<ResourceManager> m_resourceManager;

    static std::shared_ptr<Application> m_instance;

    GLboolean m_isFocused;
};

}

#endif
