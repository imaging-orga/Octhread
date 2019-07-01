#pragma once
#include "Filtre.hpp"
#include "../ProceedOcthread/SavableFiles.h"
class Filtering
{
public:
	Filtering(std::string _filename);


	void filter();
	~Filtering();

private:
	SavableFiles saveFile;
};

