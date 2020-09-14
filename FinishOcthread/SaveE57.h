#pragma once
#include "SavableFile.h"
#include "../Octhreade57/E57Simple.h"
#include "../Octhreade57/E57Foundation.h"

#include <combaseapi.h>


/// <summary>
/// Datas qui vont être utile au remplissage du fichier e57
/// </summary>
struct e57WriteData {
	std::vector<double> xData, yData, zData;
	std::vector<double> isInvalidData;
	std::vector<double> intData;
	std::vector<int> redData, greenData, blueData;

	e57WriteData(long int num = 0) { 
		xData.resize(num); yData.resize(num); zData.resize(num);
		isInvalidData.resize(num); intData.resize(num); 
		redData.resize(num); greenData.resize(num); blueData.resize(num); 
	}
};

/// <summary>
/// classe qui va s'occuper de sauvegarder un fichier e57
/// </summary>
class SaveE57 : public SavableFile
{
public:

	SaveE57(std::string _filename,  long long int _numMax, BoundingBox& _bb);

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

	int writeHeader_v2();

	/// <summary>
	/// Voir SavableFile.writefooter
	/// </summary>
	int writeFooter() override;
	~SaveE57();

	std::unique_ptr<e57::CompressedVectorWriter> dataWriter;

private:

	/// <summary>
	/// (ptr)ImageFile du e57 pour y avoir acces partout
	/// </summary>
	/// <remarks>RAII => utiliser weak_ptr</remarks>
	e57::ImageFile *imf;
	
	/// <summary>
	/// (ptr)Writer des points du e57 pour y avoir accès partout (merde noire sinon)
	/// </summary>
	/// <remarks>RAII => utiliser weak_ptr</remarks>
	e57::CompressedVectorWriter * writer;

	/// <summary>
	/// (ptr)Créer pendant le header, utilisé au niveau du footer.
	/// </summary>
	/// <remarks>RAII => utiliser weak_ptr</remarks>
	e57::CompressedVectorNode* groups;

	/// <summary>
	/// writer pour e57
	/// </summary>
	e57::Writer eWriter;

	/// <summary>
	/// L'index crée pendant la génération du header
	/// </summary>
	int scanIndex;

	/// <summary>
	/// Les données du e57 qui seront utilisés pour copier les points par chunk
	/// </summary>
	e57WriteData datas;

	/// <summary>
	/// Nombre max de points qu'on a au début
	/// </summary>
	long long int pointCount; 

	/// <summary>
	/// Taille des buffers d'écriture
	/// </summary>
	long int writerChunckSize;

	/// <summary>
	/// Nombre max de points qu'on a ecrit
	/// </summary>
	long long num_max = 0;

	std::vector<double> xData_w;
	std::vector<double> yData_w;
	std::vector<double> zData_w;
	std::vector<uint16_t> redData_w;
	std::vector<uint16_t> greenData_w;
	std::vector<uint16_t> blueData_w;
	std::vector<double> intData_w;
	std::vector<int8_t> _isInvalidData_w;
};

