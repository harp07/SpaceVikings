#include "StartScene.h"
#include "SplashScene.h"

using namespace cocos2d;

CCScene* StartScene::scene(){

	CCScene * scene = NULL;
	do{

		scene = CCScene::create();
        CC_BREAK_IF(! scene);

		StartScene *layer = StartScene::create();
		CC_BREAK_IF(!layer);

		

		scene->addChild(layer);

		

	}while(0);

	return scene;
}

bool StartScene::init(){
	
	bool bRet = false;
	do{

		CC_BREAK_IF(! CCLayer::init());

		CCSize winSize = CCDirector::sharedDirector()->getWinSize();
		
		CCSprite *splashSprite = CCSprite::spriteWithFile("startupmain.png");
		splashSprite->setPosition(ccp(winSize.width/2 , winSize.height/2));
		splashSprite->setScaleX(winSize.width/splashSprite->getContentSize().width);
		splashSprite->setScaleY(winSize.height/splashSprite->getContentSize().height);
		this->addChild(splashSprite);
		splashSprite->setOpacity(0);

		CCDelayTime*delay1=CCDelayTime::actionWithDuration(0.5f);
        CCFadeIn*fade=CCFadeIn::actionWithDuration(1.5f);
        CCDelayTime*delay2=CCDelayTime::actionWithDuration(2.0f);
        CCCallFunc*call=CCCallFunc::actionWithTarget(this,callfunc_selector(StartScene::turn));
        splashSprite->runAction(CCSequence::actions(delay1,fade,delay2,call,NULL));

		bRet = true;
	}while(0);
	return bRet;
}

void StartScene::turn(){
	//CCScene*scene=SpaceScene::node();
    CCDirector::sharedDirector()->replaceScene(CCTransitionFade::transitionWithDuration(1.1f,SplashScene::scene()));
}
