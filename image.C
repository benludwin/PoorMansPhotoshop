#include <stdlib.h>
#include <string.h>
#include <image.h>
#include <source.h>

Image::Image(void)
{
	width = 0, height = 0;
	pixel = NULL;	
}
Image::Image(int w, int h, Pixel *p)
{
	width = w, height = h;
	pixel = (Pixel *)malloc(3*w*h);
	memcpy(pixel, p, 3*height*width);
}
Image::Image(const Image &image)
{
	width = image.width, height = image.height;
	pixel = (Pixel *)malloc(3*height*width);
	memcpy(pixel, image.pixel, 3*height*width);
}
void Image::Update() const
{
	source->Update();
}
void Image::setPixel(Pixel *p)
{
	if (pixel == NULL) { pixel = (Pixel *)malloc(3*width*height); }
	memcpy(pixel, p, 3*height*width);
}
