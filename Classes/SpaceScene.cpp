#include "SpaceScene.h"
#include <stdio.h>
#include <time.h>

#define FIX_POS(_pos, _min, _max) \
    if (_pos < _min)        \
    _pos = _min;        \
else if (_pos > _max)   \
    _pos = _max;        \

using namespace cocos2d;

CCScene* SpaceScene::scene()
{
    CCScene * scene = NULL;
    do 
    {
        // 'scene' is an autorelease object
        scene = CCScene::create();
        CC_BREAK_IF(! scene);

        // 'layer' is an autorelease object
        SpaceScene *layer = SpaceScene::create();
        CC_BREAK_IF(! layer);

        // add layer as a child to scene
        scene->addChild(layer);
    } while (0);

    // return the scene
    return scene;
}

// on "init" you need to initialize your instance
bool SpaceScene::init()
{
    bool bRet = false;
    do 
	{

        CC_BREAK_IF(! CCLayer::init());
		winSize = CCDirector::sharedDirector()->getWinSize();
		level = new Level();
		level->init(1);
		level->setPosition(ccp(winSize.width/2, winSize.height/2));
		level->setScaleX(winSize.width/level->getContentSize().width);
		level->setScaleY(winSize.height/level->getContentSize().height);
		
		this->addChild(level);
		CCMenuItemImage *toggleShootingButton = CCMenuItemImage::create(
            "CloseNormal.png",
            "CloseSelected.png",
            this,
            menu_selector(SpaceScene::toggleShootingCallback));
        CC_BREAK_IF(! toggleShootingButton);
		toggleShootingButton->setPosition(ccp(winSize.width - toggleShootingButton->getContentSize().width/2, winSize.height - toggleShootingButton->getContentSize().height/2));

        CCMenu* pMenu = CCMenu::create(toggleShootingButton, NULL);
        pMenu->setPosition(CCPointZero);
        CC_BREAK_IF(! pMenu);

        this->addChild(pMenu, 1);

		toggleMusic = GameOptions::sharedGameOptions()->getToggleMusic();
		if(toggleMusic == NULL){
			//toggleMusic = true;
		}
		toggleSFX = GameOptions::sharedGameOptions()->getToggleSFX();
		if(toggleSFX == NULL){
			//toggleSFX = true;
		}

		shipSprite = new Player();
		shipSprite->init(1);
		int levelNum = 1;
        CC_BREAK_IF(! shipSprite);
		shipSprite->setPosition(ccp(winSize.width/2,shipSprite->getContentSize().height/2 + 150));
        this->addChild(shipSprite, 0);
		if(toggleMusic){
			CocosDenshion::SimpleAudioEngine::sharedEngine()->playBackgroundMusic(std::string(CCFileUtils::sharedFileUtils()->fullPathFromRelativePath("Dark_Future_0.ogg")).c_str(), true);
		}
        

		toggleShootingB = false;
		_projectiles = new CCArray;
		_enemies = new CCArray;
		_enemyProjectiles = new CCArray;
		time = level->getLevelTime();
		score = 0;
		enemyNum = 0;
		gameOver = false;
		upgradeTime = false;
		setAccelerometerEnabled(true);
		this->setTouchEnabled(true);
		if(!gameOver || upgradeTime){
			this->schedule(schedule_selector(SpaceScene::update));
			this->schedule(schedule_selector(SpaceScene::gameLogic),1.0);
			this->schedule(schedule_selector(SpaceScene::updateLabels),1.0);
		}
		HUD();
		//this->schedule(schedule_selector(SpaceScene::enemyShoot),0.1);
		bRet = true;
    } while (0);
    return bRet;
}

void SpaceScene::gameLogic(float dt){
	if(shipSprite->getCurLives() != 0 && time == -1){
		//level->setLevelTime(0);
		//time = 0;

		upgradeCounter = 5 + ((int)score/100);
		UpgradeMenu();
		upgradeTime = true;
		this->setTouchEnabled(false);
		
		//level->setBCompleted1(true);
	} else if (shipSprite->getCurLives() <= 0){
		GameOverMenu();
		//level->setBCompleted1(false);
	} else {
		if(enemyNum <= 10){
			this->spawnEnemies();
			enemyNum += 1;
		}
	}
	
}

void SpaceScene::toggleShoot(float dt){
	if(toggleShootingB){
		shipShoot();
	}
}

void SpaceScene::ccTouchesEnded(cocos2d::CCSet* touches, cocos2d::CCEvent* event){
	/*CCDirector* pDir = CCDirector::sharedDirector();
	CCSize shipSize = shipSprite->getContentSize();
	CCPoint ptNow = shipSprite->getPosition();
	CCPoint ptTemp = pDir->convertToUI(ptNow);

	CCTouch* touch = (CCTouch*)(touches->anyObject());
	CCPoint location = touch->getLocationInView();
	location = CCDirector::sharedDirector()->convertToGL(location);

	FIX_POS(location.x,(shipSize.width/4.0), (winSize.width - shipSize.width/4.0));
	FIX_POS(location.y,(shipSize.height/2.0), (winSize.height - shipSize.height/2.0));

	CCFiniteTimeAction* shipMove = CCMoveTo::actionWithDuration((float)1.0,ccp(location.x,shipSprite->getContentSize().height/2+150));
	shipSprite->runAction(CCSequence::actions(shipMove, NULL));*/
	
	if(!toggleShootingB){
		shipShoot();
	}
	//shipSprite->setPosition(location);
}

void SpaceScene::didAccelerate(CCAcceleration* pAccelerationValue){
	CCDirector* pDir = CCDirector::sharedDirector();

	CCSize shipSize = shipSprite->getContentSize();
	if(shipSprite == NULL){
		return;
	}
	CCPoint ptNow = shipSprite->getPosition();
	CCPoint ptTemp = pDir->convertToUI(ptNow);

	ptTemp.x += pAccelerationValue->x * winSize.height/500;
	shipSprite->setRotation(pAccelerationValue->x*10);
	/*shipSprite->setSkewX(pAccelerationValue->x*10);
	shipSprite->setSkewY(-pAccelerationValue->x*10);*/
	ptTemp.y -= pAccelerationValue->y * 0.0f;

	CCPoint ptNext = pDir->convertToGL(ptTemp);
	FIX_POS(ptNext.x,(shipSize.width/2.0), (winSize.width - shipSize.width/2.0));
	FIX_POS(ptNext.y,(shipSize.height/2.0), (winSize.height - shipSize.height/2.0));
	shipSprite->setPosition(ptNext);
}

void SpaceScene::shipShoot(){
	projectile = new Projectile();
	projectile->init(shipSprite->getCurWeapon());
	projectile->setPosition(ccp(shipSprite->getPosition().x,shipSprite->getContentSize().height + shipSprite->getContentSize().height));

	projectile->setTag(1);
	_projectiles->addObject(projectile);
	this->addChild(projectile);

	int realY = winSize.height + (projectile->getContentSize().height/2);
	int realX = projectile->getPosition().x;
	CCPoint realDest = ccp(realX, realY);

	int offRealX = realX - projectile->getPosition().x;
	int offRealY = realY - projectile->getPosition().y;
	float length = sqrtf((offRealX * offRealX) + (offRealY * offRealY));
	float velocity = 480/1;
	float realMoveDuration = length/velocity*shipSprite->getAttackSpeed();

	CCFiniteTimeAction* boomBoom = CCMoveTo::actionWithDuration(realMoveDuration,realDest);
	CCFiniteTimeAction* boomBoomFin = CCCallFuncN::actionWithTarget(this, callfuncN_selector(SpaceScene::spriteMoveFinished));
	projectile->runAction(CCSequence::actions(boomBoom,boomBoomFin, NULL));

	//projectile->runAction(CCSequence::actions(CCMoveTo::actionWithDuration(realMoveDuration, realDest),
			//CCCallFuncN::actionWithTarget(this, callfuncN_selector(SpaceScene::spriteMoveFinished)), NULL));
	//if(toggleSFX){
		//CocosDenshion::SimpleAudioEngine::sharedEngine()->playEffect("player_attack_4.ogg");
	//}
}

void SpaceScene::enemyShoot(float dt){	
	Enemy *enemyT = this->getEnemy();
	if(rand()%1000 == 0){		
		enemyProjectile = new Projectile();
	
		enemyProjectile->init(enemyT->getCurWeapon());
		enemyProjectile->setPosition(ccp(enemyT->getPosition().x,enemyT->getPosition().y));

		enemyProjectile->setTag(3);
		_enemyProjectiles->addObject(enemyProjectile);
		this->addChild(enemyProjectile);

		//CCRect projectileRect = CCRectMake(enemyProjectile->getPosition().x - (projectile->getContentSize().width/2),
											/*projec->getPosition().y - (projectile->getContentSize().height/2),
											projectile->getContentSize().width, projectile->getContentSize().height);*/


		int realY = winSize.height + (enemyProjectile->getContentSize().height/2);
		int realX = enemyProjectile->getPosition().x;
		CCPoint realDest = ccp(realX, realY);

		int offRealX = realX - enemyProjectile->getPosition().x;
		int offRealY = realY - enemyProjectile->getPosition().y;

		//enemyProjectile->runAction(CCSequence::actions(CCMoveTo::actionWithDuration((float)3.0f, ccp(shipSprite->getPosition().x*(rand() % (int)winSize.width/200),shipSprite->getPosition().y-100)),
		//CCCallFuncN::actionWithTarget(this, callfuncN_selector(SpaceScene::spriteMoveFinished)), NULL));

		CCFiniteTimeAction* boomBoom = CCMoveTo::actionWithDuration((float)4.0,ccp(shipSprite->getPosition().x*(rand() % (int)winSize.width/200),shipSprite->getPosition().y-100));
		CCFiniteTimeAction* boomBoomFin = CCCallFuncN::actionWithTarget(this, callfuncN_selector(SpaceScene::spriteMoveFinished));
		enemyProjectile->runAction(CCSequence::actions(boomBoom,boomBoomFin, NULL));
		if(toggleSFX){
			CocosDenshion::SimpleAudioEngine::sharedEngine()->playEffect(std::string(CCFileUtils::sharedFileUtils()->fullPathFromRelativePath("enemy_shot_v2.ogg")).c_str(),false);
		}
	} 
		
}

void SpaceScene::spawnEnemies(){
	int enemyI = 0;
	
	if((rand()%2)==0){
		enemy = new Enemy();
		enemy->init(1);
		enemyI = 1;
	} else {
		enemy = new Enemy();
		enemy->init(2);
		enemyI = 2;
	}
	int minY = enemy->getContentSize().width/2;
	int maxY = winSize.width - enemy->getContentSize().width/2;
	int rangeY = maxY - minY;
	int actualY = (rand() %rangeY) + minY;

	enemy->setPosition(ccp(actualY,winSize.height + (enemy->getContentSize().height/2)));
	enemy->setTag(2);
	_enemies->addObject(enemy);
	this->addChild(enemy);

	int minDuration = enemy->getMinMoveDuration();
	int maxDuration = enemy->getMaxMoveDuration();
	int rangeDuration = maxDuration - minDuration;
	int actualDuration = (rand() %rangeDuration)+minDuration;
	unsigned int f, t;
	int random = rand() % 1;


	if(enemyI == 1){
		CCFiniteTimeAction* enemyMove = CCMoveTo::actionWithDuration((float)actualDuration,ccp(actualY,winSize.height/2.0));
		//CCFiniteTimeAction* enemyMoveDone = CCCallFuncN::actionWithTarget(this, callfuncN_selector(SpaceScene::enemyMoveFinished));
		enemy->runAction(CCSequence::actions(enemyMove, NULL));
	} else if (enemyI == 2){
		CCFiniteTimeAction* enemyMove = CCMoveTo::actionWithDuration((float)actualDuration,ccp(actualY,winSize.height/1.5));
		//CCFiniteTimeAction* enemyMoveDone = CCCallFuncN::actionWithTarget(this, callfuncN_selector(SpaceScene::enemyMoveFinished));
		enemy->runAction(CCSequence::actions(enemyMove, NULL));
	}
}

void SpaceScene::updateLabels(float dt){
	time -= 1;
	char timeChar[100];
	sprintf(timeChar,"%d",time+1);
	_timeLabel->setString(timeChar);
	//level->setLevelTime(time);
	int lives = shipSprite->getCurLives();
	char livesChar[100];
	sprintf(livesChar,"%d",lives);
	_livesLabel->setString(livesChar);
}

void SpaceScene::setEnemy(Enemy *enemy){
	this->actEnemy = enemy;
}

Enemy *SpaceScene::getEnemy(){
	return this->actEnemy;
}

void SpaceScene::update(float dt){	
	
	CCArray *projectilesToDelete = new CCArray;
	CCObject* arrayItem;
	CCARRAY_FOREACH(_projectiles,arrayItem){
		CCSprite *projectile = (CCSprite*)(arrayItem);
		CCRect projectileRect = CCRectMake(projectile->getPosition().x - (projectile->getContentSize().width/2),
											projectile->getPosition().y - (projectile->getContentSize().height/2),
											projectile->getContentSize().width, projectile->getContentSize().height);

			CCArray *targetsToDelete = new CCArray;
			CCObject* arrayItem2;
			
			CCARRAY_FOREACH(_enemies,arrayItem2){
				
				Enemy *target = (Enemy*)(arrayItem2);
				enemyShoot(0.5);
				setEnemy(target);
				CCRect targetRect = CCRectMake(target->getPosition().x - (target->getContentSize().width/2),
												target->getPosition().y - (target->getContentSize().height/2),
												target->getContentSize().width, target->getContentSize().height);
				CCRect playerRect = CCRectMake(shipSprite->getPosition().x - (shipSprite->getContentSize().width/2),
												shipSprite->getPosition().y - (shipSprite->getContentSize().height/2),
												shipSprite->getContentSize().width/1.5, shipSprite->getContentSize().height/1.5);
					
					CCArray *enemyProjectilesToDelete = new CCArray;
					CCObject* arrayItemE;
					CCARRAY_FOREACH(_enemyProjectiles,arrayItemE){
						CCSprite *enemyProje = (CCSprite*)(arrayItemE);
						CCRect enemyProjectileRect = CCRectMake(enemyProje->getPosition().x - (enemyProje->getContentSize().width/2),
												enemyProje->getPosition().y - (enemyProje->getContentSize().height/2),
												enemyProje->getContentSize().width, enemyProje->getContentSize().height);
					if (projectileRect.intersectsRect(targetRect)){
						projectilesToDelete->addObject(projectile);
						int health = target->getCurHp() - shipSprite->getCurDamage();
						if (health <= 0){
							if(toggleSFX){
								CocosDenshion::SimpleAudioEngine::sharedEngine()->playEffect(std::string(CCFileUtils::sharedFileUtils()->fullPathFromRelativePath("explosion_5.ogg")).c_str());
							}
							targetsToDelete->addObject(target);
							score += target->getCurScore()*shipSprite->getScoreMultiplier();
							char scoreChar[1000];
							sprintf(scoreChar,"%d",score);
							_scoreLabel->setString(scoreChar);
							enemyNum -= 1;
							explosion(target->getPosition());
						} else {
							target->setCurHp(health);
							smoke(target->getPosition());
						}
					} 
					if (enemyProjectileRect.intersectsRect(playerRect)){
						enemyProjectilesToDelete->addObject(enemyProje);
						int playerLives = shipSprite->getCurLives();
						playerLives -= 1;
						shipSprite->setCurLives(playerLives);
						char livesChar[100];
						sprintf(livesChar,"%d",playerLives);
						_livesLabel->setString(livesChar);
						smoke(shipSprite->getPosition());
					}
					}
					CCARRAY_FOREACH(enemyProjectilesToDelete,arrayItemE){
						CCSprite* enemyProjectil = (CCSprite*)(arrayItemE);
						_enemyProjectiles->removeObject(enemyProjectil);
						this->removeChild(enemyProjectil,true);
						
					}
				enemyProjectilesToDelete->release();
			}
			CCARRAY_FOREACH(targetsToDelete,arrayItem2){
				Enemy *target = (Enemy*)(arrayItem2);
				_enemies->removeObject(target);
				this->removeChild(target,true);
			}

		if(targetsToDelete->count() > 0){
			projectilesToDelete->addObject(projectile);
		}
		targetsToDelete->release();
	}

	CCARRAY_FOREACH(projectilesToDelete,arrayItem){
		CCSprite* projectile = (CCSprite*)(arrayItem);
		_projectiles->removeObject(projectile);
		this->removeChild(projectile,true);
	}
	projectilesToDelete->release();



}

void SpaceScene::spriteMoveFinished(CCNode* sender){
	CCSprite *sprite = (CCSprite *)sender;
	this->removeChild(sprite, true);

	if(sprite->getTag() == 2){
		_enemies->removeObject(sprite);
	} else if (sprite->getTag() == 1){
		_projectiles->removeObject(sprite);
	} else if (sprite->getTag() == 3){
		_enemyProjectiles->removeObject(sprite);
	}
}

void SpaceScene::toggleShootingCallback(CCObject* pSender)
{
    if(!toggleShootingB){
		this->schedule(schedule_selector(SpaceScene::toggleShoot),0.2);
		toggleShootingB = true;
	} else if (toggleShootingB){
		toggleShootingB = false;
	}
}

void SpaceScene::explosion(CCPoint location){

	m_emitter = CCParticleExplosion::create();
	m_emitter->retain();
	m_emitter->setPosition(location);
	m_emitter->setSpeed(1.0f);
	m_emitter->setLife(0.5f);
	m_emitter->setGravity(ccp(0.0f,-200.0f));
	m_emitter->setTotalParticles(50);
	this->addChild(m_emitter, 1);

	m_emitter->setTexture( CCTextureCache::sharedTextureCache()->addImage("fireball2.png"));

	m_emitter->setAutoRemoveOnFinish(true);
}

void SpaceScene::smoke(CCPoint location){

	m_emitter = CCParticleSmoke::create();
	m_emitter->retain();
	m_emitter->setPosition(location);

	ccColor4F startColor = {0.5f, 0.5f, 0.5f, 1.0f};
	    m_emitter->setStartColor(startColor);

	    ccColor4F startColorVar = {0.5f, 0.5f, 0.5f, 1.0f};
	    m_emitter->setStartColorVar(startColorVar);

	    ccColor4F endColor = {0.1f, 0.1f, 0.1f, 0.2f};
	    m_emitter->setEndColor(endColor);

	    ccColor4F endColorVar = {0.1f, 0.1f, 0.1f, 0.2f};
	    m_emitter->setEndColorVar(endColorVar);

	    m_emitter->setScale(0.5f);
	    m_emitter->setGravity(ccp(0.0f,-200.0f));
	    m_emitter->setLife(2.0f);
	    m_emitter->setTotalParticles(10);
	    m_emitter->setDuration(1.0f);

	this->addChild(m_emitter, 1);

	m_emitter->setTexture( CCTextureCache::sharedTextureCache()->addImage("fireball2.png"));

	m_emitter->setAutoRemoveOnFinish(true);
}


void SpaceScene::HUD(){
	_timeLabel = CCLabelTTF::create("","Arial",20);
		_timeLabelDesc = CCLabelTTF::create("","Arial",20);
		_scoreLabel = CCLabelTTF::create("","Arial",20);
		_scoreLabelDesc = CCLabelTTF::create("","Arial",20);
		_livesLabel = CCLabelTTF::create("","Arial",20);
		_livesLabelDesc = CCLabelTTF::create("","Arial",20);
		//_objectivesLabel = CCLabelTTF::labelWithString("","Cybertown Subterranean",20);

		_timeLabel->setPosition(ccp(winSize.width-60,winSize.height - 20));
		_timeLabelDesc->setPosition(ccp(winSize.width-120,winSize.height - 20));
		_scoreLabel->setPosition(ccp(120,winSize.height - 20));
		_scoreLabelDesc->setPosition(ccp(60,winSize.height - 20));
		_livesLabel->setPosition(ccp(120,winSize.height - 50));
		_livesLabelDesc->setPosition(ccp(60,winSize.height - 50));
		//_objectivesLabel->setPosition(ccp(winSize.width/2, winSize.height - 70));

		_timeLabel->setString("0");
		_timeLabelDesc->setString("Time: ");
		_scoreLabel->setString("0");
		_scoreLabelDesc->setString("Score: ");
		_livesLabel->setString("0");
		_livesLabelDesc->setString("Lives: ");
		//_objectivesLabel->setString("Objectives");

		this->addChild(_timeLabel);
		this->addChild(_timeLabelDesc);
		this->addChild(_scoreLabel);
		this->addChild(_scoreLabelDesc);
		this->addChild(_livesLabel);
		this->addChild(_livesLabelDesc);
		//this->addChild(_objectivesLabel);
}

void SpaceScene::UpgradeMenu(){
	do{
		/*CCSprite *bg_menu = CCSprite::spriteWithFile("menu.png", CCRectMake(0,0,winSize.width,winSize.height));
		bg_menu->setPosition(ccp(winSize.width/2 , winSize.height/2));
		this->addChild(bg_menu,1);*/

		dummyTitle = CCSprite::spriteWithFile("upgrade2.png", CCRectMake(0,0,482,59));
		dummyTitle->setPosition(ccp(winSize.width/2 , winSize.height/1.2 - dummyTitle->getContentSize().height));
		this->addChild(dummyTitle);

		dummyTitle1 = CCSprite::spriteWithFile("lives2.png", CCRectMake(0,0,200,50));
		dummyTitle1->setPosition(ccp(winSize.width/2 -100, dummyTitle->getPosition().y - dummyTitle1->getContentSize().height - 60));
		this->addChild(dummyTitle1,2);

		dummyTitle2 = CCSprite::spriteWithFile("damage2.png", CCRectMake(0,0,200,50));
		dummyTitle2->setPosition(ccp(winSize.width/2 -100, dummyTitle1->getPosition().y - dummyTitle2->getContentSize().height - 45));
		this->addChild(dummyTitle2,2);

		dummyTitle3 = CCSprite::spriteWithFile("multiplier2.png", CCRectMake(0,0,200,50));
		dummyTitle3->setPosition(ccp(winSize.width/2 -100, dummyTitle2->getPosition().y - dummyTitle3->getContentSize().height - 45));
		this->addChild(dummyTitle3,2);

		dummyTitle4 = CCSprite::spriteWithFile("attackspeed2.png", CCRectMake(0,0,200,50));
		dummyTitle4->setPosition(ccp(winSize.width/2 -100, dummyTitle3->getPosition().y - dummyTitle4->getContentSize().height - 45));
		this->addChild(dummyTitle4,2);

		CCMenuItemImage *nextLevel = CCMenuItemImage::create(
			"continue.png",
            "continue.png",
			this,
			menu_selector(SpaceScene::menu_NextLevel));
		CC_BREAK_IF(!nextLevel);
		nextLevel->setPosition(ccp(winSize.width/2, dummyTitle4->getPosition().y - nextLevel->getContentSize().height - 50));

		CCMenuItemImage *UpgradeStat1 = CCMenuItemImage::create(
			"plus2.png",
            "plus2.png",
			this,
			menu_selector(SpaceScene::menu_UpgradeStat1));
		CC_BREAK_IF(!UpgradeStat1);
		UpgradeStat1->setPosition(ccp(dummyTitle1->getPosition().x + dummyTitle1->getContentSize().width/2 + UpgradeStat1->getContentSize().width/2, dummyTitle1->getPosition().y));

		CCMenuItemImage *UpgradeStat2 = CCMenuItemImage::create(
			"plus2.png",
            "plus2.png",
			this,
			menu_selector(SpaceScene::menu_UpgradeStat2));
		CC_BREAK_IF(!UpgradeStat2);
		UpgradeStat2->setPosition(ccp(dummyTitle2->getPosition().x + dummyTitle2->getContentSize().width/2 + UpgradeStat2->getContentSize().width/2, dummyTitle2->getPosition().y));

		CCMenuItemImage *UpgradeStat3 = CCMenuItemImage::create(
			"plus2.png",
            "plus2.png",
			this,
			menu_selector(SpaceScene::menu_UpgradeStat3));
		CC_BREAK_IF(!UpgradeStat3);
		UpgradeStat3->setPosition(ccp(dummyTitle3->getPosition().x + dummyTitle3->getContentSize().width/2 + UpgradeStat3->getContentSize().width/2, dummyTitle3->getPosition().y));

		CCMenuItemImage *UpgradeStat4 = CCMenuItemImage::create(
			"plus2.png",
            "plus2.png",
			this,
			menu_selector(SpaceScene::menu_UpgradeStat4));
		CC_BREAK_IF(!UpgradeStat4);
		UpgradeStat4->setPosition(ccp(dummyTitle4->getPosition().x + dummyTitle4->getContentSize().width/2 + UpgradeStat4->getContentSize().width/2, dummyTitle4->getPosition().y));
		CCMenuItemImage *backToMain = CCMenuItemImage::create(
			"menubutton.png",
            "menubutton.png",
			this,
			menu_selector(SpaceScene::menu_Back));
		CC_BREAK_IF(!backToMain);
		backToMain->setPosition(ccp(winSize.width/2, nextLevel->getPosition().y - backToMain->getContentSize().height - 50));

		backMenu = CCMenu::create(nextLevel,backToMain, UpgradeStat1, UpgradeStat2, UpgradeStat3, UpgradeStat4 , NULL);
		CC_BREAK_IF(!backMenu);

		backMenu->setPosition(ccp(0,0));

		this->addChild(backMenu, 2);

		char upgrade1char[100];
		sprintf(upgrade1char,"%d",shipSprite->getCurLives());

		upgrade1Label = CCLabelTTF::create(upgrade1char,"CybertownSubterranean",20);
		upgrade1Label->setPosition(ccp(UpgradeStat1->getPosition().x + UpgradeStat1->getContentSize().width/2 + upgrade1Label->getContentSize().width/2 + 50,UpgradeStat1->getPosition().y));
		this->addChild(upgrade1Label,2);

		char upgrade2char[100];
		sprintf(upgrade2char,"%d",shipSprite->getCurDamage());

		upgrade2Label = CCLabelTTF::create(upgrade2char,"CybertownSubterranean",20);
		upgrade2Label->setPosition(ccp(UpgradeStat2->getPosition().x + UpgradeStat2->getContentSize().width/2 + upgrade2Label->getContentSize().width/2 + 50,UpgradeStat2->getPosition().y));
		this->addChild(upgrade2Label,2);

		char upgrade3char[100];
		sprintf(upgrade3char,"%.2f",shipSprite->getScoreMultiplier());

		upgrade3Label = CCLabelTTF::create(upgrade3char,"CybertownSubterranean",20);
		upgrade3Label->setPosition(ccp(UpgradeStat3->getPosition().x + UpgradeStat3->getContentSize().width/2 + upgrade3Label->getContentSize().width/2 + 50,UpgradeStat3->getPosition().y));
		this->addChild(upgrade3Label,2);

		char upgrade4char[100];
		sprintf(upgrade4char,"%.2f",shipSprite->getAttackSpeed());

		upgrade4Label = CCLabelTTF::create(upgrade4char,"CybertownSubterranean",20);
		upgrade4Label->setPosition(ccp(UpgradeStat4->getPosition().x + UpgradeStat4->getContentSize().width/2 + upgrade4Label->getContentSize().width/2 + 50,UpgradeStat4->getPosition().y));
		this->addChild(upgrade4Label,2);

	}while(0);
}

void SpaceScene::menu_NextLevel(CCObject* pSender){
	if(upgradeCounter == 0){
		this->setTouchEnabled(true);
		this->scene()->removeChildByTag(2,true);
		time = level->getLevelTime();
		this->removeChild(dummyTitle, true);
		this->removeChild(dummyTitle1, true);
		this->removeChild(dummyTitle2, true);
		this->removeChild(dummyTitle3, true);
		this->removeChild(dummyTitle4, true);
		this->removeChild(backMenu, true);
		this->removeChild(upgrade1Label,true);
		this->removeChild(upgrade2Label,true);
		this->removeChild(upgrade3Label,true);
		this->removeChild(upgrade4Label,true);
		level->init(2);
		level->setPosition(ccp(winSize.width/2,winSize.height/2));
		shipSprite->init(2);
	}
}

void SpaceScene::menu_UpgradeStat1(CCObject* pSender){
	if(upgradeCounter){
		shipSprite->setCurLives(shipSprite->getCurLives() + 5);
		char upgrade1char[100];
		sprintf(upgrade1char,"%d",shipSprite->getCurLives());
		upgrade1Label->setString(upgrade1char);
		ups.setCurLives(shipSprite->getCurLives());
		upgradeCounter --;
		if(toggleSFX){
			CocosDenshion::SimpleAudioEngine::sharedEngine()->playEffect(std::string(CCFileUtils::sharedFileUtils()->fullPathFromRelativePath("menu_01.ogg")).c_str());
		}
	}
}
void SpaceScene::menu_UpgradeStat2(CCObject* pSender){
	if(upgradeCounter){
		shipSprite->setCurDamage(shipSprite->getCurDamage() + 1);
		char upgrade2char[100];
		sprintf(upgrade2char,"%d",shipSprite->getCurDamage());
		upgrade2Label->setString(upgrade2char);
		ups.setCurDamage(shipSprite->getCurDamage());
		upgradeCounter --;
		if(toggleSFX){
			CocosDenshion::SimpleAudioEngine::sharedEngine()->playEffect(std::string(CCFileUtils::sharedFileUtils()->fullPathFromRelativePath("menu_01.ogg")).c_str());
		}
	}
}
void SpaceScene::menu_UpgradeStat3(CCObject* pSender){
	if(upgradeCounter){
		shipSprite->setScoreMultiplier(shipSprite->getScoreMultiplier() + 0.10);
		char upgrade3char[100];
		sprintf(upgrade3char,"%.2f",shipSprite->getScoreMultiplier());
		upgrade3Label->setString(upgrade3char);
		ups.setScoreMultiplier(shipSprite->getScoreMultiplier());
		upgradeCounter --;
		if(toggleSFX){
			CocosDenshion::SimpleAudioEngine::sharedEngine()->playEffect(std::string(CCFileUtils::sharedFileUtils()->fullPathFromRelativePath("menu_01.ogg")).c_str());
		}
	}
}
void SpaceScene::menu_UpgradeStat4(CCObject* pSender){
	if(upgradeCounter){
		shipSprite->setAttackSpeed(shipSprite->getAttackSpeed() + 0.10);
		char upgrade4char[100];
		sprintf(upgrade4char,"%.2f",shipSprite->getAttackSpeed());
		upgrade4Label->setString(upgrade4char);
		ups.setAttackSpeed(shipSprite->getAttackSpeed());
		upgradeCounter --;
		if(toggleSFX){
			CocosDenshion::SimpleAudioEngine::sharedEngine()->playEffect(std::string(CCFileUtils::sharedFileUtils()->fullPathFromRelativePath("menu_01.ogg")).c_str());
		}
	}
}

void SpaceScene::menu_Back(CCObject* pSender)
{
	if(toggleSFX){
		CocosDenshion::SimpleAudioEngine::sharedEngine()->playEffect(std::string(CCFileUtils::sharedFileUtils()->fullPathFromRelativePath("menu_01.ogg")).c_str());
	}
	CocosDenshion::SimpleAudioEngine::sharedEngine()->stopBackgroundMusic();
	CocosDenshion::SimpleAudioEngine::sharedEngine()->stopAllEffects();
	CCDirector::sharedDirector()->replaceScene(HelloWorld::scene());
}

void SpaceScene::GameOverMenu(){
	do{
		gameOver = true;
		CCSprite *bg_menu = CCSprite::spriteWithFile("gameover.png", CCRectMake(0,0,winSize.width,winSize.height));
		bg_menu->setPosition(ccp(winSize.width/2 , winSize.height/2));
		this->addChild(bg_menu,1);

		CCMenuItemImage *backToMain = CCMenuItemImage::create(
			"menubutton.png",
            "menubutton.png",
			this,
			menu_selector(SpaceScene::menu_Back));
		CC_BREAK_IF(!backToMain);
		backToMain->setPosition(ccp(winSize.width/2, 400));

		CCMenu *backMenu = CCMenu::create(backToMain, NULL);
		CC_BREAK_IF(!backMenu);

		backMenu->setPosition(ccp(0,0));
		CocosDenshion::SimpleAudioEngine::sharedEngine()->stopBackgroundMusic();
		CocosDenshion::SimpleAudioEngine::sharedEngine()->stopAllEffects();

		this->addChild(backMenu, 1);
	}while(0);
}
