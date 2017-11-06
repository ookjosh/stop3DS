// Frame.h
#ifndef FRAME_H
#define FRAME_H

#include <3ds.h>
#include <vector>
#include "layer.h"

class Frame {
	
public:
	Frame();
	void draw(u8* fb);
	void clear(int frame);

private:
	std::vector<Layer> layers;
};

// Constructor for frame class. Populates the layers vector.
Frame::Frame() {
	layers.push_back(Layer(COLOR_FULL_BYTES));
	layers.push_back(Layer(COLOR_FULL_BYTES));
};
// Draws all of the layers contained in the layers vector.
void Frame::draw(u8* fb) {
	for (std::vector<Layer>::iterator it = layers.begin(); it != layers.end(); ++it) {
		it->draw(fb);
	}
};

void Frame::clear(int frame) {

};

#endif