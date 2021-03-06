#include "SavePTX.h"



SavePTX::SavePTX(std::string _filename, unsigned long long int _numMax, BoundingBox & _bb) : SavableFile(_filename, _numMax, _bb), m_stream(std::ofstream(_filename, std::ios::out))
{
	m_numEffective = 0;
	writeHeader();
}

SavePTX::~SavePTX()
{
}

int SavePTX::write(std::vector<mypt3d>& pts) {
	for (int i = 0; i < pts.size(); ++i) {
		mypt3d& pt = pts[i];
		float intens = pt.intensity;
		int n_intens = (intens * 4096) - 2048;
		m_stream << pt.x << " " << pt.y << " " << pt.z << " " <<
			n_intens << " " << (unsigned)pt.r << " " << (unsigned)pt.g << " " << (unsigned)pt.b << "\n";
		m_numEffective++;
	}
	return 1;
}

int SavePTX::writeHeader() {
	m_stream << "                   " << std::endl; //On laisse de l'espace pour �crire le nombre de points APRES!
	return 1;
}

int SavePTX::writeFooter() {
	m_stream.seekp(0);
	m_stream << m_numEffective;
	m_stream.close();
	return 1;
}