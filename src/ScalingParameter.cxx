#include "ScalingParameter.h"

ScalingParameter::ScalingParameter(const ScalingParameter& g) {

	experimentNumber	= g.GetExperimentNumbers();
	experimentNRM		= g.GetExperimentNRMs();

}
ScalingParameter& ScalingParameter::operator = (const ScalingParameter& g){

	experimentNumber	= g.GetExperimentNumbers();
	experimentNRM		= g.GetExperimentNRMs();

	return *this;

}
