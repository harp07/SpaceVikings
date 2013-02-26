#include "Taco.h"

using namespace cocos2d;

CCScene* Taco::scene(){

	CCScene * scene = NULL;
	do{

		scene = CCScene::create();
        CC_BREAK_IF(! scene);

		Taco *layer = Taco::create();
		CC_BREAK_IF(!layer);

		

		scene->addChild(layer);

		

	}while(0);

	return scene;
}

bool Taco::init(){
	
	bool bRet = false;
	do{

		CC_BREAK_IF(! CCLayer::init());
		//
		CCSize winSize = CCDirector::sharedDirector()->getWinSize();
		CCSprite *untitled = CCSprite::create("Untitled.png", CCRectMake(0,0,50,50));
		untitled->setPosition(ccp(untitled->getContentSize().width/2, winSize.height/2));
		this->addChild(untitled);

		CCMenuItemImage *backToMain = CCMenuItemImage::create(
			"dummyMenuUp.png",
            "dummyMenuDown.png",
			this,
			menu_selector(Taco::menu_Back));

		CC_BREAK_IF(!backToMain);

		CCMenu *backMenu = CCMenu::create(backToMain, NULL);
		CC_BREAK_IF(!backMenu);

		backMenu->setPosition(ccp(winSize.width/2, winSize.height/2));

		this->addChild(backMenu);
		//
        bRet = true;
    } while (0);

    return bRet;

}

void Taco::menu_Back(CCObject* pSender)
{
	if(GameOptions::sharedGameOptions()->getToggleSFX() == true){
		CocosDenshion::SimpleAudioEngine::sharedEngine()->playEffect(std::string(CCFileUtils::sharedFileUtils()->fullPathFromRelativePath("menu_01.ogg")).c_str(),false);
	}
	CCDirector::sharedDirector()->replaceScene(HelloWorld::scene());
}
