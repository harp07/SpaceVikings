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
		
		CCSprite *bg_menu = CCSprite::create("menu.png", CCRectMake(0,0,winSize.width,winSize.height));
		bg_menu->setPosition(ccp(winSize.width/2,winSize.height/2));
		bg_menu->setScaleX(winSize.width/bg_menu->getContentSize().width);
		bg_menu->setScaleY(winSize.height/bg_menu->getContentSize().height);
		this->addChild(bg_menu);

		CCSprite *optionsTitle = CCSprite::create("options.png", CCRectMake(0,0,636,106));
		optionsTitle->setPosition(ccp(winSize.width/2 , winSize.height - optionsTitle->getContentSize().height/2));
		this->addChild(optionsTitle);
		
		CCSprite *soundFxTitle = CCSprite::create("soundfx.png", CCRectMake(0,0,400,100));
		soundFxTitle->setPosition(ccp(250, winSize.height/2));
		this->addChild(soundFxTitle);

		CCSprite *musicTitle = CCSprite::create("music.png", CCRectMake(0,0,400,100));
		musicTitle->setPosition(ccp(soundFxTitle->getPosition().x, soundFxTitle->getPosition().y + 18 + musicTitle->getContentSize().height));
		this->addChild(musicTitle);

		//CCSprite *particlesTitle = CCSprite::create("soundfx.png", CCRectMake(0,0,400,100));
		CCLabelTTF *particlesTitle = CCLabelTTF::create("Particles     ","Arial",60);
		particlesTitle->setPosition(ccp(musicTitle->getPosition().x + 45, musicTitle->getPosition().y + 50 + particlesTitle->getContentSize().height));
		this->addChild(particlesTitle);

		/*CCSprite *particlesTitle = CCSprite::spriteWithFile("PlaceHolderLabel.png", CCRectMake(0,0,200,50));
		particlesTitle->setPosition(ccp(soundFxTitle->getPosition().x, soundFxTitle->getPosition().y - 10 - musicTitle->getContentSize().height));
		this->addChild(particlesTitle);*/

		// musicTitle->getContentSize().y
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
		toggleMusicOn->setPosition(ccp(musicTitle->getPosition().x + musicTitle->getContentSize().width/2
				+ toggleMusicOn->getContentSize().width/2, musicTitle->getPosition().y));
		if(GameOptions::sharedGameOptions()->getToggleMusic() == true){
			toggleMusicOn->setVisible(true);
		}else{
			toggleMusicOn->setVisible(false);
		}

		toggleMusicOff = CCMenuItemImage::create(
					"OFF_button.png",
		            "ON_button.png",
					this,
					menu_selector(OptionsMenu::menu_ToggleMusic));
		CC_BREAK_IF(!toggleMusicOff);
		toggleMusicOff->setPosition(ccp(musicTitle->getPosition().x + musicTitle->getContentSize().width/2
				+ toggleMusicOff->getContentSize().width/2, musicTitle->getPosition().y));
		if(GameOptions::sharedGameOptions()->getToggleMusic() == true){
			toggleMusicOff->setVisible(false);
		} else {
			toggleMusicOff->setVisible(true);
		}

		toggleSoundFXOn = CCMenuItemImage::create(
			"ON_button.png",
            "OFF_button.png",
			this,
			menu_selector(OptionsMenu::menu_ToggleSoundFX));
		CC_BREAK_IF(!toggleSoundFXOn);
		toggleSoundFXOn->setPosition(ccp(soundFxTitle->getPosition().x + soundFxTitle->getContentSize().width/2
				+ toggleSoundFXOn->getContentSize().width/2, soundFxTitle->getPosition().y));
		if(GameOptions::sharedGameOptions()->getToggleSFX() == true){
			toggleSoundFXOn->setVisible(true);
		} else {
			toggleSoundFXOn->setVisible(false);
		}

		toggleSoundFXOff = CCMenuItemImage::create(
					"OFF_button.png",
		            "ON_button.png",
					this,
					menu_selector(OptionsMenu::menu_ToggleSoundFX));
		CC_BREAK_IF(!toggleSoundFXOff);
		toggleSoundFXOff->setPosition(ccp(soundFxTitle->getPosition().x + soundFxTitle->getContentSize().width/2
				+ toggleSoundFXOff->getContentSize().width/2, soundFxTitle->getPosition().y));
		if(GameOptions::sharedGameOptions()->getToggleSFX() == true){
			toggleSoundFXOff->setVisible(false);
		} else {
			toggleSoundFXOff->setVisible(true);
		}

		toggleParticlesOn = CCMenuItemImage::create(
					"ON_button.png",
		            "OFF_button.png",
					this,
					menu_selector(OptionsMenu::menu_ToggleParticles));
				CC_BREAK_IF(!toggleParticlesOn);
				toggleParticlesOn->setPosition(ccp(particlesTitle->getPosition().x + particlesTitle->getContentSize().width/2
						+ toggleParticlesOn->getContentSize().width/2, particlesTitle->getPosition().y));
				if(GameOptions::sharedGameOptions()->getToggleParts() == true){
					toggleParticlesOn->setVisible(true);
				} else {
					toggleParticlesOn->setVisible(false);
				}

		toggleParticlesOff = CCMenuItemImage::create(
					"OFF_button.png",
		            "ON_button.png",
					this,
					menu_selector(OptionsMenu::menu_ToggleParticles));
		CC_BREAK_IF(!toggleParticlesOff);
		toggleParticlesOff->setPosition(ccp(particlesTitle->getPosition().x + particlesTitle->getContentSize().width/2
				+ toggleParticlesOff->getContentSize().width/2, particlesTitle->getPosition().y));
		if(GameOptions::sharedGameOptions()->getToggleParts() == true){
			toggleParticlesOff->setVisible(false);
		} else {
			toggleParticlesOff->setVisible(true);
		}

		/*CCMenuItemImage *toggleAccellerometerOn = CCMenuItemImage::create(
			"OnUp.png",
            "onDown.png",
			this,
			menu_selector(OptionsMenu::menu_ToggleAccel));
		CC_BREAK_IF(!toggleMusicOn);
		toggleAccellerometerOn->setPosition(ccp(particlesTitle->getPosition().x + particlesTitle->getContentSize().width/2 + toggleAccellerometerOn->getContentSize().width/2, particlesTitle->getPosition().y));*/


		CCMenu *backMenu = CCMenu::create(backToMain, toggleMusicOn, toggleMusicOff, toggleSoundFXOn, toggleSoundFXOff, toggleParticlesOn, toggleParticlesOff, NULL); //toggleAccellerometerOn, NULL);
		CC_BREAK_IF(!backMenu);

		backMenu->setPosition(ccp(0,0));

		this->addChild(backMenu);

        bRet = true;
    } while (0);

    return bRet;

}

void OptionsMenu::menu_Back(CCObject* pSender)
{
	if(GameOptions::sharedGameOptions()->getToggleSFX() == true){
		CocosDenshion::SimpleAudioEngine::sharedEngine()->playEffect(std::string(CCFileUtils::sharedFileUtils()->fullPathFromRelativePath("menu_01.ogg")).c_str(),false);
	}
	CCDirector::sharedDirector()->replaceScene(HelloWorld::scene());
}

void OptionsMenu::menu_ToggleMusic(CCObject* pSender){
	if(GameOptions::sharedGameOptions()->getToggleSFX() == true){
		CocosDenshion::SimpleAudioEngine::sharedEngine()->playEffect(std::string(CCFileUtils::sharedFileUtils()->fullPathFromRelativePath("menu_01.ogg")).c_str(),false);
	}
	if(this->getToggleMusic() == 0){
		this->setToggleMusic(1);
		GameOptions::sharedGameOptions()->setToggleMusic(true);
		//std::cout << "Enable Music" << std::endl;
		toggleMusicOn->setVisible(true);
		toggleMusicOff->setVisible(false);
	}else{
		this->setToggleMusic(0);
		GameOptions::sharedGameOptions()->setToggleMusic(false);
		//std::cout << "Disable Music" << std::endl;
		toggleMusicOn->setVisible(false);
		toggleMusicOff->setVisible(true);
	}
}
void OptionsMenu::menu_ToggleSoundFX(CCObject* pSender){
	if(GameOptions::sharedGameOptions()->getToggleSFX() == true){
		CocosDenshion::SimpleAudioEngine::sharedEngine()->playEffect(std::string(CCFileUtils::sharedFileUtils()->fullPathFromRelativePath("menu_01.ogg")).c_str(),false);
	}
	if(this->getToggleSoundFX() == 0){
		this->setToggleSoundFX(1);
		GameOptions::sharedGameOptions()->setToggleSFX(true);
		//std::cout << "Enable SFX" << std::endl;
		toggleSoundFXOn->setVisible(true);
		toggleSoundFXOff->setVisible(false);
	}else{
		this->setToggleSoundFX(0);
		GameOptions::sharedGameOptions()->setToggleSFX(false);
		//std::cout << "Disable SFX" << std::endl;
		toggleSoundFXOn->setVisible(false);
		toggleSoundFXOff->setVisible(true);
	}
}
void OptionsMenu::menu_ToggleParticles(CCObject* pSender){
	if(GameOptions::sharedGameOptions()->getToggleSFX() == true){
		CocosDenshion::SimpleAudioEngine::sharedEngine()->playEffect(std::string(CCFileUtils::sharedFileUtils()->fullPathFromRelativePath("menu_01.ogg")).c_str(),false);
	}
	if(this->getToggleParticles() == 0){
		this->setToggleParticles(1);
		GameOptions::sharedGameOptions()->setToggleParts(true);
		//std::cout << "Enable SFX" << std::endl;
		toggleParticlesOn->setVisible(true);
		toggleParticlesOff->setVisible(false);
	}else{
		this->setToggleParticles(0);
		GameOptions::sharedGameOptions()->setToggleParts(false);
		//std::cout << "Disable SFX" << std::endl;
		toggleParticlesOn->setVisible(false);
		toggleParticlesOff->setVisible(true);
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
