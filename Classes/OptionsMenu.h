#ifndef __OPTIONS_MENU_H__
#define __OPTIONS_MENU_H__

#include "cocos2d.h"
#include "SimpleAudioEngine.h"
#include "HelloWorldScene.h"
#include "GameOptions.h"

class OptionsMenu : public cocos2d::CCLayer
{
public:
	virtual bool init();

	static cocos2d::CCScene* scene();

	CREATE_FUNC(OptionsMenu);
	
	cocos2d::CCMenuItemImage *toggleMusicOn;
	cocos2d::CCMenuItemImage *toggleMusicOff;
	cocos2d::CCMenuItemImage *toggleSoundFXOn;
	cocos2d::CCMenuItemImage *toggleSoundFXOff;
	cocos2d::CCMenuItemImage *toggleParticlesOn;
	cocos2d::CCMenuItemImage *toggleParticlesOff;


	void menu_Back(CCObject* pSender);
	void menu_ToggleMusic(CCObject* pSender);
	void menu_ToggleSoundFX(CCObject* pSender);
	void menu_ToggleParticles(CCObject* pSender);

	CC_SYNTHESIZE(int, _toggleMusic, ToggleMusic);
	CC_SYNTHESIZE(int, _toggleSoundFX, ToggleSoundFX);
	CC_SYNTHESIZE(int, _toggleParticles, ToggleParticles);

};

#endif //end __OPTIONS_MENU_H__
