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
		toggleShootingButton = CCMenuItemImage::create(
            "toggleShoot.png",
            "toggleShootSelected.png",
            this,
            menu_selector(SpaceScene::toggleShootingCallback));
        CC_BREAK_IF(! toggleShootingButton);
        //toggleShootingButton->setScale(2.0f);
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
		toggleParticles = GameOptions::sharedGameOptions()->getToggleParts();
		if(toggleParticles == NULL){
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
        
		_projectiles = new CCArray;
		_enemies = new CCArray;
		_enemyProjectiles = new CCArray;
		time = level->getLevelTime();
		score = enemyNum = 0;
		tempAttackDmg = 1;
		attack1B = canShoot = true;
		gameOver = upgradeTime = toggleShootingB = attack2B = attack3B = false;
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
		unscheduleAllSelectors();
		
		//level->setBCompleted1(true);
	} else if (shipSprite->getCurLives() <= 0){
		GameOverMenu();
		unscheduleAllSelectors();
		//level->setBCompleted1(false);
	} else {
		if(enemyNum <= 10){
			this->spawnEnemies();
			enemyNum += 1;
		}
	}
	
}

void SpaceScene::toggleShoot(float dt){
	this->unschedule(schedule_selector(SpaceScene::shipShoot));
	if(toggleShootingB && canShoot){
		shipShoot();
		canShoot = false;
		this->schedule(schedule_selector(SpaceScene::setShoot), 0.5+tempAttackDmg);
	}
}

void SpaceScene::ccTouchesBegan(cocos2d::CCSet* touches, cocos2d::CCEvent* event){
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
	if (canShoot){
		shipShoot();
		canShoot = false;
		this->schedule(schedule_selector(SpaceScene::setShoot), 0.5+tempAttackDmg);
	}
	if(!toggleShootingB){
		this->schedule(schedule_selector(SpaceScene::shipShoot),0.5+tempAttackDmg);
	}
	//shipSprite->setPosition(location);
}

void SpaceScene::setShoot(float dt){
	canShoot = true;
}

void SpaceScene::ccTouchesEnded(cocos2d::CCSet* touches, cocos2d::CCEvent* event){
	if(!toggleShootingB){
		this->unschedule(schedule_selector(SpaceScene::shipShoot));
		//shipShoot();
	}
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
	if(attack1B){
		tempAttackDmg = 0.0f;
		projectile1 = new Projectile();
		projectile1->init(shipSprite->getCurWeapon());
		projectile1->setPosition(ccp(shipSprite->getPosition().x,shipSprite->getContentSize().height + shipSprite->getContentSize().height));
		projectile1->setTag(1);
		_projectiles->addObject(projectile1);
		this->addChild(projectile1);

		int realY = winSize.height + (projectile1->getContentSize().height/2);
		int realX = projectile1->getPosition().x;
		CCPoint realDest1 = ccp(projectile1->getPosition().x, realY);

		int offRealX = realX - projectile1->getPosition().x;
		int offRealY = realY - projectile1->getPosition().y;
		float length = sqrtf((offRealX * offRealX) + (offRealY * offRealY));
		float velocity = 480/1;
		float realMoveDuration = length/velocity*shipSprite->getAttackSpeed();
		CCFiniteTimeAction* projectile1Path = CCMoveTo::actionWithDuration(realMoveDuration,realDest1);
		CCFiniteTimeAction* projectile1Finish = CCCallFuncN::actionWithTarget(this, callfuncN_selector(SpaceScene::spriteMoveFinished));

		projectile1->runAction(CCSequence::actions(projectile1Path,projectile1Finish, NULL));
	}
	if (attack2B){
		tempAttackDmg = 0.5f;
		projectile1 = new Projectile();
		projectile1->init(shipSprite->getCurWeapon());
		projectile1->setPosition(ccp(shipSprite->getPosition().x,shipSprite->getContentSize().height + shipSprite->getContentSize().height));
		projectile1->setTag(1);
		_projectiles->addObject(projectile1);
		this->addChild(projectile1);

			int realY = winSize.height + (projectile1->getContentSize().height/2);
			int realX = projectile1->getPosition().x;
			CCPoint realDest1 = ccp(projectile1->getPosition().x, realY);

			int offRealX = realX - projectile1->getPosition().x;
			int offRealY = realY - projectile1->getPosition().y;
			float length = sqrtf((offRealX * offRealX) + (offRealY * offRealY));
			float velocity = 480/1;
			float realMoveDuration = length/velocity*shipSprite->getAttackSpeed();

		CCFiniteTimeAction* projectile1Path = CCMoveTo::actionWithDuration(realMoveDuration,realDest1);
		CCFiniteTimeAction* projectile1Finish = CCCallFuncN::actionWithTarget(this, callfuncN_selector(SpaceScene::spriteMoveFinished));
		projectile1->runAction(CCSequence::actions(projectile1Path,projectile1Finish, NULL));

		projectile2 = new Projectile();
		projectile2->init(shipSprite->getCurWeapon());
		projectile2->setPosition(ccp(shipSprite->getPosition().x + projectile2->getContentSize().width,shipSprite->getContentSize().height + shipSprite->getContentSize().height));
		projectile2->setTag(1);
		_projectiles->addObject(projectile2);
		this->addChild(projectile2);
		CCPoint realDest2 = ccp(projectile2->getPosition().x, realY);
		CCFiniteTimeAction* projectile2Path = CCMoveTo::actionWithDuration(realMoveDuration,realDest2);
		CCFiniteTimeAction* projectile2Finish = CCCallFuncN::actionWithTarget(this, callfuncN_selector(SpaceScene::spriteMoveFinished));
		projectile2->runAction(CCSequence::actions(projectile2Path,projectile2Finish, NULL));
		//shipSprite->setAttackSpeed(shipSprite->getAttackSpeed()*1.66);
	}
	if (attack3B){
		tempAttackDmg = 1.0f;
		projectile1 = new Projectile();
		projectile1->init(shipSprite->getCurWeapon());
		projectile1->setPosition(ccp(shipSprite->getPosition().x,shipSprite->getContentSize().height + shipSprite->getContentSize().height));
		projectile1->setTag(1);
		_projectiles->addObject(projectile1);
		this->addChild(projectile1);

			int realY = winSize.height + (projectile1->getContentSize().height/2);
			int realX = projectile1->getPosition().x;
			CCPoint realDest1 = ccp(projectile1->getPosition().x, realY);

			int offRealX = realX - projectile1->getPosition().x;
			int offRealY = realY - projectile1->getPosition().y;
			float length = sqrtf((offRealX * offRealX) + (offRealY * offRealY));
			float velocity = 480/1;
			float realMoveDuration = length/velocity*shipSprite->getAttackSpeed();

		CCFiniteTimeAction* projectile1Path = CCMoveTo::actionWithDuration(realMoveDuration,realDest1);
		CCFiniteTimeAction* projectile1Finish = CCCallFuncN::actionWithTarget(this, callfuncN_selector(SpaceScene::spriteMoveFinished));
		projectile1->runAction(CCSequence::actions(projectile1Path,projectile1Finish, NULL));

		projectile2 = new Projectile();
		projectile2->init(shipSprite->getCurWeapon());
		projectile2->setPosition(ccp(shipSprite->getPosition().x + projectile2->getContentSize().width,shipSprite->getContentSize().height + shipSprite->getContentSize().height/2));
		projectile2->setTag(1);
		_projectiles->addObject(projectile2);
		this->addChild(projectile2);
		CCPoint realDest2 = ccp(projectile2->getPosition().x, realY);
		CCFiniteTimeAction* projectile2Path = CCMoveTo::actionWithDuration(realMoveDuration,realDest2);
		CCFiniteTimeAction* projectile2Finish = CCCallFuncN::actionWithTarget(this, callfuncN_selector(SpaceScene::spriteMoveFinished));
		projectile2->runAction(CCSequence::actions(projectile2Path,projectile2Finish, NULL));

		projectile3 = new Projectile();
		projectile3->init(shipSprite->getCurWeapon());
		projectile3->setPosition(ccp(shipSprite->getPosition().x - projectile3->getContentSize().width,shipSprite->getContentSize().height + shipSprite->getContentSize().height/2));
		projectile3->setTag(1);
		_projectiles->addObject(projectile3);
		this->addChild(projectile3);
		CCPoint realDest3 = ccp(projectile3->getPosition().x, realY);
		CCFiniteTimeAction* projectile3Path = CCMoveTo::actionWithDuration(realMoveDuration,realDest3);
		CCFiniteTimeAction* projectile3Finish = CCCallFuncN::actionWithTarget(this, callfuncN_selector(SpaceScene::spriteMoveFinished));
		projectile3->runAction(CCSequence::actions(projectile3Path,projectile3Finish, NULL));
		//shipSprite->setAttackSpeed(shipSprite->getAttackSpeed()*1.33);
	}

	//projectile->runAction(CCSequence::actions(CCMoveTo::actionWithDuration(realMoveDuration, realDest),
			//CCCallFuncN::actionWithTarget(this, callfuncN_selector(SpaceScene::spriteMoveFinished)), NULL));
	if(toggleSFX){
		CocosDenshion::SimpleAudioEngine::sharedEngine()->playEffect(std::string(CCFileUtils::sharedFileUtils()->fullPathFromRelativePath("player_fire_4.ogg")).c_str(),false);
	}
}

void SpaceScene::setAttack1(CCObject* pSender){
	attack1B = true;
	attack2B = false;
	attack3B = false;
	//shipSprite->setCurDamage(tempAttackDmg);
}

void SpaceScene::setAttack2(CCObject* pSender){
	attack1B = false;
	attack2B = true;
	attack3B = false;
	//shipSprite->setCurDamage(tempAttackDmg/20);
}

void SpaceScene::setAttack3(CCObject* pSender){
	attack1B = false;
	attack2B = false;
	attack3B = true;
	//shipSprite->setCurDamage(tempAttackDmg/30);
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

		CCFiniteTimeAction* boomBoom = CCMoveTo::actionWithDuration((float)4.0-(shipSprite->getAttackSpeed()-1.0f),ccp(shipSprite->getPosition().x*(rand() % (int)winSize.width/200),shipSprite->getPosition().y-100));
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
							if(toggleParticles){
								explosion(target->getPosition());
							}
						} else {
							target->setCurHp(health);
							if(toggleParticles){
								smoke(target->getPosition());
							}
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
						if(toggleParticles){
							smoke(enemyProje->getPosition());
						}
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
		this->schedule(schedule_selector(SpaceScene::toggleShoot),0.5);
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

	ccColor4F startColor = {1.0f, 0.1f, 0.1f, 1.0f};
	m_emitter->setStartColor(startColor);

	//ccColor4F startColorVar = {1.0f, 0.1f, 0.1f, 1.0f};
	//m_emitter->setStartColorVar(startColorVar);

	ccColor4F endColor = {0.1f, 0.1f, 0.1f, 0.2f};
	m_emitter->setEndColor(endColor);

	ccColor4F endColorVar = {0.1f, 0.1f, 0.1f, 0.2f};
	m_emitter->setEndColorVar(endColorVar);

	m_emitter->setTexture( CCTextureCache::sharedTextureCache()->addImage("playerfire.png"));
	this->addChild(m_emitter, 1);



	m_emitter->setAutoRemoveOnFinish(true);
}

void SpaceScene::smoke(CCPoint location){

	m_emitter = CCParticleSmoke::create();
	m_emitter->retain();
	m_emitter->setPosition(location);

	ccColor4F startColor = {0.1f, 0.1f, 0.1f, 1.0f};
	    m_emitter->setStartColor(startColor);

	    //ccColor4F startColorVar = {0.5f, 0.f, 0.5f, 1.0f};
	    //m_emitter->setStartColorVar(startColorVar);

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

		_timeLabel->setPosition(ccp(winSize.width-toggleShootingButton->getContentSize().width*2,winSize.height - 20));
		_timeLabelDesc->setPosition(ccp(winSize.width-toggleShootingButton->getContentSize().width*2-60,winSize.height - 20));
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

		do{
		CCMenuItemImage *attackMenuItem1 = CCMenuItemImage::create("attack1Button.png","attack1ButtonSelected.png",this,menu_selector(SpaceScene::setAttack1));
		//CCMenuItemImage *attackMenuItem1Selected = CCMenuItemImage::create("attack1ButtonSelected.png","attack1ButtonSelected.png",this,menu_selector(SpaceScene::setAttack1));
		//CCMenuItemToggle *attack1Toggle = CCMenuItemToggle::createWithTarget(this,menu_selector(SpaceScene::setAttack1),attackMenuItem1,attackMenuItem1Selected);
		CC_BREAK_IF(!attackMenuItem1);
		attackMenuItem1->setPosition(0,0);
		//attackMenuItem1Selected->setPosition(0,0);
		//attack1Toggle->setPosition(0,0);

		CCMenuItemImage *attackMenuItem2 = CCMenuItemImage::create("attack2Button.png","attack2ButtonSelected.png",this,menu_selector(SpaceScene::setAttack2));
		//CCMenuItemImage *attackMenuItem2Selected = CCMenuItemImage::create("attack2ButtonSelected.png","attack2ButtonSelected.png",this,menu_selector(SpaceScene::setAttack2));
		//CCMenuItemToggle *attack2Toggle = CCMenuItemToggle::createWithTarget(this,menu_selector(SpaceScene::setAttack2),attackMenuItem2,attackMenuItem2Selected);
		CC_BREAK_IF(!attackMenuItem2);
		attackMenuItem2->setPosition(attackMenuItem1->getPosition().x + (attackMenuItem2->getContentSize().width*2),0);
		//attackMenuItem2Selected->setPosition(attackMenuItem1->getPosition().x + (attackMenuItem2->getContentSize().width*2),0);
		//attack2Toggle->setPosition(attack1Toggle->getPosition().x + (attackMenuItem2->getContentSize().width*2),0);

		CCMenuItemImage *attackMenuItem3 = CCMenuItemImage::create("attack3Button.png","attack3ButtonSelected.png",this,menu_selector(SpaceScene::setAttack3));
		//CCMenuItemImage *attackMenuItem3Selected = CCMenuItemImage::create("attack3ButtonSelected.png","attack3ButtonSelected.png",this,menu_selector(SpaceScene::setAttack3));
		//CCMenuItemToggle *attack3Toggle = CCMenuItemToggle::createWithTarget(this,menu_selector(SpaceScene::setAttack3),attackMenuItem3,attackMenuItem3Selected);
		CC_BREAK_IF(!attackMenuItem3);
		attackMenuItem3->setPosition(attackMenuItem2->getPosition().x + (attackMenuItem3->getContentSize().width*2),0);
		//attackMenuItem3Selected->setPosition(attackMenuItem2->getPosition().x + (attackMenuItem3->getContentSize().width*2),0);
		//attack3Toggle->setPosition(attack2Toggle->getPosition().x + (attackMenuItem3->getContentSize().width*2),0);



		attackMenu = CCMenu::create(attackMenuItem1,attackMenuItem2,attackMenuItem3, NULL);
		attackMenu->setPosition(ccp(winSize.width/2-(attackMenuItem1->getContentSize().width*2),0 + attackMenuItem1->getContentSize().height));
		this->addChild(attackMenu);
		/*
		if(attack1B){
							attackMenuItem1->setVisible(false);
							attackMenuItem1Selected->setVisible(true);
							attackMenuItem2->setVisible(true);
							attackMenuItem2Selected->setVisible(false);
							attackMenuItem3->setVisible(true);
							attackMenuItem3Selected->setVisible(false);
						} else if (attack2B){
							attackMenuItem1->setVisible(true);
							attackMenuItem1Selected->setVisible(false);
							attackMenuItem2->setVisible(false);
							attackMenuItem2Selected->setVisible(true);
							attackMenuItem3->setVisible(true);
							attackMenuItem3Selected->setVisible(false);
						} else if (attack3B){
							attackMenuItem1->setVisible(true);
							attackMenuItem1Selected->setVisible(false);
							attackMenuItem2->setVisible(true);
							attackMenuItem2Selected->setVisible(false);
							attackMenuItem3->setVisible(false);
							attackMenuItem3Selected->setVisible(true);
						}
						*/
		}while(0);
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

		char upgradechar[100];
		sprintf(upgradechar,"Upgrades Remaining: %d",upgradeCounter);

		upgradeLabel = CCLabelTTF::create(upgradechar,"CybertownSubterranean",30);
		upgradeLabel->setPosition(ccp(winSize.width/2,UpgradeStat4->getPosition().y - UpgradeStat4->getContentSize().height));
		ccColor3B colourTest = {0,200,0};
		upgradeLabel->setColor(colourTest);
		this->addChild(upgradeLabel,2);

		char upgrade1char[100];
		sprintf(upgrade1char,"%d",shipSprite->getCurLives());

		upgrade1Label = CCLabelTTF::create(upgrade1char,"CybertownSubterranean",30);
		upgrade1Label->setPosition(ccp(UpgradeStat1->getPosition().x + UpgradeStat1->getContentSize().width/2 + upgrade1Label->getContentSize().width/2 + 50,UpgradeStat1->getPosition().y));
		upgrade1Label->setColor(colourTest);
		this->addChild(upgrade1Label,2);

		char upgrade2char[100];
		sprintf(upgrade2char,"%.2f",shipSprite->getCurDamage());

		upgrade2Label = CCLabelTTF::create(upgrade2char,"CybertownSubterranean",30);
		upgrade2Label->setPosition(ccp(UpgradeStat2->getPosition().x + UpgradeStat2->getContentSize().width/2 + upgrade2Label->getContentSize().width/2 + 50,UpgradeStat2->getPosition().y));
		upgrade2Label->setColor(colourTest);
		this->addChild(upgrade2Label,2);

		char upgrade3char[100];
		sprintf(upgrade3char,"%.2f",shipSprite->getScoreMultiplier());

		upgrade3Label = CCLabelTTF::create(upgrade3char,"CybertownSubterranean",30);
		upgrade3Label->setPosition(ccp(UpgradeStat3->getPosition().x + UpgradeStat3->getContentSize().width/2 + upgrade3Label->getContentSize().width/2 + 50,UpgradeStat3->getPosition().y));
		upgrade3Label->setColor(colourTest);
		this->addChild(upgrade3Label,2);

		char upgrade4char[100];
		sprintf(upgrade4char,"%.2f",shipSprite->getAttackSpeed());

		upgrade4Label = CCLabelTTF::create(upgrade4char,"CybertownSubterranean",30);
		upgrade4Label->setPosition(ccp(UpgradeStat4->getPosition().x + UpgradeStat4->getContentSize().width/2 + upgrade4Label->getContentSize().width/2 + 50,UpgradeStat4->getPosition().y));
		upgrade4Label->setColor(colourTest);
		this->addChild(upgrade4Label,2);

	}while(0);
}

void SpaceScene::menu_NextLevel(CCObject* pSender){
	if(upgradeCounter == 0){
		tempAttackDmg = 1;
		attack1B = canShoot = true;
		gameOver = upgradeTime = toggleShootingB = attack2B = attack3B = false;
		setAccelerometerEnabled(true);
		this->setTouchEnabled(true);
		this->scene()->removeChildByTag(2,true);
		time = level->getLevelTime();
		this->removeChild(dummyTitle, true);
		this->removeChild(dummyTitle1, true);
		this->removeChild(dummyTitle2, true);
		this->removeChild(dummyTitle3, true);
		this->removeChild(dummyTitle4, true);
		this->removeChild(backMenu, true);
		this->removeChild(upgradeLabel,true);
		this->removeChild(upgrade1Label,true);
		this->removeChild(upgrade2Label,true);
		this->removeChild(upgrade3Label,true);
		this->removeChild(upgrade4Label,true);
		level->init(2);
		level->setPosition(ccp(winSize.width/2,winSize.height/2));
		shipSprite->init(2);
		tempAttackDmg = shipSprite->getCurDamage();
		this->schedule(schedule_selector(SpaceScene::update));
		this->schedule(schedule_selector(SpaceScene::gameLogic),1.0);
		this->schedule(schedule_selector(SpaceScene::updateLabels),1.0);

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
		sprintf(upgrade1char, "Upgrades Remaining: %d",upgradeCounter);
		upgradeLabel->setString(upgrade1char);
		if(toggleSFX){
			CocosDenshion::SimpleAudioEngine::sharedEngine()->playEffect(std::string(CCFileUtils::sharedFileUtils()->fullPathFromRelativePath("menu_01.ogg")).c_str());
		}
	}
}
void SpaceScene::menu_UpgradeStat2(CCObject* pSender){
	if(upgradeCounter){
		shipSprite->setCurDamage(shipSprite->getCurDamage() + 1.0f);
		char upgrade2char[100];
		sprintf(upgrade2char,"%.2f",shipSprite->getCurDamage());
		upgrade2Label->setString(upgrade2char);
		ups.setCurDamage(shipSprite->getCurDamage());
		upgradeCounter --;
		sprintf(upgrade2char, "Upgrades Remaining: %d",upgradeCounter);
		upgradeLabel->setString(upgrade2char);
		if(toggleSFX){
			CocosDenshion::SimpleAudioEngine::sharedEngine()->playEffect(std::string(CCFileUtils::sharedFileUtils()->fullPathFromRelativePath("menu_01.ogg")).c_str());
		}
	}
}
void SpaceScene::menu_UpgradeStat3(CCObject* pSender){
	if(upgradeCounter){
		shipSprite->setScoreMultiplier(shipSprite->getScoreMultiplier() + 0.02);
		char upgrade3char[100];
		sprintf(upgrade3char,"%.2f",shipSprite->getScoreMultiplier());
		upgrade3Label->setString(upgrade3char);
		ups.setScoreMultiplier(shipSprite->getScoreMultiplier());
		upgradeCounter --;
		sprintf(upgrade3char, "Upgrades Remaining: %d",upgradeCounter);
		upgradeLabel->setString(upgrade3char);
		if(toggleSFX){
			CocosDenshion::SimpleAudioEngine::sharedEngine()->playEffect(std::string(CCFileUtils::sharedFileUtils()->fullPathFromRelativePath("menu_01.ogg")).c_str());
		}
	}
}
void SpaceScene::menu_UpgradeStat4(CCObject* pSender){
	if(upgradeCounter){
		shipSprite->setAttackSpeed(shipSprite->getAttackSpeed() + 0.02);
		char upgrade4char[100];
		sprintf(upgrade4char,"%.2f",shipSprite->getAttackSpeed());
		upgrade4Label->setString(upgrade4char);
		ups.setAttackSpeed(shipSprite->getAttackSpeed());
		upgradeCounter --;
		sprintf(upgrade4char, "Upgrades Remaining: %d",upgradeCounter);
		upgradeLabel->setString(upgrade4char);
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
		CCSprite *bg_menu = CCSprite::create("gameover.png");
		bg_menu->setPosition(ccp(winSize.width/2 , winSize.height/2));
		bg_menu->setScaleX(winSize.width/bg_menu->getContentSize().width);
		bg_menu->setScaleY(winSize.height/bg_menu->getContentSize().height);
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
