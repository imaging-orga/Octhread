#pragma once
#include "OpenableFile.h"

#include "../Octhreade57/E57Simple.h"
#include "../Octhreade57/E57Foundation.h"

enum E57_TYPE { NON_UNIFIED = 0, UNIFIED };

/// <summary>
/// Class pour ouvrir un fichier e57
/// </summary>
class e57File : public OpenableFile
{
public:
	e57File(std::string _name, long MAXPOINTSNUMPERNODE);
	~e57File() = default;

	/// <summary>
	/// Voir OpenableFile.getBoundingBox
	/// </summary>
	/// <returns></returns>
	BoundingBox getBoundingBox();
	
	/// <summary>
	/// Voir OpenableFile.distMax
	/// </summary>
	/// <param name="distMax"></param>
	void read(float distMax = 0) override;

private :
	E57_TYPE e_type;
	/// <summary>
	/// Lit un fichier Unifié 
	/// </summary>
	void read_Unified();

	/// <summary>
	/// Lit un fichier Non Unifié
	/// </summary>
	/// <param name="distMax"></param>
	/// <returns></returns>
	void read_NonUnified(float distMax);

	/// <summary>
	/// Recuperer la boundingBox d'un fichier non unifié
	/// </summary>
	/// <returns></returns>
	std::pair<pt3d, pt3d> read_NonUnifiedForBoundingBox();


};

