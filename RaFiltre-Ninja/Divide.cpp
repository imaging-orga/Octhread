#include "Divide.h"

#include <cstdarg>

Divide::Divide(std::string _name, unsigned _number) : m_name(_name), numberOfDivision(_number)
{
}

/*choisir entre 2, 4 et 8*/
void Divide::CutOcsave()
{
	std::vector<std::string> filenames;
	addOcsaveFile(filenames); // Creer X fichier de la forme m_name\\.Ocsave_n, avec n compris entre 0 et numberOfDivision
	std::string OcName = m_name + "\\.OcSave";

	std::string line;
	std::ifstream fileRead(OcName, std::ios::in);
	std::vector<std::ofstream> filesWrite;
	filesWrite.resize(numberOfDivision);
	for (int i = 0; i < numberOfDivision; ++i) {
		filesWrite[i].open(filenames[i], std::ios::out);
	}

	fillOcsaveN(filesWrite);
	//std::vector<std::vector<std::string>> tokens;
	//tokens.resize(numberOfDivision);
	//fillWithGoodNames(tokens);
	//while (getline(fileRead, line)) {
	//	std::istringstream ss(line);
	//	std::string first;
	//	ss >> first;
	//	std::string first2 = getFirstNAlphaNum(first, 2);
	//	for (int i = 0; i < tokens.size(); ++i) {
	//		if (firstFormat(first2, tokens[i])) {
	//			fileWrite[i] << line << "\n";
	//		}
	//	}
	//}
	for (int i = 0; i < filesWrite.size(); ++i)
		filesWrite[i].close();
	fileRead.close();
}


void Divide::addOcsaveFile(std::vector<std::string>& vec)
{
	for (int i = 0; i < numberOfDivision; ++i) {
		std::string n_tmp = m_name + "\\.OcSave_" + std::to_string(i);
		vec.push_back(n_tmp);
	}
}

bool Divide::firstFormat(std::string stringToCompare, std::vector<std::string>& tokens) {
	bool ret = false;
	for (int i = 0; i < tokens.size(); ++i) {
		if (stringToCompare == tokens[i])
			ret = true;
	}
	return ret;
}

std::string Divide::getFirstNAlphaNum(std::string& token, unsigned number) {
	if (token == "0")
		return token;
	else if (token.size() >= number)
		return token.substr(0, number);
	else
		return "";
}

unsigned long long Divide::getNumMaxPoints() {
	std::string newName = m_name + "\\.OcSave";
	std::string line;
	std::ifstream file(newName, std::ios::in);
	unsigned long long cpt = 0;
	while (std::getline(file, line)) {
		std::istringstream ss(line);
		unsigned long numPoints;
		std::string nameFile;
		ss >> nameFile >> numPoints;
		cpt += numPoints;
	}

	return cpt;
}
void Divide::fillOcsaveN(std::vector<std::ofstream>& filesWrite) {
	//Il faut :
	//remplir tokens 1 et tokens 2 avec les "0", "00" , "01" les mieux.
	//tout d'abord il faut calculer le nombre max points qu'il y aura.
	unsigned long long numMax = getNumMaxPoints(); // Recupere le nombre de points max
	//Pour le moment, on divise par le nombre n'imporet comment et si jamais on arrive a faire mieux on fera mieux
	//On lit les X premiers fichiers jusqu'a atteindre numMax_divided;
	
	unsigned long long numMaxDivided = numMax / filesWrite.size();
	unsigned long long count = 0;
	int offset = 0;
	std::string newName = m_name + "\\.OcSave";
	std::ifstream fileRead(newName, std::ios::in);
	std::string line;
	//First line, avec la boite englobante globale
	std::getline(fileRead, line);
	for (int i = 0; i < filesWrite.size(); ++i) {
		filesWrite[i] << line << "\n";
	}

	while (std::getline(fileRead, line)) {
		std::istringstream ss(line);
		std::string name;
		unsigned long number;
		ss >> name >> number;
		count += number;
		filesWrite[offset] << line << "\n";

		if (count > numMaxDivided) {
			offset++;
			count = 0;
		}
	}
}


Divide::~Divide()
{
}
