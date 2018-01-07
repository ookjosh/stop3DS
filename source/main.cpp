#include <3ds.h>
#include <stdio.h>
#include <string.h> // memcpy
#include <vector>
#include <malloc.h>
#include <deque>
#include <functional>

#include "color.h"
#include "animation.h"
#include "gui.h"

#include "appstate.h"
#include "input.h"

bool animating = false;
int tick = 0;
int curFrame = 0;
// fps_ticks/60 is fps
int fps_ticks = 60; 
bool onionSkin = false;


Animation currentAnimation;
TopScreen topScreen;

GlobalState& g = GlobalState::getInstance();

InputManager input;

int main(int argc, char **argv) {
	gfxInitDefault();
	//consoleInit(GFX_TOP, NULL);

	gfxSetDoubleBuffering(GFX_BOTTOM, true);
	gfxSetDoubleBuffering(GFX_TOP, false);

	// Initialize colors
	initColors();

	g.currentAnimation = &currentAnimation;
	g.topGui = &topScreen;

	// Assign our exit key (unimplemented but w/e!)
	input.getSignal(KEY_START).connect([](){g.exitApplication = true;});
	input.setInputs(MODE_DRAWING);

	// Testing signals... :-D
	//input.testSignal.connect([](){g.currentColor=16;});
	//input.getSignal(KEY_DUP).connect([](){g.currentColor=16;});

	//topScreen.drawRect(50, 50, 100, 100, colorList.at(currentColor+4));
	//topScreen.fillRect(0,0, 40, 240, colorList.at(g.currentColor+4));
	//topScreen.fillRect(360,0, 40, 240, colorList.at(g.currentColor+4));

	while (aptMainLoop()) {

		//printf("\x1b[0;0HUp: Layer 1");
		//printf("\x1b[1;0HDown: Layer 0");
		//printf("\x1b[2;0HR: Clear current layer");

		input.update(MODE_DRAWING);

		// Update bottom framebuffer.
		u8* fb = gfxGetFramebuffer(GFX_BOTTOM, GFX_LEFT, NULL, NULL);
		currentAnimation.update(fb);
		//currentAnimation.getScene(0).getFrame(0).getLayer(0).draw(fb);

		u8* topfb = gfxGetFramebuffer(GFX_TOP, GFX_LEFT, NULL, NULL);

		topScreen.drawDemoGui();
		
		topScreen.update(topfb);


		gfxFlushBuffers();
		gfxSwapBuffers();
		gspWaitForVBlank();
	}

	gfxExit();

	return 0;
}