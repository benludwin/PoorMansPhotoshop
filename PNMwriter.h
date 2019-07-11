#ifndef PNMWRITER_H
#define PNMWRITER_H

#include <sink.h>

class PNMwriter : public Sink
{
	public:
		void Write(const char *filename);
		const char * SinkName() { return "PNMwriter"; };
};

#endif
