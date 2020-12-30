#include "AppDelegate.h"
#include "SplashScene.h"

USING_NS_CC;

AppDelegate::AppDelegate() {

}

AppDelegate::~AppDelegate()
{
}

bool AppDelegate::applicationDidFinishLaunching() {
    auto director = Director::getInstance();
    auto glview = director->getOpenGLView();

    FileUtils::getInstance()->addSearchPath("fonts");
    FileUtils::getInstance()->addSearchPath("iphone");
    FileUtils::getInstance()->addSearchPath("sounds");

    if (!glview) {
        glview = GLViewImpl::create("Flappy Bird");
        glview->setFrameSize(384, 568);
        glview->setDesignResolutionSize(384,568,ResolutionPolicy::NO_BORDER);
        director->setOpenGLView(glview);
    }
    director->setDisplayStats(true);
    auto scene = SplashScene::createScene();
    director->runWithScene(scene);

    return true;
}

void AppDelegate::applicationDidEnterBackground() {
}

void AppDelegate::applicationWillEnterForeground() {
}