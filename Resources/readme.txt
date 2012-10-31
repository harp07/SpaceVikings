#include "SimpleAudioEngine.h"

// cpp with cocos2d-x
// For gunfire
CocosDenshion::SimpleAudioEngine::sharedEngine()->playEffect(
"sound.ogg");

//in the init()
// cpp with cocos2d-x
// for the
CocosDenshion::SimpleAudioEngine::sharedEngine()->playBackgroundMusic(
"background-music.ogg", true);