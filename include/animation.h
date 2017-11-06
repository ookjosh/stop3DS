// Animation.h
/*
	This defines animations and their associated data.	

*/

class Animation {
public:
	void update();
private:
	std::vector<Scene> scenes;
	unsigned int currentScene = 0;
};

// Updates current frame.
void Animation::update() {
	scenes.at(currentScene).update();
};







