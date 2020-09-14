#pragma once
#include "SavableFile.h"
#include <memory>

/// <summary>
/// classe qui va s'occuper de sauvegarder un fichier PTS
/// </summary>
class SavePTS : public SavableFile
{
public:
	SavePTS(std::string _filename, unsigned long long int _numMax, BoundingBox& _bb);
	~SavePTS();
	
	SavePTS(const SavePTS&) = delete;
	SavePTS& operator= (const SavePTS&) = delete;
	

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
	std::ofstream m_stream;
	unsigned long long int m_numEffective;
};

