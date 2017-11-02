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

bool animating = false;
int tick = 0;
int curFrame = 0;

u8 color[3] = {(u8)0x7F, 0, (u8)0x7F};

void drawPixel(u16 x, u16 y, Color c) {
	int pixelIndex = x*240*3 - y*3;
	// Very important :-D
	if (pixelIndex < 0) pixelIndex = 0;
 
	screenArr.at(zindex).at(pixelIndex) = c.b;
	screenArr.at(zindex).at(pixelIndex + 1) = c.g;
	screenArr.at(zindex).at(pixelIndex + 2) = c.r;
}

void drawBlock(int x, int y, Color color) {

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

		if (kDown & KEY_LEFT) {
			if (currentColor > 1) {
				currentColor--;
			}
		}

		if (kDown & KEY_RIGHT) {
			if (currentColor < colorList.size()-1) {
				currentColor++;
			}
		}

		if (kDown & KEY_A) {
			screenArr.push_back(std::vector<u8>(240*320*3, 255));
			printf("Allocated array\n");
		}

		if (kDown & KEY_B) {
			animating = !animating;
		}

		drawBlock(touch.px, touch.py, colorList.at(currentColor));


		printf("\x1b[0;0HIndex: %d", zindex);
		printf("\x1b[1;0HArraySize: %d", screenArr.size());
		printf("\x1b[2;0HUsed memory: %dkB\n", screenArr.size() * 240 * 320 * 3 / 1024);

		u8* fb = gfxGetFramebuffer(GFX_BOTTOM, GFX_LEFT, NULL, NULL);
		printf("\x1b[4;0Harray location: %d", &screenArr[0]);
		printf("\x1b[5;0Harray value: %d", screenArr.at(zindex).at(touch.px));
		printf("\x1b[6;0Harray size: %d, %d", screenArr.size(), screenArr.at(zindex).size());
		
		printf("\x1b[8;0HX: %d, Y: %d          ", touch.px, touch.py);
		printf("\x1b[9;0HColor: %d", currentColor);
		printf("\x1b[10;0HR: %d, G: %d, B: %d", colorList.at(currentColor).r,
			colorList.at(currentColor).g, colorList.at(currentColor).b);
		//screenArr.at(zindex).at(10*240*3 - 10*3) = 0xA2;//color[0];
		//screenArr.at(zindex).at(10*240*3 - 10*3 + 1) = 0xDE;
		//screenArr.at(zindex).at(10*240*3 - 10*3 + 2) = 0xBF;

		if (animating) {
			tick++;
			if (tick > 60) {
				curFrame++;
				if (curFrame > screenArr.size() - 1) {
					curFrame = 0;
				}
				tick = 0;
			}
			std::copy(screenArr.at(curFrame).begin(), screenArr.at(curFrame).end(), fb);
		} else {
			std::copy(screenArr.at(zindex).begin(), screenArr.at(zindex).end(), fb);	
		}
		

		gfxFlushBuffers();
		gfxSwapBuffers();
		gspWaitForVBlank();
	}

	gfxExit();

	return 0;
}