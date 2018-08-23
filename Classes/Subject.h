#pragma once
#include "Observer.h"
#include <vector>

namespace cocos2d {
    class Sprite;
}

//////////////////////////////////////////////////////////////////////////
// Subject class it the part of the OBSERVER pattern
class Subject {
public:
    void registerObserver(Observer& observer);
    void removeObserver(Observer& observer);

    void notifyGroundAdded(const FieldPos& pos);
    void notifyWallAdded(const FieldPos& pos);
    void notifyFigureAdded();

    void notifyGameOver();
    void notifySpriteRemoved(cocos2d::Sprite* sprite);
    void notifyFigureRotated();

private:
    std::vector<Observer*> observers_;
};
