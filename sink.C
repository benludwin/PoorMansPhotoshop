#include <sink.h>
#include <string.h>
#include <logging.h>

void CheckSum::OutputCheckSum(const char *filename)
{
	if (image1 == NULL)
	{
		char msg[128];
		sprintf(msg, "%s: no input1!", SinkName());
		DataFlowException exc(SinkName(), msg);
		throw exc;
	
	}
	FILE *f_out = fopen((char *)filename, "w"); 
	int width = image1->getWidth(), height = image1->getHeight();
	unsigned char red = 0, green = 0, blue = 0;
	int outputIndex;

	for (int i = 0; i < width; i++)
	{
		for (int j = 0; j < height; j++)
		{
			outputIndex = (j * width) + i;
			red += image1->getPixel()[outputIndex].r;
			green += image1->getPixel()[outputIndex].g;
			blue += image1->getPixel()[outputIndex].b;
		}
	}
	fprintf(f_out, "CHECKSUM: %u, %u, %u\n", red, green, blue);
	fclose(f_out);
}
