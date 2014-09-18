#ifndef _XKS_APPLICATION_H_
#define _XKS_APPLICATION_H_

#include "OpenGLAppBase.h"
#include "CubeShader.h"
#include "World.h"
#include "Camera.h"
#include "ResourceManager.h"

class World;
class Application : public OpenGLAppBase
{
public:
	
	static Application* getInstance();

	virtual void Resize(int width, int height);
	virtual void Draw();
	virtual void Load();
	virtual void Unload();
	//Dodana metoda Update
	virtual void Update(double dt);
	virtual void Focus(int foc) { m_isFocused = foc;}

	virtual void KeyAction(int key, int scancode, int action, int mods);
	virtual void MouseAction(int button, int action, int mods);

	float GetMouseSpeed() const {return m_mouseSpeed;}
	float GetClippingDistance() const {return m_clippingDistance;}
	float GetAspect() const {return m_aspect;}
	float GetFoV() const {return m_FoV;}

	~Application();

private:

	Application();
	float m_mouseSpeed;
	float m_clippingDistance, m_aspect, m_FoV;

	World* m_world;

	static Application* m_instance;

	GLboolean m_isFocused;
};

#endif