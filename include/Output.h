#ifndef Output_h
#define Output_h

#include "GOSIASimFitter.h"
#include "GOSIAReader.h"
#include "ScalingParameter.h"
#include <algorithm>

class Output {

	public:
		Output(GOSIASimFitter*);
		~Output()	{;}
		void			Write(const char*);

	private:
		void			CalculateScaling();
		GOSIASimFitter		*fitter;

		std::string workingDir;
		std::vector<double>			parameters;			/*!< Matrix elements for both beam and target, and common scaling factors */
		std::vector<MatrixElement>		ME_Beam;			/*!< Beam matrix elements - Preset to relate parameters to beam matrix elements */
		std::vector<RelativeMatrixElement>	ME_BeamRel;			/*!< Beam matrix elements - Preset to relate parameters to beam matrix elements */
		std::vector<MatrixElement>		ME_Target;			/*!< Target matrix elements - Preset to relate parameters to target matrix elements */
		std::vector<RelativeMatrixElement>	ME_TargetRel;			/*!< Beam matrix elements - Preset to relate parameters to beam matrix elements */
		std::vector<ScalingParameter>		scalingParameters;		/*!< Scaling parameters - common to both target and beam excitations */

		std::vector<TMatrixD>			correctionFactors_Beam;		/*!< Point correction factor for the beam (accounts for the angular distribution of the cross section) */
		std::vector<TMatrixD>			correctionFactors_Target;	/*!< Point correction factor for the target (accounts for the angular distribution of the cross section) */

		std::vector<ExperimentData>		exptData_Beam;			/*!< Beam excitation experimental data (one vector entry for each data subset) */
		std::vector<ExperimentData>		exptData_Target;		/*!< Target excitation experimental data (one vector entry for each data subset) */

		std::vector<LitLifetime>		litLifetimes_Beam;		/*!< Literature data for the beam, lifetimes */
		std::vector<LitLifetime>		litLifetimes_Target;		/*!< Literature data for the target, lifetimes */

		std::vector<LitBranchingRatio>		litBranchingRatios_Beam;	/*!< Literature data for the beam, branching ratios */
		std::vector<LitBranchingRatio>		litBranchingRatios_Target;	/*!< Literature data for the target, branching ratios */

		std::vector<LitMixingRatio>		litMixingRatios_Beam;		/*!< Literature data for the beam, mixing ratios */
		std::vector<LitMixingRatio>		litMixingRatios_Target;		/*!< Literature data for the target, mixing ratios */

		std::vector<LitMatrixElement>		litMatrixElements_Beam;		/*!< Literature data for the beam, matrix elements */
		std::vector<LitMatrixElement>		litMatrixElements_Target;		/*!< Literature data for the target, matrix elements */

		std::vector<float>			expt_weights;

		std::string				beamBSTFile;
		std::string				targetBSTFile;

};

#endif
