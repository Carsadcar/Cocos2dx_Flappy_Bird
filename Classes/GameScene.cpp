#include "GameScene.h"
#include "Definitions.h"
#include "GameOverScene.h"

USING_NS_CC;

Scene* GameScene::createScene()
{
    // 'scene' is an autorelease object
    auto scene = Scene::createWithPhysics();
    auto layer = GameScene::create();

    scene->getPhysicsWorld()->setGravity(Vec2::ZERO);

    scene->addChild(layer);

    return scene;
}

bool GameScene::init()
{
    if (!LayerColor::initWithColor(Color4B::BLACK))
    {
        return false;
    }

    auto visibleSize = Director::getInstance()->getVisibleSize();

    auto backgroundSprite = Sprite::create("Background.png");

    backgroundSprite->setPosition(visibleSize.width / 2, visibleSize.height / 2);

    addChild(backgroundSprite);

    auto edgeBody = PhysicsBody::createEdgeBox(backgroundSprite->getContentSize(), PHYSICSBODY_MATERIAL_DEFAULT, 3);
    edgeBody->setCollisionBitmask(OBSTACLE_COLLISION_BITMASK);
    edgeBody->setContactTestBitmask(true);

    auto edgeNode = Node::create();

    edgeNode->setPosition(visibleSize.width / 2, visibleSize.height / 2);

    edgeNode->setPhysicsBody(edgeBody);

    addChild(edgeNode);

    schedule(schedule_selector(GameScene::SpawnPipe),PIPE_SPAWN_FREQUENCY * visibleSize.width);

    flappyBird = Sprite::create("Ball.png");
    auto flappyBody = PhysicsBody::createCircle(flappyBird->getContentSize().height);
    flappyBody->setCollisionBitmask(BIRD_COLLISION_BITMASK);
    flappyBody->setContactTestBitmask(true);
    isFalling = true;

    flappyBird->setPhysicsBody(flappyBody);
    flappyBird->setPosition(visibleSize.width/2,visibleSize.height/2);
    addChild(flappyBird,100);

    auto contactListener = EventListenerPhysicsContact::create();
    contactListener->onContactBegin = CC_CALLBACK_1(GameScene::onContactBegin, this);
    _eventDispatcher->addEventListenerWithSceneGraphPriority(contactListener, this);

    auto mouseListener = EventListenerMouse::create();
    mouseListener->onMouseDown = CC_CALLBACK_1(GameScene::OnTouchBegan, this);
    _eventDispatcher->addEventListenerWithSceneGraphPriority(mouseListener, this);

    score = 0;

    __String* tempScore = __String::createWithFormat("%i", score);

    scoreLabel = Label::createWithTTF(tempScore->getCString(), "Marker Felt.ttf", visibleSize.height * SCORE_FONT_SIZE);
    scoreLabel->setColor(Color3B::WHITE);
    scoreLabel->setPosition(visibleSize.width / 2, 0.75 * visibleSize.height);
    addChild(scoreLabel, 1000);

    scheduleUpdate();

    auto audio = CocosDenshion::SimpleAudioEngine::getInstance();
    audio->preloadEffect("Wing.mp3");
    audio->preloadEffect("Hit.mp3");
    audio->preloadEffect("Point.mp3");

    return true;
}

void GameScene::SpawnPipe(float dt) {
    auto topPipe = Sprite::create("pipe.png");
    auto bottomPipe = Sprite::create("pipe.png");

    auto topPipeBody = PhysicsBody::createBox(topPipe->getContentSize());
    auto bottomPipeBody = PhysicsBody::createBox(bottomPipe->getContentSize());

    auto random = CCRANDOM_0_1();

    if (random < LOWER_SCREEN_PIPE_THRESHOLD)
        random = LOWER_SCREEN_PIPE_THRESHOLD;
    else if (random > UPPER_SCREEN_PIPE_THRESHOLD)
        random = UPPER_SCREEN_PIPE_THRESHOLD;

    auto topPipePosition = random * Director::getInstance()->getVisibleSize().height + topPipe->getContentSize().height / 2;

    topPipeBody->setDynamic(false);
    bottomPipeBody->setDynamic(false);
    topPipeBody->setCollisionBitmask(OBSTACLE_COLLISION_BITMASK);
    bottomPipeBody->setCollisionBitmask(OBSTACLE_COLLISION_BITMASK);
    topPipeBody->setContactTestBitmask(true);
    bottomPipeBody->setContactTestBitmask(true);

    topPipe->setPhysicsBody(topPipeBody);
    bottomPipe->setPhysicsBody(bottomPipeBody);

    topPipe->setPosition(Director::getInstance()->getVisibleSize().width+topPipe->getContentSize().width/2,topPipePosition);
    bottomPipe->setPosition(topPipe->getPositionX(), topPipe->getPositionY()-topPipe->getContentSize().height-Sprite::create("Ball.png")->getContentSize().height*PIPE_GAP);

    addChild(topPipe);
    addChild(bottomPipe);

    auto topPipeAction = MoveBy::create(PIPE_MOVEMENT_SPEED* Director::getInstance()->getVisibleSize().width,Vec2(-Director::getInstance()->getVisibleSize().width*1.5,0));
    auto bottomPipeAction = topPipeAction->clone();

    topPipe->runAction(topPipeAction);
    bottomPipe->runAction(bottomPipeAction);

    auto pointNode = Node::create();
    auto pointBody = PhysicsBody::createBox(Size(1,Sprite::create("Ball.png")->getContentSize().height* PIPE_GAP));
    pointBody->setDynamic(false);
    pointBody->setContactTestBitmask(true);
    pointBody->setCollisionBitmask(POINT_COLLISION_BITMASK);
    pointNode->setPhysicsBody(pointBody);
    auto pointAction = topPipeAction->clone();
    pointNode->runAction(pointAction);
    pointNode->setPosition(topPipe->getPositionX(), (topPipe->getPositionY() + bottomPipe->getPositionY()) / 2);
    addChild(pointNode);

}

bool GameScene::onContactBegin(cocos2d::PhysicsContact& contact)
{
    PhysicsBody* a = contact.getShapeA()->getBody();
    PhysicsBody *b= contact.getShapeB()->getBody();

    if ((a->getCollisionBitmask()==BIRD_COLLISION_BITMASK && b->getCollisionBitmask()==OBSTACLE_COLLISION_BITMASK) ||
        (b->getCollisionBitmask() == BIRD_COLLISION_BITMASK && a->getCollisionBitmask()== OBSTACLE_COLLISION_BITMASK)) {
        CocosDenshion::SimpleAudioEngine::getInstance()->playEffect("Hit.mp3");

        auto scene = GameOverScene::createScene(score);

        Director::getInstance()->replaceScene(TransitionFade::create(TRANSITION_TIME,scene));
    }
    else if ((a->getCollisionBitmask() == BIRD_COLLISION_BITMASK && b->getCollisionBitmask() == POINT_COLLISION_BITMASK) ||
        (b->getCollisionBitmask() == BIRD_COLLISION_BITMASK && a->getCollisionBitmask() == POINT_COLLISION_BITMASK)) {
        CocosDenshion::SimpleAudioEngine::getInstance()->playEffect("Point.mp3");

        score++;

        __String* tempScore = __String::createWithFormat("%i", score);

        scoreLabel->setString(tempScore->getCString());
    }

    return true;
}

bool GameScene::OnTouchBegan(cocos2d::EventMouse* event)
{
    CocosDenshion::SimpleAudioEngine::getInstance()->playEffect("Wing.mp3");

    Fly();

    scheduleOnce(schedule_selector(GameScene::StopFlying), BIRD_FLY_DURATION);

    return true;
}

void GameScene::update(float dt)
{
    Fall();
}

void GameScene::Fall()
{
    if (isFalling) {
        flappyBird->setPosition(Director::getInstance()->getVisibleSize().width/2, flappyBird->getPositionY()-BIRD_FALLING_SPEED* Director::getInstance()->getVisibleSize().height);
    }
    else {
        flappyBird->setPosition(Director::getInstance()->getVisibleSize().width / 2, flappyBird->getPositionY() + BIRD_FALLING_SPEED * Director::getInstance()->getVisibleSize().height);
    }
}

