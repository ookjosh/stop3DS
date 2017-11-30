// Application state
/*
  Contains the global state of the application. Only things that must be
  accessible can live here - everything else should either be stateless or
  derive state from this information.
 */

#ifndef _APP_STATE_H
#define _APP_STATE_H

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