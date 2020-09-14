#ifndef _PARAMETERS_HPP
#define _PARAMETERS_HPP
#include <string>

namespace PARAMS {

	/// <summary>
	/// les paramètres qui seront utilisés pour filtrer
	/// </summary>
	/// <item>
	/// <term>previz</term>
	/// <description>Est ce qu'on doit lancer une prévisualisation</description>
	/// </item>	
	/// <item>
	/// <term>do_distance</term>
	/// <description>Faire le filtre de distance ou non</description>
	/// </item>	
	/// <item>
	/// <term>do_downsample</term>
	/// <description>Faire le filtre downsample ou non</description>
	/// </item>	
	/// <item>
	/// <term>do_removeOutliers</term>
	/// <description>Faire le filtre outliers ou non</description>
	/// </item>	
	/// <item>
	/// <term>do_correctionGamma</term>
	/// <description>Faire la correction gamma ou non</description>
	/// </item>	
	/// <item>
	/// <term>do_tree</term>
	/// <description>Si l'arbre est déjà fait ou non</description>
	/// </item>	
	/// <item>
	/// <term>do_div</term>
	/// <description>Diviser le fichier en plusieurs ou non</description>
	/// <remarks>A faire pour le moment</remarks>
	/// </item>	
	/// <item>
	/// <term>do_potree</term>
	/// <description>Formater le résultat pour le transformer en potree ou non</description>
	/// </item>	
	/// <item>
	/// <term>do_nonUnified</term>
	/// <description>Si le fichier de sortie doit être unifié ou non</description>
	/// </item>	
	struct filter_params {

		bool previz;
		
		std::string nameIn, nameOut, nameDir;
		bool do_distance;
		bool do_downSample;
		bool do_removeOutliers;
		bool do_correctionGamma;
		bool do_tree;
		bool do_div;
		bool do_potree;
		bool do_nonUnified;
		bool do_cut;
		bool do_nocut;

		float distance_min, distance_max;
		int nonUnified_option;
		float downSample_size;
		int removeOutliers_meanK; float removeOutliers_devMultThresh, removeOutliers_distance;
		long tree_sizeLeaf;
		int num_div;
		float cut_overlap;
		std::string cut_fileIn;
		float nocut_X, nocut_Y, nocut_Z;

		filter_params(
			bool _do_previz = false,
			bool _distance = false, float _distance_min = 0.0, float _distance_max = 0.0,
			bool _downSample = false, float _downSample_size = 0.0,
			bool _removeOutliers = false, int _removeOutliers_meanK = 20, float _removeOutliers_devMultThresh = 1.0, float _removeOutliers_distance = 0.0,
			bool _correctionGamma = false, bool _do_tree = false, long _tree_sizeLeaf = 1024 * 1024 * 64,
			int _num_div = 1, bool _do_div = false, bool _do_potree = false,
			bool _do_nonUnified = false, int _nonUnified_option = -1,
			bool _do_cut = false, float _cut_overlap = -1, std::string _cut_fileIn = "",
			bool _do_nocut = false, float _nocut_X = 0.0, float _nocut_Y = 0.0, float _nocut_Z = 0.0 
		) : do_distance(_distance), distance_min(_distance_min), distance_max(_distance_max),
			do_downSample(_downSample), downSample_size(_downSample_size),
			do_removeOutliers(_removeOutliers), removeOutliers_meanK(_removeOutliers_meanK), removeOutliers_devMultThresh(_removeOutliers_devMultThresh), removeOutliers_distance(_removeOutliers_distance),
			do_tree(_do_tree), tree_sizeLeaf(_tree_sizeLeaf), previz(_do_previz), num_div(_num_div), do_div(_do_div), do_potree(_do_potree), do_nonUnified(_do_nonUnified),
			nonUnified_option(_nonUnified_option),
			do_cut(_do_cut), cut_overlap(_cut_overlap), cut_fileIn(_cut_fileIn),
			do_nocut(_do_nocut), nocut_X(_nocut_X), nocut_Y(_nocut_Y),  nocut_Z(_nocut_Z)
		{}

		filter_params operator=(const filter_params& other) {  
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
			removeOutliers_distance = other.removeOutliers_distance;
			tree_sizeLeaf = other.tree_sizeLeaf;
			previz = other.previz;
			num_div = other.num_div;
			do_potree = other.do_potree;
			do_nonUnified = other.do_nonUnified;
			nonUnified_option = other.nonUnified_option;
			do_cut = other.do_cut;
			cut_overlap = other.cut_overlap;
			cut_fileIn = other.cut_fileIn;
			do_nocut = other.do_nocut;
			nocut_X = other.nocut_X;
			nocut_Y = other.nocut_Y;
			nocut_Z = other.nocut_Z;
		}
	};
}
#endif //_PARAMETERS_HPP