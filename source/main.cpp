#include <3ds.h>
#include <stdio.h>
#include <string.h> // memcpy
#include <vector>
#include <malloc.h>
#include <time.h>

std::vector<u8*> screenArray;

std::vector<std::vector<u8>> screenArr;

std::vector<u8> funkyBuffer(240*320*3, 0x7F);

u8 whiteBuffer[240*320*3];

unsigned int zindex = 0;

//u8* mappedMemory;


void drawPixel(int x, int y, u8* color) {
	screenArray.at(zindex)[(x*240*3 - y*3)] = color[0];
	screenArray.at(zindex)[(x*240*3 - y*3)+1] = color[1];
	screenArray.at(zindex)[(x*240*3 - y*3)+2] = color[2];
}

void drawBlock(int x, int y, u8* color) {

	drawPixel(x,y, color);
	drawPixel(x,y+1, color);
	drawPixel(x+1, y, color);
	drawPixel(x+1, y+1, color);

}

u8 color[3] = {(u8)0x7F, 0, (u8)0x7F};

int main(int argc, char **argv) {
	gfxInitDefault();
	consoleInit(GFX_TOP, NULL);

	gfxSetDoubleBuffering(GFX_BOTTOM, true);

	memset(whiteBuffer, 0xFF, sizeof(whiteBuffer));

	screenArray.push_back(whiteBuffer);

	screenArr.push_back(funkyBuffer);


	while (aptMainLoop()) {
		clock_t start = clock();
		hidScanInput();
		u32 kDown = hidKeysDown();

		touchPosition touch;

		hidTouchRead(&touch);

		if (kDown & KEY_START) {
			break;
		}

		if (kDown & KEY_UP) {
			if (screenArray.size() - 1 > zindex) {
				zindex++;
			}
		}

		if (kDown & KEY_DOWN) {
			if (zindex > 0) {
				zindex--;
			}
		}

		if (kDown & KEY_A) {
			// C++ requires casts for malloc.
			//u8* arr = (u8*)malloc(240*320*3);
			//memset(arr, 0, 240*320*3);
			//screenArray.push_back(arr);
			std::vector<u8> newBuf(240*320*3, 255);
			screenArr.push_back(newBuf);
			printf("Allocated array\n");
		}

		if (kDown & KEY_B) {
			// For fun to see if the function below actually works 
			// (mappableSpaceFree)
			//mappedMemory = static_cast<u8*>(mappableAlloc(0x2000));
		}

		printf("\x1b[0;0HIndex: %d", zindex);
		printf("\x1b[1;0HArraySize: %d", screenArray.size());
		// Max is apparently around 93MB, with each screen 250k or so.
		printf("\x1b[2;0HUsed memory: %dkB", screenArray.size() * 240 * 320 * 3 / 1024);
		//printf("\x1b[3;0HFree Mappable: %d", mappableSpaceFree());


		drawBlock(touch.px, touch.py, color);

		u8* fb = gfxGetFramebuffer(GFX_BOTTOM, GFX_LEFT, NULL, NULL);

		//memcpy(fb, screenArray[zindex], 240*320*3);
		std::copy(screenArr.at(0).begin(), screenArr.at(0).end(), fb );

		clock_t end = clock();
		printf("\x1b[5;0HLoop time: %f", (end-start)/CLOCKS_PER_SEC * 1000.0);


		gfxFlushBuffers();
		gfxSwapBuffers();

		gspWaitForVBlank();
	}

	gfxExit();

	return 0;
}