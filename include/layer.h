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
	void drawBlock4(int x, int y, int index);

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
	//for (int i = 0; i < canvas.size(); i++) {

	int debugCount = 0;
	int debugCount2 = 0;
	for (int i =  0; i < COLOR_FULL_BYTES; i++) {

		// Forward declaration of paletted index ahead of switch
		int pIndex = 0;
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

				if (canvas[i/3] == 0) {
					// Transparent pixel, don't copy anthing
					i+=2;
					pIndex += 1;
					debugCount += 1;
				} else {
					Color px = gState.gColors.getColor(canvas[i/3]);
					framebuffer[i] = px.b;
					framebuffer[i+1] = px.g;
					framebuffer[i+2] = px.r;
					i+=2;
					pIndex += 1;
					debugCount2 += 1;
				}

				

				break;

			case COLOR_16:

				break;

			default:
				printf("ERROR in Layer::draw\n");
				break;
		}
		
	}

	printf("Debug 1: %d, %d\n", debugCount, debugCount2);
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
	int pixelIndex = 0;

	switch (paletteType) {
		case COLOR_FULL:
			{
				pixelIndex = x*240*3 - y*3;
				if (pixelIndex < 0 || pixelIndex > canvas.size()) {
					// How do I want to handle this???
					return;
				}
				Color c = gState.gColors.getColor(index);
				canvas.at(pixelIndex) = c.b;
				canvas.at(pixelIndex+1) = c.g;
				canvas.at(pixelIndex+2) = c.r;
			}
			break;
		case COLOR_256:
			pixelIndex = x*240 - y;
			if (pixelIndex < 0 || pixelIndex > canvas.size()) {
				// How do I want to handle this???
				return;
			}
			canvas.at(pixelIndex) = index;
			break;
		case COLOR_16:
			canvas.at(pixelIndex) = index;
			break;
	}
}

void Layer::drawLine(int x1, int y1, int x2, int y2, Color c) {

	std::vector<Point> points = line(Point({.x = x1, .y = y1}), Point({.x = x2, .y = y2}));
	
	for (std::vector<Point>::iterator it = points.begin(); it != points.end(); ++it) {
		drawPixel(it->x, it->y, c);
	}

}

// Draws a line between (x1,y1) and (x2,y2) of palette color index.
void Layer::drawLine(int x1, int y1, int x2, int y2, int index) {
	std::vector<Point> points = line(Point({.x = x1, .y = y1}), Point({.x = x2, .y = y2}));
	
	for (std::vector<Point>::iterator it = points.begin(); it != points.end(); ++it) {
		drawPixel(it->x, it->y, index);
	}
}



void Layer::drawBlock4(int x, int y, Color color) {
	drawPixel(x,y, color);
	drawPixel(x,y+1, color);
	drawPixel(x+1, y, color);
	drawPixel(x+1, y+1, color);
}

void Layer::drawBlock4(int x, int y, int index) {
	drawPixel(x,y, index);
	drawPixel(x,y+1, index);
	drawPixel(x+1, y, index);
	drawPixel(x+1, y+1, index);
}

#endif