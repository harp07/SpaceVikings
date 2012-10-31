#ifndef _PROJECTILE_H_
#define _PROJECTILE_H_

#include "cocos2d.h"
#include "Player.h"

class Projectile : public cocos2d::CCSprite{
public:
	void init(int type);
};

#endif