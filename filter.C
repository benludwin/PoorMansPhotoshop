#include <stdlib.h>
#include <string.h>
#include <filter.h>
#include <logging.h>
#include <image.h>
#include <sys/time.h>

int update_limit(0);

const char *Filter::SourceName() { return FilterName(); };
const char *Filter::SinkName() { return FilterName(); };

void Filter::Update()
{
	update_limit += 1;
	if (update_limit > 1024)
	{
		char msg[128];
		sprintf(msg, "%s: Update Limit Reached: Check for Circular Dependency in log.txt\n", SourceName());
		DataFlowException exc(SourceName(), msg);
		throw exc;
	}
	
	if (image1 != NULL) 
	{
	        char msg[128];
		sprintf(msg, "%s: about to update input1\n", SourceName());
		Logger::LogEvent(msg);
		image1->Update(); 
		sprintf(msg, "%s: done updating input1\n", SourceName());
		Logger::LogEvent(msg);
	}
	if (image2 != NULL) 
	{ 
	        char msg[128];
		sprintf(msg, "%s: about to update input2\n", SourceName());
		Logger::LogEvent(msg);
		image2->Update(); 
		sprintf(msg, "%s: done updating input2\n", SourceName());
		Logger::LogEvent(msg);
	}
	char msg[128];
	sprintf(msg, "%s: about to execute\n", SourceName());
	Logger::LogEvent(msg);
	struct timeval starttime;
	struct timeval endtime;
	gettimeofday(&starttime, 0);
	Execute();
	gettimeofday(&endtime, 0);
	double seconds = double(endtime.tv_sec - starttime.tv_sec) + double(endtime.tv_usec - starttime.tv_usec) / 1000000.;
	sprintf(msg, "%s: done executing, took %f\n", SourceName(), seconds);
	Logger::LogEvent(msg);
}
void Shrinker::Execute()
{
	if (image1 == NULL)
	{
		char msg[128];
		sprintf(msg, "%s: no input1!\n", SinkName());
		DataFlowException exc(SinkName(), msg);
		throw exc;
	}
	int width = image1->getWidth(), height = image1->getHeight();
	int outwidth = width/2, outheight = height/2;
	int inputIndex, outputIndex;

	image.ResetSize(outwidth, outheight);
	Pixel * pixel = (Pixel*)malloc(outwidth * outheight * 3);

	for(int i = 0; i < outwidth; i++)
	{
		for(int j = 0; j < outheight; j++)
		{
			outputIndex = ((j * outwidth) + i);
			inputIndex = ((j*2)*width) + (i*2);
			pixel[outputIndex].r = image1->getPixel()[inputIndex].r;
			pixel[outputIndex].g = image1->getPixel()[inputIndex].g;
			pixel[outputIndex].b = image1->getPixel()[inputIndex].b;
		
		}
	}
	image.setPixel(pixel);
	free(pixel);
}
void LRCombine::Execute()
{
	if (image1 == NULL)
	{
		char msg[128];
		sprintf(msg, "%s: no input1!\n", SinkName());
		DataFlowException exc(SinkName(), msg);
		throw exc;
	}
	if (image2 == NULL)
	{
		char msg[128];
		sprintf(msg, "%s: no input2!\n", SinkName());
		DataFlowException exc(SinkName(), msg);
		throw exc;
	}
	int height1 = image1->getHeight(), height2 = image2->getHeight();
	if (height1 != height2)
	{
		char msg[128];
		sprintf(msg, "%s: heights must match: %d, %d\n", SourceName(), image1->getHeight(), image2->getHeight());
		DataFlowException exc(SinkName(), msg);
		throw exc;
	}
	int width1 = image1->getWidth(), width2 = image2->getWidth();
	int outwidth = width1 + width2, outheight = (height1+height2)/2;
	int inputIndex, outputIndex;
	
	image.ResetSize(outwidth, outheight);
	Pixel * pixel = (Pixel*)malloc(outwidth * outheight * 3);
	
	for(int i = 0; i < width1; i++)
	{
		for(int j = 0; j < outheight; j++)
		{
			inputIndex = (j * width1) + i;
			outputIndex = (j * outwidth) + i;
			pixel[outputIndex].r = image1->getPixel()[inputIndex].r;
			pixel[outputIndex].g = image1->getPixel()[inputIndex].g;
			pixel[outputIndex].b = image1->getPixel()[inputIndex].b;
		}
	}
	for(int i = 0; i < width2; i++)
	{
		for(int j = 0; j < outheight; j++)
		{
			inputIndex = (j * width2) + i;
			outputIndex = ((j * outwidth) + i) + width1;
			pixel[outputIndex].r = image2->getPixel()[inputIndex].r;
			pixel[outputIndex].g = image2->getPixel()[inputIndex].g;
			pixel[outputIndex].b = image2->getPixel()[inputIndex].b;
		}
	}
	image.setPixel(pixel);
	free(pixel);
}
void TBCombine::Execute()
{
	if (image1 == NULL)
	{
		char msg[128];
		sprintf(msg, "%s: no input1!\n", SinkName());
		DataFlowException exc(SinkName(), msg);
		throw exc;
	}
	if (image2 == NULL)
	{
		char msg[128];
		sprintf(msg, "%s: no input2!\n", SinkName());
		DataFlowException exc(SinkName(), msg);
		throw exc;
	}
	int width1 = image1->getWidth(), width2 = image2->getWidth();
	if (width1 != width2)
	{
		char msg[128];
		sprintf(msg, "%s: widths must match: %d, %d\n", SourceName(), image1->getWidth(), image2->getWidth());
		DataFlowException exc(SinkName(), msg);
		throw exc;
	}
	int height1 = image1->getHeight(), height2 = image2->getHeight();
	int outwidth = (width1 + width2)/2, outheight = height1 + height2;
	int inputIndex, outputIndex;
	
	image.ResetSize(outwidth, outheight);
	Pixel * pixel = (Pixel*)malloc(outwidth * outheight * 3);
	
	for(int i = 0; i < outwidth; i++)
	{
		for(int j = 0; j < height1; j++)
		{
			inputIndex = (j * width1) + i;
			outputIndex = (j * outwidth) + i;
			pixel[outputIndex].r = image1->getPixel()[inputIndex].r;
			pixel[outputIndex].g = image1->getPixel()[inputIndex].g;
			pixel[outputIndex].b = image1->getPixel()[inputIndex].b;
		}
	}
	for(int i = 0; i < outwidth; i++)
	{
		for(int j = 0; j < height2; j++)
		{
			inputIndex = (j * width2) + i;
			outputIndex = (j+height1)*outwidth+i;
			pixel[outputIndex].r = image2->getPixel()[inputIndex].r;
			pixel[outputIndex].g = image2->getPixel()[inputIndex].g;
			pixel[outputIndex].b = image2->getPixel()[inputIndex].b;
		}
	}
	image.setPixel(pixel);
	free(pixel);
}
void Blender::Execute()
{
	if (image1 == NULL)
	{
		char msg[128];
		sprintf(msg, "%s: no input1!\n", SinkName());
		DataFlowException exc(SinkName(), msg);
		throw exc;
	}
	if (image2 == NULL)
	{
		char msg[128];
		sprintf(msg, "%s: no input2!\n", SinkName());
		DataFlowException exc(SinkName(), msg);
		throw exc;
	}
	int width1 = image1->getWidth(), width2 = image2->getWidth();
	int height1 = image1->getHeight(), height2 = image2->getHeight();
	if (width1 != width2)
	{
		char msg[128];
		sprintf(msg, "%s: widths must match: %d, %d\n", SourceName(), image1->getWidth(), image2->getWidth());
		DataFlowException exc(SinkName(), msg);
		throw exc;
	}
	if (height1 != height2)
	{
		char msg[128];
		sprintf(msg, "%s: heights must match: %d, %d\n", SourceName(), image1->getHeight(), image2->getHeight());
		DataFlowException exc(SinkName(), msg);
		throw exc;
	}
	if (factor > 1.0 || factor < 0.0)
	{
		char msg[128];
		sprintf(msg, "Invalid factor for Blender: %f\n", factor);
		DataFlowException exc(SinkName(), msg);
		throw exc;
	}
	int outwidth = (width1 + width2)/2, outheight = (height1 + height2)/2;
	double v1 = factor, v2 = 1-factor;
	int outputIndex;
	
	image.ResetSize(outwidth, outheight);
	Pixel * pixel = (Pixel*)malloc(outwidth * outheight * 3);
	
	for(int i = 0; i < outwidth; i++)
	{
		for(int j = 0; j < outheight; j++)
		{
			outputIndex = (j * outwidth) + i;
			pixel[outputIndex].r = (image1->getPixel()[outputIndex].r * v1) + (image2->getPixel()[outputIndex].r * v2);
			pixel[outputIndex].g = (image1->getPixel()[outputIndex].g * v1) + (image2->getPixel()[outputIndex].g * v2);
			pixel[outputIndex].b = (image1->getPixel()[outputIndex].b * v1) + (image2->getPixel()[outputIndex].b * v2);
		}
	}
	image.setPixel(pixel);
	free(pixel);
}
void Mirror::Execute()
{
	int outwidth = image1->getWidth(), outheight = image1->getHeight();
	int inputIndex, outputIndex;

	image.ResetSize(outwidth, outheight);
	Pixel * pixel = (Pixel *)malloc(outwidth * outheight * 3);
	
	if (image1 == NULL)
	{
		char msg[128];
		sprintf(msg, "%s: no input1!\n", SinkName());
		DataFlowException exc(SinkName(), msg);
		throw exc;
	}
	for (int i = 0; i < outwidth; i++)
	{
		for (int j = 0; j < outheight; j++)
		{
			inputIndex = (j * outwidth) + i;
			outputIndex = (j * outwidth) + (outwidth - i - 1);
			pixel[outputIndex].r = image1->getPixel()[inputIndex].r;
			pixel[outputIndex].g = image1->getPixel()[inputIndex].g;
			pixel[outputIndex].b = image1->getPixel()[inputIndex].b;
		}
	}
	image.setPixel(pixel);
	free(pixel);
}
void Rotate::Execute()
{
	if (image1 == NULL)
	{
		char msg[128];
		sprintf(msg, "%s: no input1!\n", SinkName());
		DataFlowException exc(SinkName(), msg);
		throw exc;
	}
	int width = image1->getWidth(), height = image1->getHeight();
	int outwidth = height, outheight = width;
	int inputIndex, outputIndex;

	Pixel * pixel = (Pixel *)malloc(width * height * 3);
	image.ResetSize(outwidth, outheight);

	for (int i = 0; i < outwidth; i++)
	{
		for (int j = 0; j < outheight; j++)
		{
			inputIndex = (i * width) + j;
			outputIndex = (j * outwidth) + (outwidth - i - 1);
			pixel[outputIndex].r = image1->getPixel()[inputIndex].r;
			pixel[outputIndex].g = image1->getPixel()[inputIndex].g;
			pixel[outputIndex].b = image1->getPixel()[inputIndex].b;
		}
	}
	image.setPixel(pixel);
	free(pixel);
}
void Subtract::Execute()
{
	if (image1 == NULL)
	{
		char msg[128];
		sprintf(msg, "%s: no input1!\n", SinkName());
		DataFlowException exc(SinkName(), msg);
		throw exc;
	}
	if (image2 == NULL)
	{
		char msg[128];
		sprintf(msg, "%s: no input2!\n", SinkName());
		DataFlowException exc(SinkName(), msg);
		throw exc;
	}
	int width1 = image1->getWidth(), width2 = image2->getWidth();
	int height1 = image1->getHeight(), height2 = image2->getHeight();
	if (width1 != width2)
	{
		char msg[128];
		sprintf(msg, "%s: widths must match: %d, %d\n", SourceName(), image1->getWidth(), image2->getWidth());
		DataFlowException exc(SinkName(), msg);
		throw exc;
	}
	if (height1 != height2)
	{
		char msg[128];
		sprintf(msg, "%s: heights must match: %d, %d\n", SourceName(), image1->getHeight(), image2->getHeight());
		DataFlowException exc(SinkName(), msg);
		throw exc;
	}
	int outwidth = width1, outheight = height1;
	int outputIndex;

	image.ResetSize(outwidth, outheight);
	Pixel * pixel = (Pixel *)malloc(outwidth * outheight * 3);

	for (int i = 0; i < outwidth; i++)
	{
		for (int j = 0; j < outheight; j++)
		{
			outputIndex = (j * outwidth) + i;
			if (image1->getPixel()[outputIndex].r > image2->getPixel()[outputIndex].r)
			{
				pixel[outputIndex].r = (image1->getPixel()[outputIndex].r) 
							- (image2->getPixel()[outputIndex].r);
			} else {
				pixel[outputIndex].r = 0;
			}
			if (image1->getPixel()[outputIndex].g > image2->getPixel()[outputIndex].g)
			{
				pixel[outputIndex].g = (image1->getPixel()[outputIndex].g) 
							- (image2->getPixel()[outputIndex].g);
			} else {
				pixel[outputIndex].g = 0;
			}
			if (image1->getPixel()[outputIndex].b > image2->getPixel()[outputIndex].b)
			{
				pixel[outputIndex].b = (image1->getPixel()[outputIndex].b) 
							- (image2->getPixel()[outputIndex].b);
			} else {
				pixel[outputIndex].b = 0;
			}
		}
	}
	image.setPixel(pixel);
	free(pixel);
}
void Grayscale::Execute()
{
	if (image1 == NULL)
	{
		char msg[128];
		sprintf(msg, "%s: no input1!\n", SinkName());
		DataFlowException exc(SinkName(), msg);
		throw exc;
	}
	int outwidth = image1->getWidth(), outheight = image1->getHeight();
	int outputIndex;

	image.ResetSize(outwidth, outheight);
	Pixel * pixel = (Pixel *)malloc(outwidth * outheight * 3);

	for (int i = 0; i < outwidth; i++)
	{
		for (int j = 0; j < outheight; j++)
		{
			outputIndex = (j * outwidth) + i;
			pixel[outputIndex].r = (image1->getPixel()[outputIndex].r / 5) + (image1->getPixel()[outputIndex].g / 2) + (image1->getPixel()[outputIndex].b / 4);
			pixel[outputIndex].g = (image1->getPixel()[outputIndex].r / 5) + (image1->getPixel()[outputIndex].g / 2) + (image1->getPixel()[outputIndex].b / 4);
			pixel[outputIndex].b = (image1->getPixel()[outputIndex].r / 5) + (image1->getPixel()[outputIndex].g / 2) + (image1->getPixel()[outputIndex].b / 4);
		}
	}
	image.setPixel(pixel);
	free(pixel);	
}
void Blur::Execute()
{
	if (image1 == NULL)
	{
		char msg[128];
		sprintf(msg, "%s: no input1!\n", SinkName());
		DataFlowException exc(SinkName(), msg);
		throw exc;
	}
	int outwidth = image1->getWidth(), outheight = image1->getHeight();
	int outputIndex;

	image.ResetSize(outwidth, outheight);
	Pixel * pixel = (Pixel *)malloc(outwidth * outheight * 3);
	
	for (int i = 0; i < outwidth; i++)
	{
		for (int j = 0; j < outheight; j++)
		{
			if (i == 0 || i == outwidth - 1 || j == 0 || j == outheight - 1)
			{
				outputIndex = (j * outwidth) + i;
				pixel[outputIndex].r = image1->getPixel()[outputIndex].r;
				pixel[outputIndex].g = image1->getPixel()[outputIndex].g;
				pixel[outputIndex].b = image1->getPixel()[outputIndex].b;
			} else
			{
				outputIndex = (j * outwidth) + i;
				pixel[outputIndex].r = (image1->getPixel()[outputIndex-1].r / 8) + (image1->getPixel()[outputIndex+1].r / 8)
				       	+ (image1->getPixel()[outputIndex+outwidth].r / 8) + (image1->getPixel()[outputIndex+outwidth-1].r / 8) + (image1->getPixel()[outputIndex+outwidth+1].r / 8)
				       	+ (image1->getPixel()[outputIndex-outwidth].r / 8) + (image1->getPixel()[outputIndex-outwidth-1].r / 8) + (image1->getPixel()[outputIndex-outwidth+1].r / 8);
				pixel[outputIndex].g = (image1->getPixel()[outputIndex-1].g / 8) + (image1->getPixel()[outputIndex+1].g / 8)
				       	+ (image1->getPixel()[outputIndex+outwidth].g / 8) + (image1->getPixel()[outputIndex+outwidth-1].g / 8) + (image1->getPixel()[outputIndex+outwidth+1].g / 8)
				       	+ (image1->getPixel()[outputIndex-outwidth].g / 8) + (image1->getPixel()[outputIndex-outwidth-1].g / 8) + (image1->getPixel()[outputIndex-outwidth+1].g / 8);
				pixel[outputIndex].b = (image1->getPixel()[outputIndex-1].b / 8) + (image1->getPixel()[outputIndex+1].b / 8)
				       	+ (image1->getPixel()[outputIndex+outwidth].b / 8) + (image1->getPixel()[outputIndex+outwidth-1].b / 8) + (image1->getPixel()[outputIndex+outwidth+1].b / 8)
				       	+ (image1->getPixel()[outputIndex-outwidth].b / 8) + (image1->getPixel()[outputIndex-outwidth-1].b / 8) + (image1->getPixel()[outputIndex-outwidth+1].b / 8);
			}
		}
	}
	image.setPixel(pixel);
	free(pixel);
}
