#pragma once

#include <string>
#include <stdarg.h>
#include <ctime>

const bool _default_en_debug_logger = false;

class Logger {
public:
	Logger(std::string prefix, bool enableDebugLog) {
		this->enabledDebugLevel = enableDebugLog;
		this->prefix = prefix;
	}

	Logger(std::string prefix) : enabledDebugLevel(_default_en_debug_logger) {
		this->prefix = prefix;
	}

	void Info(std::string msg, ...) {
		va_list ap;
		va_start(ap, msg);
		std::string str = fmt(msg, ap);
		va_end(ap);
		printf_s(this->format, currentTime().c_str(), I.c_str(), this->prefix.c_str(), str.c_str());
	}

	void Err(std::string msg, ...) {
		va_list ap;
		va_start(ap, msg);
		std::string str = fmt(msg, ap);
		va_end(ap);
		printf_s(this->format, currentTime().c_str(), E.c_str(), this->prefix.c_str(), str.c_str());
	}

	void Fatal(std::string msg, ...) {
		va_list ap;
		va_start(ap, msg);
		std::string str = fmt(msg, ap);
		va_end(ap);
		printf_s(this->format, currentTime().c_str(), F.c_str(), this->prefix.c_str(), str.c_str());
		exit(1);
	}

	void Debug(std::string msg, ...) {
		if (!enabledDebugLevel) {
			return;
		}
		va_list ap;
		va_start(ap, msg);
		std::string str = fmt(msg, ap);
		va_end(ap);
		printf_s(this->format, currentTime().c_str(), D.c_str(), this->prefix.c_str(), str.c_str());
	}


private:
	std::string prefix;
	const char* format = "%s::%s::%s::%s\n"; // [Time][Level][Prefix] Message
	const char* date_format = "%Y-%m-%d.%X";
	bool enabledDebugLevel;

	// levels
	std::string I = "INFO";
	std::string E = "ERROR";
	std::string D = "DEBUG";
	std::string F = "FATAL";

	std::string fmt(std::string f, va_list args) {
		char* buf = new char[1024];
		vsnprintf(buf, 1024, f.c_str(), args);
		return std::string(buf);
	}

	std::string currentTime() {
		time_t     now = time(0);
		struct tm  tstruct;
		char       buf[80];
		localtime_s(&tstruct, &now);
		strftime(buf, sizeof(buf), date_format, &tstruct);

		return std::string(buf);
	}
};