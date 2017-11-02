#include <3ds.h>
#include <stdio.h>
#include <string.h> // memcpy
#include <vector>
#include <malloc.h>
#include <time.h>

std::vector<std::vector<u8>> screenArr;

std::vector<u8> funkyBuffer(240*320*3, 0x7F);

u8 whiteBuffer[240*320*3];

unsigned int zindex = 0;

u8 color[3] = {(u8)0x7F, 0, (u8)0x7F};

int main(int argc, char **argv) {
	gfxInitDefault();
	consoleInit(GFX_TOP, NULL);

	gfxSetDoubleBuffering(GFX_BOTTOM, true);

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

		printf("\x1b[0;0HIndex: %d", zindex);
		printf("\x1b[1;0HArraySize: %d", screenArr.size());
		printf("\x1b[2;0HUsed memory: %dkB\n", screenArr.size() * 240 * 320 * 3 / 1024);

		u8* fb = gfxGetFramebuffer(GFX_BOTTOM, GFX_LEFT, NULL, NULL);
		printf("\x1b[3;0Hfb location: %d", fb);
		printf("\x1b[4;0Harray location: %d", &screenArr[0]);
		printf("\x1b[5;0Harray value: %d", screenArr.at(zindex).at(touch.px));
		std::copy(screenArr.at(zindex).begin(), screenArr.at(zindex).end(), fb);

		gfxFlushBuffers();
		gfxSwapBuffers();
		gspWaitForVBlank();
	}

	gfxExit();

	return 0;
}