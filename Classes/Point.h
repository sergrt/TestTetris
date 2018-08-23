#pragma once

//////////////////////////////////////////////////////////////////////////
// Strong type for field position
template<typename T, typename Tag>
class Position {
    T x_;
    T y_;
public:
    Position(T x, T y)
        : x_{ x }, y_{ y } {
    }
    T x() const {
        return x_;
    }
    T y() const {
        return y_;
    }
    void setX(T x) {
        x_ = x;
    }
    void setY(T y) {
        y_ = y;
    }
};

using FieldPos = Position<int, struct FieldPositionTag>;
using ScreenPos = Position<float, struct ScreenPositionTag>;