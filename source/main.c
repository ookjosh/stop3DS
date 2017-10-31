#include <3ds.h>
#include <stdio.h>
#include <string.h>
//#include <vector>
//#include <malloc.h>

u8 picBuffer[240*400*3];
u8 picBufferAlpha[240*400*2];
bool en_alpha = false;
bool old_en_alpha = false;
float alpha = 1.0;

//std::vector<u8> vBuffer(240*400*3);

enum ColorList {
	BLACK,
	WHITE,
	RED,
	GREEN,
	BLUE,
	PURPLE,
	YELLOW,
	FUNKY
};

enum ColorList currentColor = BLACK;

u8 black[3] = {0,0,0};
u8 white[3] = {255,255,255};
u8 red[3] = {0, 0, 255};
u8 green[3] = {0, 255, 0};
u8 blue[3] = {255, 0, 0};
u8 purple[3] = {0x7F, 0, 0x7F};
u8 yellow[3] = {0, 0x7F, 0x7F};
u8 funky[3] = {0xa0, 0x15, 0x2f};

u8* colors[8] = {
	black, white, red, green, blue, purple, yellow, funky
};

u8* color;

#define RED 0
#define GREEN 1
#define BLUE 2
#define ALPHA 3

// thanks http://marcodiiga.github.io/rgba-to-rgb-conversion
void alphaify(u8* background, u8* color) {
	int alpha = color[ALPHA];

	color[RED] = (255 - alpha) * background[RED] + (alpha * color[RED]);
	color[GREEN] = (255 - alpha) * background[GREEN] + (alpha * color[GREEN]);
	color[BLUE] = (255 - alpha) * background[BLUE] + (alpha * color[BLUE]);
}


// Makes the frame half opaque.
void alphaifyFrame(u8* frame) {
	float alpha = .5; // Half opacity.

	for (int i = 0; i < 240*400*3; i++) {
		frame[i] = (u8)((1 - alpha) * 255 + (alpha * frame[i]));
	}
}

void drawPixel(int x, int y, u8* color) {
	picBuffer[(x*240*3 - y*3)] = color[0];
	picBuffer[(x*240*3 - y*3)+1] = color[1];
	picBuffer[(x*240*3 - y*3)+2] = color[2];
}

void drawSpectrum() {
	int y = 0;
	int c = 0;
	for (y; y < 20; y++) {
		for (int x = 0; x < 256; x++) {
			if (x%16 == 0) {
				c+=16;
			}
			picBuffer[(x*240*3 - y*3)] = c-16;
			picBuffer[(x*240*3 - y*3)+1] = 0;
			picBuffer[(x*240*3 - y*3)+2] = 0x00;
		}
	}
	for (y; y < 40; y++) {
		for (int x = 0; x < 256; x++) {
			picBuffer[(x*240*3 - y*3)] = 0;
			picBuffer[(x*240*3 - y*3)+1] = x;
			picBuffer[(x*240*3 - y*3)+2] = 0x00;
		}
	}
	for (y; y < 60; y++){
		for (int x = 0; x < 256; x++) {
			picBuffer[(x*240*3 - y*3)] = 0;
			picBuffer[(x*240*3 - y*3)+1] = 0;
			picBuffer[(x*240*3 - y*3)+2] = x;
		}
	}

	// Alphaified
	u8 color[4];
	u8 background[4];
	memset(background, 0xFF, sizeof(background));
	memset(color, 0, sizeof(color));
	color[3] = 0xFF;
	for (y; y < 80; y++){
		for (int x = 0; x < 256; x++) {
			color[4] = x;
			alphaify(background, color);
			picBuffer[(x*240*3 - y*3)] = 0;
			picBuffer[(x*240*3 - y*3)+1] = 0;
			picBuffer[(x*240*3 - y*3)+2] = color[3];
		}
	}


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

	// Init our picBuffer to be all the same color
	for (int i = 0; i < sizeof(picBuffer); i++) {
		picBuffer[i] = 0xFF;
	}

	//drawSpectrum();

	while (aptMainLoop()) {
		hidScanInput();
		u32 kDown = hidKeysDown();

		touchPosition touch;

		hidTouchRead(&touch);

		if (kDown & KEY_A) {
			alphaifyFrame(picBuffer);
			en_alpha = !en_alpha;
		}

		if (kDown & KEY_UP) {
			//if (alpha < 255 - 10) {
				alpha= alpha*2;
			//}
		}

		if (kDown & KEY_DOWN) {
			//if (alpha > 10) {
				alpha=alpha/2;
				//malloc(100000);
			//}
		}

		if (kDown & KEY_L) {
			currentColor = currentColor+1;
			if (currentColor > FUNKY) {
				currentColor = BLACK;
			}
		}


		color = colors[currentColor];

		drawBlock(touch.px, touch.py, color);
		if (en_alpha){
			//drawBlock(touch.px, touch.py);
			//alphaifyFrame(picBuffer);
			//en_alpha = false;
		} else {
			drawSpectrum();
		}
 		
		

		u8* fb = gfxGetFramebuffer(GFX_BOTTOM, GFX_LEFT, NULL, NULL);

		// Update framebuffer
		memcpy(fb, picBuffer, sizeof(picBuffer));

		//memcpy(fb, vBuffer.data(), vBuffer.size());//sizeof(vBuffer));	

		printf("\x1b[0;0HFormat: %d", gfxGetScreenFormat(GFX_BOTTOM));
		printf("\x1b[1;0HAlpha: %f0.2", alpha);
		printf("\x1b[2;0HResult: %f0.2", (0xFF * (1/alpha)));
		printf("\x1b[3;0HResult: %d", 16 * (256/(16*3)) - 16);
		printf("\x1b[4;0HMem Free: %d", linearSpaceFree());
		printf("\x1b[5;0HMem Free: %d", mappableSpaceFree());



		gfxFlushBuffers();
		gfxSwapBuffers();

		gspWaitForVBlank();
	}

	gfxExit();

	return 0;
}