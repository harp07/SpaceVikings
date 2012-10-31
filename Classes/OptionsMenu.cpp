#include "OptionsMenu.h"

using namespace cocos2d;

#define X_OFFSET 25

CCScene* OptionsMenu::scene(){

	CCScene * scene = NULL;
	do{

		scene = CCScene::create();
        CC_BREAK_IF(! scene);

		OptionsMenu *layer = OptionsMenu::create();
		CC_BREAK_IF(!layer);

		

		scene->addChild(layer);

		

	}while(0);

	return scene;
}

bool OptionsMenu::init(){
	
	bool bRet = false;
	do{
		setToggleMusic(1);
		setToggleSoundFX(1);

		CC_BREAK_IF(! CCLayer::init());

		CCSize winSize = CCDirector::sharedDirector()->getWinSize();
		
		CCSprite *bg_menu = CCSprite::spriteWithFile("menu.png", CCRectMake(0,0,winSize.width,winSize.height));
		bg_menu->setPosition(ccp(winSize.width/2 , winSize.height/2));
		this->addChild(bg_menu);

		CCSprite *dummyTitle1 = CCSprite::spriteWithFile("options.png", CCRectMake(0,0,636,106));
		dummyTitle1->setPosition(ccp(winSize.width/2 , winSize.height - dummyTitle1->getContentSize().height/2));
		this->addChild(dummyTitle1);
		
		CCSprite *dummyTitle3 = CCSprite::spriteWithFile("soundfx.png", CCRectMake(0,0,400,100));
		dummyTitle3->setPosition(ccp(250, winSize.height/2));
		this->addChild(dummyTitle3);

		CCSprite *dummyTitle2 = CCSprite::spriteWithFile("music.png", CCRectMake(0,0,400,100));
		dummyTitle2->setPosition(ccp(dummyTitle3->getPosition().x, dummyTitle3->getPosition().y + 10 + dummyTitle2->getContentSize().height));
		this->addChild(dummyTitle2);

		/*CCSprite *dummyTitle4 = CCSprite::spriteWithFile("PlaceHolderLabel.png", CCRectMake(0,0,200,50));
		dummyTitle4->setPosition(ccp(dummyTitle3->getPosition().x, dummyTitle3->getPosition().y - 10 - dummyTitle2->getContentSize().height));
		this->addChild(dummyTitle4);*/

		// dummyTitle2->getContentSize().y
		CCMenuItemImage *backToMain = CCMenuItemImage::create(
			"menubutton.png",
            "menubutton.png",
			this,
			menu_selector(OptionsMenu::menu_Back));
		CC_BREAK_IF(!backToMain);
		backToMain->setPosition(ccp(winSize.width/2, winSize.height/2.5));

		toggleMusicOn = CCMenuItemImage::create(
			"ON_button.png",
            "OFF_button.png",
			this,
			menu_selector(OptionsMenu::menu_ToggleMusic));
		CC_BREAK_IF(!toggleMusicOn);
		toggleMusicOn->setPosition(ccp(dummyTitle2->getPosition().x + dummyTitle2->getContentSize().width/2
				+ toggleMusicOn->getContentSize().width/2, dummyTitle2->getPosition().y));

		toggleMusicOff = CCMenuItemImage::create(
					"OFF_button.png",
		            "ON_button.png",
					this,
					menu_selector(OptionsMenu::menu_ToggleMusic));
		CC_BREAK_IF(!toggleMusicOff);
		toggleMusicOff->setPosition(ccp(dummyTitle2->getPosition().x + dummyTitle2->getContentSize().width/2
				+ toggleMusicOff->getContentSize().width/2, dummyTitle2->getPosition().y));
		toggleMusicOff->setVisible(false);

		toggleSoundFXOn = CCMenuItemImage::create(
			"ON_button.png",
            "OFF_button.png",
			this,
			menu_selector(OptionsMenu::menu_ToggleSoundFX));
		CC_BREAK_IF(!toggleSoundFXOn);
		toggleSoundFXOn->setPosition(ccp(dummyTitle3->getPosition().x + dummyTitle3->getContentSize().width/2
				+ toggleSoundFXOn->getContentSize().width/2, dummyTitle3->getPosition().y));

		toggleSoundFXOff = CCMenuItemImage::create(
					"OFF_button.png",
		            "ON_button.png",
					this,
					menu_selector(OptionsMenu::menu_ToggleSoundFX));
		CC_BREAK_IF(!toggleSoundFXOff);
		toggleSoundFXOff->setPosition(ccp(dummyTitle3->getPosition().x + dummyTitle3->getContentSize().width/2
				+ toggleSoundFXOff->getContentSize().width/2, dummyTitle3->getPosition().y));
		toggleSoundFXOff->setVisible(false);

		/*CCMenuItemImage *toggleAccellerometerOn = CCMenuItemImage::create(
			"OnUp.png",
            "onDown.png",
			this,
			menu_selector(OptionsMenu::menu_ToggleAccel));
		CC_BREAK_IF(!toggleMusicOn);
		toggleAccellerometerOn->setPosition(ccp(dummyTitle4->getPosition().x + dummyTitle4->getContentSize().width/2 + toggleAccellerometerOn->getContentSize().width/2, dummyTitle4->getPosition().y));*/


		CCMenu *backMenu = CCMenu::create(backToMain, toggleMusicOn, toggleMusicOff, toggleSoundFXOn, toggleSoundFXOff, NULL); //toggleAccellerometerOn, NULL);
		CC_BREAK_IF(!backMenu);

		backMenu->setPosition(ccp(0,0));

		this->addChild(backMenu);

        bRet = true;
    } while (0);

    return bRet;

}

void OptionsMenu::menu_Back(CCObject* pSender)
{
	CCDirector::sharedDirector()->replaceScene(HelloWorld::scene());
}

void OptionsMenu::menu_ToggleMusic(CCObject* pSender){
	if(this->getToggleMusic() == 0){
		this->setToggleMusic(1);
		GameOptions::sharedGameOptions()->setToggleMusic(true);
		std::cout << "Enable Music" << std::endl;
		toggleMusicOn->setVisible(true);
		toggleMusicOff->setVisible(false);
	}else{
		this->setToggleMusic(0);
		GameOptions::sharedGameOptions()->setToggleMusic(false);
		std::cout << "Disable Music" << std::endl;
		toggleMusicOn->setVisible(false);
		toggleMusicOff->setVisible(true);
	}
}
void OptionsMenu::menu_ToggleSoundFX(CCObject* pSender){
	if(this->getToggleSoundFX() == 0){
		this->setToggleSoundFX(1);
		GameOptions::sharedGameOptions()->setToggleSFX(true);
		std::cout << "Enable SFX" << std::endl;
		toggleSoundFXOn->setVisible(true);
		toggleSoundFXOff->setVisible(false);
	}else{
		this->setToggleSoundFX(0);
		GameOptions::sharedGameOptions()->setToggleSFX(false);
		std::cout << "Disable SFX" << std::endl;
		toggleSoundFXOn->setVisible(false);
		toggleSoundFXOff->setVisible(true);
	}
}
//void OptionsMenu::menu_ToggleAccel(CCObject* pSender){
//	if(!this->getToggleAccl()){
//		this->setToggleAccl(1);
//	}
//	else{
//		this->setToggleAccl(0);
//	}
//}
