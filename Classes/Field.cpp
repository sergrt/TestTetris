#include "Field.h"
#include "Point.h"

//#include <stdlib.h>
#include <cocos2d.h>


Field::Field() {
    
}
void Field::initialize() {
    //nextFigureType = Figure::getRandomType()

    field_.resize(Tetris::Field::height);

    for (int i = 0; i < field_.size(); ++i) {
        for (int j = 0; j < field_[i].size(); ++j) {
            field_[i][j] = nullptr;

            if (i < Tetris::Field::height - Tetris::Field::topSpace) {
                if (i == 0) { // ground
                    //field_[i][j] = (cocos2d::Sprite*)1;
                    notifyGroundAdded(FieldPos{ j, i });
                    // actual sprite pointer will be received in setSprite()
                } else if (j == 0 || j == Tetris::Field::width - 1) { // walls
                    //field_[i][j] = (cocos2d::Sprite*)1;
                    notifyWallAdded(FieldPos{ j, i });
                    // actual sprite pointer will be received in setSprite()
                }
            }
        }
    }
}
void Field::setWall(const FieldPos& pos, cocos2d::Sprite* sprite) {
    field_[pos.y()][pos.x()] = sprite;
}
void Field::cleanup() {
    //field_.resize(GameParameters::fieldHeight);
    //figure_->cleanup();
    //figure_.reset();
    for (int i = 1; i < field_.size(); ++i) {
        for (int j = 1; j < field_[i].size() - 1; ++j) {
            if (field_[i][j]) {
                notifySpriteRemoved(field_[i][j]);
                field_[i][j] = nullptr;
            }
        }
    }
}
bool Field::canMoveDown() {
    auto pos = figure_->pos();
    int newX = pos.x();
    int newY = pos.y() - 1;

    return checkFieldVsFigure(newX, newY);
}
bool Field::canMoveLeft() {
    auto pos = figure_->pos();
    int newX = pos.x() - 1;
    int newY = pos.y();

    return checkFieldVsFigure(newX, newY);
}
bool Field::canMoveRight() {
    auto pos = figure_->pos();
    int newX = pos.x() + 1;
    int newY = pos.y();

    return checkFieldVsFigure(newX, newY);
}
bool Field::checkFieldVsFigure(int newX, int newY) {
    for (int i = newY, row = 0; i < newY + patternSize; ++i, ++row) {
        if (i >= 0 && i < Tetris::Field::height) {
            for (int j = newX, col = 0; j < newX + patternSize; ++j, ++col) {
                if (j >= 0 && j < Tetris::Field::width) {
                    //if (figure_->figure_[row][col] != 0 && field_[i][j] != 0)
                    if ((*figure_)[row][col] != 0 && field_[i][j] != 0)
                        return false;
                }
            }
        }
    }
    return true;
}
void Field::settleFigure() {
    auto pos = figure_->pos();
    int x = pos.x();
    int y = pos.y();


    for (int i = y, row = 0; i < y + patternSize; ++i, ++row) {
        if (i >= 0 && i < Tetris::Field::height) {
            for (int j = x, col = 0; j < x + patternSize; ++j, ++col) {
                if (j >= 0 && j < Tetris::Field::width) {
                    //if (figure_->figure_[row][col] != nullptr)
                    if ((*figure_)[row][col] != nullptr)
                        field_[i][j] = (*figure_)[row][col];
                }
            }
        }
    }


}

void Field::spawnNewFigure() {
    //figure_ = std::make_unique<Figure>(Figure::Type::L, Point{ GameParameters::fieldWidth / 2, GameParameters::fieldHeight - 1 });
    
    //figure_ = Figure::createRandom(Point{ GameParameters::fieldWidth / 2, 5 });
    const FieldPos pos = FieldPos{ Tetris::Field::width / 2 - Tetris::Figure::patternSize / 2, Tetris::Field::height - Tetris::Figure::patternSize };
    figure_ = Figure::createRandom(pos);

    if (!checkFieldVsFigure(pos.x(), pos.y())) {
        figure_.reset();
        notifyGameOver();
    } else {
        notifyFigureAdded();
    }
}

void Field::moveFigureLeft() {
    figure_->moveLeft();
}
void Field::moveFigureRight() {
    figure_->moveRight();
}
void Field::moveFigureDown() {
    figure_->moveDown();
}

bool Field::isMovingLR() const {
    return figure_->isMovingLR();
}
bool Field::isMovingDown() const {
    return figure_->isMovingDown();
}
void Field::setMovingLR(bool f) {
    figure_->setMovingLR(f);
}
void Field::setMovingDown(bool f) {
    figure_->setMovingDown(f);
}
std::map<cocos2d::Sprite*, int> Field::removeCompleteRows() {
    
    std::vector<int> completedRows;
    for (int i = 1; i < field_.size(); ++i) {
        bool complete = true;
        for (int j = 1; j < field_[i].size() - 1; ++j) {
            if (field_[i][j] == 0) {
                complete = false;
                break;
            }
        }

        if (complete) {
            completedRows.push_back(i);
            
            // actual remove sprites
            for (int x = 1; x < field_[i].size() - 1; ++x) {
                if (field_[i][x] != nullptr) {
                    notifySpriteRemoved(field_[i][x]);
                    field_[i][x] = nullptr;
                }
            }
        }
    }

    // Collect sprites that need to be shifted down
    std::map<cocos2d::Sprite*, int> res;
    for (int i = 1; i < field_.size(); ++i) {
        const bool rowIncomplete = !std::binary_search(std::cbegin(completedRows), std::cend(completedRows), i);
        if (rowIncomplete) {
            const auto lastLesserThanCur = std::find_if(std::crbegin(completedRows), std::crend(completedRows), [i](const int val) {
                return val < i;
            });

            if (lastLesserThanCur != std::crend(completedRows)) {
                const int shiftDistance = std::distance(lastLesserThanCur, std::crend(completedRows));
                for (int j = 1; j < field_[i].size() - 1; ++j) {
                    if (field_[i][j] != nullptr) {
                        // shift down and remember distance for visualization
                        res[field_[i][j]] = shiftDistance;
                        field_[i - shiftDistance][j] = field_[i][j];
                        field_[i][j] = nullptr;
                    }
                }
            }
        }
    }

    return res;
}

void Field::rotate() {
    auto backup = *figure_;

    figure_->rotate();
    auto pos = figure_->pos();
    if (!checkFieldVsFigure(pos.x(), pos.y())) {
        figure_.reset(new Figure(backup));
    } else {
        notifyFigureRotated();
    }
}

Figure::PatternIterator Field::currentFigureBegin() const {
    return figure_->begin();
}
Figure::PatternIterator Field::currentFigureEnd() const {
    return figure_->end();
}

FieldPos Field::currentFigurePos() const {
    return figure_->pos();
}

Figure::Color Field::currentFigureColor() const {
    return figure_->color();
}