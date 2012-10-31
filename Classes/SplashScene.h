#ifndef __SPLASH_SCENE_H__
#define __SPLASH_SCENE_H__

#include "cocos2d.h"

class SplashScene : public cocos2d::CCLayer{
public:
	virtual bool init();

	static cocos2d::CCScene* scene();

	CREATE_FUNC(SplashScene);

	void ccTouchesEnded(cocos2d::CCSet* touches, cocos2d::CCEvent* event);
};

#endif