#ifndef _XKS_OPENGL_APP_BASE_
#define _XKS_OPENGL_APP_BASE_

#include "stdafx.h"
#include "Screen.h"

namespace XKS {

class ApplicationWindow {
 public:
    ApplicationWindow();

    void Run(std::unique_ptr<Screen> startScreen);

    virtual ~ApplicationWindow();

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

    void StartNewScreen(std::unique_ptr<Screen> newScreen) {
        m_screen->Unload();
        m_screen.reset(newScreen.release());
        m_screen->Load();
    }

    void CloseApplication() {
        glfwSetWindowShouldClose(m_window, GL_TRUE);
        m_screen->Unload();
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

 private:
    void Initialize();
    void MainLoop();
    void OnResize(int witdth, int height);

    std::unique_ptr<Screen> m_screen;

    static void WindowResizeCallback(GLFWwindow* window, int width, int height);
    static void WindowCloseCallback(GLFWwindow* window);
    static void WindowFocusCallback(GLFWwindow* window, int focus);
    static void WindowKeyCallback(GLFWwindow* window, int key, int scancode, int action, int mods);
    static void WindowMouseButtonCallback(GLFWwindow* window, int button, int action, int mods);

    static std::unique_ptr<ApplicationWindow> m_instance;

    GLFWwindow* m_window;
};

}
#endif
