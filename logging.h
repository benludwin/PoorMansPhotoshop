#ifndef  LOGGING_H
#define  LOGGING_H

#include <exception>
#include <stdio.h>
#include <string.h>

using std::exception;

class DataFlowException : public exception {
	public:
        	DataFlowException(const char *type, const char *error);
    		virtual const char  *what() const throw() { return msg; };

	protected:
    		char msg[1024];
};


class Logger
{
	public:
		static void LogEvent(const char *event);
    		static void Finalize() { fclose(logger); };

  	private:
    		static FILE  *logger;
};

#endif
