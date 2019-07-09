#pragma once
#include "Filtre.hpp"
#include "../FinishOcthread/SaveFactor.hpp"
class Filtering
{
public:
	Filtering(std::string _filename);


	void filter();
	~Filtering();

private:
	std::string m_name;
	SavableFile* save;
};

