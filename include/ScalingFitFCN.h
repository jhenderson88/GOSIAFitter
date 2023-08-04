#ifndef ScalingFitFCN_h
#define ScalingFitFCN_h

#include <ctime>
#include <cmath>
#include <vector>

class ScalingFitFCN{ // : public ROOT::Minuit2::FCNBase{

	public:

		ScalingFitFCN()								{	ClearAll();			}
		~ScalingFitFCN()							{	;				}

		double 	up() const	 						{ return theErrorDef;			}	/*!< Required by ROOT::Minimizer */
		double 	operator()(const double*);											/*!< Required by ROOT::Minimizer */
		void	SetErrorDef(double def)						{ theErrorDef = def;			}	/*!< Required by ROOT::Minimizer */	

		void	ClearAll()
		{
			exptData.clear();
			exptUnc.clear();
			calcData.clear();
		}

		void	AddTransition(double e, double e_u, double c)
		{
			exptData.push_back(e);
			exptUnc.push_back(e_u);
			calcData.push_back(c);
		}

		void	SetData(std::vector<double> e, std::vector<double> e_u, std::vector<double> c)
		{
			exptData 	= e;
			exptUnc		= e_u;
			calcData	= c;
		}

	private:

		std::vector<double>		exptData;
		std::vector<double>		exptUnc;
		std::vector<double>		calcData;

		double				theErrorDef;

};

//	Example functionality for combining cocktail beams

class ScalingFitFCNMulti{ // : public ROOT::Minuit2::FCNBase{

	public:

		ScalingFitFCNMulti()							{	ClearAll();			}
		~ScalingFitFCNMulti()							{	;				}

		double 	up() const	 						{ return theErrorDef;			}	/*!< Required by ROOT::Minimizer */
		double 	operator()(const double*);											/*!< Required by ROOT::Minimizer */
		void	SetErrorDef(double def)						{ theErrorDef = def;			}	/*!< Required by ROOT::Minimizer */	

		void	ClearAll()
		{
			exptData_Target.clear();
			exptUnc_Target.clear();
			exptData_Beam1.clear();
			exptUnc_Beam1.clear();
			exptData_Beam2.clear();
			exptUnc_Beam2.clear();
			calcData_Beam1.clear();
			calcData_Beam2.clear();
			calcData_Target1.clear();
			calcData_Target2.clear();
		}

		void	SetBeam1Data(std::vector<double> e, std::vector<double> e_u, std::vector<double> c)
		{
			exptData_Beam1 	= e;
			exptUnc_Beam1	= e_u;
			calcData_Beam1	= c;
		}
		void	SetBeam2Data(std::vector<double> e, std::vector<double> e_u, std::vector<double> c)
		{
			exptData_Beam2 	= e;
			exptUnc_Beam2	= e_u;
			calcData_Beam2	= c;
		}
		void	SetTargetData(std::vector<double> e, std::vector<double> e_u, std::vector<double> c1, std::vector<double> c2)
		{
			exptData_Target		= e;
			exptUnc_Target		= e_u;
			calcData_Target1	= c1;
			calcData_Target2	= c2;
		}

	private:

		std::vector<double>		exptData_Target;
		std::vector<double>		exptUnc_Target;
		std::vector<double>		exptData_Beam1;
		std::vector<double>		exptUnc_Beam1;
		std::vector<double>		exptData_Beam2;
		std::vector<double>		exptUnc_Beam2;
		std::vector<double>		calcData_Beam1;
		std::vector<double>		calcData_Beam2;
		std::vector<double>		calcData_Target1;
		std::vector<double>		calcData_Target2;

		double				theErrorDef;

};

#endif
