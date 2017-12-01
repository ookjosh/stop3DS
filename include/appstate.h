// Application state
/*
  Contains the global state of the application. Only things that must be
  accessible can live here - everything else should either be stateless or
  derive state from this information.
 */

#ifndef _APP_STATE_H
#define _APP_STATE_H

#include "animation.h"


enum {
	MODE_MENU,
	MODE_DRAWING
};

class GlobalState {
public:

	// Use: GlobalState& state = getInstance();
	static GlobalState& getInstance() {
		static GlobalState instance;
		return instance;
	}

	int x = 0;
	int appMode = MODE_DRAWING;

	// We use a pointer because it can be reassigned.
	Animation* currentAnimation = NULL;
	// Could easily be inside the Animation construct.
	int currentScene = 0;
	int currentFrame = 0;
	int currentLayer = 0;


	u8 currentColor = 0;
	bool animating = false;
	int fps_ticks = 5;
	bool onionSkin = false;

	bool exitApplication = false;

	GlobalState(GlobalState const&) = delete; // Remain unimplemented
	void operator=(GlobalState const&) = delete; // Remain unimplemented
private:

	//int x = 0;
	GlobalState() {};

	// For C++11 these should have `= delete;` instead and be public.
	//GlobalState(GlobalState const&); // Remain unimplemented
	//void operator=(GlobalState const&); // Remain unimplemented
};


#endif