#include "GameOptions.h"

using namespace cocos2d;

GameOptions* GameOptions::m_Singleton = NULL;

GameOptions::GameOptions(){

}
GameOptions* GameOptions::sharedGameOptions(){
	if(NULL == m_Singleton){
		m_Singleton = new GameOptions();
	}
	return m_Singleton;
}


void GameOptions::setToggleMusic(bool toggle){
	toggleMusic = toggle;
}

bool GameOptions::getToggleMusic(){
	return toggleMusic;
}
void GameOptions::setToggleSFX(bool toggle){
	toggleSFX = toggle;
}
bool GameOptions::getToggleSFX(){
	return toggleSFX;
}
