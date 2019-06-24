#pragma once
#include <string>

class MyFile
{
public:
	MyFile(std::string filename);
	~MyFile();

	void createFile();

private:
	std::string m_filename;
};

