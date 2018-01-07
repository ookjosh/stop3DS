// Input.h

#include "appstate.h"
//For now...
#include "color.h"

#include "signal.h"

class InputManager {
public:
	InputManager();
	void update(int appMode);

	void setInputs(int mode);

	// Testing...
	Signal<> testSignal;
	Signal<>& getSignal(int key);
private:

	GlobalState& globalState = GlobalState::getInstance();

	u16 oldTouchX;
	u16 oldTouchY;
	u16 currentTouchX;
	u16 currentTouchY;
	u32 kDown;
	touchPosition touch;
	touchPosition oldTouch;
	bool touchLastTick = false;

	void handleTouch(touchPosition touch);
	void handleKeys(u32 kDown);

	Signal<> signal_A;
	Signal<> signal_B;
	Signal<> signal_X;
	Signal<> signal_Y;
	Signal<> signal_DUP;
	Signal<> signal_DDOWN;
	Signal<> signal_DLEFT;
	Signal<> signal_DRIGHT;
	Signal<> signal_L;
	Signal<> signal_R;
	Signal<> signal_ZL;
	Signal<> signal_ZR;
	Signal<> signal_START;
	Signal<> signal_SELECT;
	Signal<> signal_TOUCH;

	
};

Signal<>& InputManager::getSignal(int key) {
	switch (key) {
		case KEY_A:
			return signal_A;
			break;
		case KEY_B:
			return signal_B;
			break;
		case KEY_X:
			return signal_X;
			break;
		case KEY_Y:
			return signal_Y;
			break;
		// DPad only. KEY_UP/etc is dpad or circle pad.
		case KEY_DUP:
			return signal_DUP;
			break;
		case KEY_DDOWN:
			return signal_DDOWN;
			break;
		case KEY_DLEFT:
			return signal_DLEFT;
			break;
		case KEY_DRIGHT:
			return signal_DRIGHT;
			break;
		case KEY_START:
			return signal_START;
			break;
		case KEY_SELECT:
			return signal_SELECT;
			break;
		case KEY_L:
			return signal_L;
			break;
		case KEY_R:
			return signal_R;
			break;
		case KEY_ZL:
			return signal_ZL;
			break;
		case KEY_ZR:
			return signal_ZR;
			break;
		case KEY_TOUCH:
			return signal_TOUCH;
			break;

		default:

			break;
	}
	
}

InputManager::InputManager() {

}

// Updates history of inputs then acts on them based on what
// mode the app is in.
void InputManager::update(int appMode) {
	// update inputs
	hidScanInput();
	kDown = hidKeysDown();
	hidTouchRead(&touch);

	// Act on them
	handleTouch(touch);
	handleKeys(kDown);

	
}

void InputManager::handleKeys(u32 kDown) {
	if (kDown & KEY_START) {
		signal_START.emit();
	}
	if (kDown & KEY_UP) {
		signal_DUP.emit();
	}

	if (kDown & KEY_LEFT) {
		signal_DLEFT.emit();
	}

	if (kDown & KEY_DOWN) {
		signal_DDOWN.emit();
	}

	if (kDown & KEY_RIGHT) {
		signal_DRIGHT.emit();
	}

	if (kDown & KEY_A) {
		signal_A.emit();
	}

	if (kDown & KEY_B) {
		signal_B.emit();
	}

	if (kDown & KEY_X) {
		signal_X.emit();
	}

	if (kDown & KEY_Y) {
		signal_Y.emit();
	}

	if (kDown & KEY_L) {
		signal_L.emit();
	}

	if (kDown & KEY_R) {
		signal_R.emit();
	}
}

void InputManager::handleTouch(touchPosition touch) {
	if (MODE_DRAWING) {
		//printf("\x1b[0;0HX: %d, Y: %d, %s\n", touch.px, touch.py, touchLastTick ? "true": "false");
		bool touchThisTick = false;
		u32 held = keysHeld();
		if (held & KEY_TOUCH || kDown & KEY_TOUCH) {
			touchThisTick = true;
			//printf("Touch\n");
		}
		currentTouchX = touch.px;
		currentTouchY = touch.py;



		//printf("Last: %d, Now: %d\n", touchLastTick, touchThisTick);

		if (touchLastTick == touchThisTick && (oldTouchX == currentTouchX && oldTouchY == currentTouchY)) {
			//printf("No change\n");
			// No change, no need to draw anything.
			//return;
		}

		int penSize = 0;
		switch (globalState.currentTool) {
			case PEN_1:
					penSize = 1;
				break;
			case PEN_4:
					penSize = 4;
				break;
			case FLOOD:
					if (touchThisTick)
					globalState.currentAnimation->getScene(globalState.currentScene).getFrame(globalState.currentFrame).getLayer(globalState.currentLayer).floodFill(currentTouchX, currentTouchY, globalState.currentColor);		
				break;
		}

		///
		if (touchLastTick && touchThisTick) {
			
			globalState.currentAnimation->getScene(globalState.currentScene).getFrame(globalState.currentFrame).getLayer(globalState.currentLayer).drawLine(currentTouchX, currentTouchY, oldTouchX, oldTouchY, penSize, globalState.currentColor);
		} else if (touchThisTick) {
				globalState.currentAnimation->getScene(globalState.currentScene).getFrame(globalState.currentFrame).getLayer(globalState.currentLayer).drawPixel(currentTouchX, currentTouchY, penSize, globalState.currentColor);	

		}
		///

		touchLastTick = touchThisTick;
		oldTouchX = currentTouchX;
		oldTouchY = currentTouchY;

	} else if (MODE_MENU) {

	}
}


// Sets our input assignments in a centralized location. This makes assigning/reassigning signals and their functions
// very simple.
void InputManager::setInputs(int mode) {
	switch (mode) {
		// Our main mode for 99% of things so far.
		case MODE_DRAWING:
			signal_A.connect([&](){});
			signal_B.connect([&](){});
			signal_X.connect([&](){
				if (globalState.topMenu == 3) {
					globalState.currentColor = 16+globalState.subMenu;		
				}

				if (globalState.topMenu == 2) {
					switch (globalState.subMenu) {
						case 0:
							globalState.currentTool = PEN_1;
							break;
						case 1:
							globalState.currentTool = PEN_4;
							break;
						case 2:
							globalState.currentTool = FLOOD;
							break;
					}
				}
			});
			signal_Y.connect([&](){});
			signal_SELECT.connect([&](){});
			signal_START.connect([&](){});
			signal_R.connect([&](){
				int index = globalState.topGui->getMenu().index();
				globalState.topGui->getMenu().at(index).increment();
				globalState.topGui->fillRect(50,20,50,50, colorList.at(5));
			});
			signal_L.connect([&](){
				globalState.topMenu--;
				if (globalState.topMenu < 0) {
					globalState.topMenu = 0;
				}
				int index = globalState.topGui->getMenu().index();
				globalState.topGui->getMenu().at(index).decrement();
			});
			signal_ZR.connect([&](){});
			signal_ZL.connect([&](){});
			signal_DRIGHT.connect([&](){
				globalState.topMenu++;
				
			});
			signal_DLEFT.connect([&](){
				globalState.topMenu--;
				if (globalState.topMenu < 0) {
					globalState.topMenu = 0;
				}
			});
			signal_DUP.connect([&](){
				if (globalState.hasSubMenu) {
					globalState.subMenu++;
				}
			});
			signal_DDOWN.connect([&](){
				if (globalState.hasSubMenu) {
					globalState.subMenu--;
				}
			});


			break;

		case MODE_MENU:
			break;
		default:
			break;

	}
}