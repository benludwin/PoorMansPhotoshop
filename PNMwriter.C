#include <PNMwriter.h>
#include <stdio.h>

void PNMwriter::Write(const char *filename)
{
	FILE *f_out;
	f_out = fopen((char*)filename, "wb");
	fprintf(f_out, "P6\n%d %d\n%d\n", image1->getWidth(), image1->getHeight(), 255);
	fwrite(image1->getPixel(), 3 * image1->getWidth(), image1->getHeight(), f_out);
	fclose(f_out);
}
