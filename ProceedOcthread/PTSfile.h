#pragma once
#include "OpenableFile.h"

/// <summary>
/// Class pour ouvrir un fichier PTS
/// </summary>
class PTSfile : public OpenableFile {

public:
	PTSfile(std::string _name, long MAXPOINTSNUMPERNODE);
	~PTSfile() = default;

	/// <summary>
	/// voir OpenableFile.PTSfile
	/// </summary>
	/// <returns></returns>
	BoundingBox getBoundingBox();
	
	/// <summary>
	/// Voir OpenableFile.read
	/// </summary>
	/// <param name="distMax"></param>
	void read(float distMax = 0) override;
private:

	/// <summary>
	/// Fonction de read_
	/// </summary>
	/// <param name="file"></param>
	/// <param name="distMax"></param>
	/// <param name="num"></param>
	void read_(std::ifstream& file, float distMax, long num);

	/// <summary>
	/// Nombre max de point par noeud
	/// </summary>
	long int maxPointsPerNode;
};