#ifndef _ENEMY_H_
#define _ENEMY_H_

#include "cocos2d.h"
#include "Projectile.h"

class Enemy : public cocos2d::CCSprite{
public:
	CC_SYNTHESIZE(int, _curHp, CurHp);
	CC_SYNTHESIZE(int, _curScore, CurScore);
	CC_SYNTHESIZE(int, _curWeapon, CurWeapon);
	CC_SYNTHESIZE(int, _minMoveDuration, MinMoveDuration);
	CC_SYNTHESIZE(int, _maxMoveDuration, MaxMoveDuration);

	void init(int type);
	void shoot(cocos2d::CCPoint Location, cocos2d::CCScene *sceneS);
	void spriteMoveFinished(CCNode* sender);
};
#endif