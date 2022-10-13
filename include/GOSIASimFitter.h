#ifndef GOSIASimFitter_h
#define GOSIASimFitter_h


#include "TFile.h"

#include "GOSIASimMinFCN.h"
#include "ScalingParameter.h"
#include <chrono>

//#include "Minuit2/Minuit2Minimizer.h"
#include "Math/Functor.h"
#include "Math/Factory.h"
#include "Math/Minimizer.h"
#include "Math/MinimizerOptions.h"
//#include "Minuit2/FunctionMinimum.h"
//#include "Minuit2/MnUserParameterState.h"
//#include "Minuit2/MnMachinePrecision.h"
//#include "Minuit2/MnMigrad.h" 
//#include "Minuit2/MnMinos.h"
//#include "Minuit2/MnContours.h"
//#include "Minuit2/MnPlot.h"

#include <iostream>

class GOSIASimMinFCN;

///
///	\class CoulExSimFitter
///
///	\brief Calls the CoulExSimMinFCN class along with the ROOT::Minimizer
///	to perform a chi-squared minimization for simultaneous beam and 
///	target analyses
///
///	Very similar to the CoulExFitter class, but with some duplicated 
///	objects and functions to allow for the simultaneous fitting of two
///	data sets.
///
///	A requirement of the ROOT::Minimizer package is a single vector/array
///	of parameters. As such, all matrix elements (beam and target) need to
///	be entered as a single set of ordered values. The convention used is
///	to enter in the order of: beam, target, scaling. In principle, 
///	switching the target/beam order has no impact on the calculations, it
///	is just the convention used here (scaling parameters *must* be last,
///	however). 
///
///	A number of methods and algorithms can be used (as defined in the
///	ROOT::Minimizer documentation in more detail):
///
///	Method		| Algorithm
///	-------------------------------------
///	Minuit2		| Migrad
///			| Simplex
///			| Combined (default)
///			| Scan
///			| Fumili2
///	GSLMultiMin	| ConjugateFR
///			| ConjugatePR
///			| BFGS
///			| BFGS2
///			| SteepestDescent
///	GSLMultiFit	|
///	GSLSimAn	|
///	Genetic		|
///

class GOSIASimFitter {

	public:
		GOSIASimFitter();
		virtual	~GOSIASimFitter()	{;}
		GOSIASimFitter(const GOSIASimFitter& g);			/*!< Copy constructor */
		GOSIASimFitter& operator = (const GOSIASimFitter& g);		/*!< Assignment operator */

    void UpdateMEs(); /* set the matrix elements in the nucleus objects to reflect those in the parameters */
    void WriteBST();
  void SetWorkingDir(std::string s) { workingDir = s; }
		void	DoFit(const char* method = "Minuit2", const char* algorithm = "Combined" );	/*!< Perform fitting routine with a user defined method and algorithm (default: Minuit2, Combined) */

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
		
		virtual	void	ClearAll();			/*!< Completely clear all previous input */
		
		void	DefineExperiment(double t);					/*!< Defines a new experiment with t = theta_cm */
		void	AddBeamData(int i, int init, int fin, double c, double e);	/*!< Add experimental data to experiment defined by index i between initial (init) and final (fin) states with yield (c) and uncertainty (e)  */
		void	AddTargetData(int i, int init, int fin, double c, double e);	/*!< Add experimental data to experiment defined by index i between initial (init) and final (fin) states with yield (c) and uncertainty (e)  */
		
		void	AddBeamLifetime(int,double,double);			/*!< Add literature lifetime data for the beam particle */
    void	AddBeamHalfLife(int,double,double);			/*!< Add literature lifetime data for the beam particle */
		void	AddBeamBranchingRatio(int,int,int,double,double);	/*!< Add literature branching ratio data for the beam particle*/
		void	AddBeamMixingRatio(int,int,double,double);		/*!< Add literature mixing ratio data for the beam particle*/
		void	AddBeamMatrixElement(int,int,int,double,double);		/*!< Add literature matrix element data for the beam particle*/
		void	AddTargetLifetime(int,double,double);			/*!< Add literature lifetime data for the target particle*/
    void	AddTargetHalfLife(int,double,double);			/*!< Add literature lifetime data for the beam particle */
		void	AddTargetBranchingRatio(int,int,int,double,double);	/*!< Add literature branching ratio data for the target particle*/
		void	AddTargetMixingRatio(int,int,double,double);		/*!< Add literature mixing ratio data for the target particle*/
		void	AddTargetMatrixElement(int,int,int,double,double);		/*!< Add literature matrix element data for the target particle*/

		void	AddBeamFittingMatrixElement(int,int,int,double,double,double);		/*!< Add a fitting matrix element for the beam */
    void  AddBeamRelativeMatrixElement(int,int,int,int,int,int,double,double,double,bool=false,bool=false);		/*!< Add a fitting matrix element for the beam */
  void SetBeamFittingMatrixElement(int lambda, int init, int fin, double ME, double LL, double UL);
		void	AddTargetFittingMatrixElement(int,int,int,double,double,double);	/*!< Add a fitting matrix element for the target */
    void  AddTargetRelativeMatrixElement(int,int,int,int,int,int,double,double,double,bool=false,bool=false);		/*!< Add a fitting matrix element for the beam */
		void	CreateScalingParameter(std::vector<int>);				/*!< Add a scaling parameter, with common scaling experiments defined by their indices in a vector of int */

		// Scaling parameters are common to both target and beam
		std::vector<ScalingParameter>	GetScalingParameters()		const	{ return scalingParameters;		}	/*!< Return vector of ScalingParameter objects */
		void	SetScalingParameters(std::vector<ScalingParameter> s)		{ scalingParameters = s;		}	/*!< Define vector of ScalingParameter objects */
		void	AddScalingParameter(ScalingParameter s)				{ scalingParameters.push_back(s);	}	/*!< Append ScalingParameter object to vector */
		void	ClearScalingParameters()					{ scalingParameters.clear();		}	/*!< Clear scaling parameters */

		void	SetBeamMatrixElements(std::vector<MatrixElement> m)	   	{ matrixElements_Beam = m;		}	/*!< Define vector of fitting MatrixElements for the beam */
		std::vector<MatrixElement>	GetBeamMatrixElements() 	const	{ return matrixElements_Beam;		}	/*!< Return vector of fitting MatrixElements for the beam */
		void	ClearBeamMatrixElements()					{ matrixElements_Beam.clear();		}	/*!< Clear vector of fitting MatrixElements for the beam */
		void	AddBeamMatrixElement(MatrixElement m)				{ matrixElements_Beam.push_back(m);	}	/*!< Add a matrix element for the beam nucleus */
		void	SetTargetMatrixElements(std::vector<MatrixElement> m)	   	{ matrixElements_Target = m;		}	/*!< Define vector of fitting MatrixElements for the target */
		std::vector<MatrixElement>	GetTargetMatrixElements() 	const	{ return matrixElements_Target;		}	/*!< Return vector of fitting MatrixElements for the target */
		void	ClearTargetMatrixElements()					{ matrixElements_Target.clear();	}	/*!< Clear vector of fitting MatrixElements for the target*/
		void	AddTargetMatrixElement(MatrixElement m)				{ matrixElements_Target.push_back(m);	}	/*!< Add a matrix element for the target nucleus */

		void	SetBeamData(std::vector<ExperimentData> d)		   	{ exptData_Beam = d;			}	/*!< Define vector of beam ExperimentData */
		std::vector<ExperimentData>	GetBeamData() 			const	{ return exptData_Beam;			}	/*!< Return vector of beam ExperimentData */
		void	SetTargetData(std::vector<ExperimentData> d)		   	{ exptData_Target = d;			}	/*!< Define vector of target ExperimentData */
		std::vector<ExperimentData>	GetTargetData() 		const	{ return exptData_Target;		}	/*!< Return vector of target ExperimentData */

		void	SetBeamLitLifetimes(std::vector<LitLifetime> l)			{ litLifetimes_Beam = l;		}	/*!< Define the vector of beam LitLifetime objects defining the literature lifetime data for fitting */
		std::vector<LitLifetime>	GetBeamLitLifetimes() 		const	{ return litLifetimes_Beam;		}	/*!< Return the vector of beam LitLifetime objects defining the literature lifetime data for fitting */
		void	SetTargetLitLifetimes(std::vector<LitLifetime> l)		{ litLifetimes_Target = l;		}	/*!< Define the vector of target LitLifetime objects defining the literature lifetime data for fitting */
		std::vector<LitLifetime>	GetTargetLitLifetimes() 	const	{ return litLifetimes_Target;		}	/*!< Return the vector of target LitLifetime objects defining the literature lifetime data for fitting */

		void	SetBeamLitBranching(std::vector<LitBranchingRatio> b) 		{ litBranchingRatios_Beam = b;		}	/*!< Define the vector of beam LitBranchingRatio objects defining the literature branching ratio data for fitting */
		std::vector<LitBranchingRatio>	GetBeamLitBranching() 		const	{ return litBranchingRatios_Beam;	}	/*!< Return the vector of beam LitBranchingRatio objects defining the literature branching ratio data for fitting */
		void	SetTargetLitBranching(std::vector<LitBranchingRatio> b) 	{ litBranchingRatios_Target = b;	}	/*!< Define the vector of target LitBranchingRatio objects defining the literature branching ratio data for fitting */
		std::vector<LitBranchingRatio>	GetTargetLitBranching() 	const	{ return litBranchingRatios_Target;	}	/*!< Return the vector of target LitBranchingRatio objects defining the literature branching ratio data for fitting */
	
		void	SetBeamLitMixing(std::vector<LitMixingRatio> m)				{ litMixingRatios_Beam = m;		}	/*!< Define the vector of beam LitMixingRatio objects defining the literature mixing ratio data for fitting */
		std::vector<LitMixingRatio>	GetBeamLitMixing() 			const	{ return litMixingRatios_Beam;		}	/*!< Return the vector of beam LitMixingRatio objects defining the literature mixing ratio data for fitting */
		void	SetTargetLitMixing(std::vector<LitMixingRatio> m)			{ litMixingRatios_Target = m;		}	/*!< Define the vector of target LitMixingRatio objects defining the literature mixing ratio data for fitting */
		std::vector<LitMixingRatio>	GetTargetLitMixing() 			const	{ return litMixingRatios_Target;	}	/*!< Return the vector of target LitMixingRatio objects defining the literature mixing ratio data for fitting */
	
		void	SetBeamLitMatrixElement(std::vector<LitMatrixElement> m)		{ litMatrixElements_Beam = m;		}	/*!< Define the vector of beam LitMatrixElement objects defining the literature mixing ratio data for fitting */
		std::vector<LitMatrixElement>	GetBeamLitMatrixElement() 		const	{ return litMatrixElements_Beam;		}	/*!< Return the vector of beam LitMatrixElement objects defining the literature mixing ratio data for fitting */
		void	SetTargetLitMatrixElement(std::vector<LitMatrixElement> m)		{ litMatrixElements_Target = m;		}	/*!< Define the vector of target LitMatrixElement objects defining the literature mixing ratio data for fitting */
		std::vector<LitMatrixElement>	GetTargetLitMatrixElement() 		const	{ return litMatrixElements_Target;	}	/*!< Return the vector of target LitMatrixElement objects defining the literature mixing ratio data for fitting */

		std::vector<TMatrixD>	GetEffectiveCrossSection_Beam()		const	{ return EffectiveCrossSection_Beam;	}	/*!< Return the beam's "effective cross section" = direct population + feeding */
		std::vector<TMatrixD>	GetEffectiveCrossSection_Target() 	const	{ return EffectiveCrossSection_Target;	}	/*!< Return the target's "effective cross section" = direct population + feeding */

		void	ClearEffectiveCrossSection_Beam()				{ EffectiveCrossSection_Beam.clear();		}
		void	ClearEffectiveCrossSection_Target()				{ EffectiveCrossSection_Target.clear();		}

		void	AddEffectiveCrossSection_Beam(TMatrixD m)			{ EffectiveCrossSection_Beam.push_back(m);	}
		void	AddEffectiveCrossSection_Target(TMatrixD m)			{ EffectiveCrossSection_Target.push_back(m);	}

		void	SetBaseBeamNucleus(Nucleus* nucl)				{ fNucleus_Target_Base = *nucl;		}	/*!< Define the base beam nucleus (not to be varied in fitting) */
		Nucleus				GetBaseBeamNucleus() 		const	{ return fNucleus_Beam_Base;		}	/*!< Return the base beam nucleus (not to be varied in fitting) */
		void	SetBaseTargetNucleus(Nucleus* nucl)				{ fNucleus_Target_Base = *nucl;		}	/*!< Define the base target nucleus (not to be varied in fitting) */
		Nucleus				GetBaseTargetNucleus() 		const	{ return fNucleus_Target_Base;		}	/*!< Return the base target nucleus (not to be varied in fitting) */
	
		void	SetBeamNucleus(Nucleus *nucl)					{ fNucleus_Beam = *nucl;		}	/*!< Define the fitting beam nucleus (varied in fitting) */
		Nucleus				GetBeamNucleus() 		const	{ return fNucleus_Beam;			}	/*!< Return the fitting beam nucleus (varied in fitting) */
		void	SetTargetNucleus(Nucleus *nucl)					{ fNucleus_Target = *nucl;		}	/*!< Define the fitting target nucleus (varied in fitting) */
		Nucleus				GetTargetNucleus() 		const	{ return fNucleus_Target;		}	/*!< Return the fitting target nucleus (varied in fitting) */

		void	AddBeamCorrectionFactor(TMatrixD);										/*!< Add beam point calculation correction factors (append) */
		void	SetBeamCorrectionFactor(int i, TMatrixD);									/*!< Define beam point calculation correction factors for experiment i */
		std::vector<TMatrixD> GetBeamCorrectionFactors()		const	{ return correctionFactors_Beam;	}	/*!< Return beam point calculation correction factors */
		void	AddTargetCorrectionFactor(TMatrixD);										/*!< Add beam point calculation correction factors (append) */
		void	SetTargetCorrectionFactor(int i, TMatrixD);									/*!< Define beam point calculation correction factors for experiment i */
		std::vector<TMatrixD> GetTargetCorrectionFactors()		const	{ return correctionFactors_Target;	}	/*!< Return beam point calculation correction factors */

		void	Print() const;	/*!< Print fitting details (formatted) */

		void	SetMaxIterations(int i)						{ maxIter = i;				}	/*!< Define number of iterations (Minuit) */
		void	SetMaxFunctionCalls(int i)					{ maxCalls = i;				}	/*!< Define number of calls (GSL) */
		void	SetTolerance(double d)						{ fitTolerance = d;			}	/*!< Define required tolerance */

		int	GetMaxIterations()					const	{ return maxIter;			}	/*!< Return number of iterations (Minuit) */
		int	GetMaxFunctionCalls()					const	{ return maxCalls;			}	/*!< Return number of calls (GSL) */
		double	GetTolerance()						const	{ return fitTolerance;			}	/*!< Return required tolerance */

		void	SetNthreads(int n)						{ nThreads = n;				}	/*!< Define number of allowed cores */
		int	GetNthreads()						const 	{ return nThreads;			}	/*!< Return number of allowed cores */
	
		void	SetVerbose(bool b = true)					{ verbose = b;				}	/*!< Define verbocity */
		bool	GetVerbose()						const	{ return verbose;			}	/*!< Return verbocity */

		TMatrixD	GetCovarianceMatrix()				const	{ return covMat;			}	/*!< Return covariance matrix from fit 	*/
		TMatrixD	GetCorrelationMatrix()				const	{ return corMat;			}	/*!< Return correlation matrix from fit 	*/

		void	SetDoFullUncertainty(bool b = true)				{ fDoFullUnc = b;			} 	/*!< Define whether to do a complete MINOS uncertainty analysis (slow)	*/
		bool	DoFullUncertainty()					const	{ return fDoFullUnc;			} 	/*!< Return whether to do a complete MINOS uncertainty analysis (slow)	*/

		void	SetLikelihoodFit(bool b = true)					{ fLikelihood = b;			}	/*!< Define whether we do a log-likelihood based fit (default: chi-squared) */
		bool	LikelihoodFit()						const	{ return fLikelihood;			}	/*!< Return whether we do a log-likelihood based fit (default: chi-squared) */

		double			GetFitChisq()				const	{ return chisq;				}
		std::vector<double>	GetFitParameters()			const	{ return parameters;			}	/*!< Return fit parameters - note that these will be updated with the fit result after the fit - Used in MCMC methods */
		std::vector<double>	GetFitUL()				const	{ return par_UL;			}	/*!< Return the fit parameter upper limits - Used in MCMC methods */
		std::vector<double>	GetFitLL()				const	{ return par_LL;			}	/*!< Return the fit parameter lower limits - Used in MCMC methods */

    void FixAllBeamMatrixElements();
    void UnFixAllBeamMatrixElements();
    void FixBeamMatrixElements(std::vector<std::vector<int> >);
    void UnFixBeamMatrixElements(std::vector<std::vector<int> >);
	
		void	ClearFitParameters()						
		{ 
			parameters.clear();			
			par_UL.clear();
			par_LL.clear();
		}			
		void	SetFitParameters(std::vector<double> p)				{ parameters = p;			}	/*!< Set the fit parameters - note that these will be updated after the fit has been performed - Used in MCMC methods */
		void	SetFitUL(std::vector<double> p)					{ par_UL = p;				}	/*!< Set the fit parameter upper limits - Used in MCMC methods */
		void	SetFitLL(std::vector<double> p)					{ par_LL = p;				}	/*!< Set the fit parameter lower limits - Used in MCMC methods */

		void	AddFitParameter(double p, double ll, double ul)			
		{ 
			parameters.push_back(p);		
			par_LL.push_back(ll);
			par_UL.push_back(ul);
		}

		void	SetFittingParameter(size_t i, double v)				{ parameters.at(i) = v;			}	/*!< Set an individual fitting parameter - Used in MCMC methods */

		void	SetWeightingFactors(std::vector<float> v)			{ expt_weights = v;			}
		void	AddWeightingFactor(float f);
		void	SetWeightingFactor(int i, float f);
		std::vector<float>	GetWeightingFactor()			const	{ return expt_weights;			}

		std::vector<int>	GetBeamMappingInit()			const	{ return beamMapping_i;			}
		std::vector<int>	GetBeamMappingFinal()			const	{ return beamMapping_f;			}
		std::vector<int>	GetBeamMappingLambda()			const	{ return beamMapping_l;			}
                
		std::vector<int>	GetTargetMappingInit()			const	{ return targetMapping_i;		}
		std::vector<int>	GetTargetMappingFinal()			const	{ return targetMapping_f;		}
		std::vector<int>	GetTargetMappingLambda()		const	{ return targetMapping_l;		}

    void WriteYieldGraphs(TFile *file, std::vector<double> angles, std::vector<double> norms);
	private:

  std::string workingDir;
		std::vector<int>		index;

		double				chisq;				/*!< Chisq following fit */
		std::vector<double>		parameters;			/*!< Matrix elements + scaling factors */
		std::vector<double>		par_LL;				/*!< Matrix elements + scaling factors - LOWER LIMIT */
		std::vector<double>		par_UL;				/*!< Matrix elements + scaling factors - UPPER LIMIT */
		std::vector<MatrixElement>	matrixElements_Beam;		/*!< Preset to related parameters to matrix elements (beam) */
    std::vector<RelativeMatrixElement>	relativeElements_Beam;		/*!< Preset to related parameters to matrix elements (beam) */
		std::vector<MatrixElement>	matrixElements_Target;		/*!< Preset to related parameters to matrix elements (target) */
    std::vector<RelativeMatrixElement>	relativeElements_Target;		/*!< Preset to related parameters to matrix elements (target) */
		std::vector<ScalingParameter>	scalingParameters;		/*!< Common scaling parameters */

		std::vector<TMatrixD>		correctionFactors_Beam;		/*!< Point corrections for the beam nucleus excitation */
		std::vector<TMatrixD>		correctionFactors_Target;	/*!< Point corrections for the target nucleus excitation*/

		std::vector<ExperimentData>	exptData_Beam;			/*!< Beam excitation experimental data (one vector entry for each data subset) */
		std::vector<ExperimentData>	exptData_Target;		/*!< Target excitation experimental data (one vector entry for each data subset) */

		std::vector<LitLifetime>	litLifetimes_Beam;		/*!< Literature data for the beam, lifetimes */
		std::vector<LitLifetime>	litLifetimes_Target;		/*!< Literature data for the target, lifetimes */

		std::vector<LitBranchingRatio>	litBranchingRatios_Beam;	/*!< Literature data for the beam, branching ratios */
		std::vector<LitBranchingRatio>	litBranchingRatios_Target;	/*!< Literature data for the target, branching ratios */

		std::vector<LitMixingRatio>	litMixingRatios_Beam;		/*!< Literature data for the beam, mixing ratios */
		std::vector<LitMixingRatio>	litMixingRatios_Target;		/*!< Literature data for the target, mixing ratios */

		std::vector<LitMatrixElement>	litMatrixElements_Beam;		/*!< Literature data for the beam, matrix elements */
		std::vector<LitMatrixElement>	litMatrixElements_Target;	/*!< Literature data for the target, matrix elements */

		double				theErrorDef;

		std::vector<TMatrixD>		EffectiveCrossSection_Beam;
		std::vector<TMatrixD>		EffectiveCrossSection_Target;

		Nucleus				fNucleus_Beam;
		Nucleus				fNucleus_Target;
		Nucleus				fNucleus_Beam_Base;
		Nucleus				fNucleus_Target_Base;

		int				maxIter;
		int				maxCalls;
		double				fitTolerance;

		int				nThreads;

		bool				first;
		bool				verbose;

		TMatrixD			covMat;
		TMatrixD			corMat;

		bool				fDoFullUnc;

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
