#include "SeveralFileFunctions.h"

namespace GROUP {
	void createXYZFromFolder(std::string _dir_name)
	{
		boost::filesystem::path p(_dir_name);
		std::string just_name = p.stem().string();
		CreateDirectory(just_name.c_str(), NULL);

		std::vector<std::string> names = FileOct::nameOfNon0files(_dir_name);
		std::vector<std::string> write_names;
		for (std::string name : names) {
			std::string tmp = just_name+"//"+name+"-f.xyz";
			write_names.push_back(tmp);
		}

		for (int i = 0; i < names.size(); ++i) {
			UNIQUE::createXYZFromFile(_dir_name, names[i], write_names[i]);
		}
		getchar();
	}
};