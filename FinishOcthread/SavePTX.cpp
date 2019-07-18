#include "SavePTX.h"






SavePTX::SavePTX(std::string _filename, long int _numMax, BoundingBox & _bb) : SavableFile(_filename, _numMax, _bb)
{
	m_numTotal = 0;
}

int SavePTX::write(std::vector<mypt3d>& pts)
{
	//On écrit tout le vecteur 1 par 1 en gardant le nombre de points qu'on a écrit
	std::ofstream file(p_name, std::ios::out | std::ios::app);
	for (int i = 0; i < pts.size(); ++i) {
		file << pts[i].x << " " << pts[i].y << " " << pts[i].z << " " << pts[i].intensity << " " << (unsigned)pts[i].r << " " << (unsigned)pts[i].g << " " << (unsigned)pts[i].b << "\n";
	}
	file.close();

	m_numTotal += pts.size();
	return pts.size();
}

int SavePTX::writeHeader()
{
	std::ofstream file(p_name, std::ios::out | std::ios::app);
	file << "0 0 0\n";
	file << "0 0 0\n";
	file << "0 0 0\n";
	file << "0 0 0\n";

	file << "1 0 0 0\n";
	file << "0 1 0 0\n";
	file << "0 0 1 0\n";
	file << "0 0 0 1\n";
	file.close();
	return 0;
}

int SavePTX::writeFooter()
{
	std::ifstream in(p_name);;
	std::ofstream out("tmp");
	//writing rows and cols
	out << m_numTotal << "\n1\n";
	out << in.rdbuf();
	out.close();
	in.close();

	remove(p_name.c_str());
	rename("tmp", p_name.c_str());

	return 0;
}

SavePTX::~SavePTX()
{
}
