#ifndef _XKS_OPENGL_APP_BASE_
#define _XKS_OPENGL_APP_BASE_

#include "stdafx.h"

class OpenGLAppBase {
 public:
    OpenGLAppBase();
    void Run();
    virtual ~OpenGLAppBase();

    int GetKey(int key) {
        return glfwGetKey(m_window, key);
    }
    void GetCursorPos(double& x, double& y) {
        glfwGetCursorPos(m_window, &x, &y);
    }
    void SetCursorPos(double x, double y) {
        glfwSetCursorPos(m_window, x, y);
    }
    void GetWindowWidthHeight(int& w, int& h) const {
        w = m_windowWidth;
        h = m_windowHeight;
    }

 protected:
    virtual void Load() {
    }
    virtual void Unload() {
    }
    virtual void Resize(int width, int height) {
    }
    virtual void Draw();
    virtual void Update(double dt) {
    }
    virtual void Focus(int) {
    }

    virtual void KeyAction(int key, int scancode, int action, int mods) {
    }
    virtual void MouseAction(int button, int action, int mods) {
    }

    int m_windowWidth;
    int m_windowHeight;
    GLFWwindow* m_window;

 private:
    void Initialize();
    void Destroy(bool unload = true);
    void MainLoop();
    void OnResize(int witdth, int height);

    static void WindowResizeCallback(GLFWwindow* window, int width, int height);
    static void WindowCloseCallback(GLFWwindow* window);
    static void WindowFocusCallback(GLFWwindow* window, int focus);
    static void WindowKeyCallback(GLFWwindow* window, int key, int scancode, int action, int mods);
    static void WindowMouseButtonCallback(GLFWwindow* window, int button, int action, int mods);

    static OpenGLAppBase *m_instance;
};

#endif
