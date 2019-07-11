#ifndef SOURCE_H
#define SOURCE_H
#include <image.h>

class Source
{
	protected:
		virtual void Execute() {};
		virtual const char * SourceName() = 0;
	public:
		Image image;
		Source (void) { image.setSource(this);  };
		Image * GetOutput() { return &image; };
		virtual void Update();
};
class Color : public Source
{
	private:
		int width, height;
		unsigned char red, green, blue;
	public:
		const char * SourceName() { return "Color"; };
		void Execute();
		Color(int w, int h, unsigned char r, unsigned char g, unsigned char b);
};
#endif
