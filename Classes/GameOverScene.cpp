#include "GameOverScene.h"
#include "GameScene.h"
#include "MainMenuScene.h"
#include "Definitions.h"

USING_NS_CC;

Scene* GameOverScene::createScene(unsigned int tempScore)
{
    gameScore = tempScore;
    // 'scene' is an autorelease object
    auto scene = Scene::create();
    auto layer = GameOverScene::create();

    scene->addChild(layer);

    return scene;
}

bool GameOverScene::init()
{
    if (!LayerColor::initWithColor(Color4B::BLACK))
    {
        return false;
    }

    auto visibleSize = Director::getInstance()->getVisibleSize();

    auto backgroundSprite = Sprite::create("Background.png");

    backgroundSprite->setPosition(visibleSize.width / 2, visibleSize.height / 2);

    addChild(backgroundSprite);

    auto retryItem = MenuItemImage::create("Retry Button.png", "Retry Button Clicked.png", CC_CALLBACK_0(GameOverScene::GoToGameScene, this));
    retryItem->setPosition(visibleSize.width/2,visibleSize.height/4*3);

    auto mainMenuItem = MenuItemImage::create("Menu Button.png", "Menu Button Clicked.png", CC_CALLBACK_0(GameOverScene::GoToMainMenuScene, this));
    mainMenuItem->setPosition(visibleSize.width / 2, visibleSize.height / 4);

    auto menu = Menu::create(retryItem,mainMenuItem,NULL);
    menu->setPosition(Point::ZERO);

    addChild(menu);

    UserDefault* def = UserDefault::getInstance();
    auto highScore = def->getIntegerForKey("HIGHSCORE", 0);

    if (gameScore > highScore) {
        highScore = gameScore;

        def->setIntegerForKey("HIGHSCORE", highScore);
    }

    def->flush();

    std::string lol = def->getXMLFilePath();

    __String* tempScore = __String::createWithFormat("%i", gameScore);

    auto scoreLabel = Label::createWithTTF(tempScore->getCString(), "Marker Felt.ttf", visibleSize.height * SCORE_FONT_SIZE);
    scoreLabel->setColor(Color3B::WHITE);
    scoreLabel->setPosition(visibleSize.width / 4, visibleSize.height/4*2);
    addChild(scoreLabel, 1000);

    __String* tempHighScore = __String::createWithFormat("%i", highScore);

    auto highScoreLabel = Label::createWithTTF(tempHighScore->getCString(), "Marker Felt.ttf", visibleSize.height * SCORE_FONT_SIZE);
    highScoreLabel->setColor(Color3B::YELLOW);
    highScoreLabel->setPosition(visibleSize.width / 4*3, visibleSize.height / 4 * 2);
    addChild(highScoreLabel, 1000);

    return true;
}

void GameOverScene::GoToMainMenuScene()
{
    auto scene = MainMenuScene::createScene();

    Director::getInstance()->replaceScene(TransitionFade::create(TRANSITION_TIME,scene));
}

void GameOverScene::GoToGameScene()
{
    auto scene = GameScene::createScene();

    Director::getInstance()->replaceScene(TransitionFade::create(TRANSITION_TIME, scene));
}

unsigned int GameOverScene::gameScore = 0;

