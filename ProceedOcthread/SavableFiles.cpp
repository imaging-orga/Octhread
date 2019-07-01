#include "SavableFiles.h"



SavableFiles::SavableFiles(std::string _name) : m_name(_name), numPoints(0)
{
	std::string extension = boost::filesystem::extension(_name);
	std::transform(extension.begin(), extension.end(), extension.begin(), ::tolower);

	if (extension == "ptx")
		m_type = T_PTX;
	if (extension == "e57")
		m_type = T_E57;

	m_TMPname = remove_extension(_name) + "TMP";
}


SavableFiles::~SavableFiles()
{
}

void SavableFiles::finishWriting()
{
	if (m_type == T_PTX)
		writeHeader();
	if (m_type == T_E57)
		writePTX(pts);
}

void SavableFiles::write(std::vector<mypt3d>& pts)
{
	if (m_type == T_PTX)
		writeE57(pts);
	if (m_type == T_E57)
		writePTX(pts);
}

std::string remove_extension(const std::string& filename) {
	size_t lastdot = filename.find_last_of(".");
	if (lastdot == std::string::npos) return filename;
	return filename.substr(0, lastdot);
}

void SavableFiles::writeHeader()
{
	std::ofstream outputFile(m_name);
	std::ifstream inputFile(m_TMPname);
	std::string tempString;
	outputFile << numPoints << "\n"; // nombre de ligne
	outputFile << "1\n"; // nombre de colonnes

	outputFile << "0 0 0\n";
	outputFile << "0 0 0\n";
	outputFile << "0 0 0\n";
	outputFile << "0 0 0\n";
	outputFile << "1 0 0 0\n";
	outputFile << "0 1 0 0\n";
	outputFile << "0 0 1 0\n";
	outputFile << "0 0 0 1\n";

	outputFile << inputFile.rdbuf();

	inputFile.close();
	outputFile.close();

	std::remove(m_TMPname.c_str());
}

int SavableFiles::writePTX(std::vector<mypt3d>& pts)
{
	std::ofstream file(m_TMPname, std::ios::out | std::ios::app);

	// On rempli le fichier avec pts



	return 0;
}
