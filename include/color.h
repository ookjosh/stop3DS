// Color.h
#ifndef COLOR_H
#define COLOR_H

// Types
typedef struct Color {
	u8 r;
	u8 g;
	u8 b;
};

// Forward declare functions
void addColor(u8 r, u8 g, u8 b);
void addColor(Color c);



std::vector<Color> colorList(8);

void initColors() {
	addColor(0,0,0);
	addColor(255, 255, 255);
	addColor(0,0,255);
	addColor(0,255,0);
	addColor(255,0,0);
	addColor(0x7F, 0, 0x7F);
	addColor(0, 0x7F, 0x7F);
	addColor(0xa0,0x15,0x2f);
}

void addColor(u8 r, u8 g, u8 b) {
	colorList.push_back(Color{.r=r, .g=g, .b=b});
}

void addColor(Color c) {
	colorList.push_back(c);
}


#endif