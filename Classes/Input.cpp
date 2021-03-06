#include "Input.h"
#include "Game.h"

Input* Input::s_pInstance;																			// Singleton for Input

/*
	Is the key indicated by its key code pressed
*/
bool Input::isKeyPressed(cocos2d::EventKeyboard::KeyCode code) {	
	//CCLOG("IS KEY PRESSED");
	if (keys.find(code) != keys.end())																// Check if the key is currently pressed by seeing it it's in the std::map keys
		return true;

	return false;
}

/*
	Get the current game time in milliseconds
*/
float Input::getTimeTick() {
	timeval time;
	gettimeofday(&time, NULL);
	unsigned long millisecs = (time.tv_sec * 1000) + (time.tv_usec / 1000);
	return (float)millisecs;
}

/*
	Is key pressed functions reconfigured to use with keyboard input in the menus
*/
bool Input::isKeyPressedMenu(cocos2d::EventKeyboard::KeyCode code) {
	bool buttonWasPressed = false;

	//CCLOG("IS KEY PRESSED");
	if (keys.find(code) != keys.end()) {															// Check if the key is currently pressed by seeing it it's in the std::map keys
		if (getTimeTick() > m_nextBtnTime) {
			buttonWasPressed = true;
			m_nextBtnTime = getTimeTick() + m_buttonRate;
		}
		else {
			buttonWasPressed = false;
		}
	}

	return buttonWasPressed;
}

/*
	How long has the key been pressed for
*/
double Input::keyPressedDuration(cocos2d::EventKeyboard::KeyCode code) {
	if (!isKeyPressed(cocos2d::EventKeyboard::KeyCode::KEY_CTRL))
		return 0;																					// Not pressed, so no duration yet

	// Return the amount of time that has elapsed between now and when the user
	// first started holding down the key in milliseconds
	// Start time is the value we hold in our std::map keys
	return std::chrono::duration_cast<std::chrono::milliseconds>
		(std::chrono::high_resolution_clock::now() - keys[code]).count();
}

/*
	Initialise the Input class
	Cross platform: keyboard input needs to be disabled on mobile 
	or the virtual keyboard pops up on screen looking for input
*/
void Input::init(cocos2d::Layer *layer, cocos2d::EventDispatcher *eventDispatcher) {
	if (CC_TARGET_PLATFORM == CC_PLATFORM_WIN32 || CC_TARGET_PLATFORM == CC_PLATFORM_LINUX ||
		CC_TARGET_PLATFORM == CC_PLATFORM_MAC) {
		cocos2d::EventListenerKeyboard* eventListener = cocos2d::EventListenerKeyboard::create();	// JOR replaced auto specifier
		cocos2d::Director::getInstance()->getOpenGLView()->setIMEKeyboardState(true);

		eventListener->onKeyPressed = [=](cocos2d::EventKeyboard::KeyCode keyCode, 
			cocos2d::Event* event) {
			// If a key already exists, do nothing as it will already have a time stamp
			// Otherwise, set's the timestamp to now
			if (keys.find(keyCode) == keys.end()) {
				keys[keyCode] = std::chrono::high_resolution_clock::now();
			}
		};

		eventListener->onKeyReleased = [=](cocos2d::EventKeyboard::KeyCode keyCode, cocos2d::Event* event) {
			keys.erase(keyCode);																	// remove the key.  std::map.erase() doesn't care if the key doesnt exist
		};

		eventDispatcher->addEventListenerWithSceneGraphPriority(eventListener, layer);				// Add an event listener to the scene
	}

	CCLOG("Level %d: Input Initialised", Game::Instance()->getLevel());
}