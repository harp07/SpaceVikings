#include "HelloWorldScene.h"
#include <stdio.h>
#define SCALE 0.75f
#define X_OFFSET 0
#define Y_OFFSET 10

using namespace cocos2d;

CCScene* HelloWorld::scene()
{
    CCScene * scene = NULL;
    do 
    {
        // 'scene' is an autorelease object
        scene = CCScene::create();
        CC_BREAK_IF(! scene);

        // 'layer' is an autorelease object
        HelloWorld *layer = HelloWorld::create();
        CC_BREAK_IF(! layer);

        // add layer as a child to scene
        scene->addChild(layer);

		

    } while (0);

    // return the scene
    return scene;
}

// on "init" you need to initialize your instance
bool HelloWorld::init()
{
    bool bRet = false;
    do 
    {
        //////////////////////////////////////////////////////////////////////////
        // super init first
        //////////////////////////////////////////////////////////////////////////

        CC_BREAK_IF(! CCLayer::init());

        //////////////////////////////////////////////////////////////////////////
        // add your codes below...
        //////////////////////////////////////////////////////////////////////////

        // 1. Add a menu item with "X" image, which is clicked to quit the program.

		CCSize winSize = CCDirector::sharedDirector()->getWinSize();

		CCSprite *bg_menu = CCSprite::create("menu.png", CCRectMake(0,0,winSize.width,winSize.height));
		bg_menu->setPosition(ccp(winSize.width/2, winSize.height/2));
		this->addChild(bg_menu);

		CCSprite *title = CCSprite::spriteWithFile("title.png", CCRectMake(0,0,900,	300));
		title->setScale(0.5f);
		title->setPosition(ccp(winSize.width/2,winSize.height - title->getContentSize().height/5));
		this->addChild(title);

        // Create a "close" menu item with close icon, it's an auto release object.
        CCMenuItemImage *menu_Start = CCMenuItemImage::create(
            "start.png",
            "start.png",
            this,
            menu_selector(HelloWorld::menuStartCall));
        CC_BREAK_IF(! menu_Start);

        // Place the menu item bottom-right conner.
		menu_Start->setPosition(ccp(X_OFFSET,winSize.height/1.25 - menu_Start->getContentSize().height/2));
		menu_Start->setScale(SCALE);

		CCMenuItemImage *menu_Instruct = CCMenuItemImage::create("instructions.png",
            "instructions.png",
            this,
            menu_selector(HelloWorld::menuInstrCall));
        CC_BREAK_IF(! menu_Instruct);

		menu_Instruct->setPosition(ccp(X_OFFSET, menu_Start->getPosition().y - Y_OFFSET - menu_Instruct->getContentSize().height/2));
		menu_Instruct->setScale(SCALE);

		CCMenuItemImage *menu_Options = CCMenuItemImage::create("options.png",
            "options.png",
            this,
            menu_selector(HelloWorld::menuOptCall));
        CC_BREAK_IF(! menu_Options);

		menu_Options->setPosition(ccp(X_OFFSET, menu_Instruct->getPosition().y - Y_OFFSET - menu_Options->getContentSize().height/2));
		menu_Options->setScale(SCALE);

		CCMenuItemImage *menu_Credits = CCMenuItemImage::create("credits.png",
            "credits.png",
            this,
            menu_selector(HelloWorld::menuCreditCall));
        CC_BREAK_IF(! menu_Credits);

		menu_Credits->setPosition(ccp(X_OFFSET,menu_Options->getPosition().y - Y_OFFSET - menu_Credits->getContentSize().height/2));
		menu_Credits->setScale(SCALE);

		CCMenuItemImage *menu_Exit = CCMenuItemImage::create("exit.png",
            "exit.png",
            this,
            menu_selector(HelloWorld::menuExitCall));
        CC_BREAK_IF(! menu_Exit);

		menu_Exit->setPosition(ccp(X_OFFSET,menu_Credits->getPosition().y - Y_OFFSET - menu_Exit->getContentSize().height/2));
		menu_Exit->setScale(SCALE);

        // Create a menu with the "close" menu item, it's an auto release object.
        CCMenu* menu = CCMenu::create(menu_Start, menu_Options, menu_Instruct, menu_Credits, menu_Exit, NULL);
		menu->setPosition(ccp(winSize.width/2, 0));
        CC_BREAK_IF(! menu);

        // Add the menu to HelloWorld layer as a child layer.
        this->addChild(menu, 1);

        bRet = true;
    } while (0);

    return bRet;
}

void HelloWorld::menuStartCall(CCObject* pSender)
{
	CCSize winSize = CCDirector::sharedDirector()->getWinSize();
	CocosDenshion::SimpleAudioEngine::sharedEngine()->playEffect(std::string(CCFileUtils::sharedFileUtils()->fullPathFromRelativePath("start_game.ogg")).c_str(),false);
	if(GameOptions::sharedGameOptions()->getToggleMusic() == true){
		CCLabelTTF *loadingLabel = CCLabelTTF::create("","Arial",40);
		loadingLabel->setPosition(ccp(winSize.width/2,winSize.height/2.5));
		loadingLabel->setString("Loading Music");
		this->addChild(loadingLabel);
		CCDelayTime*delay2=CCDelayTime::actionWithDuration(0.5f);
		CCCallFunc*call=CCCallFunc::actionWithTarget(this,callfunc_selector(HelloWorld::turn));
		runAction(CCSequence::actions(delay2,call,NULL));
	} else {
		CCDirector::sharedDirector()->replaceScene(SpaceScene::scene());
	}
}

void HelloWorld::turn(){
	CCDirector::sharedDirector()->replaceScene(SpaceScene::scene());
}

void HelloWorld::menuInstrCall(CCObject* pSender)
{
	CocosDenshion::SimpleAudioEngine::sharedEngine()->playEffect(std::string(CCFileUtils::sharedFileUtils()->fullPathFromRelativePath("menu_01.ogg")).c_str(),false);
	CCDirector::sharedDirector()->replaceScene(InstructionsScene::scene());
}
void HelloWorld::menuOptCall(CCObject* pSender)
{
	CocosDenshion::SimpleAudioEngine::sharedEngine()->playEffect(std::string(CCFileUtils::sharedFileUtils()->fullPathFromRelativePath("menu_01.ogg")).c_str(),false);
	CCDirector::sharedDirector()->replaceScene(OptionsMenu::scene());
}
void HelloWorld::menuCreditCall(CCObject* pSender)
{
	CocosDenshion::SimpleAudioEngine::sharedEngine()->playEffect(std::string(CCFileUtils::sharedFileUtils()->fullPathFromRelativePath("menu_01.ogg")).c_str(),false);
	CCDirector::sharedDirector()->replaceScene(Credits::scene());
}
void HelloWorld::menuExitCall(CCObject* pSender)
{
    // "close" menu item clicked
    CCDirector::sharedDirector()->end();
}

