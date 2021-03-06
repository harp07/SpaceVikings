#ifndef __GAME_OPTIONS_H__
#define __GAME_OPTIONS_H__

#include "cocos2d.h"

class GameOptions {
private:
	GameOptions();
	static GameOptions* m_Singleton;
public:
	static GameOptions* sharedGameOptions();
	bool toggleMusic;
	bool toggleSFX;
	bool toggleParts;
	void setToggleMusic(bool toggle);
	bool getToggleMusic();
	void setToggleSFX(bool toggle);
	bool getToggleSFX();
	void setToggleParts(bool toggle);
	bool getToggleParts();
};


#endif
