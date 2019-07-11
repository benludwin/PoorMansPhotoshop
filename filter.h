#ifndef FILTER_H
#define FILTER_H

#include <source.h>
#include <sink.h>

class Filter : public Source, public Sink
{
	protected:
		virtual const char * FilterName() = 0;
		virtual const char * SourceName();
		virtual	const char * SinkName();
	public:
		virtual void Update();
};
class Shrinker : public Filter
{
	public:
		const char * FilterName() { return "Shrinker"; } ;
		void Execute();
};
class LRCombine : public Filter
{
	public:
		const char * FilterName() { return "LRCombine"; } ;
		void Execute();
};
class TBCombine	: public Filter
{
	public:
		const char * FilterName() { return "TBCombine"; } ;
		void Execute();
};
class Blender : public Filter
{
	private:
		double factor;
	public:
		const char * FilterName() { return "Blender"; } ;
		void SetFactor(double d) { factor = d; };
		void Execute();
};
class Mirror : public Filter
{
	public:
		const char * FilterName() { return "Mirror"; } ;
		void Execute();
};
class Rotate : public Filter
{
	public:
		const char * FilterName() { return "Rotate"; } ;
		void Execute();
};
class Subtract : public Filter
{
	public:
		const char * FilterName() { return "Subtract"; } ;
		void Execute();
};
class Grayscale : public Filter
{
	public:
		const char * FilterName() { return "Grayscale"; } ;
		void Execute();
};
class Blur : public Filter
{
	public:
		const char * FilterName() { return "Blur"; } ;
		void Execute();
};
#endif
