#ifndef __UPGRADES_H__
#define __UPGRADES_H__

#include "cocos2d.h"

class Upgrades : public cocos2d::CCObject{
	CC_SYNTHESIZE(int, _curLives, CurLives);
	CC_SYNTHESIZE(int, _curDamage, CurDamage);
	CC_SYNTHESIZE(float, _attackSpeed, AttackSpeed);
	CC_SYNTHESIZE(float, _scoreMultiplier, ScoreMultiplier);
};

#endif