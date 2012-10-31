#ifndef __HELLOWORLD_SCENE_H__
#define __HELLOWORLD_SCENE_H__

#include "cocos2d.h"
#include "SimpleAudioEngine.h"
#include "Taco.h"
#include "InstructionsScene.h"
#include "OptionsMenu.h"
#include "Credits.h"
#include "SpaceScene.h"
//#include "vsbMenu.h"

class HelloWorld : public cocos2d::CCLayer
{
public:
    // Here's a difference. Method 'init' in cocos2d-x returns bool, instead of returning 'id' in cocos2d-iphone
    virtual bool init();  

    // there's no 'id' in cpp, so we recommand to return the exactly class pointer
    static cocos2d::CCScene* scene();
    
	//Taco Scene
	//cocos2d::CCScene *tacoScene = Taco::scene();
	

    // a selector callback
    void menuStartCall(CCObject* pSender);
	void menuInstrCall(CCObject* pSender);
	void menuOptCall(CCObject* pSender);
	void menuCreditCall(CCObject* pSender);
	void menuExitCall(CCObject* pSender);
	void turn();

    // implement the "static node()" method manually
    CREATE_FUNC(HelloWorld);
};



#endif  // __HELLOWORLD_SCENE_H__
