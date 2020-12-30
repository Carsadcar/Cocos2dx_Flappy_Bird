#pragma once

#include "cocos2d.h"
#include "SimpleAudioEngine.h"

class GameScene : public cocos2d::LayerColor
{
public:
    static cocos2d::Scene* createScene();
    virtual bool init();

    CREATE_FUNC(GameScene);

 private:
     bool isFalling;
     cocos2d::Sprite* flappyBird;
     unsigned int score;
     cocos2d::Label* scoreLabel;

     void SpawnPipe(float dt);
     bool onContactBegin(cocos2d::PhysicsContact& contact);
     bool OnTouchBegan(cocos2d::EventMouse* event);
     void update(float dt);
     void StopFlying(float dt) { isFalling = true; }
     void Fall();
     void Fly() { isFalling = false; }
};