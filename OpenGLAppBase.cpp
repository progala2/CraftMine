#include "stdafx.h"
#include "OpenGLAppBase.h"

using namespace std;

OpenGLAppBase *OpenGLAppBase::m_instance = NULL;

void OpenGLAppBase::WindowResizeCallback(GLFWwindow* window, int width, int height)
{
	if (m_instance)
		m_instance->OnResize(width, height);
} 

void OpenGLAppBase::WindowKeyCallback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
	if (m_instance)
	{
		m_instance->KeyAction(key, scancode, action, mods);
	}
}

void OpenGLAppBase::WindowCloseCallback(GLFWwindow* window)
{
	if (m_instance)
		m_instance->Destroy();
}

void OpenGLAppBase::WindowFocusCallback(GLFWwindow* window, int focus)
{
	if (m_instance)
	{
		m_instance->Focus(focus);
	}
}

void OpenGLAppBase::WindowMouseButtonCallback(GLFWwindow* window, int button, int action, int mods)
{
	if (m_instance)
	{
		m_instance->MouseAction(button, action, mods);
	}
}

void OpenGLAppBase::Initialize()
{
	if (!glfwInit())
		throw exception("Blad glfwInit!");
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 2);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
	m_window = glfwCreateWindow(m_windowWidth, m_windowHeight, "OpenGL Tutorial", NULL, NULL);
	if (m_window == nullptr)
		throw exception("Blad glfwOpenWindow!");
	glfwMakeContextCurrent(m_window);
	glfwSetWindowSizeCallback(m_window, OpenGLAppBase::WindowResizeCallback);
	glfwSetWindowCloseCallback(m_window, OpenGLAppBase::WindowCloseCallback);
	glfwSetWindowFocusCallback(m_window, OpenGLAppBase::WindowFocusCallback); 
	glfwSetKeyCallback(m_window, OpenGLAppBase::WindowKeyCallback);
	glfwSetMouseButtonCallback(m_window, OpenGLAppBase::WindowMouseButtonCallback);
	glewExperimental = GL_TRUE;
	if (glewInit() != GLEW_OK)
		throw exception("Blad glewInit!");
	glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
	Load();
}

void OpenGLAppBase::Draw()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glfwSwapBuffers(m_window);
}

void OpenGLAppBase::MainLoop()
{
	 double lastTime = glfwGetTime();
	int nbFrames = 0;
	double time = glfwGetTime();
	do
	{
		Draw();
		double newTime = glfwGetTime();
		Update(newTime - time);
		time = newTime;
		// Measure speed
		
		glfwPollEvents();
		double currentTime = glfwGetTime();
		nbFrames++;
		if ( currentTime - lastTime >= 1.0 ){ // If last prinf() was more than 1 sec ago
			
			// printf and reset timer
			char str[30];
			sprintf_s(str, 30, "%.1f frames/sec\n", nbFrames/(currentTime - lastTime));
			glfwSetWindowTitle(m_window, str);
			lastTime = currentTime;
			nbFrames = 0;
		}
	} while (!glfwWindowShouldClose(m_window));
}

void OpenGLAppBase::Run()
{
	try
	{
		Initialize();
		MainLoop();
	}
	catch (exception e)
	{
		cerr << e.what() << endl;
		system("pause");
	}
}

void OpenGLAppBase::Destroy(bool unload /* = true */)
{
	if (unload)
		Unload();
}

OpenGLAppBase::OpenGLAppBase()
	: m_windowWidth(800), m_windowHeight(600)
{
	if (m_instance)
		throw exception("Nie mozna stworzyc kolejnej instancji klasy!");
	m_instance = this;
}

OpenGLAppBase::~OpenGLAppBase()
{
	m_instance = NULL;
	Destroy(false);
	glfwDestroyWindow(m_window);
	glfwTerminate();
}

void OpenGLAppBase::OnResize(int width, int height)
{
	m_windowWidth = width;
	m_windowHeight = height;
	glViewport(0, 0, m_windowWidth, m_windowHeight);
	Resize(width, height);
}