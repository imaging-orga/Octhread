#pragma once
#include "OpenableFile.h"
#include "lasreader.hpp"

/// <summary>
/// Class pour ouvrir un fichier LAS
/// </summary>
class LASfile : public OpenableFile {
public :
	LASfile(std::string _name, long MAXPOINTSNUMPERNODE);
	~LASfile() = default;

	/// <summary>
	/// Voir OpenanbleFile.getBoundingBox
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
	/// Nombre de points max par noeuds
	/// </summary>
	long int maxPointsPerNode;

	/// <summary>
	/// Reader pour fichier LAS
	/// </summary>
	LASreader* lasreader;
};