#ifndef _LEVEL_H_
#define _LEVEL_H_

#include "cocos2d.h"

class Level : public cocos2d::CCSprite {
public:
	//cocos2d::CCSize winSize;
	CC_SYNTHESIZE(int, _curLevel, CurLevel);
	CC_SYNTHESIZE(int, _lvlTime, LevelTime);
	CC_SYNTHESIZE(int, _curObjectives, CurObjectives);
	CC_SYNTHESIZE(bool, _bCompleted1, BCompleted1);
	CC_SYNTHESIZE(bool, _bCompleted2, BCompleted2);
	CC_SYNTHESIZE(bool, _bCompleted3, BCompleted3);

	bool init(int type);
};

#endif