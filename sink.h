#ifndef SINK_H
#define SINK_H
#include <image.h>
#include <string.h>

class Sink
{
	protected:
		const Image *image1;
		const Image *image2;
		virtual const char * SinkName() = 0;
	public:
		Sink(void) { image1 = NULL, image2 = NULL; };
		void SetInput(const Image *i1) { image1 = i1; };
		void SetInput2(const Image *i2) { image2 = i2; };
};
class CheckSum : public Sink
{
	public:
		const char * SinkName() { return "CheckSum"; };
		void OutputCheckSum(const char *filename);
};
#endif
