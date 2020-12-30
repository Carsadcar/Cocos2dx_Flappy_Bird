#include "MainMenuScene.h"
#include "GameScene.h"
#include "Definitions.h"

USING_NS_CC;

Scene* MainMenuScene::createScene()
{
    // 'scene' is an autorelease object
    auto scene = Scene::create();
    auto layer = MainMenuScene::create();

    scene->addChild(layer);

    return scene;
}

bool MainMenuScene::init()
{
    if (!LayerColor::initWithColor(Color4B::BLACK))
    {
        return false;
    }

    auto visibleSize = Director::getInstance()->getVisibleSize();

    auto backgroundSprite = Sprite::create("Background.png");

    backgroundSprite->setPosition(visibleSize.width / 2, visibleSize.height / 2);

    addChild(backgroundSprite);

    auto playItem = MenuItemImage::create("Play Button.png", "Play Button Clicked.png", CC_CALLBACK_0(MainMenuScene::GoToGameScene,this));
    auto menu = Menu::create(playItem, NULL);

    playItem->setPosition(visibleSize.width / 2, visibleSize.height / 2);
    menu->setPosition(Point::ZERO);

    auto titleSprite = Sprite::create("Title.png");

    titleSprite->setPosition(visibleSize.width/2,visibleSize.height-titleSprite->getContentSize().height);
    addChild(titleSprite);

    addChild(menu);

    return true;
}

void MainMenuScene::GoToGameScene()
{
    auto scene = GameScene::createScene();

    Director::getInstance()->replaceScene(TransitionFade::create(TRANSITION_TIME, scene));
}

