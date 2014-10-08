/*
 * Screen.h
 *
 *  Created on: Oct 2, 2014
 *      Author: p.rogala
 */

#ifndef SCREEN_H_
#define SCREEN_H_

namespace XKS {

class Screen {
 public:
    virtual void Load() = 0;
    virtual void Unload() = 0;
    virtual void Update(double dt) { }
    virtual void Draw() { }
    virtual void Resize(int width, int height) { }
    virtual void Focus(int) { }
    virtual void KeyAction(int key, int scancode, int action, int mods) { }
    virtual void MouseAction(int button, int action, int mods) { }
};
}

#endif /* SCREEN_H_ */
