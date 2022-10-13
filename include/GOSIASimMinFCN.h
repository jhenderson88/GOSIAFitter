#ifndef GOSIASimMinFCN_h
#define GOSIASimMinFCN_h

#include "ScalingParameter.h"
#include "Nucleus.h"
//#include "Minuit2/FCNBase.h"
#include "Literature.h"
#include "ExperimentalInput.h"
#include "MatrixElement.h"
#include "TransitionRates.h"
#include "GOSIAReader.h"
#include "ScalingFitFCN.h"

//#include "Minuit2/Minuit2Minimizer.h"
#include "Math/Functor.h"
#include "Math/Factory.h"
#include "Math/Minimizer.h"
//#include "Minuit2/FunctionMinimum.h"
//#include "Minuit2/MnUserParameterState.h"
//#include "Minuit2/MnMigrad.h" 
//#include "Minuit2/MnMinos.h"
//#include "Minuit2/MnContours.h"
//#include "Minuit2/MnPlot.h"

#include <ctime>
#include <thread>
#include <iomanip>
#include <vector>
#include <chrono>
#include <stdio.h>
#include <stdlib.h>

class MatrixElements;
class ExperimentData;
class LitLifetime;
class LitBranchingRatio;
class LitMixingRatio;
class LitMatrixElement;

///
///	\class CoulExSimMinFCN
///
///	\brief Contains the definition of the chi-squared function used in simultaneous minimization
///	of beam and target nuclei
///
///	Normalization to target excitation is a powerful method for the study of radioactive ion beams
///	with Coulomb excitation. The normalization for the target and beam are the same (same target
///	thickness, beam intensity, etc.), so they can be fitted simultaneously.
///
///	This class is very similar to the "normal" CoulExMinFCN, except that the inputs and parameters
///	are provided for both the beam and target, with the exception of common scaling parameters.
///
class GOSIASimMinFCN { // : public ROOT::Minuit2::FCNBase{

	public : 

		GOSIASimMinFCN(std::vector<ExperimentData> d_beam, std::vector<ExperimentData> d_target) 
											{ 
												exptData_Beam	= d_beam;
												exptData_Target	= d_target;													
												verbose = false;  
												iter = 0;	
												nThreads = 1;
												fLikelihood 	= false;
                        workingDir = ".";

												size_t	Nexpts = 0;
												if(exptData_Beam.size() > exptData_Target.size())
													Nexpts = exptData_Beam.size();
												else
													Nexpts = exptData_Target.size();

												expt_weights.resize(Nexpts);
												std::fill(expt_weights.begin(),expt_weights.end(),1);

											}	/*!< Construct object with vector of experimental data to be fit */
		virtual ~GOSIASimMinFCN()						{;					}

		void	SetupCalculation();	/*!< Prepare the calculation */

		virtual void ClearAll();	/*!< Clear all vectors */

  void SetWorkingDir(std::string s) { workingDir = s; }
  
		void	SetBeamGOSIAInput(std::string s)				{ beamGOSIAFile_inp = s;		}
		void	SetTargetGOSIAInput(std::string s)				{ targetGOSIAFile_inp = s;		}

		std::string	GetBeamGOSIAInput()				const	{ return beamGOSIAFile_inp;		}
		std::string	GetTargetGOSIAInput()				const	{ return targetGOSIAFile_inp;		}

		void	SetBeamGOSIAOutput(std::string s)				{ beamGOSIAFile_out = s;		}
		void	SetTargetGOSIAOutput(std::string s)				{ targetGOSIAFile_out = s;		}

		std::string	GetBeamGOSIAOutput()				const	{ return beamGOSIAFile_out;		}
		std::string	GetTargetGOSIAOutput()				const	{ return targetGOSIAFile_out;		}

		void	SetBeamBST(std::string s)					{ beamBSTFile = s;			}
		void	SetTargetBST(std::string s)					{ targetBSTFile = s;			}

		std::string	GetBeamBST()					const	{ return beamBSTFile;			}
		std::string	GetTargetBST()					const	{ return targetBSTFile;			}

		void	SetBeamMapping(std::vector<int> i, std::vector<int> f, std::vector<int> l)
		{
			beamMapping_i	= i;
			beamMapping_f	= f;
			beamMapping_l	= l;
		}
		void	SetTargetMapping(std::vector<int> i, std::vector<int> f, std::vector<int> l)
		{
			targetMapping_i	= i;
			targetMapping_f	= f;
			targetMapping_l	= l;
		}

		std::vector<int>	GetBeamMapping_I()			const	{ return beamMapping_i;			}
		std::vector<int>	GetBeamMapping_F()			const	{ return beamMapping_f;			}
		std::vector<int>	GetBeamMapping_L()			const	{ return beamMapping_l;			}

		std::vector<int>	GetTargetMapping_I()			const	{ return targetMapping_i;		}
		std::vector<int>	GetTargetMapping_F()			const	{ return targetMapping_f;		}
		std::vector<int>	GetTargetMapping_L()			const	{ return targetMapping_l;		}

		std::vector<ScalingParameter>	GetScalingParameters()			{ return scalingParameters;		}	/*!< Return the vector of ScalingParameter objects for fitting */
		void	SetScalingParameters(std::vector<ScalingParameter> s)		{ scalingParameters = s;		}	/*!< Define the vector of ScalingParameter objects for fitting */
		void	AddScalingParameter(ScalingParameter s)				{ scalingParameters.push_back(s);	}	/*!< Append a new ScalingParameter to the vector */
		void	ClearScalingParameters()					{ scalingParameters.clear();		}	/*!< Clear the vector of ScalingParameter objects */

		double 	up() const	 						{ return theErrorDef;			}	/*!< Required by ROOT::Minimizer */
		double 	operator()(const double*);											/*!< Required by ROOT::Minimizer */
		void	SetErrorDef(double def)						{ theErrorDef = def;			}	/*!< Required by ROOT::Minimizer */

		// The below are duplicated for beam and target excitation:
		void	SetBeamMatrixElements(std::vector<MatrixElement> m)		{ ME_Beam = m;				}	/*!< Define the vector of beam MatrixElement objects to be fitted */
		std::vector<MatrixElement>	GetBeamMatrixElements() 	const	{ return ME_Beam;			}	/*!< Return the vector of beam MatrixElement objects to be fitted */
  void	SetBeamRelativeElements(std::vector<RelativeMatrixElement> m)		{ ME_BeamRel = m;				}	/*!< Define the vector of beam RelativeMatrixElement objects to be varied */  
  std::vector<RelativeMatrixElement>	GetBeamRelativeElements() const {	return ME_BeamRel; }	/*!< Define the vector of beam RelativeMatrixElement objects to be varied */
		void	SetTargetMatrixElements(std::vector<MatrixElement> m)		{ ME_Target = m;			}	/*!< Define the vector of target MatrixElement objects to be fitted */
		std::vector<MatrixElement>	GetTargetMatrixElements() 	const	{ return ME_Target;			}	/*!< Return the vector of target MatrixElement objects to be fitted */
  void	SetTargetRelativeElements(std::vector<RelativeMatrixElement> m)		{ ME_TargetRel = m;				}	/*!< Define the vector of beam RelativeMatrixElement objects to be varied */  
  std::vector<RelativeMatrixElement>	GetTargetRelativeElements() const {	return ME_TargetRel; }	/*!< Define the vector of beam RelativeMatrixElement objects to be varied */

		void	SetBeamData(std::vector<ExperimentData> d)			{ exptData_Beam = d;			}	/*!< Define the vector of beam ExperimentData objects (one for each experiment) */
		std::vector<ExperimentData>	GetBeamData() 			const	{ return exptData_Beam;			}	/*!< Return the vector of beam ExperimentData objects (one for each experiment) */
		void	SetTargetData(std::vector<ExperimentData> d)			{ exptData_Target = d;			}	/*!< Define the vector of target ExperimentData objects (one for each experiment) */
		std::vector<ExperimentData>	GetTargetData()			const	{ return exptData_Target;		}	/*!< Return the vector of target ExperimentData objects (one for each experiment) */

		void	SetBeamLitLifetimes(std::vector<LitLifetime> l)			{ litLifetimes_Beam = l;		}	/*!< Define the vector of beam LitLifetime objects defining the literature lifetime data for fitting */
		std::vector<LitLifetime>	GetBeamLitLifetimes() 		const	{ return litLifetimes_Beam;		}	/*!< Return the vector of beam LitLifetime objects defining the literature lifetime data for fitting */
		void	SetTargetLitLifetimes(std::vector<LitLifetime> l)		{ litLifetimes_Target = l;		}	/*!< Define the vector of target LitLifetime objects defining the literature lifetime data for fitting */
		std::vector<LitLifetime>	GetTargetLitLifetimes() 	const	{ return litLifetimes_Target;		}	/*!< Return the vector of target LitLifetime objects defining the literature lifetime data for fitting */

		void	SetBeamLitBranching(std::vector<LitBranchingRatio> b) 		{ litBranchingRatios_Beam = b;		}	/*!< Define the vector of beam LitBranchingRatio objects defining the literature branching ratio data for fitting */
		std::vector<LitBranchingRatio>	GetBeamLitBranching() 		const	{ return litBranchingRatios_Beam;	}	/*!< Return the vector of beam LitBranchingRatio objects defining the literature branching ratio data for fitting */
		void	SetTargetLitBranching(std::vector<LitBranchingRatio> b) 	{ litBranchingRatios_Target = b;	}	/*!< Define the vector of target LitBranchingRatio objects defining the literature branching ratio data for fitting */
		std::vector<LitBranchingRatio>	GetTargetLitBranching() 	const	{ return litBranchingRatios_Target;	}	/*!< Return the vector of target LitBranchingRatio objects defining the literature branching ratio data for fitting */
	
		void	SetBeamLitMixing(std::vector<LitMixingRatio> m)			{ litMixingRatios_Beam = m;		}	/*!< Define the vector of beam LitMixingRatio objects defining the literature mixing ratio data for fitting */
		std::vector<LitMixingRatio>	GetBeamLitMixing() 		const	{ return litMixingRatios_Beam;		}	/*!< Return the vector of beam LitMixingRatio objects defining the literature mixing ratio data for fitting */
		void	SetTargetLitMixing(std::vector<LitMixingRatio> m)		{ litMixingRatios_Target = m;		}	/*!< Define the vector of target LitMixingRatio objects defining the literature mixing ratio data for fitting */
		std::vector<LitMixingRatio>	GetTargetLitMixing() 		const	{ return litMixingRatios_Target;	}	/*!< Return the vector of target LitMixingRatio objects defining the literature mixing ratio data for fitting */
	
		void	SetBeamLitMatrixElements(std::vector<LitMatrixElement> m)		{ litMatrixElements_Beam = m;		}	/*!< Define the vector of beam LitMatrixElement objects defining the literature mixing ratio data for fitting */
		std::vector<LitMatrixElement>	GetBeamLitMatrixElements() 		const	{ return litMatrixElements_Beam;		}	/*!< Return the vector of beam LitMatrixElement objects defining the literature mixing ratio data for fitting */
		void	SetTargetLitMatrixElements(std::vector<LitMatrixElement> m)		{ litMatrixElements_Target = m;		}	/*!< Define the vector of target LitMatrixElement objects defining the literature mixing ratio data for fitting */
		std::vector<LitMatrixElement>	GetTargetLitMatrixElements() 		const	{ return litMatrixElements_Target;	}	/*!< Return the vector of target LitMatrixElement objects defining the literature mixing ratio data for fitting */

		std::vector<TMatrixD>	GetEffectiveCrossSection_Beam()		const	{ return EffectiveCrossSection_Beam;	}	/*!< Return the beam's "effective cross section" = direct population + feeding */
		std::vector<TMatrixD>	GetEffectiveCrossSection_Target() 	const	{ return EffectiveCrossSection_Target;	}	/*!< Return the target's "effective cross section" = direct population + feeding */

		void	SetBaseBeamNucleus(Nucleus* nucl)				{ fNucleus_Target_Base = *nucl;		}	/*!< Define the base beam nucleus (not to be varied in fitting) */
		Nucleus				GetBaseBeamNucleus() 		const	{ return fNucleus_Beam_Base;		}	/*!< Return the base beam nucleus (not to be varied in fitting) */
		void	SetBaseTargetNucleus(Nucleus* nucl)				{ fNucleus_Target_Base = *nucl;		}	/*!< Define the base target nucleus (not to be varied in fitting) */
		Nucleus				GetBaseTargetNucleus() 		const	{ return fNucleus_Target_Base;		}	/*!< Return the base target nucleus (not to be varied in fitting) */
	
		void	SetBeamNucleus(Nucleus *nucl)					{ fNucleus_Beam = *nucl;		}	/*!< Define the fitting beam nucleus (varied in fitting) */
		Nucleus				GetBeamNucleus() 		const	{ return fNucleus_Beam;			}	/*!< Return the fitting beam nucleus (varied in fitting) */
		void	SetTargetNucleus(Nucleus *nucl)					{ fNucleus_Target = *nucl;		}	/*!< Define the fitting target nucleus (varied in fitting) */
		Nucleus				GetTargetNucleus() 		const	{ return fNucleus_Target;		}	/*!< Return the fitting target nucleus (varied in fitting) */

		void	SetBeamCorrectionFactors(std::vector<TMatrixD> v)		{ correctionFactors_Beam = v;		}	/*!< Define the vector of correction factors between point and integrated cross sections for the beam */
		std::vector<TMatrixD> GetBeamCorrectionFactors()		const	{ return correctionFactors_Beam;	}	/*!< Return the vector of correciton factors between point and integrated cross sections for the beam */
		void	SetTargetCorrectionFactors(std::vector<TMatrixD> v)		{ correctionFactors_Target = v;		}	/*!< Define the vector of correction factors between point and integrated cross sections for the target */
		std::vector<TMatrixD> GetTargetCorrectionFactors()		const	{ return correctionFactors_Target;	}	/*!< Return the vector of correciton factors between point and integrated cross sections for the target */

		// The following is unchanged from CoulExMinFCN:
		void	SetNpar(int n)							{ nPar = n;				}	/*!< Define the number of fitting parameters (fitting matrix elements + scaling parameters) */
		int	GetNpar()						const	{ return nPar;				}	/*!< Return the number of fitting parameters (fitting matrix elements + scaling parameters) */

		void	SetVerbose(bool b = true)					{ verbose = b;				}	/*!< Define the verbocity of the minimization */	
		bool	GetVerbose() 						const	{ return verbose;			}	/*!< Return the verbocity of the minimization */

		void	SetIter(int i)							{ nIterations = i;			}	/*!< Define the number of iterations (MINUIT2) */
		void 	SetCalls(int i)							{ nCalls = i;				}	/*!< Define the number of function calls (GSL) */

		void	SetNthreads(int n)						{ nThreads = n;				}	/*!< Define the number of cores the function is allowed to use */
		int	GetNthreads()						const	{ return nThreads;			}	/*!< Return the number of cores the function is allowed to use */

		double	GetParameter(int i)					const	{ return parameters.at(i);		}	/*!< Return the fitting parameter indexed by i */
		std::vector<double> GetParameters()				const	{ return parameters;			}	/*!< Return the vector of fitting parameters */

		void	ResetIter()							{ iter = 0;				}	/*!< Reset the iteration number */

		void	SetLikelihoodFit(bool b = true)					{ fLikelihood = b;			}	/*!< Define whether we do a log-likelihood based fit (default: chi-squared) */
		bool	LikelihoodFit()						const	{ return fLikelihood;			}	/*!< Return whether we do a log-likelihood based fit (default: chi-squared) */

		void	SetWeights(std::vector<float> v)				{ expt_weights = v;			}	
		std::vector<float>	GetWeights()				const	{ return expt_weights;			}	

	private :

  std::string workingDir;
		std::vector<double>		parameters;			/*!< Matrix elements for both beam and target, and common scaling factors */
		std::vector<MatrixElement>	ME_Beam;			/*!< Beam matrix elements - Preset to relate parameters to beam matrix elements */
  std::vector<RelativeMatrixElement>	ME_BeamRel;			/*!< Beam matrix elements - Preset to relate parameters to beam matrix elements */
		std::vector<MatrixElement>	ME_Target;			/*!< Target matrix elements - Preset to relate parameters to target matrix elements */
  std::vector<RelativeMatrixElement>	ME_TargetRel;			/*!< Beam matrix elements - Preset to relate parameters to beam matrix elements */
		std::vector<ScalingParameter>	scalingParameters;		/*!< Scaling parameters - common to both target and beam excitations */

		std::vector<TMatrixD>		correctionFactors_Beam;		/*!< Point correction factor for the beam (accounts for the angular distribution of the cross section) */
		std::vector<TMatrixD>		correctionFactors_Target;	/*!< Point correction factor for the target (accounts for the angular distribution of the cross section) */

		std::vector<ExperimentData>	exptData_Beam;			/*!< Beam excitation experimental data (one vector entry for each data subset) */
		std::vector<ExperimentData>	exptData_Target;		/*!< Target excitation experimental data (one vector entry for each data subset) */

		std::vector<LitLifetime>	litLifetimes_Beam;		/*!< Literature data for the beam, lifetimes */
		std::vector<LitLifetime>	litLifetimes_Target;		/*!< Literature data for the target, lifetimes */

		std::vector<LitBranchingRatio>	litBranchingRatios_Beam;	/*!< Literature data for the beam, branching ratios */
		std::vector<LitBranchingRatio>	litBranchingRatios_Target;	/*!< Literature data for the target, branching ratios */

		std::vector<LitMixingRatio>	litMixingRatios_Beam;		/*!< Literature data for the beam, mixing ratios */
		std::vector<LitMixingRatio>	litMixingRatios_Target;		/*!< Literature data for the target, mixing ratios */

		std::vector<LitMatrixElement>	litMatrixElements_Beam;		/*!< Literature data for the beam, matrix elements */
		std::vector<LitMatrixElement>	litMatrixElements_Target;		/*!< Literature data for the target, matrix elements */

		double				theErrorDef;

		std::vector<TMatrixD>		EffectiveCrossSection_Beam;
		std::vector<TMatrixD>		EffectiveCrossSection_Target;

		Nucleus				fNucleus_Beam;
		Nucleus				fNucleus_Target;
		Nucleus				fNucleus_Beam_Base;
		Nucleus				fNucleus_Target_Base;

		int				nPar;

		TransitionRates			fRates;

		bool				verbose;

		int				nCalls;
		int				nIterations;
		int				iter;

		int				nThreads;

		std::vector<int>		exptIndex;

		bool				fLikelihood;

		std::string			beamGOSIAFile_inp;
		std::string			targetGOSIAFile_inp;

		std::string			beamGOSIAFile_out;
		std::string			targetGOSIAFile_out;

		std::string			beamBSTFile;
		std::string			targetBSTFile;

		std::vector<int>		beamMapping_i;
		std::vector<int>		beamMapping_f;
		std::vector<int>		beamMapping_l;

		std::vector<int>		targetMapping_i;
		std::vector<int>		targetMapping_f;
		std::vector<int>		targetMapping_l;

		std::vector<float>		expt_weights;

};

#endif
