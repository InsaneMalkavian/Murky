#include "logger.h"
#include <Windows.h>

Logger Log::logger(FALSE);

Logger::Logger(string filename, int append) {
	Create(filename.c_str(), append);
}

Logger::Logger(int append) {
	Create("system.log", append);
}

Logger::~Logger() {
	if (info)
		fclose(info);
}

errno_t Logger::Create(string filename, int append) {
	info = error = NULL;
	return fopen_s(&info, filename.c_str(), append ? "a" : "w" );
}

int Logger::IsActive(void) {
	return info ? 1 : 0; // we could simply return pointer to info
	}

int Logger::AddLine(string to_file) {
	int res = -1;
	if (info) {
		res = fputs (to_file.c_str(),info);
		fputc('\n',info);
		fflush (info);
		}
	return res;
	}

int Logger::AddLine(wstring to_file) {
	int res = -1;
	if (info) {
		char *str = new char[to_file.length()+1];
		sprintf(str,"%ls",to_file.c_str());
		res = fputs (str,info);
		fputc('\n',info);
		fflush (info);
		delete []str;
		}
	return res;
	}

int Logger::AddLine(char *to_file) {
	int res = -1;
	if (info) {
		res = fputs (to_file,info);
		fputc('\n',info);
		fflush (info);
		}
	return res;
	}
