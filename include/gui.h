// GUI.h
#ifndef GUI_H
#define GUI_H

// Top screen gui stuff... of sorts
#include "globals.h"
#include "color.h"

class TopScreen {
public:
	TopScreen();
	void update(u8* fb);
	void drawLine(int x1, int y1, int x2, int y2, Color c);
	void drawRect(int x, int y, int w, int h, Color c);
	void fillRect(int x, int y, int w, int h, Color c);

private:
	std::vector<u8> canvas;
};

TopScreen::TopScreen() {
	canvas = std::vector<u8>(400*240*3, 0);
}

void TopScreen::update(u8* framebuffer) {
	// Draw GUI icons and animation frame
	for (int i = 0; i < canvas.size(); i++) {
		framebuffer[i] = canvas[i];
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

#endif