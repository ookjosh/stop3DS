#include <3ds.h>
#include <stdio.h>
#include <string.h> // memcpy
#include <vector>
#include <malloc.h>

#include "color.h"
#include "animation.h"

std::vector<std::vector<u8>> screenArr;

std::vector<u8> canvas(240*320*3, 0);

std::vector<u8> funkyBuffer(240*320*3, 0x7F);

u8 whiteBuffer[240*320*3];

unsigned int zindex = 0;

bool animating = false;
int tick = 0;
int curFrame = 0;
// fps_ticks/60 is fps
int fps_ticks = 60; 
bool onionSkin = false;

u8 color[3] = {(u8)0x7F, 0, (u8)0x7F};

Animation currentAnimation;

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

void onionFrame(u8* frame) {
	float alpha = 0.5;
	for (int i = 0; i < 240*320*3; i++) {
		frame[i] = (u8)((1 - alpha) * 255 + (alpha * frame[i]));
	}
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

		if (kDown & KEY_X) {
			fps_ticks -= 5;
		}

		if (kDown & KEY_Y) {
			fps_ticks += 5;
		}

		if (kDown & KEY_L) {
			onionSkin = !onionSkin;
		}

		if (kDown & KEY_R) {
			printf("CleARING\n");
			currentAnimation.getScene(0).getFrame(0).getLayer(0).clear();
		}

		//drawBlock(touch.px, touch.py, colorList.at(currentColor));


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

		//currentAnimation.getScene(0).getFrame(0).getLayer(0).drawPixel(touch.px, touch.py, colorList.at(currentColor));

		currentAnimation.update(fb);

		/*
		if (animating) {
			tick++;
			// Where 60 ticks = 1 second
			if (tick > fps_ticks) {
				curFrame++;
				// Loop back to first frame
				if (curFrame > screenArr.size() - 1) {
					curFrame = 0;
				}
				tick = 0;
			}
			std::copy(screenArr.at(curFrame).begin(), screenArr.at(curFrame).end(), fb);
		} else {
			// Need onionskinning and a previous frame to skin.
			if (onionSkin && zindex > 0) {
				// Get copy to alphaify
				std::vector<u8> temp = screenArr.at(zindex - 1);
				// Make it transparent
				onionFrame(&temp[0]);
				// Set that as the base of our canvas
				canvas = temp;

				// Now we need to put only new information on top of this
				// onion skin. For now, ignore any pixel that's the background
				// color of the frames.
				// TODO: use an iterator for the vector instead?
				u8* current_layer = &screenArr.at(zindex)[0];
				for (int i = 0; i < screenArr.at(zindex).size(); i++) {
					if (current_layer[i] != 0xFF) {
						canvas[i] = current_layer[i]; 
					}
				}

			} else {
			 	canvas = screenArr.at(zindex);	
			}

			std::copy(canvas.begin(), canvas.end(), fb);
		}
		*/

		gfxFlushBuffers();
		gfxSwapBuffers();
		gspWaitForVBlank();
	}

	gfxExit();

	return 0;
}