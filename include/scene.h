// Scene.h
#ifndef SCENE_H
#define SCENE_H

#include <3ds.h>
#include <vector>
#include "frame.h"
#include "appstate.h"

class Scene {
public:
	Scene();
	void update(u8* fb);
	void addFrame();
	void insertFrame();
	void deleteFrame();
	void clearFrame(int frame);
	void setAnimating(bool anim);
	void setTicksPerFrame(int count);

	Frame& getFrame(int index);

private:
	std::vector<Frame> frames;
	unsigned int currentFrame = 0;
	unsigned int ticks = 0;
	unsigned int ticksPerFrame = 60;
	bool animating = false;

	GlobalState& gState = GlobalState::getInstance();
};

Scene::Scene() {
	addFrame();
}

// Updates scene if necessary, passing current framebuffer.
void Scene::update(u8* fb) {
	//printf("Number of frames: %d\n", frames.size());
	
	if (gState.addFrame) {
		addFrame();
		gState.addFrame = false;
	}

	getFrame(gState.currentFrame).draw(fb);

	// For animating
	/*
	if (ticks > ticksPerFrame) {
		currentFrame++;
		if (currentFrame > frames.size() - 1) {
			currentFrame = 0;
		}
		ticks = 0;
	}
	frames.at(currentFrame).draw(fb);
	if (animating) {
		ticks++;	
	}
	*/
};

// Adds frame to end of vector.
void Scene::addFrame() {
	frames.push_back(Frame());
}

// Inserts a new frame after current one.
void Scene::insertFrame() {
	frames.insert(frames.begin() + currentFrame, Frame());
}

// Deletes the current frame
void Scene::deleteFrame() {
	if (currentFrame != 0) {
		frames.erase(frames.begin() + currentFrame);	
	}
	if (currentFrame == 0) {
		clearFrame(currentFrame);
	}	
}

// Clears a frame (resets it to empty).
void Scene::clearFrame(int frame) {
	frames.at(currentFrame).clear(frame);
}

// Sets if we are animating the scene or not.
void Scene::setAnimating(bool anim) {
	animating = anim;
}

// Updates scene's framerate.
void Scene::setTicksPerFrame(int count) {
	ticksPerFrame = count;
}

// Returns a reference to the frame of interest.
Frame& Scene::getFrame(int index) {
	if (index > frames.size()) {
		// ERROR! TODO: handle this well.
		return frames.at(0);
	}
	return frames.at(index);
}

#endif