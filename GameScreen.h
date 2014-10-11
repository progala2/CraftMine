/*
 * GameScreen.h
 *
 *  Created on: Oct 8, 2014
 *      Author: p.rogala
 */

#ifndef XKS_GAMESCREEN_H_
#define XKS_GAMESCREEN_H_
#include "stdafx.h"
#include "Screen.h"
#include "World.h"

namespace XKS {

class GameScreen : public Screen {
 public:
    GameScreen();
    virtual ~GameScreen();

    virtual void Load();
    virtual void Unload();
    virtual void Update(double dt);
    virtual void Draw();
    virtual void Resize(int width, int height);
    virtual void KeyAction(int key, int scancode, int action, int mods);
    virtual void MouseAction(int button, int action, int mods);
 private:
	 std::shared_ptr<World> m_world;
};
}



#endif /* GAMESCREEN_H_ */
