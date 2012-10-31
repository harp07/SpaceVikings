#ifndef __SPACE_SCENE_H__
#define __SPACE_SCENE_H__

#include "cocos2d.h"

#include "Enemy.h"
#include "Player.h"
#include "SimpleAudioEngine.h"
#include "Projectile.h"
#include "Level.h"
#include "GameOptions.h"
#include "HelloWorldScene.h"
#include "Upgrades.h"
//#include "../../external/Box2D/Box2D.h"

class SpaceScene : public cocos2d::CCLayer
{
public:
	Enemy *actEnemy;

    // Here's a difference. Method 'init' in cocos2d-x returns bool, instead of returning 'id' in cocos2d-iphone
    virtual bool init(); 

    // there's no 'id' in cpp, so we recommand to return the exactly class pointer
    static cocos2d::CCScene* scene();
    
    // a selector callback
    void toggleShootingCallback(CCObject* pSender);

	void ccTouchesEnded(cocos2d::CCSet* touches, cocos2d::CCEvent* event);

	void shipShoot();

	void spriteMoveFinished(CCNode* sender);

	void spawnEnemies();

	void update (float dt);

	void updateLabels(float dt);

	void toggleShoot(float dt);

	void gameLogic(float dt);

	void enemyShoot(float dt);

	void enemyMoveFinished(CCNode* sender);

	void HUD();

	void setEnemy(Enemy *enemy);

	void UpgradeMenu();

	void GameOverMenu();

	void menu_Back(CCObject* pSender);
	void menu_UpgradeStat1(CCObject* pSender);
	void menu_UpgradeStat2(CCObject* pSender);
	void menu_UpgradeStat3(CCObject* pSender);
	void menu_UpgradeStat4(CCObject* pSender);
	void menu_NextLevel(CCObject* pSender);
	cocos2d::CCMenu *backMenu;
	cocos2d::CCSprite *dummyTitle;
	cocos2d::CCSprite *dummyTitle1;
	cocos2d::CCSprite *dummyTitle2;
	cocos2d::CCSprite *dummyTitle3;
	cocos2d::CCSprite *dummyTitle4;

	cocos2d::CCParticleSystem *m_emitter;
	void explosion(cocos2d::CCPoint location);
	void smoke(cocos2d::CCPoint location);

	Enemy *getEnemy();

	int testInt;

	Upgrades ups;

	virtual void didAccelerate(cocos2d::CCAcceleration* pAccelerationValue);

	Player *shipSprite;
	cocos2d::CCSize winSize;
	float _shipPointsPerSecY;
	bool toggleShootingB;
	Enemy *enemy;
	Projectile *projectile;
	Projectile *enemyProjectile;
	Level *level;
	int time;
	int upgradeCounter;
	int score;
	int enemyNum;
	bool toggleMusic;
	bool toggleSFX;
	bool gameOver;
	bool upgradeTime;
	int levelNum;

	cocos2d::CCLabelTTF *_timeLabel;
	cocos2d::CCLabelTTF *_timeLabelDesc;
	cocos2d::CCLabelTTF *_scoreLabel;
	cocos2d::CCLabelTTF *_scoreLabelDesc;
	cocos2d::CCLabelTTF *_livesLabel;
	cocos2d::CCLabelTTF *_livesLabelDesc;
	cocos2d::CCLabelTTF *upgrade1Label;
	cocos2d::CCLabelTTF *upgrade2Label;
	cocos2d::CCLabelTTF *upgrade3Label;
	cocos2d::CCLabelTTF *upgrade4Label;
	//cocos2d::CCLabelTTF *_objectivesLabel;

    // implement the "static node()" method manually
    CREATE_FUNC(SpaceScene);

private:
	cocos2d::CCArray *_projectiles;
	cocos2d::CCArray *_enemies;
	cocos2d::CCArray *_enemyProjectiles;
	
};

#endif  // __HELLOWORLD_SCENE_H__
