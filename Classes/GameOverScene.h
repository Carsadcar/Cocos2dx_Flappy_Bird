#pragma once

#include "cocos2d.h"

class GameOverScene : public cocos2d::LayerColor
{
public:
    static cocos2d::Scene* createScene(unsigned int tempScore);
    virtual bool init();

    CREATE_FUNC(GameOverScene);

private:
    static unsigned int gameScore;

    void GoToMainMenuScene();
    void GoToGameScene();
};