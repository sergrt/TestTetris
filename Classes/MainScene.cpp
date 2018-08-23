#include "MainScene.h"
#include "SimpleAudioEngine.h"
#include "Point.h"

USING_NS_CC;

namespace Screen {
    ScreenPos fieldBottom{ 0.0f, 0.0f };

    ScreenPos fieldPosToScreenPos(const FieldPos& pos) {
        return { fieldBottom.x() + pos.x() * Tetris::Sprites::brickSpriteSize,
            fieldBottom.y() + pos.y() * Tetris::Sprites::brickSpriteSize };
    }
}


Scene* MainScene::createScene()
{
    return MainScene::create();
}
MainScene::~MainScene() {
    field_.removeObserver(*this);
}

void MainScene::addedGround(const FieldPos& pos) {
    auto nodeBottom = Sprite::create(R"(images\background\ground.png)");
    //auto nodeBottom = new Sprite();
    //nodeBottom->setTexture(R"(c:\workspace\TestTetris\TestTetris\Resources\images\background\ground.png)");
    
    nodeBottom->setAnchorPoint(Vec2(0.0f, 0.0f));
    auto screenPos = Screen::fieldPosToScreenPos(pos);
    nodeBottom->setPosition(Vec2(screenPos.x(), screenPos.y()));
    field_.setWall(pos, nodeBottom);
    this->addChild(nodeBottom, -1);
}
void MainScene::addedWall(const FieldPos& pos) {
    auto nodeWall = Sprite::create(R"(images\background\wall.png)");
    //auto nodeWall = new Sprite();
    //nodeWall->setTexture(R"(c:\workspace\TestTetris\TestTetris\Resources\images\background\wall.png)");
    
    nodeWall->setAnchorPoint(Vec2(0.0f, 0.0f));
    auto screenPos = Screen::fieldPosToScreenPos(pos);
    nodeWall->setPosition(Vec2(screenPos.x(), screenPos.y()));
    field_.setWall(pos, nodeWall);
    this->addChild(nodeWall, -1);
}
void MainScene::addedFigure() {
    currentFigure_ = std::make_unique<Layer>();
    setSpritesToCurrentFigure();
    this->addChild(currentFigure_.get(), 1);
    //figure_.targetPos_ = figure_.placeholder_->getPosition();
}



// Print useful error message instead of segfaulting when files are not there.
static void problemLoading(const char* filename)
{
    printf("Error while loading: %s\n", filename);
    printf("Depending on how you compiled you might have to add 'Resources/' in front of filenames in HelloWorldScene.cpp\n");
}
void MainScene::initializeGame() {
    field_.initialize();
    setScore(0);
    field_.spawnNewFigure();
}
void MainScene::setScore(int score) {
    score_ = score;
    scoreLabel_->setString(std::to_string(score_));
}
void MainScene::addScore(int score) {
    score_ += score;
    scoreLabel_->setString(std::to_string(score_));
}
// on "init" you need to initialize your instance
bool MainScene::init()
{
    field_.registerObserver(*this);
    //////////////////////////////
    // 1. super init first
    if (!Scene::init())
    {
        return false;
    }

    auto visibleSize = Director::getInstance()->getVisibleSize();
    Vec2 origin = Director::getInstance()->getVisibleOrigin();

    
    
    scoreLabel_ = Label::createWithTTF("99999", "fonts/Marker Felt.ttf", 22);
    this->addChild(scoreLabel_, -1);
    scoreLabel_->setAlignment(TextHAlignment::CENTER);
    scoreLabel_->setPosition(visibleSize.width - scoreLabel_->getBoundingBox().size.width / 2 - 10, visibleSize.height / 2 - 1.5*scoreLabel_->getBoundingBox().size.height);


    auto label = Label::createWithTTF("Score", "fonts/Marker Felt.ttf", 22);
    this->addChild(label, -1);
    label->setAlignment(TextHAlignment::CENTER);
    label->setPosition(scoreLabel_->getPosition().x, scoreLabel_->getPosition().y + 1.5 * label->getBoundingBox().size.height);
    

    Screen::fieldBottom = ScreenPos(visibleSize.width / 2.0 - (Tetris::Sprites::brickSpriteSize * Tetris::Field::width / 2),
        visibleSize.height / 2.0 - (Tetris::Sprites::brickSpriteSize * Tetris::Field::height / 2));

    initializeGame();

    //spawnFigure();
    //spawnNewFigure();


    auto listener = EventListenerKeyboard::create();
    listener->onKeyPressed = CC_CALLBACK_2(MainScene::onKeyPressed, this);
    listener->onKeyReleased = CC_CALLBACK_2(MainScene::onKeyReleased, this);
    _eventDispatcher->addEventListenerWithSceneGraphPriority(listener, this);

    this->scheduleUpdate();

    return true;
}
void MainScene::setFigurePosition(const FieldPos& pos) {
    auto screenPos = Screen::fieldPosToScreenPos(pos);
    currentFigure_->setPosition(screenPos.x(), screenPos.y());
}
void MainScene::setSpritesToCurrentFigure() {
    for (auto i = field_.currentFigureBegin(); i != field_.currentFigureEnd(); ++i) {
        if (*i != 0) {
            if (*i != spritePlaceholder) {
                (*i)->stopAllActions();
                currentFigure_->removeChild(*i);
                delete *i;
            }

            
            const auto color = field_.currentFigureColor();
            std::string textureFileName;
            if (color == Figure::Color::blue)
                textureFileName = R"(images\bricks\blue.png)";
            else if(color == Figure::Color::green)
                textureFileName = R"(images\bricks\green.png)";
            else if (color == Figure::Color::red)
                textureFileName = R"(images\bricks\red.png)";
            else if (color == Figure::Color::yellow)
                textureFileName = R"(images\bricks\yellow.png)";
            else
                throw std::runtime_error("Unhandled color");

            auto sprite = new Sprite();
            sprite->retain();
            sprite->setTexture(textureFileName);

            sprite->setAnchorPoint(Vec2(0.0f, 0.0f));
            currentFigure_->addChild(sprite);
            sprite->setPosition(Tetris::Sprites::brickSpriteSize * i.x(), Tetris::Sprites::brickSpriteSize * i.y());
            *i = sprite;
            
        }
    }
    setFigurePosition(field_.currentFigurePos());
    
    
  
}

void MainScene::setPlannedCoords() {
    while (!plannedCoordsChanges_.empty()) {
        auto p = plannedCoordsChanges_.front();
        plannedCoordsChanges_.pop();
        if (p.first < 0)
            field_.moveFigureLeft();
        if (p.first > 0)
            field_.moveFigureRight();
        if (p.second < 0)
            field_.moveFigureDown();
    }
}
void MainScene::startMovement(MoveDirection direction) {
    field_.setMovingLR(true);
    currentFigure_->stopAllActions();

    auto transitTime = Tetris::Gameplay::transitTime;
    
    Vec2 targetPos = currentFigure_->getPosition();
    if (direction == MoveDirection::Left)
        //figure_.targetPos_.x -= Tetris::Sprites::brickSpriteSize;
        targetPos.x -= Tetris::Sprites::brickSpriteSize;
    else if (direction == MoveDirection::Right)
        //figure_.targetPos_.x += Tetris::Sprites::brickSpriteSize;
        targetPos.x += Tetris::Sprites::brickSpriteSize;
    else if (direction == MoveDirection::Down) {
        //figure_.targetPos_.y -= Tetris::Sprites::brickSpriteSize;
        targetPos.y -= Tetris::Sprites::brickSpriteSize;
        transitTime = Tetris::Gameplay::gravityTime;
    }


    auto moveTo = MoveTo::create(transitTime, targetPos);

    {
        int x = 0;
        int y = 0;
        if (direction == MoveDirection::Left)
            x = -1;
        else if (direction == MoveDirection::Right)
            x = 1;
        else if (direction == MoveDirection::Down)
            y = -1;

        plannedCoordsChanges_.push(std::make_pair(x, y));
    }
    
    auto updateCoordAction = CallFunc::create([this]() {
        //OutputDebugStringA("--> ENTER");
        field_.setMovingLR(false);
        field_.setMovingDown(false);
        setPlannedCoords();
        
        //OutputDebugStringA("--> EXIT");
    });
    
    

    auto sequence = Sequence::create(moveTo, updateCoordAction, nullptr);
    currentFigure_->runAction(sequence);
}

void MainScene::onKeyPressed(EventKeyboard::KeyCode key, cocos2d::Event* event) {
    //log("KEY = %d", key);

    keyPressed_.push_back(key);
    //processKeyboard(key);
    
    keyPlanned_ = key;
    gameCycle();
    
}
MainScene::MoveDirection MainScene::moveDirectionFromKey(cocos2d::EventKeyboard::KeyCode key) {
    if (key == EventKeyboard::KeyCode::KEY_LEFT_ARROW)
        return MoveDirection::Left;
    if (key == EventKeyboard::KeyCode::KEY_RIGHT_ARROW)
        return MoveDirection::Right;
    
    return MoveDirection::None;
}
void MainScene::processKeyboard(cocos2d::EventKeyboard::KeyCode key) {
    if (key == EventKeyboard::KeyCode::KEY_LEFT_ARROW && field_.canMoveLeft()
        || key == EventKeyboard::KeyCode::KEY_RIGHT_ARROW && field_.canMoveRight()) {
        startMovement(moveDirectionFromKey(key));
    } else if (key == EventKeyboard::KeyCode::KEY_UP_ARROW) {
        field_.rotate();
    }
}
void MainScene::onKeyReleased(EventKeyboard::KeyCode key, cocos2d::Event* event) {
    //log("KEY = %d released", key);
    keyPressed_.erase(std::find(std::cbegin(keyPressed_), std::cend(keyPressed_), key));
}
void MainScene::gameCycle() {
    if (!field_.isMovingLR() && !field_.isMovingDown()) {
        if (keyPlanned_ && fallDownTimer < Tetris::Gameplay::gravityTimerTolerance) {
            processKeyboard(*keyPlanned_);
            keyPlanned_.reset();
        } else {
            updateGravity(0.0);
        }
        //setPlannedCoords();
    }

    if (!field_.isMovingLR() && !field_.isMovingDown()) {
        if (!keyPressed_.empty()) {
            if (moveNextDelay > Tetris::Gameplay::keyHoldRepeatInterval) {
                processKeyboard(keyPressed_.back());
                moveNextDelay = 0.0f;
            }
        } else {
            moveNextDelay = 0.0f;
        }
    }
}
void MainScene::update(float delta) {
    // update timers
    if (!field_.isMovingLR() && !keyPressed_.empty()) {
        moveNextDelay += delta;
    }
    fallDownTimer += delta;

    gameCycle();
    return;
    /*
    if (!field_.isMovingLR() && !field_.isMovingDown()) {
        if (keyPlanned_.first != cocos2d::EventKeyboard::KeyCode::KEY_ALT && fallDownTimer < Tetris::Gameplay::gravityTimerTolerance) {
            processKeyboard(keyPlanned_.first);
            keyPlanned_.first = EventKeyboard::KeyCode::KEY_ALT;
        } else {
            updateGravity(0.0);
        }
        //setPlannedCoords();
    }
    
    if (!field_.isMovingLR()) {
        if (!keyPressed_.empty()) {
            moveNextDelay += delta;
            if (moveNextDelay > 0.15) {
                
                if (!field_.isMovingDown()) {
                    if (setNewPos(moveDirectionFromKey(keyPressed_.back())))
                        startMovement(keyPressed_.back());
                }

                moveNextDelay = 0.0f;

                
            }
        } else {
            moveNextDelay = 0.0f;
        }
        
    }
    fallDownTimer += delta;
    //updateGravity(delta);
    */
}

void MainScene::updateGravity(float delta) {
    //fallDownTimer += delta;

    if (fallDownTimer > Tetris::Gameplay::gravityTimer) {
        if (field_.canMoveDown()) {
            startMovement(MoveDirection::Down);
        } else {
            field_.settleFigure();
            // move all child nodes to this
            auto children = currentFigure_->getChildren();
            for (auto c : children) {
                auto p = c->getParent()->convertToWorldSpace(c->getPosition());
                c->setPosition(p);
                currentFigure_->removeChild(c, true);
                this->addChild(c, 1);
                
                //currentFigure_->removeChild(c, true);
                //this
                //c->
            }
            currentFigure_->stopAllActions();
            currentFigure_->removeFromParentAndCleanup(true);
            //removeChild(currentFigure_.get());
            currentFigure_.reset();
            auto bricksToMoveDown = field_.removeCompleteRows();
            int maxRowsBurned = 0;
            for (auto b : bricksToMoveDown) {
                auto newPosition = b.first->getPosition();
                newPosition.y -= Tetris::Sprites::brickSpriteSize * b.second;
                auto moveTo = MoveTo::create(Tetris::Gameplay::gravityTime, newPosition);
                b.first->runAction(moveTo);

                if (b.second > maxRowsBurned)
                    maxRowsBurned = b.second;
            }
            if (maxRowsBurned != 0) {
                addScore(maxRowsBurned == 1 ? 100
                    : (maxRowsBurned == 2 ? 300
                        : (maxRowsBurned == 3 ? 700
                            : 1500)));
            }
            field_.spawnNewFigure();
        }
        fallDownTimer = 0.0f;
    }
}

void MainScene::gameOver() {
    // clean up everything

    // clean up current figure
    // no need - game over can be sent only on spawn new figure
    /*
    auto children = currentFigure_->getChildren();
    for (auto c : children) {
        currentFigure_->removeChild(c, true);
        delete c;
    }
    delete currentFigure_;
    */
    // clean up field
    //this->removeAllChildren();
    
    field_.cleanup();

    // start new game
    //initializeGame();
    setScore(0);
    field_.spawnNewFigure();
    
}
void MainScene::spriteRemoved(cocos2d::Sprite* sprite) {
    sprite->stopAllActions();
    this->removeChild(sprite);
    //sprite->removeFromParentAndCleanup(true);
    
    delete sprite;
}
void MainScene::rotatedFigure() {
    currentFigure_->removeAllChildrenWithCleanup(true);
    setSpritesToCurrentFigure();
}