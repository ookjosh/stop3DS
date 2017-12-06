// Layer.h
#ifndef LAYER_H
#define LAYER_H

#include <3ds.h>
#include <vector>
#include <queue>
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
	void drawPixel(int x, int y, int size, int index);
	void drawLine(int x1, int y1, int x2, int y2, Color c);
	void drawLine(int x1, int y1, int x2, int y2, int size, int index);
	void drawBlock4(int x, int y, Color c);
	void drawBlock4(int x, int y, int index);

	void floodFill(int x, int y, int index);
	void floodIterative(int x, int y, int current_index, int target_index);

private:
	std::vector<u8> canvas;
	int zIndex = 0;
	int paletteType = 0;

	GlobalState& gState = GlobalState::getInstance();

	void floodRecursion(int x, int y, int index, std::vector<int>& list);
	void floodRecursion(int x, int y, int current_index, int target_index);
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

void Layer::drawPixel(int x, int y, int size, int index) {
	if (size > 1) {
		for (int i = 0; i < size; i++) {
			drawPixel(x, y, index);
			drawPixel(x, y+i, index);
			drawPixel(x+i, y, index);
			drawPixel(x+i, y+i, index);
		}	
	} else {
		drawPixel(x, y, index);
	}
	
}

void Layer::drawLine(int x1, int y1, int x2, int y2, Color c) {

	std::vector<Point> points = line(Point({.x = x1, .y = y1}), Point({.x = x2, .y = y2}));
	
	for (std::vector<Point>::iterator it = points.begin(); it != points.end(); ++it) {
		drawPixel(it->x, it->y, c);
	}

}

// Draws a line between (x1,y1) and (x2,y2) of palette color index.
void Layer::drawLine(int x1, int y1, int x2, int y2, int size, int index) {
	std::vector<Point> points = line(Point({.x = x1, .y = y1}), Point({.x = x2, .y = y2}));
	
	for (std::vector<Point>::iterator it = points.begin(); it != points.end(); ++it) {
		drawPixel(it->x, it->y, size, index);
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


// Only implemented for 256 colors atm.
void Layer::floodFill(int x, int y, int index) {
	/*
		1. Get pixel index for coord
		2. Check that pixels color
		3. If color != index, continue
		4. Change that color to index
		5. Check each adjacent pixel
			a. If same as original color add to list
			b. Repeat 5 for each that does so
		6. Once list is complete, iterate through it
		   drawing each pixel = index.
	*/
	int pixelIndex = x*240 - y;
	int original = canvas[pixelIndex];
	if (original == index) return;

	floodIterative(x,y,original,index);
	/*
	// Allocate full size so we don't have to realloc
	std::vector<int> pixelsToChange;
	pixelsToChange.reserve(320*240);
	int pixelIndex = x*240 - y;
	if (pixelIndex < 0 || pixelIndex > canvas.size()) {
				// How do I want to handle this???
				return;
	}
	int originalColor = canvas[pixelIndex];
	if (originalColor == index) {
		// Already target color so don't worry.
		return;
	}

	floodRecursion(x,y, originalColor, index);
	//floodRecursion(x, y, originalColor, pixelsToChange);

	for (std::vector<int>::iterator it = pixelsToChange.begin(); it != pixelsToChange.end(); ++it) {
		//canvas[*it] = index;
	}
	*/

}


// Checks pixel (x,y) to see if it matches color "target_index", if so adds to
// list and recursively checks surrounding pixels.
void Layer::floodRecursion(int x, int y, int target_index, std::vector<int>& list) {
	if(x < 0 || x > 320 || y < 0 || y > 240) {
		return;
	}
	printf("R: %d, %d, %d\n", x, y, list.size());
	int pixelIndex = x*240 - y;
	if (pixelIndex < 0 || pixelIndex > canvas.size()) {
				// How do I want to handle this???
				return;
	}

	if (canvas[pixelIndex] == target_index) {
		list.push_back(pixelIndex);
		floodRecursion(x+1, y, target_index, list);
		//floodRecursion(x-1, y, target_index, list);
		//floodRecursion(x, y+1, target_index, list);
		floodRecursion(x, y-1, target_index, list);
	}

}

void Layer::floodRecursion(int x, int y, int current_index, int target_index) {
	printf("R: %d, %d, %d\n", x, y, current_index);
	if(x < 0 || x > 320 || y < 0 || y > 240) {
		return;
	}
	int pixelIndex = x*240 - y;
	if (pixelIndex < 0 || pixelIndex > canvas.size()) {
				// How do I want to handle this???
				return;
	}

	if (canvas[pixelIndex] == current_index) {
		canvas[pixelIndex] = target_index;
		floodRecursion(x+1, y, current_index, target_index);
		floodRecursion(x-1, y, current_index, target_index);
		floodRecursion(x, y+1, current_index, target_index);
		//floodRecursion(x, y-1, current_index, target_index);
	}

}

void Layer::floodIterative(int x, int y, int current_index, int target_index) {
	
	std::queue<int> inProgress;
	std::vector<int> flood;

	flood.reserve(320*240);

	int cx = x;
	int cy = y;

	int pixelIndex = cx*240 - cy;

	
	//int getX = (pixelIndex) / 240 + 1;
	//int getY = 240 - (pixelIndex % 240);
	// Start our queue.
	inProgress.push(pixelIndex);
	
	while (!inProgress.empty()) {
			//if (canvas[(cx+1)*240 - cy] == current_index) {
			auto getX = [](int index) { return (index) / 240 + 1; };
			auto getY = [](int index) { return 240 - (index % 240);};
			auto getIndex = [](int x, int y) {return (x*240 - y) > 0 ? (x*240-y) : 0; };

			
			cx = getX(inProgress.front()-240);
			cy = getY(inProgress.front());
			pixelIndex = getIndex(cx, cy);
			//printf("X: %d\n", pixelIndex);
			
			if (canvas[pixelIndex] == current_index && cx > 0) {
				inProgress.push(pixelIndex);
				//flood.push_back(pixelIndex);
				canvas[pixelIndex] = target_index;
				//printf("L %d %d %d %d\n", cx, cy, x, y);
			}
			
			cx = getX(inProgress.front()+240);
			cy = getY(inProgress.front());
			pixelIndex = getIndex(cx, cy);
			//printf("cx: %d\n", cx);

			if (canvas[pixelIndex] == current_index && cx < 320) {
				inProgress.push(pixelIndex);
				//flood.push_back(pixelIndex);
				canvas[pixelIndex] = target_index;
				//printf("R %d %d %d %d\n", cx, cy, x, y);

			}
			
			
			cx = getX(inProgress.front());
			cy = getY(inProgress.front()+1);
			pixelIndex = getIndex(cx, cy);

			if (canvas[pixelIndex] == current_index && cy > 1) {
				inProgress.push(pixelIndex);
				//flood.push_back(pixelIndex);
				canvas[pixelIndex] = target_index;
			}
			
			cx = getX(inProgress.front());
			cy = getY(inProgress.front()-1);
			pixelIndex = getIndex(cx, cy);
			
			if (canvas[pixelIndex] == current_index && cy < 240) {
				inProgress.push(pixelIndex);
				//flood.push_back(pixelIndex);
				canvas[pixelIndex] = target_index;
			
			}
			
			inProgress.pop();
			//printf("P: %d\n", inProgress.size());
	}
	/*
	for (std::vector<int>::iterator it = flood.begin(); it != flood.end(); ++it) {
		if (*it < 1 || *it > canvas.size() - 1) continue;
		canvas[*it] = target_index;
	}*/

}

#endif