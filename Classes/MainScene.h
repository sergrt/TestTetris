#pragma once
#include "cocos2d.h"
#include <vector>
#include "Field.h"
#include "Observer.h"
#include "Optional.h"

class MainScene : public cocos2d::Scene, public Observer {
public:
    ~MainScene();
    static cocos2d::Scene* createScene();
    bool init() override;

    // implement the "static create()" method manually
    CREATE_FUNC(MainScene);

    void onKeyPressed(cocos2d::EventKeyboard::KeyCode key, cocos2d::Event* event);
    void onKeyReleased(cocos2d::EventKeyboard::KeyCode key, cocos2d::Event* event);

    void update(float delta) override;
    
    // observer
    void addedGround(const FieldPos& pos) override;
    void addedWall(const FieldPos& pos) override;
    void addedFigure() override;
    void gameOver() override;
    void spriteRemoved(cocos2d::Sprite* sprite) override;
    void rotatedFigure() override;


    Field field_;
    std::unique_ptr<cocos2d::Layer> currentFigure_;




    //struct Movable {
        enum class MoveDirection {
            Left,
            Right,
            Down,
            None
        };

        
        static MoveDirection moveDirectionFromKey(cocos2d::EventKeyboard::KeyCode key);
    //};
    //
    //Movable movable_;

    

    std::vector<cocos2d::EventKeyboard::KeyCode> keyPressed_;

    float moveNextDelay = 0.0f;
        
    void startMovement(MoveDirection direction);
    
    float fallDownTimer = 0.0f;
    
    void setFigurePosition(const FieldPos& pos);
    


    
    void initializeGame();

    void setSpritesToCurrentFigure();
    std::queue< std::pair<int, int>> plannedCoordsChanges_;
    void setPlannedCoords();
    void updateGravity(float delta);
    void gameCycle();

    

    cocos2d::Label* scoreLabel_;
    int score_;
    void setScore(int score);
    void addScore(int score);

    Utilities::optional<cocos2d::EventKeyboard::KeyCode> keyPlanned_;
    
    void processKeyboard(cocos2d::EventKeyboard::KeyCode key);
};
