// GUI.h
#ifndef GUI_H
#define GUI_H

// Top screen gui stuff... of sorts
#include "globals.h"
#include "color.h"
#include "default_font.h"

#include "appstate.h"
#include "menu.h"
#include <string>

class TopScreen {
public:
	TopScreen();
	void update(u8* fb);
	void updateAnimation(u8* frame);
	void drawLine(int x1, int y1, int x2, int y2, Color c);
	void drawRect(int x, int y, int w, int h, Color c);
	void fillRect(int x, int y, int w, int h, Color c);
	void drawCharacter(int x, int y, int index);
	void drawString(int x, int y, std::string s);
	void drawDemoGui();
	void drawMenuBox(int x, int y, int index);

private:
	std::vector<u8> canvas;
	GlobalState& gState = GlobalState::getInstance();
	Menu menu;
};

TopScreen::TopScreen() {
	canvas = std::vector<u8>(400*240*3, 0);

	// Create demo menu
	menu.addItem(MenuItem(std::string("Onion")));
	menu.addItem(MenuItem(std::string("Frame")));
	menu.addItem(MenuItem(std::string("Tool")));
	menu.addItem(MenuItem(std::string("Color")));

	menu.at(3).addSubMenu(Menu());
	menu.at(3).getSubMenu().addItem(MenuItem(std::string("Red")));
	menu.at(3).getSubMenu().addItem(MenuItem(std::string("Green")));
	menu.at(3).getSubMenu().addItem(MenuItem(std::string("Blue")));
}

void TopScreen::update(u8* framebuffer) {
	// Draw GUI icons and animation frame
	for (int i = 0; i < canvas.size(); i++) {
		framebuffer[i] = canvas[i];
	}
}

void TopScreen::updateAnimation(u8* frame) {
	// Draw current animation to our canvas.
	// TODO: SUPPORT OTHER COLOR TYPES CORRECTLY.

	// So... The frame is indexed from the bottom left corner and
	// increases up by y then over by x. So the middle section here
	// can actually be a contiguous chunk of data that is just inserted.
	// I had previously been thinking of a "normal" screen indexing where
	// (0,0) is top left and you go right and then down. For that case 
	// you would have to have extra logic to copy the right bytes to the
	// middle of the frame, but fortunately this is simpler.

	// We have an offset of 40 columns * 240 pixels per column * 3 bpp
	const int offset = 240*40*3;
	for (int i = 0; i < COLOR_FULL_BYTES; i++) {
		canvas[i+offset] = frame[i];
	}
}

void TopScreen::drawLine(int x1, int y1, int x2, int y2, Color c) {

	// Vertical lines
	// 1. determine x2 == x1
	// 2. find y2-y1. if negative step = -1, else 1
	// 3. y1 + step
	// 4. stop when y2 reached.

	int rise, run, slope;
	bool vertical = false;

	if (x2 == x1) {
		// Catch divide by zero.
		vertical = true;
		rise = (y2 - y1);

	} else {
		// Non vertical line so we get the slope.
		// Verbose here for people unfamiliar.
		rise = (y2 - y1);
		run = (x2 - x1);
		slope = rise/run;
	}
	
	// Temporary coordinate variables in case we need to manipulate them.
	// Especially for non vertical lines.
	int currentX = x1;
	int currentY = y1;
	// The index of the pixel we are drawing.
	int pixelIndex;

	// Vertical lines have infinite slope so we handle them
	// separately. The 'step' variable is to allow the points passed
	// to be left to right or right to left (e.g (50,50) to (100, 50)
	// vs (100, 50) to (50,50)).
	if (vertical) {
		int step = 1;
		if (rise < 0) {
			step = -1;
		}

		// Draws pixels in column.
		for (int i = currentY; i != y2; i+= step) {

			if (i > TOP_SCREEN_HEIGHT || i < 0) {
				continue;
			}
			pixelIndex = (currentX)*240*3 - (i)*3;
			// Check if index is valid.
			if (pixelIndex < 0) pixelIndex = -1;
			if (pixelIndex > 400*240*3) pixelIndex = -1;
			if (pixelIndex < 0) {
				// Not valid, on to next loop cycle
				continue;
			}

			canvas[pixelIndex] = c.b;
			canvas[pixelIndex+1] = c.g;
			canvas[pixelIndex+2] = c.r;
		}
		return;
	}

	// Not vertical, so we do normal plotting
	// Normal lines
	// 1. Starting x and y
	// 2. find slope
	// 3. move in x direction one step at a time, increment y by slope
	// 4. stop when x2,y2 reached.
	int step = 1;
	if (x2 < x1) step = -1;

	for (int i = currentX; i != x2; i+= step) {

		if (currentY > TOP_SCREEN_HEIGHT || currentY < 0) {
			continue;
		}

		pixelIndex = (i)*240*3 - (currentY)*3;
		// Check if valid index
		if (pixelIndex < 0) pixelIndex = -1;
		if (pixelIndex > 400*240*3) pixelIndex = -1;
		if (pixelIndex < 0) {
			// Out of bounds, on to next loop
			continue;
		}

		canvas[pixelIndex] = c.b;
		canvas[pixelIndex+1] = c.g;
		canvas[pixelIndex+2] = c.r;

		currentY += slope;
	}

}

void TopScreen::drawRect(int x, int y, int w, int h, Color c) {
	drawLine(x, y, x+w, y, c);
	drawLine(x+w, y, x+w, y+h, c);
	drawLine(x+w, y+h, x, y+h, c);
	drawLine(x, y+h, x, y, c);
}

void TopScreen::fillRect(int x, int y, int w, int h, Color c) {
	// Determine x,y
	// Find x2 vs x1 - step = 1,-1
	
	// Always starts at (x,y) and extends to (x+w, y+h)
	// so that simplifies things
	int pixelIndex;
	for (int k = y; k < y + h; k++) {	
		if (k > TOP_SCREEN_HEIGHT || k < 0) {
			continue;
		}
		for (int i = x; i < x + w; i++) {
			pixelIndex = (i)*240*3 - (k)*3;
			// Check if valid index
			if (pixelIndex < 0) pixelIndex = -1;
			if (pixelIndex > 400*240*3) pixelIndex = -1;
			if (pixelIndex < 0) {
				// Out of bounds, on to next loop
				continue;
			}

			canvas[pixelIndex] = c.b;
			canvas[pixelIndex+1] = c.g;
			canvas[pixelIndex+2] = c.r;
		}
	}


}

// Draws character from font.
void TopScreen::drawCharacter(int x, int y, int index) {
	int cx = x;
	int cy = y;

	// Each glyph is 8x8, 8 bits * 8 bytes
	for (int i = 0; i < 8; i++) {
		char current = default_font_bin[index*8 + i];
		for (int k = 8; k > 0; k--) {
			int pixelIndex = cx*240*3 - cy*3;

			bool bit = current>>k & 0x01;
			if (bit) {
				canvas[pixelIndex] = 0;
				canvas[pixelIndex+1] = 64;
				canvas[pixelIndex+2] = 255;		
			} else {
				//canvas[pixelIndex] = 0;
				//canvas[pixelIndex+1] = 0;
				//canvas[pixelIndex+2] = 0;
			}

			cx += 1;
		}

		// To make italic change this up or down 1 or 2
		cx -=8;
		cy += 1;
		
	}
}

void TopScreen::drawString(int x, int y, std::string s) {
	int xOffset = 0;
	for (std::string::iterator it = s.begin(); it != s.end(); ++it) {
		drawCharacter((xOffset)+x, y, *it);
		xOffset+=8;
	}
}

void TopScreen::drawDemoGui() {
	fillRect(0,1,400,320, colorList[1]);

	fillRect(0,40,40,40, colorList[7]);
	fillRect(0,80,40,40, colorList[6]);
	fillRect(0,120,40,40, colorList[5]);
	fillRect(0,160,40,40, colorList[4]);

	drawCharacter(16, 48, 'R');
	drawString(0, 64, "Clear");

	fillRect(360, 40, 40, 40, gState.gColors.getColor(gState.currentColor));
	drawString(360, 40, "Color");

	drawString(360,224, "Layer:");
	drawCharacter(360, 232, int_to_font(gState.currentLayer));

	// Draw menu items.
	for (int i = 0; i < menu.size(); i++) {
		if (i == gState.topMenu && gState.subMenu < 0) {
			// Highlight selected top menu item if no submenu
			drawMenuBox(45+45*i, 195, 28);
		} else {
			// Draw top menu box normal
			drawMenuBox(45+45*i, 195, 16);

			// Draw submenu with highlighting
			if (menu.at(gState.topMenu).hasSubMenu()) {
				for (int k = 0; k < menu.at(i).getSubMenu().size(); k++) {
					if (k == gState.subMenu) {
						drawMenuBox(45+45*i, 150-k*45, 28);
					} else {
						drawMenuBox(45+45*i, 150-k*45, 22);	
					}
					
					drawString(45+45*i, 150-k*45, menu.at(i).getSubMenu().at(k).getName());
				}
			}
		}

		drawString(45+45*i, 200, menu.at(i).getName());

	}

	// Update menu state. I don't like this but it could be ok...
	if (gState.topMenu > menu.size() - 1) {
		gState.topMenu = menu.size() - 1;
	}
	if (gState.topMenu < 0) {
		gState.topMenu = 0;
	}

	if (gState.subMenu < -1) {
		gState.subMenu = -1;
	}
	if (gState.subMenu > menu.at(gState.topMenu).getSubMenu().size() - 1) {
		gState.subMenu = menu.at(gState.topMenu).getSubMenu().size() - 1;
	}

	gState.hasSubMenu = menu.at(gState.topMenu).hasSubMenu();

}

void TopScreen::drawMenuBox(int x, int y, int index) {
	fillRect(x, y, 45, 45, gState.gColors.getColor(index));
	drawRect(x, y, 45, 45, gState.gColors.getColor(0));
}

#endif