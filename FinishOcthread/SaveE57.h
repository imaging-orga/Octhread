#pragma once
#include "SavableFile.h"
#include "../Octhreade57/E57Simple.h"
#include "../Octhreade57/E57Foundation.h"

#include <combaseapi.h>

/*!
 * \brief Datas qui vont �tre utile au remplissage du fichier e57
 */

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

/*!
 * Class SaveE57
 * \brief classe qui va s'occuper de sauvegarder un fichier e57
 */
class SaveE57 : public SavableFile
{
public:

	SaveE57(std::string _filename, unsigned long long int _numMax, BoundingBox& _bb);


	int write(std::vector<mypt3d>& pts) override;
	int writeHeader() override;
	int writeFooter() override;
	~SaveE57();


private:
	/*!
	 *  (ptr)ImageFile du e57 pour y avoir acces partout
	 * \todo : RAII => utiliser weak_ptr
	 */
	e57::ImageFile *imf;

	/*!
	 * (ptr)Writer des points du e57 pour y avoir acc�s partout (merde noire sinon)
	 * \todo : RAII => utiliser weak_ptr
	 */
	e57::CompressedVectorWriter * writer;

	/*!
	 * (ptr)Cr�er pendant le header, utilis� au niveau du footer.
	 * \todo : RAII => utiliser weak_ptr
	 */
	e57::CompressedVectorNode* groups;

	/*!
	 * Writer pour e57
	 */
	e57::Writer eWriter;

	/*!
	 * L'index cr�e pendant la g�n�ration du header
	 */
	int scanIndex;

	/*!
	 * Les donn�es du e57 qui seront utilis�s pour copier les points par chunk
	 * 
	 */
	e57WriteData datas;

	/*!
	 * Nombre max de points
	 */
	unsigned long long int pointCount; 

	/*!
	 * Taille des buffers d'�criture
	 * 
	 */
	long int writerChunckSize;

	/*!
	 * Nombre max de points
	 */
	long long unsigned num_max = 0;
};

