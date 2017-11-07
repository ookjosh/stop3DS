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
	canvas = std::vector<u8>(COLOR_FULL_BYTES, 0);
}

void TopScreen::update(u8* framebuffer) {
	// Draw GUI icons and animation frame
	for (int i = 0; i < canvas.size(); i++) {
		framebuffer[i] = canvas[i];
	}
}



void TopScreen::drawLine(int x1, int y1, int x2, int y2, Color c) {

	int rise, run;
	bool vertical = false;

	if (x2 == x1) {
		// Catch divide by zero.
		vertical = true;
		rise = (y2 - y1);

	} else {
		rise = (y2 - y1);
		run = (x2 - x1);
	}
	
	int currentX = x1;
	int currentY = y1;
	int pixelIndex;

	// Working.
	if (vertical) {
		// Draws pixels in column.
		for (int i = 0; i < rise; i++) {
			pixelIndex = (currentX)*240*3 - (currentY+i)*3;
			if (pixelIndex < 0) pixelIndex = 0;

			canvas[pixelIndex] = c.b;
			canvas[pixelIndex+1] = c.g;
			canvas[pixelIndex+2] = c.r;
		}
		return;
	}


	while (x2 - currentX > 0) {
		pixelIndex = currentX*240*3 -currentY*3;
		if (pixelIndex < 0) pixelIndex = 0;

		canvas[pixelIndex] = c.b;
		canvas[pixelIndex+1] = c.g;
		canvas[pixelIndex+2] = c.r;
		/*
		for (int i = 0; i < rise; i++) {
			pixelIndex = currentX*240*3 - (currentY+i)*3;
			if (pixelIndex < 0) pixelIndex = 0;

			canvas[pixelIndex] = c.b;
			canvas[pixelIndex+1] = c.g;
			canvas[pixelIndex+2] = c.r;
		}*/
		for (int i = 0; i < run; i++) {
			pixelIndex = (currentX+i)*240*3 - (currentY+(i*rise/run))*3;
			if (pixelIndex < 0) pixelIndex = 0;

			canvas[pixelIndex] = c.b;
			canvas[pixelIndex+1] = c.g;
			canvas[pixelIndex+2] = c.r;
		}

		currentY += rise;
		currentX += run;

	};


}

void TopScreen::drawRect(int x, int y, int w, int h, Color c) {
	drawLine(x, y, x+w, y, c);
	drawLine(x+w, y, x+w, y+h, c);
	drawLine(x+w, y+h, x, y+h, c);
	drawLine(x, y+h, x, y, c);
}

void TopScreen::fillRect(int x, int y, int w, int h, Color c) {

}

#endif