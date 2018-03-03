/*
	StoryScene.cpp

	Joe O'Regan
	K00203642
	03/03/2018

	Display the story between levels, the scene appears for 10 seconds, 
	or can be skipped with the continue button
*/

#include "StoryScene.h"
#include "MainMenu.h"
#include "Level1.h"
#include "Level2.h"
#include "Level3.h"
#include "Level4.h"
#include "Audio.h"
#include "Game.h"
#include "Defines.h"

/*
	Create the scene
*/
cocos2d::Scene* StoryScene::createScene() {
    cocos2d::Scene* scene = cocos2d::Scene::create();		// Create the scene. 'scene' is an autorelease object
        
	StoryScene* layer = StoryScene::create();				// Create the layer. 'layer' is an autorelease object
	    
    scene->addChild(layer);									// Add layer as a child to scene
	    
    return scene;											// Return the scene
}

/*
	Initialise the scene, displaying story and navigation buttons
*/
bool StoryScene::init() {
    // 1. super init first
    if ( !Layer::init() ) { return false; }
	
	Audio::Instance()->init();																								// Initialise the game audio
    
	visibleSize = cocos2d::Director::getInstance()->getVisibleSize();											// Get screen resolution
	cocos2d::Vec2 origin = cocos2d::Director::getInstance()->getVisibleOrigin();											// Get screen origin
    
	std::string message;

	// Continue button
	continueLbl = cocos2d::Label::createWithTTF("Press To\nContinue", "fonts/Super Mario Bros..ttf", visibleSize.height * 0.05);
	continueLbl->setColor(cocos2d::Color3B::RED);

	// Select the message and callback based on the current level number
	if (Game::Instance()->getLevel() == 1) {
		message = "The Rebel Insurgent Leadership got word of \nSupremacy activity in a neighbouring galaxy\n\nSquad Leader Mo Led his squadron into a trap\n\nWith His Squadron Decimated In The Ambush\nHe must now find his way home ...Alone";
		this->scheduleOnce(schedule_selector(StoryScene::GoToLevel1), DISPLAY_TIME_SPLASH_STORY);
		continueItem = MenuItemLabel::create(continueLbl, CC_CALLBACK_1(StoryScene::continueToLevel1, this));							// Continue to the next level menu option
	}
	else if (Game::Instance()->getLevel() == 2) {
		message = "Supremacy Fighters are still close by\nSensors have detected a number of vessels\nOur Hero has decided to trace their source\n\nHe must advance past the destroyers\nThey block the pathway back to base";
		this->scheduleOnce(schedule_selector(StoryScene::GoToLevel2), DISPLAY_TIME_SPLASH_STORY);
		continueItem = MenuItemLabel::create(continueLbl, CC_CALLBACK_1(StoryScene::continueToLevel2, this));							// Continue to the next level menu option
	}
	else if (Game::Instance()->getLevel() == 3) {
		message = "With the confidence of our hero growing\nHe has decided to venture further\nWhere No Insurgents Have Dared Enter\n\nIntel Will Be Valuable For The Cause\nMight As Well Go Down Fighting...";
		this->scheduleOnce(schedule_selector(StoryScene::GoToLevel3), DISPLAY_TIME_SPLASH_STORY);
		continueItem = MenuItemLabel::create(continueLbl, CC_CALLBACK_1(StoryScene::continueToLevel3, this));							// Continue to the next level menu option
	}
	else if (Game::Instance()->getLevel() == 4) {
		message = "A Supremacy battle base has been detected\nThe hero has decided to take his revenge\n\nTheir forces are weak after the battle\nThis may be the last chance to fight back...\n\nDestroy The Battle Base!!!";
		this->scheduleOnce(schedule_selector(StoryScene::GoToLevel4), DISPLAY_TIME_SPLASH_STORY);
		continueItem = MenuItemLabel::create(continueLbl, CC_CALLBACK_1(StoryScene::continueToLevel4, this));							// Continue to the next level menu option
	}		
	
	// Display the game title image
	gameTitleImg = cocos2d::Sprite::create("GameTitle.png");																	// Game Title image
	gameTitleImg->setPosition(cocos2d::Point(visibleSize.width * 0.1f, visibleSize.height / 2 + origin.y));						// Set position on screen
	gameTitleImg->setPosition(cocos2d::Point(visibleSize.width / 2 + origin.x, visibleSize.height * 0.9f + origin.y));			// Set position on screen
	gameTitleImg->setScale(1.75 * (visibleSize.height == 1080) ? 1.0f : 0.67f);													// Set the scale of the title image based on the screen resolution
	this->addChild(gameTitleImg);																								// Add to layer		

	// Level number label text
	levelLbl = cocos2d::Label::createWithTTF("Level " + StringUtils::toString(Game::Instance()->getLevel()),
		"fonts/Super Mario Bros..ttf", visibleSize.height * 0.075);
	levelLbl->setPosition(visibleSize.width / 2, visibleSize.height * 0.75f);
	levelLbl->setColor(cocos2d::Color3B::RED);
	this->addChild(levelLbl);

	// Story lable text
	storyLbl =  cocos2d::Label::createWithTTF(message, "fonts/Super Mario Bros..ttf", visibleSize.height * 0.05);
	storyLbl->setPosition(visibleSize.width / 2, visibleSize.height / 2);
	storyLbl->setColor(cocos2d::Color3B::GRAY);
	this->addChild(storyLbl);
	
	// Return to menu button
	returnToMenuLbl = cocos2d::Label::createWithTTF(" Return\nTo Menu", "fonts/Super Mario Bros..ttf", visibleSize.height * 0.05);
	returnToMenuLbl->setColor(cocos2d::Color3B::RED);

	// Continue to next level menu option
	//continueItem = MenuItemLabel::create(continueLbl, CC_CALLBACK_1(StoryScene::continueToLevel1, this));							// Continue to the next level menu option
	continueItem->setPosition(visibleSize.width * 0.9f, visibleSize.height * 0.1f);
	
	// Return to main menu option
	returnToMenuItem = MenuItemLabel::create(returnToMenuLbl, CC_CALLBACK_1(StoryScene::backToMenu, this));							// Go Back To Main Menu menu option
	returnToMenuItem->setPosition(visibleSize.width * 0.1f, visibleSize.height * 0.1f);
	
	menu = Menu::create(continueItem, returnToMenuItem, NULL);																		// Create the menu options
	menu->setPosition(Point::ZERO);
	this->addChild(menu);

	//backgroundSprite->runAction(cocos2d::ScaleTo::create(0.5F, (visibleSize.height == 720) ? 1.0f : 1.5f));						// Not working Android

	//auto action = cocos2d::FadeOut::create(3.0f);
	//backgroundSprite->runAction(action);

	
    return true;
}
/*
void StoryScene::sceneActions() {
	//auto actionStory = cocos2d::FadeOut::create(0.5f);
	float scale = (visibleSize.height == 1080) ? 2.25f : 1.5f;

	auto actionStory = cocos2d::ScaleTo::create(0.5f, scale);
	gameTitleImg->runAction(actionStory);
	levelLbl->runAction(actionStory);
	storyLbl->runAction(actionStory);
	continueLbl->runAction(actionStory);
	returnToMenuLbl->runAction(actionStory);
}
*/
// Callbacks for timed transitions
void StoryScene::GoToLevel1(float dt) {
	//sceneActions();
	cocos2d::Scene* scene = Level1::createScene();																		// Create the Level1 Scene  
	cocos2d::Director::getInstance()->replaceScene(cocos2d::TransitionFade::create(TRANSITION_TIME, scene));			// Change the scene with transition
}
void StoryScene::GoToLevel2(float dt) {
	//sceneActions();
	cocos2d::Scene* scene = Level2::createScene();																		// Create the Level2 Scene  
	cocos2d::Director::getInstance()->replaceScene(cocos2d::TransitionFade::create(TRANSITION_TIME, scene));			// Change the scene with transition
}
void StoryScene::GoToLevel3(float dt) {
	//sceneActions();
	cocos2d::Scene* scene = Level3::createScene();																		// Create the Level3 Scene  
	cocos2d::Director::getInstance()->replaceScene(cocos2d::TransitionFade::create(TRANSITION_TIME, scene));			// Change the scene with transition
}
void StoryScene::GoToLevel4(float dt) {
	//sceneActions();
	cocos2d::Scene* scene = Level4::createScene();																		// Create the Level4 Scene  
	cocos2d::Director::getInstance()->replaceScene(cocos2d::TransitionFade::create(TRANSITION_TIME, scene));			// Change the scene with transition
}

// Callbacks for button presses
void StoryScene::continueToLevel1(Ref* pSender) {
	//sceneActions();
	cocos2d::Scene* scene = Level1::createScene();																		// Create the Level1 Scene  
	cocos2d::Director::getInstance()->replaceScene(cocos2d::TransitionFade::create(TRANSITION_TIME, scene));			// Change the scene with transition
}
void StoryScene::continueToLevel2(Ref* pSender) {
	//sceneActions();
	cocos2d::Scene* scene = Level2::createScene();																		// Create the Level2 Scene  
	cocos2d::Director::getInstance()->replaceScene(cocos2d::TransitionFade::create(TRANSITION_TIME, scene));			// Change the scene with transition
}
void StoryScene::continueToLevel3(Ref* pSender) {
	//sceneActions();
	cocos2d::Scene* scene = Level3::createScene();																		// Create the Level3 Scene  
	cocos2d::Director::getInstance()->replaceScene(cocos2d::TransitionFade::create(TRANSITION_TIME, scene));			// Change the scene with transition
}
void StoryScene::continueToLevel4(Ref* pSender) {
	//sceneActions();
	cocos2d::Scene* scene = Level4::createScene();																		// Create the Level4 Scene  
	cocos2d::Director::getInstance()->replaceScene(cocos2d::TransitionFade::create(TRANSITION_TIME, scene));			// Change the scene with transition
}
void StoryScene::backToMenu(Ref* pSender) {
	//sceneActions();
	cocos2d::Scene* scene = MainMenu::createScene();																	// Create the Level4 Scene  
	cocos2d::Director::getInstance()->replaceScene(cocos2d::TransitionFade::create(TRANSITION_TIME, scene));			// Change the scene with transition
}

