#ifndef PNMREADER_H
#define PNMREADER_H

#include <source.h>

class PNMreader : public Source
{
	private:
		const char *filename;
	public:
		PNMreader(const char *f);
		virtual ~PNMreader();
		virtual void Execute();
		const char * SourceName() { return "PNMreader"; };
};

#endif
