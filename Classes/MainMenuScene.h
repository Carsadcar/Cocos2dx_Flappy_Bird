#pragma once

#include "cocos2d.h"
#include "SimpleAudioEngine.h"

class MainMenuScene : public cocos2d::LayerColor
{
public:
    static cocos2d::Scene* createScene();
    virtual bool init();

    CREATE_FUNC(MainMenuScene);

private:
    void GoToGameScene();
};