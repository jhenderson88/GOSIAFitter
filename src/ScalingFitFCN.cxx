#include "ScalingFitFCN.h"


double ScalingFitFCN::operator()(const double* par){

	double 	chisq = 0;

	double	scaling	= par[0];

	for(size_t i=0;i<exptData.size();i++){
		chisq	+= pow((scaling * calcData.at(i) - exptData.at(i))/exptUnc.at(i),2);
	}

	return	chisq;

}

double ScalingFitFCNMulti::operator()(const double *par){

	double	chisq 		= 0;

	double	scaling 	= par[0];
	double	fraction	= par[1];

	for(size_t i=0;i<exptData_Beam1.size();i++)
		chisq	+= pow((scaling * calcData_Beam1.at(i) * fraction  - exptData_Beam1.at(i))/exptUnc_Beam1.at(i),2);
	for(size_t i=0;i<exptData_Beam2.size();i++)
		chisq	+= pow((scaling * calcData_Beam2.at(i) * (1 - fraction)  - exptData_Beam2.at(i))/exptUnc_Beam2.at(i),2);
	for(size_t i=0;i<exptData_Target.size();i++){
		double	targetCalc	= scaling * (calcData_Target1.at(i) * fraction + calcData_Target2.at(i) * (1 - fraction));
		chisq	+= pow((targetCalc  - exptData_Target.at(i))/exptUnc_Target.at(i),2);
	}
		

	return	chisq;

}
