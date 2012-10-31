#include "Credits.h"

using namespace cocos2d;

CCScene* Credits::scene(){

	CCScene * scene = NULL;
	do{

		scene = CCScene::create();
        CC_BREAK_IF(! scene);

		Credits *layer = Credits::create();
		CC_BREAK_IF(!layer);

		

		scene->addChild(layer);

		

	}while(0);

	return scene;
}

bool Credits::init(){
	
	bool bRet = false;
	do{

		CC_BREAK_IF(! CCLayer::init());
		//
		CCSize winSize = CCDirector::sharedDirector()->getWinSize();

		CCSprite *bg_menu = CCSprite::create("credits2-3.png");
		bg_menu->setPosition(ccp(winSize.width/2, 0));
		bg_menu->setScaleX(winSize.width/bg_menu->getContentSize().width);
		bg_menu->setScaleY(winSize.height/bg_menu->getContentSize().height);
		this->addChild(bg_menu);
	/*	CCSprite *bg_menu2 = CCSprite::create("credits2-2.png", CCRectMake(0,0,800, 438));
		bg_menu2->setPosition(ccp(winSize.width/2, -1000));
		this->addChild(bg_menu2);*/

		CCMenuItemImage *backToMain = CCMenuItemImage::create(
			"menubutton.png",
            "menubutton.png",
			this,
			menu_selector(Credits::menu_Back));

		CC_BREAK_IF(!backToMain);

		CCMenu *backMenu = CCMenu::create(backToMain, NULL);
		CC_BREAK_IF(!backMenu);

		backMenu->setPosition(ccp(backToMain->getContentSize().width/2,backToMain->getContentSize().height/2));

		CCFiniteTimeAction *scroll = CCMoveTo::actionWithDuration(60.0f, ccp(winSize.width/2, 3000));

		bg_menu->runAction(CCSequence::actions(scroll, NULL));
		//bg_menu2->runAction(CCSequence::actions(scroll, NULL));

		this->addChild(backMenu);
		//
        bRet = true;
    } while (0);

    return bRet;

}

void Credits::menu_Back(CCObject* pSender)
{
	CCDirector::sharedDirector()->replaceScene(HelloWorld::scene());
}