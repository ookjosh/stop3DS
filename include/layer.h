// Layer.h
#ifndef LAYER_H
#define LAYER_H

#include <3ds.h>
#include <vector>
#include "color.h"
#include "globals.h"
#include "appstate.h"
#include "geometry.h"


class Layer {
public:
	Layer(int paletteType);
	void draw(u8* framebuffer);
	void clear();
	int getZIndex();
	void setZIndex(int index);

	void drawPixel(int x, int y, Color c);
	void drawPixel(int x, int y, int index);
	void drawLine(int x1, int y1, int x2, int y2, Color c);
	void drawLine(int x1, int y1, int x2, int y2, int index);
	void drawBlock4(int x, int y, Color c);

private:
	std::vector<u8> canvas;
	int zIndex = 0;
	int paletteType = 0;

	GlobalState& gState = GlobalState::getInstance();
};

Layer::Layer(int palType) {
	// We are setting = to new vector because that lets us 
	// fill with an initial value more succinctly (vs resize etc)
	switch (palType) {
		case COLOR_16:
			canvas = std::vector<u8>(COLOR_16_BYTES, 0);
			paletteType = palType;
			break;
		case COLOR_256:
			canvas = std::vector<u8>(COLOR_256_BYTES, 0);
			paletteType = palType;
			break;
		case COLOR_FULL:
			canvas = std::vector<u8>(COLOR_FULL_BYTES, 0);
			/*
			for (int i = 0; i < COLOR_FULL_BYTES; i++) {
				if (i % 3 == 0)
				canvas.at(i) = 0x7F;
			}*/

			paletteType = palType;
			break;
		default:
			break;
	}
};

// Draws layer to current framebuffer
// TODO: Support transparency.
// Framebuffer is full 320*240*3 bytes large, so we have to 
// take that into account when filling correctly for the different
// palette options.
void Layer::draw(u8* framebuffer) {
	for (int i = 0; i < canvas.size(); i++) {
		switch(paletteType) {
			case COLOR_FULL:
				// 1 pixel = 3 elements in canvas
				if (canvas[i] == 0 && canvas[i+1] == 0 && canvas[i+2] == 0) {
					// Don't copy anything.
					// Increment by 2 more because 3 bytes per pixel.
					i+=2;
				} else {
					framebuffer[i] = canvas[i];
					framebuffer[i+1] = canvas[i+1];
					framebuffer[i+2] = canvas[i+2];
					i+=2;
				}
				
				break;
			case COLOR_256:

				if (canvas[i] == 0) {
					// Transparent pixel, don't copy anthing
				} else {
					Color px = gState.gColors.getColor(canvas[i]);
					framebuffer[i] = px.b;
					framebuffer[i+1] = px.g;
					framebuffer[i+2] = px.r;
					// Don't do any additional increment because
					// 1 byte per pixel.
				}

				break;

			case COLOR_16:

				break;

			default:
				printf("ERROR in Layer::draw\n");
				break;
		}
		
	}
};

void Layer::clear() {
	printf("Clearing\n");
	int numBytes = COLOR_FULL_BYTES;
	if (paletteType == COLOR_16) {
		numBytes = COLOR_16_BYTES;
	} else if (paletteType == COLOR_256) {
		numBytes = COLOR_256_BYTES;
	}

	for (int i = 0; i < numBytes; i++) {
		canvas[i] = 0;
	}	

}

int Layer::getZIndex() {
	return zIndex;
};

void Layer::setZIndex(int index) {
	zIndex = index;
};

// Draws a pixel at the specified coordinates
// TODO: Support top screen
void Layer::drawPixel(int x, int y, Color c) {
	int pixelIndex = x*240*3 - y*3;
	if (pixelIndex < 0 || pixelIndex > canvas.size()) {
		// How do I want to handle this???
		return;
	}

	//printf("\nDRAWING AT %d, %d, %d\n", pixelIndex, c.b, c.r);

	canvas.at(pixelIndex) = c.b;
	canvas.at(pixelIndex+1) = c.g;
	canvas.at(pixelIndex+2) = c.r;
}

// Draws a pixel referring to a palette
void Layer::drawPixel(int x, int y, int index) {
	int pixelIndex = x*240*3 - y*3;
	if (pixelIndex < 0 || pixelIndex > canvas.size()) {
		// How do I want to handle this???
		return;
	}

	canvas.at(pixelIndex) = index;
}

void Layer::drawLine(int x1, int y1, int x2, int y2, Color c) {

	// Vertical lines
	// 1. determine x2 == x1
	// 2. find y2-y1. if negative step = -1, else 1
	// 3. y1 + step
	// 4. stop when y2 reached.

	int rise, run;
	int slope;
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
			if (pixelIndex > 320*240*3) pixelIndex = -1;
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
			if (pixelIndex > 320*240*3) pixelIndex = -1;
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

// Draws a line between (x1,y1) and (x2,y2) of palette color index.
void Layer::drawLine(int x1, int y1, int x2, int y2, int index) {

}



void Layer::drawBlock4(int x, int y, Color color) {
	drawPixel(x,y, color);
	drawPixel(x,y+1, color);
	drawPixel(x+1, y, color);
	drawPixel(x+1, y+1, color);
}

#endif