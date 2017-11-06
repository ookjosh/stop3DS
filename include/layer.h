// Layer.h
#ifndef LAYER_H
#define LAYER_H

#include <3ds.h>
#include <vector>


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
				canvas[i] = 0x7F;
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

#endif