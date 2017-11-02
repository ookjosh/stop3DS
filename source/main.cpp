#include <3ds.h>
#include <stdio.h>
#include <string.h> // memcpy
#include <vector>
#include <malloc.h>

#include "color.h"

std::vector<std::vector<u8>> screenArr;

std::vector<u8> funkyBuffer(240*320*3, 0x7F);

u8 whiteBuffer[240*320*3];

unsigned int zindex = 0;

u8 color[3] = {(u8)0x7F, 0, (u8)0x7F};

void drawPixel(u16 x, u16 y, u8* color) {
	int pixelIndex = x*240*3 - y*3;
	// Very important :-D
	if (pixelIndex < 0) pixelIndex = 0;
 
	screenArr.at(zindex).at(pixelIndex) = color[0];
	screenArr.at(zindex).at(pixelIndex + 1) = color[1];
	screenArr.at(zindex).at(pixelIndex + 2) = color[2];
}

void drawBlock(int x, int y, u8* color) {

	drawPixel(x,y, color);
	drawPixel(x,y+1, color);
	drawPixel(x+1, y, color);
	drawPixel(x+1, y+1, color);

}

int main(int argc, char **argv) {
	gfxInitDefault();
	consoleInit(GFX_TOP, NULL);

	gfxSetDoubleBuffering(GFX_BOTTOM, true);

	// Initialize colors
	initColors();

	memset(whiteBuffer, 0xFF, sizeof(whiteBuffer));

	screenArr.push_back(funkyBuffer);


	while (aptMainLoop()) {
		hidScanInput();
		u32 kDown = hidKeysDown();

		touchPosition touch;

		hidTouchRead(&touch);

		if (kDown & KEY_START) {
			break;
		}

		if (kDown & KEY_UP) {
			if (screenArr.size() - 1 > zindex) {
				zindex++;
			}
		}

		if (kDown & KEY_DOWN) {
			if (zindex > 0) {
				zindex--;
			}
		}

		if (kDown & KEY_A) {
			screenArr.push_back(std::vector<u8>(240*320*3, 255));
			printf("Allocated array\n");
		}

		if (kDown & KEY_B) {
		}

		drawBlock(touch.px, touch.py, color);


		printf("\x1b[0;0HIndex: %d", zindex);
		printf("\x1b[1;0HArraySize: %d", screenArr.size());
		printf("\x1b[2;0HUsed memory: %dkB\n", screenArr.size() * 240 * 320 * 3 / 1024);

		u8* fb = gfxGetFramebuffer(GFX_BOTTOM, GFX_LEFT, NULL, NULL);
		printf("\x1b[3;0Hfb location: %d", fb);
		printf("\x1b[4;0Harray location: %d", &screenArr[0]);
		printf("\x1b[5;0Harray value: %d", screenArr.at(zindex).at(touch.px));
		printf("\x1b[6;0Harray size: %d, %d", screenArr.size(), screenArr.at(zindex).size());
		
		printf("\x1b[8;0HX: %d, Y: %d          ", touch.px, touch.py);
		printf("\x1b[9;0HIndex: %d        ", touch.px*240*3 - touch.py*3);
		//screenArr.at(zindex).at(10*240*3 - 10*3) = 0xA2;//color[0];
		//screenArr.at(zindex).at(10*240*3 - 10*3 + 1) = 0xDE;
		//screenArr.at(zindex).at(10*240*3 - 10*3 + 2) = 0xBF;
		std::copy(screenArr.at(zindex).begin(), screenArr.at(zindex).end(), fb);

		gfxFlushBuffers();
		gfxSwapBuffers();
		gspWaitForVBlank();
	}

	gfxExit();

	return 0;
}