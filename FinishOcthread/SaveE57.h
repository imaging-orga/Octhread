#pragma once
#include "SavableFile.h"
class SaveE57 : public SavableFile
{
public:
	SaveE57(std::string _filename);
	void write() override;
	void writeHeader() override;
	~SaveE57();
};

