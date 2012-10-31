#ifndef _PLAYER_H_
#define _PLAYER_H_

#include "cocos2d.h"
#include "Projectile.h"
#include "Upgrades.h"

class Player : public cocos2d::CCSprite{
public:
	CC_SYNTHESIZE(int, _curLives, CurLives);
	CC_SYNTHESIZE(int, _curDamage, CurDamage);
	CC_SYNTHESIZE(int, _curWeapon, CurWeapon);
	CC_SYNTHESIZE(float, _attackSpeed, AttackSpeed);
	CC_SYNTHESIZE(float, _scoreMultiplier, ScoreMultiplier);
	CC_SYNTHESIZE(int, _rageMeter, RageMeter);

	void init(int type);
	Upgrades ups;
};

#endif