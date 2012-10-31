#include "Projectile.h"
#include <stdio.h>

using namespace cocos2d;

void Projectile::init(int type){
	switch(type){
	case 1:
		CCSprite::initWithFile("fireball2.png",CCRectMake(0,0,15,15));
		break;
	case 2:
		CCSprite::initWithFile("toxinball2.png",CCRectMake(0,0,15,15));
		break;
	case 3:
		CCSprite::initWithFile("playerice2.png",CCRectMake(0,0,15,30));
		break;
	}
}