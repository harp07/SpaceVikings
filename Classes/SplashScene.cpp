#include "SplashScene.h"
#include "HelloWorldScene.h"

using namespace cocos2d;

CCScene* SplashScene::scene(){

	CCScene * scene = NULL;
	do{

		scene = CCScene::create();
        CC_BREAK_IF(! scene);

		SplashScene *layer = SplashScene::create();
		CC_BREAK_IF(!layer);

		

		scene->addChild(layer);

		

	}while(0);

	return scene;
}

bool SplashScene::init(){
	
	bool bRet = false;
	do{

		CC_BREAK_IF(! CCLayer::init());

		CCSize winSize = CCDirector::sharedDirector()->getWinSize();
		
		CCSprite *splashSprite = CCSprite::create("startup.png");
		splashSprite->setPosition(ccp(winSize.width/2 , winSize.height/2));
		splashSprite->setScaleX(winSize.width/splashSprite->getContentSize().width);
		splashSprite->setScaleY(winSize.height/splashSprite->getContentSize().height);
		this->addChild(splashSprite);
		splashSprite->setOpacity(0);

		CCDelayTime*delay1=CCDelayTime::actionWithDuration(0.5f);
        CCFadeIn*fade=CCFadeIn::actionWithDuration(1.50f);
        CCDelayTime*delay2=CCDelayTime::actionWithDuration(2.0f);
        splashSprite->runAction(CCSequence::actions(delay1,fade,delay2,NULL));

		bRet = true;
		this->setTouchEnabled(true);
	}while(0);
	return bRet;
}

void SplashScene::ccTouchesEnded(cocos2d::CCSet* touches, cocos2d::CCEvent* event){
	CCTouch* touch = (CCTouch*)(touches->anyObject());
	GameOptions::sharedGameOptions()->setToggleSFX(true);
	GameOptions::sharedGameOptions()->setToggleMusic(true);
	CCDirector::sharedDirector()->replaceScene(HelloWorld::scene());
}
