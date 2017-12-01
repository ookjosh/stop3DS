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

	Layer& getLayer(int index);

private:
	std::vector<Layer> layers;
};

// Constructor for frame class. Populates the layers vector.
Frame::Frame() {
	layers.push_back(Layer(COLOR_FULL));
	layers.push_back(Layer(COLOR_FULL));
};
// Draws all of the layers contained in the layers vector.
void Frame::draw(u8* fb) {
	//printf("Number of layers: %d\n", layers.size());
	for (std::vector<Layer>::iterator it = layers.begin(); it != layers.end(); ++it) {
		it->draw(fb);
	}
};

void Frame::clear(int frame) {

};

// Returns a reference to the layer of interest.
Layer& Frame::getLayer(int index) {
	if (index > layers.size()) {
		return layers.at(0);
	}
	return layers.at(index);
}

#endif