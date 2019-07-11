#ifndef IMAGE_H
#define IMAGE_H

#include <string.h>
#include <stdlib.h>

class Source;

typedef struct
{
	unsigned char r, g, b;
} Pixel;

class Image
{
	private:
		int height, width;
		Pixel *pixel;
	public:
		Source *source;
		Image(void);
		~Image(void) { free(pixel); };
		Image(int height, int width, Pixel *pixel);
		Image(const Image &image);
		
		void Update() const;
		void setPixel(Pixel *pixel);
		void ResetSize(int w, int h) { width = w, height = h; };
		void setSource(Source *s) { if (s != NULL) { source = s; } };
		
		int getWidth() const { return width; } ;
		int getHeight() const { return height; } ;
		Pixel * getPixel() const { return pixel; } ;
};
#endif
