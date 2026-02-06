#include "Logger.h"

#include <Windows.h>

using namespace Engine;

void Logger::Log(const std::string& message) {
	OutputDebugStringA(message.c_str());
}