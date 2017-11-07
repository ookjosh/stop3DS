// Layer.h
#ifndef LAYER_H
#define LAYER_H

#include <3ds.h>
#include <vector>
#include "color.h"


#define COLOR_16 0
#define COLOR_256 1
#define COLOR_FULL 2

// 3 bytes per pixel
#define COLOR_FULL_BYTES (320*240*3)
// 1 nybble per pixel, so 320*240*(1/2)
#define COLOR_16_BYTES (320*120)
// 1 byte per pixel
#define COLOR_256_BYTES (320*240)

class Layer {
public:
	Layer(int paletteType);
	void draw(u8* framebuffer);
	void clear();
	int getZIndex();
	void setZIndex(int index);

	void drawPixel(int x, int y, Color c);
	void drawBlock4(int x, int y, Color c);

private:
	std::vector<u8> canvas;
	int zIndex = 0;
	int paletteType = 0;
};

Layer::Layer(int paletteType) {
	// We are setting = to new vector because that lets us 
	// fill with an initial value more succintly (vs resize etc)
	switch (paletteType) {
		case COLOR_16:
			canvas = std::vector<u8>(COLOR_16_BYTES, 0);
			break;
		case COLOR_256:
			canvas = std::vector<u8>(COLOR_256_BYTES, 0);
			break;
		case COLOR_FULL:
			canvas = std::vector<u8>(COLOR_FULL_BYTES, 0);
			for (int i = 0; i < COLOR_FULL_BYTES; i++) {
				if (i % 3 == 0)
				canvas.at(i) = 0x7F;
			}
			break;
		default:
			break;
	}
};

// Draws layer to current framebuffer
// TODO: Support transparency.
void Layer::draw(u8* framebuffer) {
	for (int i = 0; i < canvas.size(); i++) {
		framebuffer[i] = canvas[i];
	}
};

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

	printf("\nDRAWING AT %d, %d, %d\n", pixelIndex, c.b, c.r);

	canvas.at(pixelIndex) = c.b;
	canvas.at(pixelIndex+1) = c.g;
	canvas.at(pixelIndex+2) = c.r;
}

void Layer::drawBlock4(int x, int y, Color color) {
	drawPixel(x,y, color);
	drawPixel(x,y+1, color);
	drawPixel(x+1, y, color);
	drawPixel(x+1, y+1, color);
}

#endif