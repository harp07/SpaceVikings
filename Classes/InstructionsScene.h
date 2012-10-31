#ifndef __INSTRUCTIONS_SCENE_H__
#define __INSTRUCTIONS_SCENE_H__

#include "cocos2d.h"
#include "SimpleAudioEngine.h"
#include "HelloWorldScene.h"

class InstructionsScene : public cocos2d::CCLayer
{
public:


	virtual bool init();

	static cocos2d::CCScene* scene();

	CREATE_FUNC(InstructionsScene);

	

	void menuMenuCall(CCObject* pSender);
	void menuFwdCall(CCObject* pSender);
	void menuPrevCall(CCObject* pSender);

	cocos2d::CCSize winSize;

	cocos2d::CCSprite *instruct1;
	cocos2d::CCSprite *instruct2;
	cocos2d::CCSprite *instruct3;
	cocos2d::CCSprite *instruct4;

	int counter;
};

#endif //end __INSTRUCTIONS_SCENE_H__