#ifndef ScalingParameter_h
#define ScalingParameter_h

#include <vector>
#include <iostream>

///
///	\class ScalingParameter
///
///	\brief Define common scaling parameters for calculation to experimental
///	data
///
/// 	Due to a typical lack of absolute scaling information Coulomb excitation 
///	fits require some degree of scaling to reproduce the data. This class 
///	defines these scaling parameters in a sensible way.
///
/// 	A single scaling parameter can be coupled to one or more experiments, 
///	identified in the experimentNumber vector, containing the indices of the 
///	relevant experiments.
///
/// 	Upper and lower limits can also be defined for the purposes of aiding 
///	the fit.	
///

class ScalingParameter{

	public:
		ScalingParameter(const ScalingParameter& g);				/*!< Copy constructor */
		ScalingParameter& operator = (const ScalingParameter& g);		/*!< Assignment operator */

		ScalingParameter() 		{ 
							experimentNumber.clear();
							experimentNRM.clear();
						}
		void	AddExperiment(int i)			{ experimentNumber.push_back(i);	}	/*!< Add experiment of index, i, to this common scaling */
		void	ClearExperiments()			{ experimentNumber.clear();		}	/*!< Delete all experiments coupled to this scaling */
		void	SetExperimentVector(std::vector<int> s, std::vector<float> n)	
								{ 
									experimentNumber 	= s;			
									experimentNRM		= n;	
								}	/*!< Define the vector experimental indices coupled to this common scaling */

		std::vector<int> GetExperimentNumbers()	const	{ return experimentNumber;		}	/*!< Return the vector of experimental indices coupled to this common scaling */
		int	GetExperimentNumber(int i)	const	{
									if( (unsigned)i < experimentNumber.size() )
										return experimentNumber.at(i);
									else{
										std::cout << "Experiment number out of range" << std::endl;
										return -1;
									}
								}	/*!< Get the experiment number for index i */

		std::vector<float> GetExperimentNRMs()	const	{ return experimentNRM;			}
		float	GetExperimentNRM(int i)		const	{
									if( (unsigned)i < experimentNRM.size() )
										return experimentNRM.at(i);
									else{
										std::cout << "Experiment number out of range" << std::endl;
										return -1;
									}
								}


	private:
		std::vector<int>		experimentNumber;	/*! < Vector of experiment numbers to be commonly normalized */
		std::vector<float>		experimentNRM;		/*! < Relative normalization of each experiment */

};
#endif
