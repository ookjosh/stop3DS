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
	Scene getScene(int index);
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

Scene Animation::getScene(int index) {
	if (index > scenes.size()) {
		// Handle error here!
		// Do I want to have a special Scene that
		// can be checked against (return Error_Scene?)
		// or just return a valid scene...?
		return scenes.at(0);
	}
	return scenes.at(index);
}


#endif
