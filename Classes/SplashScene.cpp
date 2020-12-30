#include "SplashScene.h"
#include "MainMenuScene.h"
#include "Definitions.h"

USING_NS_CC;

Scene* SplashScene::createScene()
{
    // 'scene' is an autorelease object
    auto scene = Scene::create();
    auto layer = SplashScene::create();

    scene->addChild(layer);

    return scene;
}

bool SplashScene::init()
{
    if (!LayerColor::initWithColor(Color4B::BLACK))
    {
        return false;
    }

    auto visibleSize = Director::getInstance()->getVisibleSize();

    auto backgroundSprite = Sprite::create("Splash Screen.png");
    backgroundSprite->setPosition(visibleSize.width/2, visibleSize.height / 2);

    addChild(backgroundSprite);

    scheduleOnce(schedule_selector(SplashScene::GoToMainMenuScene), DISPLAY_TIME_SPLASH_SCENE);

    return true;
}

void SplashScene::GoToMainMenuScene(float dt)
{
    auto scene = MainMenuScene::createScene();

    Director::getInstance()->replaceScene(TransitionFade::create(TRANSITION_TIME,scene));
}

