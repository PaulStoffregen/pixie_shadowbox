#include <Adafruit_NeoPixel.h>

#define PIN 17
#define NUMLED 55
Adafruit_NeoPixel strip = Adafruit_NeoPixel(NUMLED, PIN, NEO_GRB + NEO_KHZ800);

// this array is used for composing every LED color
// then the whole thing is sent to NeoPixel at once.
uint32_t frame[NUMLED+1];

void setup() {
	strip.begin();
	strip.show();
	strip.show();
}

void loop() {
	background();
	shade();
	//blink(1, 2, 0xFFFF10);
	//blink(3, 1, 0xFF0020);
	refresh();
}

void blink(uint32_t x, uint32_t y, uint32_t color)
{
	static elapsedMillis msec;

	if (msec < 250) {
		frame[xy(x, y)] = 0xFFFF10;
	} else if (msec > 500) {
		msec -= 500;
	}
}

unsigned int xy(unsigned int x, unsigned int y)
{
	if (x > 4) return NUMLED;  // x from 0 to 4  (left to right)
	if (y > 10) return NUMLED; // y from 0 to 10 (top to bottom)
	switch (x) {
		case 0: return 54 - y;
		case 1: return 33 + y;
		case 2: return 32 - y;
		case 3: return 11 + y;
		case 4: return 10 - y;
	}
	return NUMLED;
}

// slightly shade the background, 0 to 40% reduction, slowly varying
void shade()
{
	static float phase=0.0;
	float gain;
	uint32_t mult;

	phase += 0.0035;
	if (phase > 6.28318) phase = 0.0;
	gain = 1.0 - sinf(phase) * 0.5;
	mult = gain * 65536.0;
	for (int i=0; i < NUMLED; i++) {
		uint32_t r = (frame[i] >> 16) & 255;
		uint32_t g = (frame[i] >> 8) & 255;
		uint32_t b = (frame[i] >> 0) & 255;
		r = (r * mult) >> 16;
		g = (g * mult) >> 16;
		b = (b * mult) >> 16;
		if (r > 255) r = 255;
		if (g > 255) g = 255;
		if (b > 255) b = 255;
		frame[i] = (r << 16) | (g << 8) | b;
	}
}

// paint the background, very gradually changing blue to blue-green
void background()
{
	static elapsedMillis msec;
	static int color=128;
	static int dir=1;

	if (msec > 118) {
		msec -= 118;
		color += dir;
		if (color > 255) {
			color = 255;
			dir = -1;
		} else if (color < 0) {
			color = 0;
			dir = 1;
		}
	}
	for (int i=0; i < NUMLED; i++) {
		//frame[i] = green2blue(color + i / 3);
		frame[i] = blue2purple(color + i / 3);
	}
}

// update all the pixels, every 15 ms
void refresh()
{
	static elapsedMillis msec;

	for (int i=0; i < NUMLED; i++) {
		strip.setPixelColor(i, frame[i]);
	}
	while (msec < 15) ; // wait until 15 from prior update
	msec = 0;
	strip.show();
}

uint32_t blue2purple(uint32_t n)
{
	float x, r, g, b;

	if (n > 255 && n < 512) n = 511 - n; // above 255 wrap back down to zero
	x = (float)n * 0.0039214; // divide by 255
	if (x > 0.99999) x = 0.99999;
	g = 0;
	b = sinf(x * 0.6 + 0.4);
	r = sinf((1.0 - b) * 0.45);
	return strip.Color(r * 255.0, g * 255.0, b * 255.0);
}

uint32_t green2blue(uint32_t n)
{
	float x, r, g, b;

	if (n > 255 && n < 512) n = 511 - n; // above 255 wrap back down to zero
	x = (float)n * 0.0039214; // divide by 255
	if (x > 0.99999) x = 0.99999;
	r = 0.0;
	b = sinf(x * 0.6 + 0.4);
	g = sinf((1.0 - b) * 0.22);
	return strip.Color(r * 255.0, g * 255.0, b * 255.0);
}




