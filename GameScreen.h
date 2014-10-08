/*
 * GameScreen.h
 *
 *  Created on: Oct 8, 2014
 *      Author: p.rogala
 */

#ifndef GAMESCREEN_H_
#define GAMESCREEN_H_

namespace XKS {

class GameScreen : public Screen {
 public:
    virtual void Load();
    virtual void Unload();
    virtual void Update(double dt);
    virtual void Draw();
};
}



#endif /* GAMESCREEN_H_ */
