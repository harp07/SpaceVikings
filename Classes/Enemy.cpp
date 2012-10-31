#include "Enemy.h"

using namespace cocos2d;

void Enemy::init(int type){
	switch(type){
	case 1:
		CCSprite::initWithFile("Enemy1saucer2.png",CCRectMake(0,0,90,59));
		this->setCurHp(2);
		this->setMinMoveDuration(5);
		this->setMaxMoveDuration(7);
		this->setCurScore(1);
		this->setCurWeapon(1);
		break;
	case 2:
		CCSprite::initWithFile("saucer2.png",CCRectMake(0,0,90,60));
		this->setCurHp(3);
		this->setMinMoveDuration(10);
		this->setMaxMoveDuration(14);
		this->setScale(1.5);
		this->setCurScore(2);
		this->setCurWeapon(2);
		break;
	}
}