#include "Figure.h"
#include "cocos2d.h"
#include "Utilities.h"

USING_NS_CC;
Sprite* spritePlaceholder = reinterpret_cast<Sprite*>(1);

Figure::PatternIterator::PatternIterator(std::array<std::array<cocos2d::Sprite*, patternSize>, patternSize>& figure)
    : figure_{ figure } {
}
Figure::PatternIterator& Figure::PatternIterator::operator++() {
    ++pos_;
    return *this;
}
Sprite*& Figure::PatternIterator::operator*() const {
    if (pos_ < patternSize * patternSize)
        return figure_[pos_ / patternSize][pos_ % patternSize];

    throw std::runtime_error("Something bad happened in PatternIterator");
}
bool Figure::PatternIterator::operator!=(const PatternIterator& other) const {
    return pos_ != other.pos_;
}
void Figure::PatternIterator::setPos(int p) {
    pos_ = p;
}
int Figure::PatternIterator::x() const {
    return pos_ % patternSize;
}
int Figure::PatternIterator::y() const {
    return pos_ / patternSize;
}

Figure::Figure(Type t, Color c, FieldPos p)
    : type_{ t }, color_{ c }, pos_{ p }, angle_{ Angle::angle0 } {

    if (type_ == Type::L) {
        figure_[3] = {                 0,                 0, 0, 0 };
        figure_[2] = { spritePlaceholder,                 0, 0, 0 };
        figure_[1] = { spritePlaceholder,                 0, 0, 0 };
        figure_[0] = { spritePlaceholder, spritePlaceholder, 0, 0 };
    } else if (type_ == Type::J) {
        figure_[3] = {                 0,                 0, 0, 0 };
        figure_[2] = {                 0, spritePlaceholder, 0, 0 };
        figure_[1] = {                 0, spritePlaceholder, 0, 0 };
        figure_[0] = { spritePlaceholder, spritePlaceholder, 0, 0 };
    } else if (type_ == Type::O) {
        figure_[3] = {                 0,                 0, 0, 0 };
        figure_[2] = {                 0,                 0, 0, 0 };
        figure_[1] = { spritePlaceholder, spritePlaceholder, 0, 0 };
        figure_[0] = { spritePlaceholder, spritePlaceholder, 0, 0 };
    } else if (type_ == Type::S) {
        figure_[3] = {                 0,                 0,                 0, 0 };
        figure_[2] = {                 0,                 0,                 0, 0 };
        figure_[1] = {                 0, spritePlaceholder, spritePlaceholder, 0 };
        figure_[0] = { spritePlaceholder, spritePlaceholder,                 0, 0 };
    } else if (type_ == Type::T) {
        figure_[3] = {                 0,                 0,                 0, 0 };
        figure_[2] = {                 0,                 0,                 0, 0 };
        figure_[1] = {                 0, spritePlaceholder,                 0, 0 };
        figure_[0] = { spritePlaceholder, spritePlaceholder, spritePlaceholder, 0 };
    } else if (type_ == Type::Z) {
        figure_[3] = {                 0,                 0,                 0, 0 };
        figure_[2] = {                 0,                 0,                 0, 0 };
        figure_[1] = { spritePlaceholder, spritePlaceholder,                 0, 0 };
        figure_[0] = {                 0, spritePlaceholder, spritePlaceholder, 0 };
    } else if (type_ == Type::I) {
        figure_[3] = {                 0,                 0,                 0,                 0 };
        figure_[2] = {                 0,                 0,                 0,                 0 };
        figure_[1] = { spritePlaceholder, spritePlaceholder, spritePlaceholder, spritePlaceholder };
        figure_[0] = {                 0,                 0,                 0,                 0 };

        // decrease pos by 1
        //--pos_.y;
    }
}
std::unique_ptr<Figure> Figure::createRandom(FieldPos p) {
    std::unique_ptr<Figure> res = std::make_unique<Figure>(Figure::randomType(), Figure::randomColor(), p);
    const int rotateCount = Utilities::genIntValue(0, 3);
    for (int i = 0; i < rotateCount; ++i)
        res->rotate();

    return res;
}
Figure::Color Figure::randomColor() {
    const int intColor = Utilities::genIntValue(0, 3);
    if (intColor == 0)
        return Color::blue;
    if (intColor == 1)
        return Color::green;
    if (intColor == 2)
        return Color::red;
    if (intColor == 3)
        return Color::yellow;

    throw std::runtime_error("Unhandled color");
}
Figure::Type Figure::randomType() {
    const int intType = Utilities::genIntValue(0, 6);
    if (intType == 0)
        return Type::L;
    if (intType == 1)
        return Type::I;
    if (intType == 2)
        return Type::J;
    if (intType == 3)
        return Type::O;
    if (intType == 4)
        return Type::S;
    if (intType == 5)
        return Type::T;
    if (intType == 6)
        return Type::Z;

    throw std::runtime_error("Unhandled figure type");
}
void Figure::moveLeft() {
    if (pos_.x() > 0)
        pos_.setX(pos_.x() - 1);
}
void Figure::moveRight() {
    if (pos_.x() < Tetris::Field::width - 1)
        pos_.setX(pos_.x() + 1);
}
void Figure::moveDown() {
    if (pos_.y() > 0)
        pos_.setY(pos_.y() - 1);
}
void Figure::rotate() {
    if (angle_ == Angle::angle0)
        angle_ = Angle::angle90;
    else if (angle_ == Angle::angle90)
        angle_ = Angle::angle180;
    else if (angle_ == Angle::angle180)
        angle_ = Angle::angle245;
    else if (angle_ == Angle::angle245)
        angle_ = Angle::angle0;

    rotateMatrix();
}

void Figure::rotateMatrix() {
    // This function looks ugly, but it seems there is no better way
    // to rotate figures. If implement true matrix rotation, there should be
    // handling of special cases (for I figure, for example, or for L and J on some angles,
    // because figures behave better when rotated over different points),
    // and this handling makes function to look even worse
    std::array<std::array<Sprite*, patternSize>, patternSize> newFigure{ {
        { 0, 0, 0, 0 },
        { 0, 0, 0, 0 },
        { 0, 0, 0, 0 },
        { 0, 0, 0, 0 } } };

    if (type_ == Type::L) {
        if (angle_ == Angle::angle90) {
            newFigure[0][0] = figure_[0][1];
            newFigure[1][0] = figure_[0][0];
            newFigure[1][1] = figure_[1][0];
            newFigure[1][2] = figure_[2][0];
        } else if (angle_ == Angle::angle180) {
            newFigure[2][0] = figure_[0][0];
            newFigure[2][1] = figure_[1][0];
            newFigure[1][1] = figure_[1][1];
            newFigure[0][1] = figure_[1][2];
        } else if (angle_ == Angle::angle245) {
            newFigure[0][0] = figure_[0][1];
            newFigure[0][1] = figure_[1][1];
            newFigure[0][2] = figure_[2][1];
            newFigure[1][2] = figure_[2][0];
        } else if (angle_ == Angle::angle0) {
            newFigure[2][0] = figure_[0][0];
            newFigure[1][0] = figure_[0][1];
            newFigure[0][0] = figure_[0][2];
            newFigure[0][1] = figure_[1][2];
        }
    }

    if (type_ == Type::J) {
        if (angle_ == Angle::angle90) {
            newFigure[1][0] = figure_[0][0];
            newFigure[0][0] = figure_[0][1];
            newFigure[0][1] = figure_[1][1];
            newFigure[0][2] = figure_[2][1];
        } else if (angle_ == Angle::angle180) {
            newFigure[2][1] = figure_[1][0];
            newFigure[2][0] = figure_[0][0];
            newFigure[1][0] = figure_[0][1];
            newFigure[0][0] = figure_[0][2];
        } else if (angle_ == Angle::angle245) {
            newFigure[0][2] = figure_[2][1];
            newFigure[1][2] = figure_[2][0];
            newFigure[1][1] = figure_[1][0];
            newFigure[1][0] = figure_[0][0];
        } else if (angle_ == Angle::angle0) {
            newFigure[0][0] = figure_[0][2];
            newFigure[0][1] = figure_[1][2];
            newFigure[1][1] = figure_[1][1];
            newFigure[2][1] = figure_[1][0];
        }
    }

    if (type_ == Type::O) {
        newFigure = figure_;
        /*
        newFigure[0][0] = figure_[0][0];
        newFigure[0][1] = figure_[0][1];
        newFigure[1][0] = figure_[1][0];
        newFigure[1][1] = figure_[1][1];
        */
    }

    if (type_ == Type::S) {
        if (angle_ == Angle::angle90) {
            newFigure[2][0] = figure_[0][0];
            newFigure[1][0] = figure_[0][1];
            newFigure[1][1] = figure_[1][1];
            newFigure[0][1] = figure_[1][2];
        } else if (angle_ == Angle::angle180) {
            newFigure[0][0] = figure_[0][1];
            newFigure[0][1] = figure_[1][1];
            newFigure[1][1] = figure_[1][0];
            newFigure[1][2] = figure_[2][0];
        } else if (angle_ == Angle::angle245) {
            newFigure[2][0] = figure_[0][0];
            newFigure[1][0] = figure_[0][1];
            newFigure[1][1] = figure_[1][1];
            newFigure[0][1] = figure_[1][2];
        } else if (angle_ == Angle::angle0) {
            newFigure[0][0] = figure_[0][1];
            newFigure[0][1] = figure_[1][1];
            newFigure[1][1] = figure_[1][0];
            newFigure[1][2] = figure_[2][0];
        }
    }

    if (type_ == Type::T) {
        if (angle_ == Angle::angle90) {
            newFigure[2][0] = figure_[0][0];
            newFigure[1][0] = figure_[0][1];
            newFigure[0][0] = figure_[0][2];
            newFigure[1][1] = figure_[1][1];
        } else if (angle_ == Angle::angle180) {
            newFigure[2][2] = figure_[2][0];
            newFigure[2][1] = figure_[1][0];
            newFigure[2][0] = figure_[0][0];
            newFigure[1][1] = figure_[1][1];
        } else if (angle_ == Angle::angle245) {
            newFigure[0][2] = figure_[2][2];
            newFigure[1][2] = figure_[2][1];
            newFigure[2][2] = figure_[2][0];
            newFigure[1][1] = figure_[1][1];
        } else if (angle_ == Angle::angle0) {
            newFigure[0][0] = figure_[0][2];
            newFigure[0][1] = figure_[1][2];
            newFigure[0][2] = figure_[2][2];
            newFigure[1][1] = figure_[1][1];
        }
    }

    if (type_ == Type::Z) {
        if (angle_ == Angle::angle90) {
            newFigure[2][1] = figure_[1][0];
            newFigure[1][1] = figure_[1][1];
            newFigure[1][0] = figure_[0][1];
            newFigure[0][0] = figure_[0][2];
        } else if (angle_ == Angle::angle180) {
            newFigure[1][0] = figure_[0][0];
            newFigure[1][1] = figure_[1][0];
            newFigure[0][1] = figure_[1][1];
            newFigure[0][2] = figure_[2][1];
        } else if (angle_ == Angle::angle245) {
            newFigure[2][1] = figure_[1][0];
            newFigure[1][1] = figure_[1][1];
            newFigure[1][0] = figure_[0][1];
            newFigure[0][0] = figure_[0][2];
        } else if (angle_ == Angle::angle0) {
            newFigure[1][0] = figure_[0][0];
            newFigure[1][1] = figure_[1][0];
            newFigure[0][1] = figure_[1][1];
            newFigure[0][2] = figure_[2][1];
        }
    }

    if (type_ == Type::I) {
        if (angle_ == Angle::angle90) {
            newFigure[3][1] = figure_[1][0];
            newFigure[2][1] = figure_[1][1];
            newFigure[1][1] = figure_[1][2];
            newFigure[0][1] = figure_[1][3];
        } else if (angle_ == Angle::angle180) {
            newFigure[1][0] = figure_[0][1];
            newFigure[1][1] = figure_[1][1];
            newFigure[1][2] = figure_[2][1];
            newFigure[1][3] = figure_[3][1];
        } else if (angle_ == Angle::angle245) {
            newFigure[3][1] = figure_[1][0];
            newFigure[2][1] = figure_[1][1];
            newFigure[1][1] = figure_[1][2];
            newFigure[0][1] = figure_[1][3];
        } else if (angle_ == Angle::angle0) {
            newFigure[1][0] = figure_[0][1];
            newFigure[1][1] = figure_[1][1];
            newFigure[1][2] = figure_[2][1];
            newFigure[1][3] = figure_[3][1];
        }
    }
    figure_ = newFigure;
}

Figure::Color Figure::color() const {
    return color_;
}

FieldPos Figure::pos() const {
    return pos_;
}

Figure::PatternIterator Figure::begin() {
    return { figure_ };
}
Figure::PatternIterator Figure::end() {
    PatternIterator f{ figure_ };
    f.setPos(patternSize * patternSize);
    return f;
}

std::array<cocos2d::Sprite*, patternSize>& Figure::operator[](int row) {
    return figure_[row];
}
bool Figure::isMovingLR() const {
    return isMovingLR_;
}
bool Figure::isMovingDown() const {
    return isMovingDown_;
}

void Figure::setMovingLR(bool moving) {
    isMovingLR_ = moving;
}
void Figure::setMovingDown(bool moving) {
    isMovingDown_ = moving;
}