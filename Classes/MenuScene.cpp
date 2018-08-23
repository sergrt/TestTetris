#include "MenuScene.h"
#include "SimpleAudioEngine.h"
#include "MainScene.h"

USING_NS_CC;

Scene* MenuScene::createScene() {
    return MenuScene::create();
}

// Print useful error message instead of segfaulting when files are not there.
static void problemLoading(const char* filename) {
    printf("Error while loading: %s\n", filename);
    printf("Depending on how you compiled you might have to add 'Resources/' in front of filenames in HelloWorldScene.cpp\n");
}

// on "init" you need to initialize your instance
bool MenuScene::init() {
    //////////////////////////////
    // 1. super init first
    if (!Scene::init())
        return false;


    const auto visibleSize = Director::getInstance()->getVisibleSize();
    const Vec2 origin = Director::getInstance()->getVisibleOrigin();


    const auto labelStart = Label::createWithSystemFont("Start game", "Arial", 22);
    if (labelStart == nullptr) {
        problemLoading("'Arial.ttf'");
        return false;
    }
    auto startMenuItem = MenuItemLabel::create(labelStart, CC_CALLBACK_1(MenuScene::startGame, this));
    startMenuItem->setColor(Color3B(255, 255, 255));

    // position the label on the center of the screen
    startMenuItem->setPosition(Vec2(origin.x + visibleSize.width / 2,
        origin.y + visibleSize.height/2 - startMenuItem->getContentSize().height));
    
    // create menu, it's an autorelease object
    auto menu = Menu::create(startMenuItem, NULL);
    menu->setPosition(Vec2::ZERO);
    this->addChild(menu, 1);

    /////////////////////////////
    // 3. add your codes below...

    auto label = Label::createWithTTF("TestTetris", "fonts/Marker Felt.ttf", 24);
    if (label == nullptr) {
        problemLoading("'fonts/Marker Felt.ttf'");
        return false;
    }
    
    // position the label on the center of the screen
    label->setPosition(Vec2(origin.x + visibleSize.width / 2,
        origin.y + visibleSize.height - label->getContentSize().height));

    // add the label as a child to this layer
    this->addChild(label, 1);

    return true;
}


void MenuScene::startGame(Ref* pSender) {
    const auto mainScene = MainScene::createScene();
    Director::getInstance()->replaceScene(mainScene);
}
