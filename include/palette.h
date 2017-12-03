#ifndef PALETTE_H
#define PALETTE_H

#include "color.h"


class Palette {
public:
	Palette();
	Color getColor(int index);
	int size();
private:
	std::vector<Color> colors;

};

Palette::Palette() {
	// Starting with just a 256 color palette.
	// Eventually have a parameter for size.
	colors.reserve(256);

	// Programmatically generate a palette.
	for (int red = 0; red < 256; red+=64) {
		for (int green = 0; green < 256; green+=64) {
			for (int blue = 0; blue < 256; blue+=64) {
				colors.push_back(Color{.r=red, .g=green, .b=blue});	
			}
		}
	}
}

Color Palette::getColor(int index) {
	return colors.at(index);
}

int Palette::size() {
	return colors.size();
}


#endif