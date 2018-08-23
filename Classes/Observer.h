#pragma once
#include "Point.h"
namespace cocos2d {
    class Sprite;
}

//class FieldPos;

//////////////////////////////////////////////////////////////////////////
// Observer class for OBSERVER pattern to use with Subject
class Observer {
public:
    virtual void addedGround(const FieldPos& pos) = 0;
    virtual void addedWall(const FieldPos& pos) = 0;
    virtual void addedFigure() = 0;

    virtual void gameOver() = 0;
    virtual void spriteRemoved(cocos2d::Sprite* sprite) = 0;
    virtual void rotatedFigure() = 0;
};
