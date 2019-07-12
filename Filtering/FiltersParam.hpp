#ifndef _PARAMETERS_HPP
#define _PARAMETERS_HPP
#include <string>

namespace PARAMS {
	/*!
	* \brief filter_params : les paramètres qui seront utilisés pour filtrer
	* \filtres : 
	*	-distance
	*	-downSample
	*	-removeOutliers
	*	-correctionGamma
	* 
	* do_tree : faire l'arbre ou non
	*
	*/
	struct filter_params {

		std::string nameIn, nameOut, nameDir;
		bool do_distance;
		bool do_downSample;
		bool do_removeOutliers;
		bool do_correctionGamma;
		bool do_tree;

		float distance_min, distance_max;
		float downSample_size;
		int removeOutliers_meanK; float removeOutliers_devMultThresh;

		filter_params(
			bool _distance = false, float _distance_min = 0.0, float _distance_max = 0.0,
			bool _downSample = false, float _downSample_size = 0.0,
			bool _removeOutliers = false, int _removeOutliers_meanK = 20, float _removeOutliers_devMultThresh = 1.0,
			bool _correctionGamma = true, bool _do_tree = true
		) : do_distance(_distance), distance_min(_distance_min), distance_max(_distance_max),
			do_downSample(_downSample), downSample_size(_downSample_size),
			do_removeOutliers(_removeOutliers), removeOutliers_meanK(_removeOutliers_meanK), removeOutliers_devMultThresh(_removeOutliers_devMultThresh),
			do_tree(_do_tree)
		{}

		filter_params operator=(const filter_params& other) {  // A faire + rebuild
			nameIn = other.nameIn;
			nameOut = other.nameOut;
			nameDir = other.nameDir;
			do_distance = other.do_distance;
			do_downSample = other.do_downSample;
			do_removeOutliers = other.do_removeOutliers;
			do_correctionGamma = other.do_correctionGamma;
			do_tree = other.do_tree;
			distance_min = other.distance_min; distance_max = other.distance_max;
			downSample_size = other.downSample_size;
			removeOutliers_meanK = other.removeOutliers_meanK; removeOutliers_devMultThresh = other.removeOutliers_devMultThresh;
		}
	};
}
#endif //_PARAMETERS_HPP