#include "Level.h"

using namespace cocos2d;

bool Level::init(int type){
	switch(type){
	case 1:
		CCSprite::initWithFile("Level1Background2.png");
		this->setBCompleted1(false);
		this->setLevelTime(60);
		break;
	case 2:
		CCSprite::initWithFile("ice2.png");
		this->setBCompleted2(false);
		this->setLevelTime(60);
		break;
	}
	return true;
}
