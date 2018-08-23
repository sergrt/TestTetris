#pragma once
#include "GameParameters.h"
#include <array>
#include "Point.h"
#include <memory>

namespace cocos2d {
    class Sprite;
}

extern cocos2d::Sprite* spritePlaceholder; // Something that will be sprite but not yet assigned.
using namespace Tetris::Figure;

//////////////////////////////////////////////////////////////////////////
// Figure class represents figure on the field
class Figure {
public:
    enum class Color {
        blue,
        yellow,
        red,
        green
    };
    ///////////////////////////////////
    // PatternIterator iterates over figure's pattern
    // to allow handy sprites manipulations
    class PatternIterator {
    public:
        PatternIterator(std::array<std::array<cocos2d::Sprite*, patternSize>, patternSize>& figure);
        PatternIterator& operator++();
        cocos2d::Sprite*& operator*() const;
        bool operator!=(const PatternIterator& other) const;
        void setPos(int p);
        int x() const;
        int y() const;
    private:
        int pos_ = 0;
        std::array<std::array<cocos2d::Sprite*, patternSize>, patternSize>& figure_;
    };
private:
    enum class Type {
        I, // OOOO

        J, // O
           // OOO

        L, //   O
           // OOO

        O, // OO
           // OO

        S, //  OO
           // OO

        T, //  O
           // OOO

        Z  // OO
           //  OO
    };
    
    enum class Angle {
        angle0,
        angle90,
        angle180,
        angle245
    };

private:
    Type type_;
    FieldPos pos_; // pos is the position of left-bottom square of pattern in field coordinates
    Angle angle_;
    Color color_;

    bool isMovingLR_ = false;
    bool isMovingDown_ = false;

    std::array<std::array<cocos2d::Sprite*, patternSize>, patternSize> figure_;

    static Color randomColor();
    static Type randomType();

    void rotateMatrix();
public:
    Figure(Type t, Color c, FieldPos p);
    static std::unique_ptr<Figure> createRandom(FieldPos p);

    Color color() const;
    void rotate();
    void moveLeft();
    void moveRight();
    void moveDown();

    bool isMovingLR() const;
    bool isMovingDown() const;

    void setMovingLR(bool moving);
    void setMovingDown(bool moving);
    
    FieldPos pos() const;

    PatternIterator begin();
    PatternIterator end();
    
    std::array<cocos2d::Sprite*, patternSize>& operator[](int row);
};
