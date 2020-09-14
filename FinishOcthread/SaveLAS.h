#pragma once
#include "SavableFile.h"
#include "laswriter.hpp"

/// <summary>
/// classe qui va s'occuper de sauvegarder un fichier LAS et LAZ
/// </summary>
class SaveLAS : public SavableFile
{
public:
	SaveLAS(std::string _filename, unsigned long long int _numMax, BoundingBox& _bb);
	~SaveLAS();

	SaveLAS(const SaveLAS&) = delete;
	/// <summary>
	/// Voir SavableFile.write
	/// </summary>
	/// <param name="pts"></param>
	/// <returns></returns>
	int write(std::vector<mypt3d>& pts) override;

	/// <summary>
	/// Voir SavableFile.writeHeader
	/// </summary>
	int writeHeader() override;

	/// <summary>
	/// Voir SavableFile.writeFooter
	/// </summary>
	int writeFooter() override;

private:

	/// <summary>
	/// Nombre de points actuel
	/// </summary>
	long long m_numEffective;

	/// <summary>
	/// Header pour fichier las
	/// </summary>
	LASheader lasheader;

	/// <summary>
	/// (ptr) writer pour fichier las
	/// </summary>
	/// <remark> utiliser weak_ptr</remark>
	LASwriter* laswriter;

	/// <summary>
	/// Offset utilisés pour la sauvegarde du LAS
	/// </summary>
	int offset_x = 0, offset_y = 0, offset_z = 0;
};

