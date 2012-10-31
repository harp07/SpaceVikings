#include "Player.h"

using namespace cocos2d;

void Player::init(int type){
	switch(type){
	case 1:
		CCSprite::initWithFile("playership_v3.png",CCRectMake(0,0,120,127));
		this->setCurWeapon(1);
		this->setCurDamage(1);
		this->setScoreMultiplier(1.0);
		this->setAttackSpeed(1.0);
		this->setCurLives(10);
		this->setScale(1.0);
		break;
	case 2:
		CCSprite::initWithFile("deathstarship.png",CCRectMake(0,0,150,107));
		this->setCurWeapon(3);
		//this->setCurDamage(2);
		//this->setCurLives(ups.getCurLives());
		////this->setAttackSpeed(ups->getAttackSpeed());
		//this->setScoreMultiplier(ups.getScoreMultiplier());
		break;
	}
}