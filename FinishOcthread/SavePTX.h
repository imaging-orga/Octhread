#pragma once
#include "SavableFile.h"

class SavePTX : public SavableFile
{
public:
	SavePTX(std::string _filename, unsigned long long int _numMax, BoundingBox& _bb);
	~SavePTX();

	SavePTX(const SavePTX&) = delete;
	SavePTX& operator= (const SavePTX&) = delete;

	/// <summary>
	/// Voir SavableFile.write
	/// </summary>
	/// <param name="ptx"></param>
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

