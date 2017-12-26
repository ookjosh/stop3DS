// Input.h

#include "appstate.h"
//For now...
#include "color.h"

#include "signal.h"

class InputManager {
public:
	InputManager();
	void update(int appMode);

	// Testing...
	Signal<> testSignal;

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
};

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
		globalState.exitApplication = true;
	}
	if (kDown & KEY_UP) {
		if (MODE_DRAWING) {
			//globalState.currentLayer = 1;
			//printf("Current layer: %d", globalState.currentLayer);
			if (globalState.hasSubMenu) {
				globalState.subMenu++;
			}
		} else if (MODE_MENU) {

		}
	}

	if (kDown & KEY_LEFT) {
		if (MODE_DRAWING) {
			
			//if (globalState.currentColor > 0) {
			//	globalState.currentColor--;
			//}
		} else if (MODE_MENU) {
			
		}
	}

	if (kDown & KEY_DOWN) {
		if (MODE_DRAWING) {
			//globalState.currentLayer = 0;
			//printf("Current layer: %d", globalState.currentLayer);
			if (globalState.hasSubMenu) {
				globalState.subMenu--;
			}

		} else if (MODE_MENU) {
			
		}
	}

	if (kDown & KEY_RIGHT) {
		if (MODE_DRAWING) {
			//if (globalState.currentColor < globalState.gColors.size()-1) {
			//	globalState.currentColor++;
			//}
		} else if (MODE_MENU) {
			
		}
	}

	if (kDown & KEY_A) {
		if (MODE_DRAWING) {
			testSignal.emit();
		} else if (MODE_MENU) {
			
		}
	}

	if (kDown & KEY_B) {
		if (MODE_DRAWING) {
			//globalState.animating = !globalState.animating;

		} else if (MODE_MENU) {
			
		}
	}

	if (kDown & KEY_X) {
		if (MODE_DRAWING) {
			//globalState.fps_ticks -= 5;
			//globalState.addFrame = true;
			//globalState.currentFrame++;

		} else if (MODE_MENU) {
			
		}
	}

	if (kDown & KEY_Y) {
		if (MODE_DRAWING) {
			//globalState.fps_ticks += 5;
			//globalState.currentFrame--;
			//if (globalState.currentFrame < 0) {
			//	globalState.currentFrame = 0;
			//}
		} else if (MODE_MENU) {
			
		}
	}

	if (kDown & KEY_L) {
		if (MODE_DRAWING) {
			//globalState.onionSkin = !globalState.onionSkin;
			globalState.topMenu--;
		} else if (MODE_MENU) {
			
		}
	}

	if (kDown & KEY_R) {
		if (MODE_DRAWING) {
			//printf("CleARING\n");
			//globalState.currentAnimation->getScene(globalState.currentScene).getFrame(globalState.currentFrame).getLayer(globalState.currentLayer).clear();
			globalState.topMenu++;
		} else if (MODE_MENU) {
			
		}
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

		if (touchLastTick && touchThisTick) {
			
			globalState.currentAnimation->getScene(globalState.currentScene).getFrame(globalState.currentFrame).getLayer(globalState.currentLayer).drawLine(currentTouchX, currentTouchY, oldTouchX, oldTouchY, 4, globalState.currentColor);
		} else if (touchThisTick) {
			printf("Dot\n");
			if (held & KEY_A) {
				printf("Held\n");
				globalState.currentAnimation->getScene(globalState.currentScene).getFrame(globalState.currentFrame).getLayer(globalState.currentLayer).floodFill(currentTouchX, currentTouchY, globalState.currentColor);		
			} else {
				globalState.currentAnimation->getScene(globalState.currentScene).getFrame(globalState.currentFrame).getLayer(globalState.currentLayer).drawPixel(currentTouchX, currentTouchY, 4, globalState.currentColor);	
			}
			

		}

		touchLastTick = touchThisTick;
		oldTouchX = currentTouchX;
		oldTouchY = currentTouchY;

	} else if (MODE_MENU) {

	}
}