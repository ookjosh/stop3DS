// globals.h
#ifndef GLOBALS_H
#define GLOBALS_H

#define COLOR_16 0
#define COLOR_256 1
#define COLOR_FULL 2

// 3 bytes per pixel
#define COLOR_FULL_BYTES (320*240*3)
// 1 nybble per pixel, so 320*240*(1/2)
#define COLOR_16_BYTES (320*120)
// 1 byte per pixel
#define COLOR_256_BYTES (320*240)

#define TOP_SCREEN_WIDTH 400
#define TOP_SCREEN_HEIGHT 240

/* To get coords from pixel index
// +1 to make up for rounding error down (seems to always
// work?)
int getX = (pixelIndex) / 240 + 1;
int getY = 240 - (pixelIndex % 240);
*/
#endif