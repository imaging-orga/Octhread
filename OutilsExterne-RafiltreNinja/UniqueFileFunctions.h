#pragma once

#include <string>
#include "../Octhread/MyFile.h"
#include "../Octhread/base.hpp"
#include "../ProceedOcthread/Proceed.h"
namespace UNIQUE {
	void createXYZFromFile(std::string _dir_name, std::string _file_name, std::string _write_name);
	void createPTSFromFile(std::string _dir_name, std::string _file_name, std::string _write_name);
	int retrieveInfo(std::string _path, std::string _extension);
	int retrieveInfoe57(std::string _path);
	int retrieveInfolasz(std::string _path);
	int retrieveInfopts(std::string _path);
};

