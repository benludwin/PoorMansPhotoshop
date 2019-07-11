#include <source.h>
#include <logging.h>
#include <stdlib.h>

void Source::Update()
{
	char msg[128];
	sprintf(msg, "%s: about to execute\n", SourceName());
	Logger::LogEvent(msg);
	Execute();
	sprintf(msg, "%s: done executing\n", SourceName());
	Logger::LogEvent(msg);
}
Color::Color(int w, int h, unsigned char r, unsigned char g, unsigned char b)
{
	width = w;
	height = h;
	red = r;
	green = g;
	blue = b;
	image.ResetSize(w, h);
}
void Color::Execute(void)
{
	if (red < 0.0 || red > 255.0)
	{
		char msg[128];
		sprintf(msg, "%s: Invalid value for red: %d\n", SourceName(), red);
		DataFlowException exc(SourceName(), msg);
		throw exc;
	}
	if (green < 0.0 || green > 255.0)
	{
		char msg[128];
		sprintf(msg, "%s: Invalid value for green: %d\n", SourceName(), green);
		DataFlowException exc(SourceName(), msg);
		throw exc;
	}
	if (blue < 0.0 || blue > 255.0)
	{
		char msg[128];
		sprintf(msg, "%s: Invalid value for blue: %d\n", SourceName(), blue);
		DataFlowException exc(SourceName(), msg);
		throw exc;
	}
	int outputIndex;
	
	Pixel * pixel = (Pixel *)malloc(width * height * 3);
	for (int i = 0; i < width; i++)
	{
		for (int j = 0; j < height; j++)
		{
			outputIndex = (j * width) + i;
			pixel[outputIndex].r = red;		
			pixel[outputIndex].g = green;		
			pixel[outputIndex].b = blue;		
		}
	}
	image.setPixel(pixel);
	free(pixel);
}
