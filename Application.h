#ifndef _XKS_OPENGL_APP_BASE_
#define _XKS_OPENGL_APP_BASE_

#include "stdafx.h"
#include "Screen.h"

namespace XKS {

class ApplicationWindow {
 public:
	static std::shared_ptr<ApplicationWindow> GetInstance() {
        if (ms_instance == nullptr)
            ms_instance.reset(new ApplicationWindow());
		return ms_instance;
	}

    void Run(std::unique_ptr<Screen> startScreen);

    ~ApplicationWindow();

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
        if (m_screen != nullptr)
            m_screen->Unload();
        m_screen.reset(newScreen.release());
        m_screen->Load();
    }

    void CloseApplication() {
        glfwSetWindowShouldClose(m_window, GL_TRUE);
        m_screen->Unload();
    }

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

    bool IsFocused() const {
        return m_isFocused;
    }
	
 private:
    
	ApplicationWindow();
    void Initialize(std::unique_ptr<Screen> startScreen);
    void MainLoop();
    void OnResize(int witdth, int height);
    void Draw();
    void Update(double dt);
    void Focus(int);
    void Destroy();
	void KeyAction(int key, int scancode, int action, int mods);
    void MouseAction(int button, int action, int mods);

	int m_windowWidth;
    int m_windowHeight;
	float m_mouseSpeed;
    float m_clippingDistance, m_aspect, m_FoV;
    bool m_isFocused;

    std::unique_ptr<Screen> m_screen;
	
    static void WindowResizeCallback(GLFWwindow* window, int width, int height);
    static void WindowCloseCallback(GLFWwindow* window);
    static void WindowFocusCallback(GLFWwindow* window, int focus);
    static void WindowKeyCallback(GLFWwindow* window, int key, int scancode, int action, int mods);
    static void WindowMouseButtonCallback(GLFWwindow* window, int button, int action, int mods);

    static std::shared_ptr<ApplicationWindow> ms_instance;

    GLFWwindow* m_window;
};

}
#endif
