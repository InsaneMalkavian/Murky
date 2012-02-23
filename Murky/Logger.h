#ifndef LOGGER_H
#define LOGGER_H

#include <string>
using namespace std;

class Logger {
public:
	Logger(string filename, int append);
	Logger(int append);
	~Logger();
	int AddLine(string filename);
	int AddLine(wstring filename);
	int AddLine(char *to_file);
	int IsActive(void);

private:
	errno_t Create(string filename, int append);
	FILE			*info;
	FILE			*error;
};

namespace Log {
	extern		Logger		logger;
	};

#endif // LOGGER_H