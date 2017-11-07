// Input.h

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

void InputManager::update() {
	hidScanInput();
	kDown = hidKeysDown();
	hidTouchRead(&touch);

	if (touch.px == 0 && touch.py == 0) {
		touchLastTick = false;
	}

	if (touchLastTick) {
		
	}



}