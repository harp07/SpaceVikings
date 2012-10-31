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
		
		instruct1 = CCSprite::create("tut1.png", CCRectMake(0,0,winSize.width,winSize.height));
		instruct1->setPosition(ccp(winSize.width/2,winSize.height/2));
		this->addChild(instruct1, 4);

		instruct2 = CCSprite::create("tut2.png", CCRectMake(0,0,winSize.width,winSize.height));
		instruct2->setPosition(ccp(winSize.width/2,winSize.height/2));
		this->addChild(instruct2, 3);

		instruct3 = CCSprite::create("tut3.png", CCRectMake(0,0,winSize.width,winSize.height));
		instruct3->setPosition(ccp(winSize.width/2,winSize.height/2));
		this->addChild(instruct3, 2);

		instruct4 = CCSprite::create("tut4.png", CCRectMake(0,0,winSize.width,winSize.height));
		instruct4->setPosition(ccp(winSize.width/2,winSize.height/2));
		this->addChild(instruct4, 1);



		CCMenuItemImage *backToMenu = CCMenuItemImage::create("menubutton.png", "menubutton.png", this, menu_selector(InstructionsScene::menuMenuCall));
		backToMenu->setPosition(ccp(backToMenu->getContentSize().width/2,backToMenu->getContentSize().height/2));

		CCMenuItemImage *nextInstr = CCMenuItemImage::create("right.png", "right.png", this, menu_selector(InstructionsScene::menuFwdCall));
		nextInstr->setPosition(ccp(winSize.width - nextInstr->getContentSize().width / 2, nextInstr->getContentSize().height/2));

		CCMenuItemImage *prevInstr = CCMenuItemImage::create("left.png", "left.png", this, menu_selector(InstructionsScene::menuPrevCall));
		prevInstr->setPosition(ccp(winSize.width - (prevInstr->getContentSize().width / 2) - (prevInstr->getContentSize().width), prevInstr->getContentSize().height/2));

		CCMenu *instrMenu = CCMenu::create(backToMenu, nextInstr, prevInstr, NULL);
		instrMenu->setPosition(ccp(0,0));

		this->addChild(instrMenu, 7); 



        bRet = true;
    } while (0);

    return bRet;

}

void InstructionsScene::menuMenuCall(CCObject* pSender){
	CCDirector::sharedDirector()->replaceScene(HelloWorld::scene());
}

void InstructionsScene::menuFwdCall(CCObject* pSender){
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

		instruct1->setPosition(ccp(winSize.width/2,winSize.height/2));

		instruct2->setPosition(ccp(winSize.width/2,winSize.height/2));

		instruct3->setPosition(ccp(winSize.width/2,winSize.height/2));

		counter = 0;
}