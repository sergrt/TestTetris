#include "Subject.h"
#include <algorithm>

void Subject::registerObserver(Observer& observer) {
    observers_.push_back(&observer);
}
void Subject::removeObserver(Observer& observer) {
    observers_.erase(std::remove(std::begin(observers_), std::end(observers_), &observer), std::end(observers_));
}

void Subject::notifyGroundAdded(const FieldPos& pos) {
    for (const auto p : observers_)
        p->addedGround(pos);
}
void Subject::notifyWallAdded(const FieldPos& pos) {
    for (const auto p : observers_)
        p->addedWall(pos);
}
void Subject::notifyFigureAdded() {
    for (const auto p : observers_)
        p->addedFigure();
}

void Subject::notifyGameOver() {
    for (const auto p : observers_)
        p->gameOver();
}
void Subject::notifySpriteRemoved(cocos2d::Sprite* sprite) {
    for (const auto p : observers_)
        p->spriteRemoved(sprite);
}
void Subject::notifyFigureRotated() {
    for (const auto p : observers_)
        p->rotatedFigure();
}
