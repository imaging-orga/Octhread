#pragma once
#include "SavableFile.h"
#include "SaveE57.h"
#include "boost/filesystem.hpp"
#include <boost/algorithm/string.hpp>
class SaveFactor {
public:
	static SavableFile* get(std::string _filename) {
		std::string ext = boost::filesystem::extension(_filename);
		
		for (auto& c : ext)
			c = tolower(c);
		
		if (ext == "e57") {
			return new SaveE57(_filename);
		}

		return nullptr;
	}
};