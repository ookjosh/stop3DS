// Input.h

#include "appstate.h"

class InputManager {
public:
	InputManager();
	void update();

private:
	u16 oldTouchX;
	u16 oldTouchY;
	u16 currentTouchX;
	u16 currentTouchY;
	u32 kDown;
	touchPosition touch;
	bool touchLastTick = false;
}

InputManager::InputManager() {

}

// Updates history of inputs then acts on them based on what
// mode the app is in.
void InputManager::update(int appMode) {
	hidScanInput();
	kDown = hidKeysDown();
	hidTouchRead(&touch);


	if (kDown & KEY_UP) {
		if (MODE_DRAWING) {

		} else if (MODE_MENU) {

		}
	}

	if (kDown & KEY_LEFT) {
		if (MODE_DRAWING) {

		} else if (MODE_MENU) {
			
		}
	}

	if (kDown & KEY_DOWN) {
		if (MODE_DRAWING) {

		} else if (MODE_MENU) {
			
		}
	}

	if (kDown & KEY_RIGHT) {
		if (MODE_DRAWING) {

		} else if (MODE_MENU) {
			
		}
	}

	if (kDown & KEY_A) {
		if (MODE_DRAWING) {

		} else if (MODE_MENU) {
			
		}
	}

	if (kDown & KEY_B) {
		if (MODE_DRAWING) {

		} else if (MODE_MENU) {
			
		}
	}

	if (kDown & KEY_X) {
		if (MODE_DRAWING) {

		} else if (MODE_MENU) {
			
		}
	}

	if (kDown & KEY_Y) {
		if (MODE_DRAWING) {

		} else if (MODE_MENU) {
			
		}
	}

	if (kDown & KEY_L) {
		if (MODE_DRAWING) {

		} else if (MODE_MENU) {
			
		}
	}
	
	if (kDown & KEY_R) {
		if (MODE_DRAWING) {

		} else if (MODE_MENU) {
			
		}
	}
}

