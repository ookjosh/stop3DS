// Frame.h

class Frame {
	
public:
	Frame();
	void draw();

private:
	std::vector<Layer> layers;
};

// Constructor for frame class. Populates the layers vector.
Frame::Frame() {
	layers.push_back(Layer background());
	layers.push_back(Layer objects());
}
// Draws all of the layers contained in the layers vector.
void Frame::draw() {
	for (std::vector<T>::iterator it = layers.begin(); it != layers.end(); ++it) {
		it->draw();
	}
}

