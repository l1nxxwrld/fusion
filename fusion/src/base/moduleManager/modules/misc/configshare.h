#pragma once

#include <windows.h>
#include <winhttp.h>
#include <fstream>
#include <sstream>
#include <iostream>

namespace cfgshare
{
	std::string UploadConfig(const std::string& filePath);
}
