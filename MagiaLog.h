#ifndef MAGIALOG_CLASS_H
#define MAGIALOG_CLASS_H

#include <iostream>

namespace MagiaLog {

	void Create(const char* object, int handle_id, const char* desc = "");
	void Delete(const char* object, int handle_id, const char* desc = "");
	void Error(const char* object, int handle_id, const char* desc = "");

}

#endif
