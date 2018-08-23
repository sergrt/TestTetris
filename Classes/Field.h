#pragma once
#include "Subject.h"
#include "GameParameters.h"
#include <array>
#include "Figure.h"
#include <map>

namespace cocos2d {
    class Sprite;
}

class Field : public Subject {
private:
    std::vector<std::array<cocos2d::Sprite*, Tetris::Field::width>> field_;
public:
    Field();
    bool canMoveDown();
    bool canMoveLeft();
    bool canMoveRight();

    void moveFigureLeft();
    void moveFigureRight();
    void moveFigureDown();

    bool isMovingLR() const;
    bool isMovingDown() const;
    void setMovingLR(bool f);
    void setMovingDown(bool f);

    void settleFigure();
    void initialize();
    void cleanup();
    void spawnNewFigure();
    std::map<cocos2d::Sprite*, int> removeCompleteRows();
    void setWall(const FieldPos& pos, cocos2d::Sprite* sprite);
    void rotate();

    Figure::PatternIterator currentFigureBegin() const;
    Figure::PatternIterator currentFigureEnd() const;

    FieldPos currentFigurePos() const;
    Figure::Color currentFigureColor() const;

private:
    bool checkFieldVsFigure(int newX, int newY);
    //Figure::Type nextFigureType;
    // add next figure orientation
    std::unique_ptr<Figure> figure_;
};
