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

		float scaleX, scaleY;

		CCSprite *bg_menu = CCSprite::create("menu.png");
		bg_menu->setPosition(ccp(winSize.width/2,winSize.height/2));
		bg_menu->setScaleX(winSize.width/bg_menu->getContentSize().width);
		bg_menu->setScaleY(winSize.height/bg_menu->getContentSize().height);
		this->addChild(bg_menu);

		CCSprite *title = CCSprite::create("title.png");
		title->setScaleX(winSize.width/title->getContentSize().width/1);
		title->setScaleY(winSize.height/title->getContentSize().height/4);
		title->setPosition(ccp(winSize.width/2,winSize.height - title->getContentSize().height/3));
		//title->setPosition(ccp(winSize.width/2,winSize.height - title->getContentSize().height/5));
		this->addChild(title);

        // Create a "close" menu item with close icon, it's an auto release object.
        menu_Start = CCMenuItemImage::create(
            "start.png",
            "startA.png",
            this,
            menu_selector(HelloWorld::menuStartCall));
        CC_BREAK_IF(! menu_Start);

        menu_Start->setScaleX(winSize.width/menu_Start->getContentSize().width/1);
        menu_Start->setScaleY(winSize.height/menu_Start->getContentSize().height/8);

        // Place the menu item bottom-right conner.
        //menu_Start->setScale(SCALE);
        menu_Start->setPosition(ccp(0,0));

		CCMenuItemImage *menu_Instruct = CCMenuItemImage::create("instructions.png",
            "instructionsA.png",
            this,
            menu_selector(HelloWorld::menuInstrCall));
        CC_BREAK_IF(! menu_Instruct);

        menu_Instruct->setPosition(ccp(X_OFFSET, menu_Start->getPosition().y - menu_Instruct->getContentSize().height * 1.119) );
        menu_Instruct->setScaleX(winSize.width/menu_Instruct->getContentSize().width/1);
        menu_Instruct->setScaleY(winSize.height/menu_Instruct->getContentSize().height/8);

		CCMenuItemImage *menu_Options = CCMenuItemImage::create("options.png",
            "optionsA.png",
            this,
            menu_selector(HelloWorld::menuOptCall));
        CC_BREAK_IF(! menu_Options);

        menu_Options->setPosition(ccp(X_OFFSET, menu_Instruct->getPosition().y - menu_Options->getContentSize().height * 1.119));
        menu_Options->setScaleX(winSize.width/menu_Options->getContentSize().width/1);
        menu_Options->setScaleY(winSize.height/menu_Options->getContentSize().height/8);

		CCMenuItemImage *menu_Credits = CCMenuItemImage::create("credits.png",
            "creditsA.png",
            this,
            menu_selector(HelloWorld::menuCreditCall));
        CC_BREAK_IF(! menu_Credits);

        menu_Credits->setPosition(ccp(X_OFFSET, menu_Options->getPosition().y - menu_Credits->getContentSize().height * 1.119));
        menu_Credits->setScaleX(winSize.width/menu_Credits->getContentSize().width/1);
        menu_Credits->setScaleY(winSize.height/menu_Credits->getContentSize().height/8);

		CCMenuItemImage *menu_Exit = CCMenuItemImage::create("exit.png",
            "exitA.png",
            this,
            menu_selector(HelloWorld::menuExitCall));
        CC_BREAK_IF(! menu_Exit);

        menu_Exit->setPosition(ccp(X_OFFSET,menu_Credits->getPosition().y - menu_Exit->getContentSize().height * 1.119));
        menu_Exit->setScaleX(winSize.width/menu_Exit->getContentSize().width/1);
        menu_Exit->setScaleY(winSize.height/menu_Exit->getContentSize().height/8);

        // Create a menu with the "close" menu item, it's an auto release object.
        selectMenu = CCMenu::create(menu_Start, menu_Options, menu_Instruct, menu_Credits, menu_Exit, NULL);
        selectMenu->setPosition(ccp(winSize.width/2, title->getPosition().y - menu_Start->getContentSize().height*1.333));
        CC_BREAK_IF(! selectMenu);

        // Add the menu to HelloWorld layer as a child layer.
        this->addChild(selectMenu, 1);

        bRet = true;

        /*CCLabelTTF *_disclaimerLabel = CCLabelTTF::create("Beta: Game was put together in 48 hours, and a week of light polish.","Arial",20);
        _disclaimerLabel->setPosition(ccp(winSize.width/2, 200));
        ccColor3B colourTest = {200,0,0};
        _disclaimerLabel->setColor(colourTest);
        this->addChild(_disclaimerLabel);*/
    } while (0);

    return bRet;
}

void HelloWorld::menuStartCall(CCObject* pSender)
{
	CCSize winSize = CCDirector::sharedDirector()->getWinSize();
	if(GameOptions::sharedGameOptions()->getToggleSFX() == true){
		CocosDenshion::SimpleAudioEngine::sharedEngine()->playEffect(std::string(CCFileUtils::sharedFileUtils()->fullPathFromRelativePath("start_game.ogg")).c_str(),false);
	}
	if(GameOptions::sharedGameOptions()->getToggleMusic() == true){
		CCSprite *loadingImage = CCSprite::create("musicLoad.png");
		loadingImage->setPosition(ccp(winSize.width/2, loadingImage->getContentSize().height*1.25));
		loadingImage->setScaleX(winSize.width/loadingImage->getContentSize().width/1);
		loadingImage->setScaleY(winSize.height/loadingImage->getContentSize().height/8);
		this->addChild(loadingImage);
		//CCLabelTTF *loadingLabel = CCLabelTTF::create("","Arial",40);
		//loadingLabel->setPosition(ccp(winSize.width/2,winSize.height/3));
		//loadingLabel->setString("Loading Music");
		//this->addChild(loadingLabel);
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
	if(GameOptions::sharedGameOptions()->getToggleSFX() == true){
		CocosDenshion::SimpleAudioEngine::sharedEngine()->playEffect(std::string(CCFileUtils::sharedFileUtils()->fullPathFromRelativePath("menu_01.ogg")).c_str(),false);
	}
	CCDirector::sharedDirector()->replaceScene(InstructionsScene::scene());
}
void HelloWorld::menuOptCall(CCObject* pSender)
{
	if(GameOptions::sharedGameOptions()->getToggleSFX() == true){
		CocosDenshion::SimpleAudioEngine::sharedEngine()->playEffect(std::string(CCFileUtils::sharedFileUtils()->fullPathFromRelativePath("menu_01.ogg")).c_str(),false);
	}
	CCDirector::sharedDirector()->replaceScene(OptionsMenu::scene());
}
void HelloWorld::menuCreditCall(CCObject* pSender)
{
	if(GameOptions::sharedGameOptions()->getToggleSFX() == true){
		CocosDenshion::SimpleAudioEngine::sharedEngine()->playEffect(std::string(CCFileUtils::sharedFileUtils()->fullPathFromRelativePath("menu_01.ogg")).c_str(),false);
	}
	CCDirector::sharedDirector()->replaceScene(Credits::scene());
}
void HelloWorld::menuExitCall(CCObject* pSender)
{
    // "close" menu item clicked
    CCDirector::sharedDirector()->end();
}

