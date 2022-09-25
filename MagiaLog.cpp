#include "MagiaLog.h"

void MagiaLog::Create(const char* object, int handle_id, const char* desc)
{
	std::cout << "CREATE " << object << " " << handle_id << " " << desc << "\n";
}

void MagiaLog::Delete(const char* object, int handle_id, const char* desc)
{
	std::cout << "DELETE " << object << " " << handle_id << " " << desc << "\n";
}

void MagiaLog::Error(const char* object, int handle_id, const char* desc)
{
	std::cout << "ERROR " << object << " " << handle_id << " " << desc << "\n";
}
