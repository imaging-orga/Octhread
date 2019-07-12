#pragma once
#include "SavableFile.h"
#include "../Octhreade57/E57Simple.h"
#include "../Octhreade57/E57Foundation.h"

#include <combaseapi.h>

/*!
 * \brief Datas qui vont être utile au remplissage du fichier e57
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

	SaveE57(std::string _filename, long int _numMax, BoundingBox& _bb);


	int write(std::vector<mypt3d>& pts) override;
	int writeHeader() override;
	int writeFooter() override;
	~SaveE57();


private:

	/*!
	 *
	 * \brief Ecrire dans le compressedVectorWriter ce qui va bien
	 *
	 * \param writer
	 * \param size_writer
	 * \param pts
	 * \param cartesianX
	 * \param cartesianY
	 * \param cartesianZ
	 * \param cartesianInvalidState
	 * \param intensity
	 * \param red
	 * \param green
	 * \param blue
	 * \return
	 */
	int  writeTe(e57::CompressedVectorWriter* writer, int size_writer, std::vector<mypt3d>& pts,
		std::vector<double>& cartesianX, std::vector<double>& cartesianY, std::vector<double>& cartesianZ,
		std::vector<double>& cartesianInvalidState,
		std::vector<double>& intensity,
		std::vector<int>& red, std::vector<int>& green, std::vector<int>& blue);



	/*!
	 *  (ptr)ImageFile du e57 pour y avoir acces partout
	 * \todo : RAII => utiliser weak_ptr
	 */
	e57::ImageFile *imf;

	/*!
	 * (ptr)Writer des points du e57 pour y avoir accès partout (merde noire sinon)
	 * \todo : RAII => utiliser weak_ptr
	 */
	e57::CompressedVectorWriter * writer;

	/*!
	 * (ptr)Créer pendant le header, utilisé au niveau du footer.
	 * \todo : RAII => utiliser weak_ptr
	 */
	e57::CompressedVectorNode* groups;

	/*!
	 * Writer pour e57
	 */
	e57::Writer eWriter;

	/*!
	 * L'index crée pendant la génération du header
	 */
	int scanIndex;

	/*!
	 * Les données du e57 qui seront utilisés pour copier les points par chunk
	 * 
	 */
	e57WriteData datas;

	/*!
	 * Nombre max de points
	 */
	long int pointCount; 

	/*!
	 * Taille des buffers d'écriture
	 * 
	 */
	long int writerChunckSize;

	/*!
	 * Nombre max de points?
	 * \todo : je crois qu'il sert à rien parcequ'on ne l'utilise pas 
	 */
	long int num_max = 0;
};

