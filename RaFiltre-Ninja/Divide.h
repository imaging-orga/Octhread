#pragma once
#include <string>
#include <vector>
#include <fstream>
#include <sstream>

#include <cstdarg>


class Divide
{
public:
	Divide(std::string _name, unsigned _number);

	void CutOcsave();
	
	~Divide();

	const unsigned numberOfDivision;
private:
	void addOcsaveFile(std::vector<std::string>& vec);
	void fillOcsaveN(std::vector<std::ofstream>& filesWrite);
	bool firstFormat(std::string stringToCompare, std::vector<std::string>& tokens);
	std::string getFirstNAlphaNum(std::string& token, unsigned number);

	unsigned long long getNumMaxPoints();
	const std::string m_name;
};

//On prends le fichier Ocsave, 
//On créer plein de petits Ocsaves et on lance le programme X fois en fonction des fichiers