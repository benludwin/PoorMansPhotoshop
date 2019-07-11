#include <PNMreader.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <logging.h>

void PNMreader::Execute()
{
	FILE *f_in = fopen(filename, "rb");
	if (f_in == NULL)
	{
		char msg[128];
		sprintf(msg, "Unable to open file: %s\n", filename);
		DataFlowException exc(SourceName(), msg);
		throw exc;
	}
	char magicNum[128];
	int width, height, maxval;

	fscanf(f_in, "%s\n%d %d\n%d\n", magicNum, &width, &height, &maxval);
	Pixel * pixel = (Pixel *)(malloc(3 * height * width));
	fread(pixel, 3*width, height, f_in);
	image.ResetSize(width, height);
	image.setPixel(pixel);
	fclose(f_in);
	free(pixel);
}
PNMreader::PNMreader(const char *f)
{
	filename = (char *)malloc(strlen(f)+1);
	strcpy((char*)filename, f);
}
PNMreader::~PNMreader()
{
	free((char*)filename);
}
