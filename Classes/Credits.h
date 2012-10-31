#ifndef __CREDITS_H__
#define __CREDITS_H__

#include "cocos2d.h"
#include "SimpleAudioEngine.h"
#include "HelloWorldScene.h"

class Credits : public cocos2d::CCLayer
{
public:


	virtual bool init();

	static cocos2d::CCScene* scene();

	CREATE_FUNC(Credits);
	
	void menu_Back(CCObject* pSender);

};

#endif //end __TACO_H__