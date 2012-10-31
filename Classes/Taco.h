#ifndef __TACO_H__
#define __TACO_H__

#include "cocos2d.h"
#include "SimpleAudioEngine.h"
#include "HelloWorldScene.h"

class Taco : public cocos2d::CCLayer
{
public:


	virtual bool init();

	static cocos2d::CCScene* scene();

	CREATE_FUNC(Taco);
	
	void menu_Back(CCObject* pSender);

};

#endif //end __TACO_H__