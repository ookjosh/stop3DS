// Scene.h

class Scene {
public:
	void update();
	void addFrame();
	void insertFrame();
	void deleteFrame();
	void clearFrame();
	void setAnimating(bool anim);
	void setTicksPerFrame(int count);

private:
	std::vector<Frame> frames
	unsigned int currentFrame = 0;
	unsigned int ticks = 0;
	unsigned int ticksPerFrame = 60;
	bool animating = false;
};

void Scene::update() {
	if (ticks > ticksPerFrame) {
		currentFrame++;
		if (currentFrame > frames.size() - 1) {
			currentFrame = 0;
		}
		ticks = 0;
	}
	frames.at(currentFrame).draw();
	if (animating) {
		ticks++;	
	}
};

// Adds frame to end of vector.
void Scene::addFrame() {
	frames.push_back(Frame());
}

// Inserts a new frame after current one.
void Scene::insertFrame() {
	frames.insert(currentFrame, Frame());
}

// Deletes the current frame
void Scene::deleteFrame() {
	if (currentFrame != 0) {
		frames.erase(currentFrame);	
	}
	if (currentFrame == 0) {
		clearFrame(currentFrame);
	}	
}

// Clears a frame (resets it to empty).
void Scene::clearFrame() {
	frames.at(currentFrame).clear();
}

// Sets if we are animating the scene or not.
void setAnimating(bool anim) {
	animating = anim;
}

// Updates scene's framerate.
void setTicksPerFrame(int count) {
	ticksPerFrame = count;
}