#include "InstructionsScene.h"

using namespace cocos2d;



CCScene* InstructionsScene::scene(){

	CCScene * scene = NULL;
	do{

		scene = CCScene::create();
        CC_BREAK_IF(! scene);

		InstructionsScene *layer = InstructionsScene::create();
		CC_BREAK_IF(!layer);

		

		scene->addChild(layer);

		

	}while(0);

	return scene;
}

bool InstructionsScene::init(){
	
	bool bRet = false;
	do{
		counter = 0;

		CC_BREAK_IF(! CCLayer::init());

		winSize = CCDirector::sharedDirector()->getWinSize();
		
		instruct1 = CCSprite::create("tut1.png");
		instruct1->setScaleX(winSize.width/instruct1->getContentSize().width);
		instruct1->setScaleY(winSize.height/instruct1->getContentSize().height/1.125);
		instruct1->setPosition(ccp(winSize.width/2,(winSize.height/2) * 1.125));
		this->addChild(instruct1, 4);

		instruct2 = CCSprite::create("tut2.png");
		instruct2->setScaleX(winSize.width/instruct2->getContentSize().width);
		instruct2->setScaleY(winSize.height/instruct2->getContentSize().height/1.125);
		instruct2->setPosition(ccp(winSize.width/2,(winSize.height/2) * 1.125));
		this->addChild(instruct2, 3);

		instruct3 = CCSprite::create("tut3.png");
		instruct3->setScaleX(winSize.width/instruct1->getContentSize().width);
		instruct3->setScaleY(winSize.height/instruct1->getContentSize().height/1.125);
		instruct3->setPosition(ccp(winSize.width/2,(winSize.height/2) * 1.125));
		this->addChild(instruct3, 2);

		instruct4 = CCSprite::create("tut4.png");
		instruct4->setScaleX(winSize.width/instruct4->getContentSize().width);
		instruct4->setScaleY(winSize.height/instruct4->getContentSize().height/1.125);
		instruct4->setPosition(ccp(winSize.width/2,(winSize.height/2) * 1.125));
		this->addChild(instruct4, 1);



		CCMenuItemImage *backToMenu = CCMenuItemImage::create("menubutton.png", "menubutton.png", this, menu_selector(InstructionsScene::menuMenuCall));
		backToMenu->setPosition(ccp(backToMenu->getContentSize().width/2,backToMenu->getContentSize().height/2));

		CCMenuItemImage *nextInstr = CCMenuItemImage::create("right.png", "right.png", this, menu_selector(InstructionsScene::menuFwdCall));
		nextInstr->setScaleX(winSize.width/nextInstr->getContentSize().width/4);
		nextInstr->setScaleY(winSize.height/nextInstr->getContentSize().height/8);
		nextInstr->setPosition(ccp(winSize.width - nextInstr->getContentSize().width/3.5, backToMenu->getPosition().y*2.05));

		CCMenuItemImage *prevInstr = CCMenuItemImage::create("left.png", "left.png", this, menu_selector(InstructionsScene::menuPrevCall));
		prevInstr->setScaleX(winSize.width/prevInstr->getContentSize().width/4);
		prevInstr->setScaleY(winSize.height/prevInstr->getContentSize().height/8);
		prevInstr->setPosition(ccp(nextInstr->getPosition().x -prevInstr->getContentSize().width/2, backToMenu->getPosition().y*2.05));

		CCMenu *instrMenu = CCMenu::create(backToMenu, nextInstr, prevInstr, NULL);
		instrMenu->setPosition(ccp(0,0));
		this->addChild(instrMenu, 7); 

        bRet = true;
    } while (0);

    return bRet;

}

void InstructionsScene::menuMenuCall(CCObject* pSender){
	if(GameOptions::sharedGameOptions()->getToggleSFX() == true){
		CocosDenshion::SimpleAudioEngine::sharedEngine()->playEffect(std::string(CCFileUtils::sharedFileUtils()->fullPathFromRelativePath("menu_01.ogg")).c_str(),false);
	}
	CCDirector::sharedDirector()->replaceScene(HelloWorld::scene());
}

void InstructionsScene::menuFwdCall(CCObject* pSender){
	if(GameOptions::sharedGameOptions()->getToggleSFX() == true){
		CocosDenshion::SimpleAudioEngine::sharedEngine()->playEffect(std::string(CCFileUtils::sharedFileUtils()->fullPathFromRelativePath("menu_01.ogg")).c_str(),false);
	}
	if(counter == 0){
		counter++;
		instruct1->setPosition(ccp(instruct1->getContentSize().width * 2, instruct1->getContentSize().height * 2));
	}
	else if(counter == 1){
		counter++;
		instruct2->setPosition(ccp(instruct1->getContentSize().width * 2, instruct1->getContentSize().height * 2));
	}
	else if(counter == 2){
		counter++;
		instruct3->setPosition(ccp(instruct1->getContentSize().width * 2, instruct1->getContentSize().height * 2));
	}
	else if(counter == 3){
		//instruct4->setPosition(ccp(1000,1000));
	}
}
void InstructionsScene::menuPrevCall(CCObject* pSender){
	if(GameOptions::sharedGameOptions()->getToggleSFX() == true){
		CocosDenshion::SimpleAudioEngine::sharedEngine()->playEffect(std::string(CCFileUtils::sharedFileUtils()->fullPathFromRelativePath("menu_01.ogg")).c_str(),false);
	}
	instruct1->setPosition(ccp(winSize.width/2,(winSize.height/2) * 1.125));
	instruct2->setPosition(ccp(winSize.width/2,(winSize.height/2) * 1.125));
	instruct3->setPosition(ccp(winSize.width/2,(winSize.height/2) * 1.125));

	counter = 0;
}
