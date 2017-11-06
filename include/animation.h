// Animation.h
/*
	This defines animations and their associated data.	

*/
#ifndef ANIMATION_H
#define ANIMATION_H

#include <3ds.h>
#include <vector>
#include "scene.h"

class Animation {
public:
	Animation();
	void update(u8* fb);
	u8* getFramebuffer();
private:
	std::vector<Scene> scenes;
	unsigned int currentScene = 0;
	u8* frameBuffer;
};


// Constructor for root animation object.
// Initializes everything, including creating a root
// scene.
Animation::Animation() {
	scenes.push_back(Scene());

}

// Updates current frame.
void Animation::update(u8* fb) {
	frameBuffer = fb;
	printf("Size %d\n", scenes.size());
	scenes.at(currentScene).update(fb);
};

// So things can reference current framebuffer.
u8* Animation::getFramebuffer() {
	return frameBuffer;
}




#endif
