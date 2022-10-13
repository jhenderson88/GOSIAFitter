#include "GOSIASimMinFCN.h"
#include "Gosia.h"

void GOSIASimMinFCN::SetupCalculation(){

	exptIndex.resize(exptData_Beam.size());
	for(unsigned int i=0;i<scalingParameters.size();i++)
		for(unsigned int s=0;s<scalingParameters.at(i).GetExperimentNumbers().size();s++)
			exptIndex[scalingParameters.at(i).GetExperimentNumbers().at(s)] = (int)i;

  if (verbose) {
	std::cout	<< std::setw(13) << std::left << "Experiment: "
			<< std::setw(14) << std::left << "Scaling index: "
			<< std::endl;
	for(unsigned int i=0;i<exptIndex.size();i++){
		std::cout	<< std::setw(13) << std::left << i+1
				<< std::setw(14) << std::left << exptIndex.at(i)
				<< std::endl;
	}
  }

}

double GOSIASimMinFCN::operator()(const double* par){
	std::cout 	<< std::setprecision(6);
	
	double chisq = 0;
	int 	NDF = 0;

	int	NDF_lit = 0;
	int	NDF_beam = 0;
	int 	NDF_targ = 0;

	typedef std::chrono::high_resolution_clock Clock;
	typedef std::chrono::milliseconds milliseconds;
	Clock::time_point t0 = Clock::now();


	Nucleus &nucl_b = fNucleus_Beam;	
	Nucleus &nucl_t = fNucleus_Target;

    
	parameters.clear();
  int parct = 0;
  int nRelBeam = 0;
  int nRelTarget = 0;
  int nBeamME = 0;
  int nTargetME = 0;
	for(unsigned int i=0;i<ME_Beam.size();i++){
    if (!ME_Beam.at(i).GetFixed()) {
      nucl_b.SetMatrixElement(ME_Beam.at(i).GetLambda(),ME_Beam.at(i).GetInitialState(),ME_Beam.at(i).GetFinalState(),par[parct]);
      parameters.push_back(par[parct]);
      ++parct;
      ++nBeamME;
    }
    else {
      nucl_b.SetMatrixElement(ME_Beam.at(i).GetLambda(),ME_Beam.at(i).GetInitialState(),ME_Beam.at(i).GetFinalState(),ME_Beam.at(i).GetMatrixElement());
    }      
	}
  //relative matrix elements
  for(unsigned int i=0;i<ME_BeamRel.size();i++){
    double me = nucl_b.GetMatrixElements().at(ME_BeamRel.at(i).GetLambdaRel())[ME_BeamRel.at(i).GetInitialStateRel()][ME_BeamRel.at(i).GetFinalStateRel()];
    if (ME_BeamRel.at(i).GetFixed()) {
      me = me * ME_BeamRel.at(i).GetRelativeElement();
    }
    else {
      me = me * par[parct];
      parameters.push_back(par[parct]);
      ++parct;
      ++nRelBeam;
    }
		nucl_b.SetMatrixElement(ME_BeamRel.at(i).GetLambda(),ME_BeamRel.at(i).GetInitialState(),ME_BeamRel.at(i).GetFinalState(),me);
	}
  
	for(unsigned int i=0;i<ME_Target.size();i++){
    if (!ME_Target.at(i).GetFixed()) {
      nucl_t.SetMatrixElement(ME_Target.at(i).GetLambda(),ME_Target.at(i).GetInitialState(),ME_Target.at(i).GetFinalState(),par[parct]);
      parameters.push_back(par[parct]);     
      ++parct;
      ++nTargetME;
    }
    else {
      nucl_t.SetMatrixElement(ME_Target.at(i).GetLambda(),ME_Target.at(i).GetInitialState(),ME_Target.at(i).GetFinalState(),ME_Target.at(i).GetMatrixElement());
    }
	}

  //relative matrix elements
  for(unsigned int i=0;i<ME_TargetRel.size();i++){
    double me = nucl_t.GetMatrixElements().at(ME_TargetRel.at(i).GetLambdaRel())[ME_TargetRel.at(i).GetInitialStateRel()][ME_TargetRel.at(i).GetFinalStateRel()];
    if (ME_TargetRel.at(i).GetFixed()) {
      me = me * ME_TargetRel.at(i).GetRelativeElement();
    }
    else {
      me = me * par[parct];
      parameters.push_back(par[parct]);
      ++parct;
      ++nRelTarget;
    }
		nucl_t.SetMatrixElement(ME_TargetRel.at(i).GetLambda(),ME_TargetRel.at(i).GetInitialState(),ME_TargetRel.at(i).GetFinalState(),me);
	}

	if(verbose){
		std::cout	<< std::endl;
		std::cout	<< parct << " Parameters:";
    int linect = 0;
    for (unsigned int i=0; i<parct; ++i) {
      if (linect%12 == 0) {
        std::cout	<< std::endl;
        std::cout << std::setw(20) << std::left;      
        if (i == 0) {
          std::cout << "Beam: ";
          linect = 0;
        }
        else {
          std::cout << " ";
        }
      }
      if (i == nBeamME) {
        std::cout << std::endl;
        std::cout << std::setw(20) << std::left << "Relative Beam: ";
        linect = 0;
      }
      else if (i == nBeamME + nRelBeam) {
        std::cout << std::endl;
        std::cout << std::setw(20) << std::left << "Target: ";
        linect = 0;
      }
      else if (i == nBeamME + nRelBeam + nRelTarget) {
        std::cout << std::endl;
        std::cout << std::setw(20) << std::left << "Relative Target: ";
        linect = 0;
      }
      std::cout	<< std::setw(12) << std::left << par[i];
      ++linect;
    }
    std::cout	<< std::endl;
	}

	// 	COMPARE WITH LITERATURE CONSTRAINTS:
	// 	First, compare with the literature for the beam:
	TransitionRates rates_b(&nucl_b);

	//rates_b.Print();

	double lifetime_chisq = 0;	
	for(unsigned int i=0;i<litLifetimes_Beam.size();i++){
		double 	tmp = 0;
		int	index		= litLifetimes_Beam.at(i).GetIndex();
		double	lifetime	= litLifetimes_Beam.at(i).GetLifetime();
		double	calcLifetime	= rates_b.GetLifetimes()[index];
		if(fLikelihood){
			double	sigma		= litLifetimes_Beam.at(i).GetUpUnc() * litLifetimes_Beam.at(i).GetDnUnc();
			double	sigma_prime	= (litLifetimes_Beam.at(i).GetUpUnc() - litLifetimes_Beam.at(i).GetDnUnc());
			chisq 			+= 0.5 * TMath::Power((calcLifetime - lifetime),2)/(sigma + sigma_prime * (calcLifetime - lifetime));
		}
		else{
			if(calcLifetime > lifetime)
				tmp = (calcLifetime - lifetime) / litLifetimes_Beam.at(i).GetUpUnc();
			else
				tmp = (calcLifetime - lifetime) / litLifetimes_Beam.at(i).GetDnUnc();
			chisq += tmp * tmp;
			lifetime_chisq += tmp*tmp;
      if (verbose) {
      std::cout	<< std::setw(14) << std::left << "Lifetime:"
                << std::setw(10) << std::left << index 
                << std::setw(10) << std::left << calcLifetime
                << std::setw(10) << std::left << lifetime
                << std::setw(10) << std::left << litLifetimes_Beam.at(i).GetUpUnc()
                << std::setw(10) << std::left << tmp*tmp 
                << std::endl;
      }
		}
		NDF++;
		NDF_lit++;
	}
	double br_chisq = 0;
	if(litBranchingRatios_Beam.size()>0 && verbose)    
		std::cout	<< "BR (Beam):" 
				<< std::endl;
	for(unsigned int i=0;i<litBranchingRatios_Beam.size();i++){
		double 	tmp 		= 0;
		int	index_init 	= litBranchingRatios_Beam.at(i).GetInitialIndex();
		int	index_final1	= litBranchingRatios_Beam.at(i).GetFinalIndex_1();
		int	index_final2	= litBranchingRatios_Beam.at(i).GetFinalIndex_2();
		double	BR		= litBranchingRatios_Beam.at(i).GetBranchingRatio();
		double  calcBR		= rates_b.GetBranchingRatios()[index_final1][index_init] / rates_b.GetBranchingRatios()[index_final2][index_init];
		if(fLikelihood){
			double	sigma		= litBranchingRatios_Beam.at(i).GetUpUnc() * litBranchingRatios_Beam.at(i).GetDnUnc();
			double	sigma_prime	= (litBranchingRatios_Beam.at(i).GetUpUnc() - litBranchingRatios_Beam.at(i).GetDnUnc());
			chisq 			+= 0.5 * TMath::Power((calcBR - BR),2)/(sigma + sigma_prime * (calcBR - BR));
		}
		else{
			if(calcBR > BR)
				tmp = (BR - calcBR) / litBranchingRatios_Beam.at(i).GetUpUnc();
			else
				tmp = (BR - calcBR) / litBranchingRatios_Beam.at(i).GetDnUnc();
			chisq += tmp * tmp;
			br_chisq += tmp*tmp;
      if (verbose) {
			std::cout	<< std::setw(10) << std::left << index_init 
					<< std::setw(10) << std::left << index_final1
					<< std::setw(10) << std::left << index_final2
					<< std::setw(10) << std::left << calcBR
					<< std::setw(10) << std::left << BR
					<< std::setw(10) << std::left << litBranchingRatios_Beam.at(i).GetUpUnc()
					<< std::setw(10) << std::left << tmp*tmp 
					<< std::endl;
      }
		}
		NDF++;
		NDF_lit++;
	}
	double mr_chisq = 0;
	if(litMixingRatios_Beam.size()>0 && verbose)
		std::cout	<< "Delta (Beam):" 
				<< std::endl;
	for(unsigned int i=0;i<litMixingRatios_Beam.size();i++){
		double 	tmp;
		int 	index_init	= litMixingRatios_Beam.at(i).GetInitialIndex();
		int	index_final	= litMixingRatios_Beam.at(i).GetFinalIndex();
		double	delta		= litMixingRatios_Beam.at(i).GetMixingRatio();
		double	calcDelta	= rates_b.GetMixingRatios()[index_final][index_init];
		if(fLikelihood){
			double	sigma		= litMixingRatios_Beam.at(i).GetUpUnc() * litMixingRatios_Beam.at(i).GetDnUnc();
			double	sigma_prime	= (litMixingRatios_Beam.at(i).GetUpUnc() - litMixingRatios_Beam.at(i).GetDnUnc());
			chisq 			+= 0.5 * TMath::Power((calcDelta - delta),2)/(sigma + sigma_prime * (calcDelta - delta));
		}
		else{
			if(calcDelta > delta)
				tmp = (delta - calcDelta) / litMixingRatios_Beam.at(i).GetUpUnc();
			else
				tmp = (delta - calcDelta) / litMixingRatios_Beam.at(i).GetDnUnc();
			chisq += tmp * tmp;		
			mr_chisq += tmp*tmp;
      if (verbose) {
			std::cout	<< std::setw(10) << std::left << index_init 
					<< std::setw(10) << std::left << index_final
					<< std::setw(10) << std::left << calcDelta
					<< std::setw(10) << std::left << delta
					<< std::setw(10) << std::left << litMixingRatios_Beam.at(i).GetUpUnc()
					<< std::setw(10) << std::left << tmp*tmp 
					<< std::endl;
      }
		}
		NDF++;
		NDF_lit++;
	}
	double me_chisq = 0;
	if(litMatrixElements_Beam.size()>0 && verbose)
		std::cout 	<< "Matrix Elements (Beam)"
				<< std::endl;
	for(unsigned int i=0;i<litMatrixElements_Beam.size();i++){
		double tmp;
		int	mult		= litMatrixElements_Beam.at(i).GetMultipolarity();
		int 	index_init	= litMatrixElements_Beam.at(i).GetInitialIndex();
		int	index_final	= litMatrixElements_Beam.at(i).GetFinalIndex();
		double	ME		= litMatrixElements_Beam.at(i).GetMatrixElement();
		double	calcME		= nucl_b.GetMatrixElements().at(mult)[index_init][index_final];
		if(fLikelihood){
			double	sigma		= litMatrixElements_Beam.at(i).GetUpUnc() * litMatrixElements_Beam.at(i).GetDnUnc();
			double	sigma_prime	= (litMatrixElements_Beam.at(i).GetUpUnc() - litMatrixElements_Beam.at(i).GetDnUnc());
			chisq 			+= 0.5 * TMath::Power((calcME - ME),2)/(sigma + sigma_prime * (calcME - ME));
		}
		else{
			if(calcME > ME)
				tmp = (ME - calcME) / litMatrixElements_Beam.at(i).GetUpUnc();
			else
				tmp = (ME - calcME) / litMatrixElements_Beam.at(i).GetDnUnc();
			chisq += tmp * tmp;		
			me_chisq += tmp*tmp;
      if (verbose) {
			std::cout	<< std::setw(10) << std::left << index_init 
					<< std::setw(10) << std::left << index_final
					<< std::setw(10) << std::left << mult
					<< std::setw(14) << std::left << calcME
					<< std::setw(14) << std::left << ME
					<< std::setw(14) << std::left << litMatrixElements_Beam.at(i).GetUpUnc()
					<< std::setw(14) << std::left << tmp*tmp 
					<< std::endl;
      }
		}
		NDF++;
		NDF_lit++;
	}
	// 	Now, compare with the literature for the target:
	TransitionRates rates_t(&nucl_t);
	for(unsigned int i=0;i<litLifetimes_Target.size();i++){
		double 	tmp = 0;
		int	index		= litLifetimes_Target.at(i).GetIndex();
		double	lifetime	= litLifetimes_Target.at(i).GetLifetime();
		double	calcLifetime	= rates_t.GetLifetimes()[index];
		if(fLikelihood){
			double	sigma		= litLifetimes_Target.at(i).GetUpUnc() * litLifetimes_Target.at(i).GetDnUnc();
			double	sigma_prime	= (litLifetimes_Target.at(i).GetUpUnc() - litLifetimes_Target.at(i).GetDnUnc());
			chisq 			+= 0.5 * TMath::Power((calcLifetime - lifetime),2)/(sigma + sigma_prime * (calcLifetime - lifetime));
		}
		else{
			if(calcLifetime > lifetime)
				tmp = (calcLifetime - lifetime) / litLifetimes_Target.at(i).GetUpUnc();
			else
				tmp = (calcLifetime - lifetime) / litLifetimes_Target.at(i).GetDnUnc();
			chisq += tmp * tmp;
			lifetime_chisq += tmp*tmp;
		}
		NDF++;
		NDF_lit++;
	}
	if(litBranchingRatios_Target.size()>0)
    if (verbose) {
		std::cout	<< "BR (Target):" 
				<< std::endl;
    }
	for(unsigned int i=0;i<litBranchingRatios_Target.size();i++){
		double 	tmp 		= 0;
		int	index_init 	= litBranchingRatios_Target.at(i).GetInitialIndex();
		int	index_final1	= litBranchingRatios_Target.at(i).GetFinalIndex_1();
		int	index_final2	= litBranchingRatios_Target.at(i).GetFinalIndex_2();
		double	BR		= litBranchingRatios_Target.at(i).GetBranchingRatio();
		double  calcBR		= rates_t.GetBranchingRatios()[index_final1][index_init] / rates_t.GetBranchingRatios()[index_final2][index_init];
		if(fLikelihood){
			double	sigma		= litBranchingRatios_Target.at(i).GetUpUnc() * litBranchingRatios_Target.at(i).GetDnUnc();
			double	sigma_prime	= (litBranchingRatios_Target.at(i).GetUpUnc() - litBranchingRatios_Target.at(i).GetDnUnc());
			chisq 			+= 0.5 * TMath::Power((calcBR - BR),2)/(sigma + sigma_prime * (calcBR - BR));
		}
		else{
			if(calcBR > BR)
				tmp = (BR - calcBR) / litBranchingRatios_Target.at(i).GetUpUnc();
			else
				tmp = (BR - calcBR) / litBranchingRatios_Target.at(i).GetDnUnc();
			chisq += tmp * tmp;
			br_chisq += tmp*tmp;
      if (verbose) {
			std::cout	<< std::setw(10) << std::left << index_init 
					<< std::setw(10) << std::left << index_final1
					<< std::setw(10) << std::left << index_final2
					<< std::setw(10) << std::left << calcBR
					<< std::setw(10) << std::left << BR
					<< std::setw(10) << std::left << litBranchingRatios_Target.at(i).GetUpUnc()
					<< std::setw(10) << std::left << tmp*tmp 
					<< std::endl;
      }
		}
		NDF++;
		NDF_lit++;
	}
	if(litMixingRatios_Target.size()>0)
    if (verbose) {
		std::cout	<< "Delta (Target):" 
				<< std::endl;
    }
	for(unsigned int i=0;i<litMixingRatios_Target.size();i++){
		double tmp;
		int 	index_init	= litMixingRatios_Target.at(i).GetInitialIndex();
		int	index_final	= litMixingRatios_Target.at(i).GetFinalIndex();
		double	delta		= litMixingRatios_Target.at(i).GetMixingRatio();
		double	calcDelta	= rates_t.GetMixingRatios()[index_final][index_init];
		if(fLikelihood){
			double	sigma		= litMixingRatios_Target.at(i).GetUpUnc() * litMixingRatios_Target.at(i).GetDnUnc();
			double	sigma_prime	= (litMixingRatios_Target.at(i).GetUpUnc() - litMixingRatios_Target.at(i).GetDnUnc());
			chisq 			+= 0.5 * TMath::Power((calcDelta - delta),2)/(sigma + sigma_prime * (calcDelta - delta));
		}
		else{
			if(calcDelta > delta)
				tmp = (delta - calcDelta) / litMixingRatios_Target.at(i).GetUpUnc();
			else
				tmp = (delta - calcDelta) / litMixingRatios_Target.at(i).GetDnUnc();
			chisq += tmp * tmp;		
			mr_chisq += tmp*tmp;
      if (verbose) {
			std::cout	<< std::setw(10) << std::left << index_init 
					<< std::setw(10) << std::left << index_final
					<< std::setw(10) << std::left << calcDelta
					<< std::setw(10) << std::left << delta
					<< std::setw(10) << std::left << litMixingRatios_Target.at(i).GetUpUnc()
					<< std::setw(10) << std::left << tmp*tmp 
					<< std::endl;
      }
		}
		NDF++;
		NDF_lit++;
	}
	if(litMatrixElements_Target.size()>0)
    if (verbose) {
		std::cout 	<< "Matrix Elements (Target)"
				<< std::endl;
    }
	for(unsigned int i=0;i<litMatrixElements_Target.size();i++){
		double tmp;
		int	mult		= litMatrixElements_Target.at(i).GetMultipolarity();
		int 	index_init	= litMatrixElements_Target.at(i).GetInitialIndex();
		int	index_final	= litMatrixElements_Target.at(i).GetFinalIndex();
		double	ME		= litMatrixElements_Target.at(i).GetMatrixElement();
		double	calcME		= nucl_t.GetMatrixElements().at(mult)[index_init][index_final];
		if(fLikelihood){
			double	sigma		= litMatrixElements_Target.at(i).GetUpUnc() * litMatrixElements_Target.at(i).GetDnUnc();
			double	sigma_prime	= (litMatrixElements_Target.at(i).GetUpUnc() - litMatrixElements_Target.at(i).GetDnUnc());
			chisq 			+= 0.5 * TMath::Power((calcME - ME),2)/(sigma + sigma_prime * (calcME - ME));
		}
		else{
			if(calcME > ME)
				tmp = (ME - calcME) / litMatrixElements_Target.at(i).GetUpUnc();
			else
				tmp = (ME - calcME) / litMatrixElements_Target.at(i).GetDnUnc();
			chisq += tmp * tmp;		
			me_chisq += tmp*tmp;
      if (verbose) {
			std::cout	<< std::setw(10) << std::left << index_init 
					<< std::setw(10) << std::left << index_final
					<< std::setw(10) << std::left << mult
					<< std::setw(14) << std::left << calcME
					<< std::setw(14) << std::left << ME
					<< std::setw(14) << std::left << litMatrixElements_Target.at(i).GetUpUnc()
					<< std::setw(14) << std::left << tmp*tmp 
					<< std::endl;
      }
		}
		NDF++;
		NDF_lit++;
	}
	double litchisq = chisq;
	//	COULEX AND STUFF:

	std::ofstream		beam_bst(workingDir+"/"+beamBSTFile);
	for(size_t i=0;i<beamMapping_i.size();i++){
		beam_bst << nucl_b.GetMatrixElements().at(beamMapping_l.at(i))[beamMapping_f.at(i)][beamMapping_i.at(i)] << "\n";
	}
	beam_bst.close();

	std::ofstream		target_bst(workingDir+"/"+targetBSTFile);
	for(size_t i=0;i<targetMapping_i.size();i++){
		target_bst 	<< nucl_t.GetMatrixElements().at(targetMapping_l.at(i))[targetMapping_f.at(i)][targetMapping_i.at(i)] << "\n";
	}
	target_bst.close();

	std::string	str;
	//str = "cd "+workingDir+"; ./gosia < "+ beamGOSIAFile_inp+"> /dev/null";
	//const char* 	c_b = str.c_str();
	//system(c_b);		// 	Run the beam GOSIA file
  RunGosia(beamGOSIAFile_inp, workingDir, "dump.out");
  RunGosia(targetGOSIAFile_inp, workingDir, "dump.out");
	//str = "cd "+workingDir+"; ./gosia < "+ targetGOSIAFile_inp+"> /dev/null";
	//const char* 	c_t = str.c_str();
	//system(c_t);	// 	Run the target GOSIA file

	GOSIAReader	beam_gosiaReader(&nucl_b,(workingDir+beamGOSIAFile_out).c_str());	//	Grab the GOSIA yields
	GOSIAReader	target_gosiaReader(&nucl_t,(workingDir+targetGOSIAFile_out).c_str());	//	Grab the GOSIA yields

	std::vector<ExperimentData>	beamCalc	= beam_gosiaReader.GetGOSIAData();
	std::vector<ExperimentData>	targetCalc	= target_gosiaReader.GetGOSIAData();
	EffectiveCrossSection_Beam.clear();	
	EffectiveCrossSection_Target.clear();	

	for(size_t i=0; i<beamCalc.size(); i++){
		TMatrixD	tmpMat;
		tmpMat.ResizeTo(rates_b.GetBranchingRatios().GetNrows(),rates_b.GetBranchingRatios().GetNcols());
		size_t	nRows = beamCalc.at(i).GetData().size();
		for(size_t j=0; j<nRows; j++){
			int	init		= beamCalc.at(i).GetData().at(j).GetInitialIndex();
			int	fina		= beamCalc.at(i).GetData().at(j).GetFinalIndex();
			double 	counts 		= beamCalc.at(i).GetData().at(j).GetCounts();
			if(false && i==0){
				std::cout	<< std::setw( 6) << std::left << init
						<< std::setw( 6) << std::left << fina
						<< std::setw(12) << std::left << counts	
						<< std::setw(12) << std::left << correctionFactors_Beam.at(i)[init][fina]
						<< std::setw(12) << std::left << correctionFactors_Beam.at(i)[init][fina] * counts
						<< std::endl;
			}
			tmpMat[fina][init]	= counts * correctionFactors_Beam.at(i)[init][fina];
			tmpMat[init][fina]	= counts * correctionFactors_Beam.at(i)[init][fina];
		}
		EffectiveCrossSection_Beam.push_back(tmpMat);
	}

	for(size_t i=0; i<targetCalc.size(); i++){
		TMatrixD	tmpMat;
		tmpMat.ResizeTo(rates_t.GetBranchingRatios().GetNrows(),rates_t.GetBranchingRatios().GetNcols());
		size_t	nRows = targetCalc.at(i).GetData().size();
		for(size_t j=0; j<nRows; j++){
			int	init		= targetCalc.at(i).GetData().at(j).GetInitialIndex();
			int	fina		= targetCalc.at(i).GetData().at(j).GetFinalIndex();
			double 	counts 		= targetCalc.at(i).GetData().at(j).GetCounts();
			if(false && i==0){
				std::cout	<< std::setw( 6) << std::left << init
						<< std::setw( 6) << std::left << fina
						<< std::setw(12) << std::left << counts	
						<< std::setw(12) << std::left << correctionFactors_Target.at(i)[init][fina]
						<< std::setw(12) << std::left << correctionFactors_Target.at(i)[init][fina] * counts
						<< std::endl;
			}
			tmpMat[fina][init]	= counts * correctionFactors_Target.at(i)[init][fina];
			tmpMat[init][fina]	= counts * correctionFactors_Target.at(i)[init][fina];
		}
		EffectiveCrossSection_Target.push_back(tmpMat);
	}

	if(verbose)
		std::cout << std::endl;

	//	Everything needs printing for both beam and target...
	if(verbose && !fLikelihood){
		std::cout 	<< std::setw( 7) << std::left << "Beam:"
				<< std::endl;
		std::cout 	<< std::setw( 4) << std::left << " ";
		std::cout	<< std::setw(10) << std::left << "Scaling:";
//		for(unsigned int t=0;t<exptData_Beam.at(exptData_Beam.size()-1).GetData().size();t++){
			std::cout 	<< std::setw( 6) << std::left << "Init:"
					<< std::setw( 6) << std::left << "Finl:"
					<< std::setw(14) << std::left << "Calc:"
					<< std::setw(14) << std::left << "Expt:"
					<< std::setw(14) << std::left << "Err:"
					<< std::setw(14) << std::left << "C/E:"
					<< std::setw(14) << std::left << "Chisq:"
      << std::setw(20) << std::left << " "
      << std::setw( 6) << std::left << "Init:"
					<< std::setw( 6) << std::left << "Finl:"
					<< std::setw(14) << std::left << "Calc:"
					<< std::setw(14) << std::left << "Expt:"
					<< std::setw(14) << std::left << "Err:"
					<< std::setw(14) << std::left << "C/E:"
					<< std::setw(14) << std::left << "Chisq:";      
//		}
		std::cout 	<< std::endl;
	}
	else if(verbose){
		std::cout 	<< std::setw( 7) << std::left << "Beam:"
				<< std::endl;
		std::cout 	<< std::setw( 4) << std::left << " ";
		std::cout	<< std::setw(10) << std::left << "Scaling:";
		for(unsigned int t=0;t<exptData_Beam.at(exptData_Beam.size()-1).GetData().size();t++){
			std::cout	<< std::setw( 6) << std::left << "Init:"
					<< std::setw( 6) << std::left << "Finl:"
					<< std::setw(10) << std::left << "Calc:"
					<< std::setw(10) << std::left << "Expt:"
					<< std::setw(10) << std::left << "Calc/Expt:"
					<< std::setw(12) << std::left << "-Ln(L) cont.:";
		}
		std::cout 	<< std::endl;
	}

	std::vector<double>	scaling;
	scaling.resize(exptData_Beam.size());
	for(size_t s=0;s<scalingParameters.size();s++){
		std::vector<double>	sc_expt;
		std::vector<double>	sc_expt_unc;
		std::vector<double>	sc_calc;
		for(size_t ss=0;ss<scalingParameters.at(s).GetExperimentNumbers().size();ss++){
			size_t i = scalingParameters.at(s).GetExperimentNumbers().at(ss);
			if(expt_weights.at(i) == 0) 
				continue;
			if(i < exptData_Beam.size()){
				for(size_t t=0;t<exptData_Beam.at(i).GetData().size();++t){
					int	index_init 	= exptData_Beam.at(i).GetData().at(t).GetInitialIndex();
					int	index_final 	= exptData_Beam.at(i).GetData().at(t).GetFinalIndex();
					double 	calcCounts 	= EffectiveCrossSection_Beam.at(i)[index_final][index_init];
					double 	exptCounts 	= exptData_Beam.at(i).GetData().at(t).GetCounts();
					double	sigma		= (exptData_Beam.at(i).GetData().at(t).GetUpUnc() + exptData_Beam.at(i).GetData().at(t).GetDnUnc())/2.;  // Average uncertainty
					sigma 	/= expt_weights.at(i);
					if(sigma > 0 && calcCounts > 0 && exptCounts > 0){
						sc_expt.push_back(exptCounts);
						sc_expt_unc.push_back(sigma);
						sc_calc.push_back(calcCounts);
					}				
				}
				for(size_t t=0;i<exptData_Beam.at(i).GetDoublet().size();++t){
					int	index_init1 	= exptData_Beam.at(i).GetDoublet().at(t).GetInitialIndex1();
					int	index_final1 	= exptData_Beam.at(i).GetDoublet().at(t).GetFinalIndex1();
					int	index_init2 	= exptData_Beam.at(i).GetDoublet().at(t).GetInitialIndex2();
					int	index_final2 	= exptData_Beam.at(i).GetDoublet().at(t).GetFinalIndex2();
					double 	calcCounts 	= EffectiveCrossSection_Beam.at(i)[index_final1][index_init1] + EffectiveCrossSection_Beam.at(i)[index_final2][index_init2];
					double 	exptCounts 	= exptData_Beam.at(i).GetDoublet().at(t).GetCounts();
					double	sigma		= (exptData_Beam.at(i).GetDoublet().at(t).GetUpUnc() + exptData_Beam.at(i).GetDoublet().at(t).GetDnUnc())/2.;  // Average uncertainty
					sigma 	/= expt_weights.at(i);
					if(sigma > 0 && calcCounts > 0 && exptCounts > 0){
						sc_expt.push_back(exptCounts);
						sc_expt_unc.push_back(sigma);
						sc_calc.push_back(calcCounts);
					}				
				}
			}
			if(i < exptData_Target.size()){
				for(size_t t=0;t<exptData_Target.at(i).GetData().size();++t){
					int	index_init 	= exptData_Target.at(i).GetData().at(t).GetInitialIndex();
					int	index_final 	= exptData_Target.at(i).GetData().at(t).GetFinalIndex();
					double 	calcCounts 	= EffectiveCrossSection_Target.at(i)[index_final][index_init];
					double 	exptCounts 	= exptData_Target.at(i).GetData().at(t).GetCounts();
					double	sigma		= (exptData_Target.at(i).GetData().at(t).GetUpUnc() + exptData_Target.at(i).GetData().at(t).GetDnUnc())/2.;  // Average uncertainty
					sigma 	/= expt_weights.at(i);
					if(sigma > 0 && calcCounts > 0 && exptCounts > 0){
						sc_expt.push_back(exptCounts);
						sc_expt_unc.push_back(sigma);
						sc_calc.push_back(calcCounts);
					}
				}
				for(size_t t=0;i<exptData_Target.at(i).GetDoublet().size();++t){
					int	index_init1 	= exptData_Target.at(i).GetDoublet().at(t).GetInitialIndex1();
					int	index_final1 	= exptData_Target.at(i).GetDoublet().at(t).GetFinalIndex1();
					int	index_init2 	= exptData_Target.at(i).GetDoublet().at(t).GetInitialIndex2();
					int	index_final2 	= exptData_Target.at(i).GetDoublet().at(t).GetFinalIndex2();
					double 	calcCounts 	= EffectiveCrossSection_Target.at(i)[index_final1][index_init1] + EffectiveCrossSection_Target.at(i)[index_final2][index_init2];
					double 	exptCounts 	= exptData_Target.at(i).GetDoublet().at(t).GetCounts();
					double	sigma		= (exptData_Target.at(i).GetDoublet().at(t).GetUpUnc() + exptData_Target.at(i).GetDoublet().at(t).GetDnUnc())/2.;  // Average uncertainty
					sigma 	/= expt_weights.at(i);
					if(sigma > 0 && calcCounts > 0 && exptCounts > 0){
						sc_expt.push_back(exptCounts);
						sc_expt_unc.push_back(sigma);
						sc_calc.push_back(calcCounts);
					}								
				}
			}
		}

		if(sc_expt.size() > 0){
			ScalingFitFCN theFCN;

			theFCN.SetData(sc_expt,sc_expt_unc,sc_calc);
		
			ROOT::Math::Minimizer *min =
				ROOT::Math::Factory::CreateMinimizer("Minuit2","Migrad");
			ROOT::Math::Functor f_init(theFCN,1);
			min->SetFunction(f_init);
			min->SetVariable(0,"Scaling",1,0.000001);
			min->SetTolerance(0.001);
			min->Minimize();


			//min->PrintResults();
		
			for(size_t ss=0;ss<scalingParameters.at(s).GetExperimentNumbers().size();ss++){
				size_t i 	= scalingParameters.at(s).GetExperimentNumbers().at(ss);
				scaling[i]	= min->X()[0];
			}
		}
		else{
			for(size_t ss=0;ss<scalingParameters.at(s).GetExperimentNumbers().size();ss++){
				size_t i 	= scalingParameters.at(s).GetExperimentNumbers().at(ss);
				scaling[i]	= 0;
			}

		}	

	}

	double			beamchisq = 0;
	std::vector<int>	beamdata;
	std::vector<double>	beamexptchisq;
	int			counter = 0;

	for(unsigned int i=0;i<exptData_Beam.size();i++){
		double	exptchisq	= 0;
		if(expt_weights.at(i) == 0) 
			continue;
		if(verbose){
			std::cout	<< std::setw(4) << std::left << i+1;
			std::cout	<< std::setw(10) << std::left << scaling.at(i);
		}
		for(unsigned int t=0;t<exptData_Beam.at(i).GetData().size();++t){
			double 	tmp 		= 0;
			int	index_init 	= exptData_Beam.at(i).GetData().at(t).GetInitialIndex();
			int	index_final 	= exptData_Beam.at(i).GetData().at(t).GetFinalIndex();
			double 	calcCounts 	= scaling.at(i) * EffectiveCrossSection_Beam.at(i)[index_final][index_init];
			double 	exptCounts 	= exptData_Beam.at(i).GetData().at(t).GetCounts();
			double	sigma		= exptData_Beam.at(i).GetData().at(t).GetUpUnc() * exptData_Beam.at(i).GetData().at(t).GetDnUnc();
			double	sigma_prime	= (exptData_Beam.at(i).GetData().at(t).GetUpUnc() - exptData_Beam.at(i).GetData().at(t).GetDnUnc());
			sigma			/= expt_weights.at(i);
			sigma_prime		/= expt_weights.at(i);
			if(exptCounts > 0 && sigma > 0){
				if(true){// && ((index_init == 1 && index_final ==  0) || (index_init == 7 && index_final == 4))){
          if (verbose) {
            if(fLikelihood){
              std::cout 	<< std::setw( 6) << std::left << index_init 
                          << std::setw( 6) << std::left << index_final 
                          << std::setw(14) << std::left << calcCounts 
                          << std::setw(14) << std::left << exptCounts 
                          << std::setw(14) << std::left << calcCounts/exptCounts
                          << std::setw(14) << std::left << 0.5 * TMath::Power((exptCounts - calcCounts),2)/(sigma + sigma_prime * (exptCounts - calcCounts)) << std::endl << "              ";
            }
            else{
              std::cout 	<< std::setw( 6) << std::left << index_init 
                          << std::setw( 6) << std::left << index_final 
                          << std::setw(14) << std::left << calcCounts 
                          << std::setw(14) << std::left << exptCounts 
                          << std::setw(14) << std::left << exptData_Beam.at(i).GetData().at(t).GetUpUnc()
                          << std::setw(14) << std::left << calcCounts/exptCounts
                          << std::setw(14) << std::left << TMath::Power((calcCounts - exptCounts)/exptData_Beam.at(i).GetData().at(t).GetUpUnc(),2);
              if (t%2 == 0) {
                std::cout << std::setw(20) << std::left << " ";
                if (t==exptData_Beam.at(i).GetData().size()-1) { std::cout << std::endl; }
              }
              else { std::cout << std::endl << std::setw(14) << std::left << " "; }
            }
          }
        }
				if(fLikelihood){
					chisq		+= 0.5 * TMath::Power((exptCounts - calcCounts),2)/(sigma + sigma_prime * (exptCounts - calcCounts));
					beamchisq	+= 0.5 * TMath::Power((exptCounts - calcCounts),2)/(sigma + sigma_prime * (exptCounts - calcCounts));
				}
				else{
					if(calcCounts > exptCounts)
						tmp 		= (calcCounts - exptCounts) / exptData_Beam.at(i).GetData().at(t).GetUpUnc();
					else
						tmp 		= (calcCounts - exptCounts) / exptData_Beam.at(i).GetData().at(t).GetDnUnc();
					chisq		+= tmp * tmp;
					beamchisq	+= tmp * tmp;
					exptchisq	+= tmp * tmp;
				}
				NDF++;
				NDF_beam++;
			}
		}
		for(unsigned int t=0;t<exptData_Beam.at(i).GetDoublet().size();++t){
			double 	tmp 		= 0;
			int	index_init1 	= exptData_Beam.at(i).GetDoublet().at(t).GetInitialIndex1();
			int	index_final1 	= exptData_Beam.at(i).GetDoublet().at(t).GetFinalIndex1();
			int	index_init2 	= exptData_Beam.at(i).GetDoublet().at(t).GetInitialIndex2();
			int	index_final2 	= exptData_Beam.at(i).GetDoublet().at(t).GetFinalIndex2();
			double 	calcCounts 	= scaling.at(i) * (EffectiveCrossSection_Beam.at(i)[index_final1][index_init1] + EffectiveCrossSection_Beam.at(i)[index_final2][index_init2]);
			double 	exptCounts 	= exptData_Beam.at(i).GetDoublet().at(t).GetCounts();
			double	sigma		= exptData_Beam.at(i).GetDoublet().at(t).GetUpUnc() * exptData_Beam.at(i).GetDoublet().at(t).GetDnUnc();
			double	sigma_prime	= (exptData_Beam.at(i).GetDoublet().at(t).GetUpUnc() - exptData_Beam.at(i).GetDoublet().at(t).GetDnUnc());
			sigma			/= expt_weights.at(i);
			sigma_prime		/= expt_weights.at(i);
			if(calcCounts > 0 && sigma > 0){
				if(true){// && ((index_init == 1 && index_final ==  0) || (index_init == 7 && index_final == 4))){
          if (verbose) {
            if(fLikelihood){
              std::cout 	<< std::setw( 6) << std::left << index_init1*100 + index_init2
                          << std::setw( 6) << std::left << index_final1*100 + index_final2
                          << std::setw(10) << std::left << calcCounts 
                          << std::setw(10) << std::left << exptCounts 
                          << std::setw(10) << std::left << calcCounts/exptCounts
                          << std::setw(12) << std::left << 0.5 * TMath::Power((exptCounts - calcCounts),2)/(sigma + sigma_prime * (exptCounts - calcCounts));
            }
            else{
              std::cout 	<< std::setw( 6) << std::left << index_init1*100 + index_init2 
                          << std::setw( 6) << std::left << index_final1*100 + index_final2
                          << std::setw(10) << std::left << calcCounts 
                          << std::setw(10) << std::left << exptCounts 
                          << std::setw(10) << std::left << exptData_Beam.at(i).GetDoublet().at(t).GetUpUnc()
                          << std::setw(10) << std::left << calcCounts/exptCounts
                          << std::setw(12) << std::left << TMath::Power((calcCounts - exptCounts)/exptData_Beam.at(i).GetData().at(t).GetUpUnc(),2);
            }
          }
				}
				if(fLikelihood){
					chisq		+= 0.5 * TMath::Power((exptCounts - calcCounts),2)/(sigma + sigma_prime * (exptCounts - calcCounts));
					beamchisq	+= 0.5 * TMath::Power((exptCounts - calcCounts),2)/(sigma + sigma_prime * (exptCounts - calcCounts));
				}
				else{
					if(calcCounts > exptCounts)
						tmp 		= (calcCounts - exptCounts) / exptData_Beam.at(i).GetData().at(t).GetUpUnc();
					else
						tmp 		= (calcCounts - exptCounts) / exptData_Beam.at(i).GetData().at(t).GetDnUnc();
					chisq		+= tmp * tmp;
					beamchisq	+= tmp * tmp;
					exptchisq	+= tmp * tmp;
				}
				NDF++;
				NDF_beam++;
			}
		}
		beamdata.push_back(i);
		beamexptchisq.push_back(exptchisq);
		if(verbose)
			std::cout << std::endl;
		counter++;
	}
	if(verbose && !fLikelihood){
		std::cout 	<< std::setw( 7) << std::left << "Target:"
				<< std::endl;
		std::cout 	<< std::setw( 4) << std::left << "";
		std::cout	<< std::setw(10) << std::left << "Scaling:";
		if(exptData_Target.size() > 0){
			for(unsigned int t=0;t<exptData_Target.at(exptData_Target.size()-1).GetData().size();t++){
				std::cout	<< std::setw( 6) << std::left << "Init:"
						<< std::setw( 6) << std::left << "Finl:"
						<< std::setw(10) << std::left << "Calc:"
						<< std::setw(10) << std::left << "Expt:"
						<< std::setw(10) << std::left << "Err:"
						<< std::setw(10) << std::left << "C/E:"
						<< std::setw(12) << std::left << "Chisq:";
			}
			std::cout 	<< std::endl;
		}
    std::cout 	<< std::endl;
	}
	else if(verbose){
		std::cout 	<< std::setw( 7) << std::left << "Target:"
				<< std::endl;
		std::cout 	<< std::setw( 4) << std::left << " ";
		std::cout	<< std::setw(10) << std::left << "Scaling:";
		if(exptData_Target.size() > 0){
			for(unsigned int t=0;t<exptData_Target.at(exptData_Target.size()-1).GetData().size();t++){
				std::cout	<< std::setw( 6) << std::left << "Init:"
						<< std::setw( 6) << std::left << "Finl:"
						<< std::setw(10) << std::left << "Calc:"
						<< std::setw(10) << std::left << "Expt:"
						<< std::setw(10) << std::left << "Calc/Expt:"
						<< std::setw(12) << std::left << "-Ln(L) cont.:";
			}
			std::cout 	<< std::endl;
		}
	}

	double			targchisq = 0;
	std::vector<int>	targdata;
	std::vector<double>	targexptchisq;
	counter = 0;

	for(unsigned int i=0;i<exptData_Target.size();i++){
		double	exptchisq	= 0;
		if(expt_weights.at(i) == 0.)
			continue;
		if(verbose){
			std::cout	<< std::setw(4) << std::left << i+1;
			std::cout	<< std::setw(10) << std::left << scaling.at(i);
		}
		for(unsigned int t=0;t<exptData_Target.at(i).GetData().size();++t){
			double 	tmp 		= 0;
			int	index_init 	= exptData_Target.at(i).GetData().at(t).GetInitialIndex();
			int	index_final 	= exptData_Target.at(i).GetData().at(t).GetFinalIndex();
			double 	calcCounts 	= scaling.at(i) * EffectiveCrossSection_Target.at(i)[index_final][index_init];
			double 	exptCounts 	= exptData_Target.at(i).GetData().at(t).GetCounts();
			double	sigma		= exptData_Target.at(i).GetData().at(t).GetUpUnc() * exptData_Target.at(i).GetData().at(t).GetDnUnc();
			double	sigma_prime	= (exptData_Target.at(i).GetData().at(t).GetUpUnc() - exptData_Target.at(i).GetData().at(t).GetDnUnc());	
			sigma			/= expt_weights.at(i);
			sigma_prime		/= expt_weights.at(i); 
			if(calcCounts > 0 && sigma > 0){
				if(true){
          if (verbose) {
					if(fLikelihood){
						std::cout 	<< std::setw( 6) << std::left << index_init 
								<< std::setw( 6) << std::left << index_final 
								<< std::setw(10) << std::left << calcCounts 
								<< std::setw(10) << std::left << exptCounts 
								<< std::setw(10) << std::left << calcCounts/exptCounts
								<< std::setw(12) << std::left << 0.5 * TMath::Power((exptCounts - calcCounts),2)/(sigma + sigma_prime * (exptCounts - calcCounts));
					}
					else{
						std::cout 	<< std::setw( 6) << std::left << index_init 
								<< std::setw( 6) << std::left << index_final 
								<< std::setw(10) << std::left << calcCounts 
								<< std::setw(10) << std::left << exptCounts 
								<< std::setw(10) << std::left << exptData_Target.at(i).GetData().at(t).GetUpUnc() 
								<< std::setw(10) << std::left << calcCounts/exptCounts
								<< std::setw(12) << std::left << TMath::Power((calcCounts - exptCounts)/exptData_Target.at(i).GetData().at(t).GetUpUnc(),2);
					}
          }
				}
				if(fLikelihood){
					chisq		+= 0.5 * TMath::Power((exptCounts - calcCounts),2)/(sigma + sigma_prime * (exptCounts - calcCounts));
					targchisq	+= 0.5 * TMath::Power((exptCounts - calcCounts),2)/(sigma + sigma_prime * (exptCounts - calcCounts));
				}
				else{
					if(calcCounts > exptCounts)
						tmp 		= (calcCounts - exptCounts) / exptData_Target.at(i).GetData().at(t).GetUpUnc();
					else
						tmp 		= (calcCounts - exptCounts) / exptData_Target.at(i).GetData().at(t).GetDnUnc();
					chisq		+= tmp * tmp;
					targchisq 	+= tmp * tmp;
					exptchisq	+= tmp * tmp;
				}
				NDF++;
				NDF_targ++;
			}
		}
		for(unsigned int t=0;t<exptData_Target.at(i).GetDoublet().size();++t){
			double 	tmp 		= 0;
			int	index_init1 	= exptData_Target.at(i).GetDoublet().at(t).GetInitialIndex1();
			int	index_final1 	= exptData_Target.at(i).GetDoublet().at(t).GetFinalIndex1();
			int	index_init2 	= exptData_Target.at(i).GetDoublet().at(t).GetInitialIndex2();
			int	index_final2 	= exptData_Target.at(i).GetDoublet().at(t).GetFinalIndex2();
			double 	calcCounts 	= scaling.at(i) * (EffectiveCrossSection_Target.at(i)[index_final1][index_init1] + EffectiveCrossSection_Target.at(i)[index_final2][index_init2]);
			double 	exptCounts 	= exptData_Target.at(i).GetDoublet().at(t).GetCounts();
			double	sigma		= exptData_Target.at(i).GetDoublet().at(t).GetUpUnc() * exptData_Target.at(i).GetDoublet().at(t).GetDnUnc();
			double	sigma_prime	= (exptData_Target.at(i).GetDoublet().at(t).GetUpUnc() - exptData_Target.at(i).GetDoublet().at(t).GetDnUnc());
			sigma			/= expt_weights.at(i);
			sigma_prime		/= expt_weights.at(i);
			if(calcCounts > 0 && sigma > 0){
				if(true){// && ((index_init == 1 && index_final ==  0) || (index_init == 7 && index_final == 4))){
          if (verbose) {
					if(fLikelihood){
						std::cout 	<< std::setw( 6) << std::left << index_init1*100 + index_init2
								<< std::setw( 6) << std::left << index_final1*100 + index_final2
								<< std::setw(10) << std::left << calcCounts 
								<< std::setw(10) << std::left << exptCounts 
								<< std::setw(10) << std::left << calcCounts/exptCounts
								<< std::setw(12) << std::left << 0.5 * TMath::Power((exptCounts - calcCounts),2)/(sigma + sigma_prime * (exptCounts - calcCounts));
					}
					else{
						std::cout 	<< std::setw( 6) << std::left << index_init1*100 + index_init2 
								<< std::setw( 6) << std::left << index_final1*100 + index_final2
								<< std::setw(10) << std::left << calcCounts 
								<< std::setw(10) << std::left << exptCounts 
								<< std::setw(10) << std::left << exptData_Target.at(i).GetDoublet().at(t).GetUpUnc()
								<< std::setw(10) << std::left << calcCounts/exptCounts
								<< std::setw(12) << std::left << TMath::Power((calcCounts - exptCounts)/exptData_Target.at(i).GetData().at(t).GetUpUnc(),2);
					}
          }
				}
				if(fLikelihood){
					chisq		+= 0.5 * TMath::Power((exptCounts - calcCounts),2)/(sigma + sigma_prime * (exptCounts - calcCounts));
					targchisq	+= 0.5 * TMath::Power((exptCounts - calcCounts),2)/(sigma + sigma_prime * (exptCounts - calcCounts));
				}
				else{
					if(calcCounts > exptCounts)
						tmp 		= (calcCounts - exptCounts) / exptData_Target.at(i).GetData().at(t).GetUpUnc();
					else
						tmp 		= (calcCounts - exptCounts) / exptData_Target.at(i).GetData().at(t).GetDnUnc();
					chisq		+= tmp * tmp;
					beamchisq	+= tmp * tmp;
					targchisq	+= tmp * tmp;
				}
				NDF++;
				NDF_targ++;
			}
		}
		targdata.push_back(i);
		targexptchisq.push_back(exptchisq);
		if(verbose)
			std::cout << std::endl;
	}

	if(verbose){
		std::cout	<< std::setw(16) << std::left << "Beam expt.:";
		for(size_t i=0;i<beamdata.size();i++)
			std::cout << std::setw(8) << std::left << beamdata.at(i);
		std::cout	<< std::endl;
		std::cout	<< std::setw(16) << std::left << "Chi-squared:";
		for(size_t i=0;i<beamexptchisq.size();i++)
			std::cout << std::setw(8) << std::left << std::setprecision(3) << beamexptchisq.at(i);
		std::cout	<< std::endl;
		std::cout	<< std::setw(16) << std::left << "Target expt.:";
		for(size_t i=0;i<targdata.size();i++)
			std::cout << std::setw(8) << std::left << targdata.at(i);
		std::cout	<< std::endl;
		std::cout	<< std::setw(16) << std::left << "Chi-squared:";
		for(size_t i=0;i<targexptchisq.size();i++)
			std::cout << std::setw(8) << std::left << std::setprecision(3) << targexptchisq.at(i);
		std::cout	<< std::endl;

	}

	if(verbose){
		std::cout 	<< std::setw(26) << std::left << "Literature chi-squared: " 
				<< std::setw(12) << std::left << std::setprecision(6) << litchisq
				<< std::setw(26) << std::left << "Beam chi-squared: " 
				<< std::setw(12) << std::left << std::setprecision(6) << beamchisq 
				<< std::setw(26) << std::left << "Target chi-squared: " 
				<< std::setw(12) << std::left << std::setprecision(6) << targchisq 
				<< std::setw(26) << std::left << "Total chi-squared: " 
				<< std::setw(12) << std::left << std::setprecision(6) << chisq 
				<< std::setw(16) << std::left << "NDF: " 
				<< std::setw( 8) << std::left << std::setprecision(6) << NDF
				<< std::setw(18) << std::left << "Chisq / NDF: " 
				<< std::setw(12) << std::left << std::setprecision(6) << chisq / (float)NDF;
		if(true){
			std::cout	<< std::endl;
			std::cout	<< std::setw(16) << std::left << "NDF lit: " 
					<< std::setw( 8) << std::left << std::setprecision(6) << NDF_lit
					<< std::setw(16) << std::left << "NDF beam: " 
					<< std::setw( 8) << std::left << std::setprecision(6) << NDF_beam
					<< std::setw(16) << std::left << "NDF target: " 
					<< std::setw( 8) << std::left << std::setprecision(6) << NDF_targ;
		}
		std::cout	<< std::endl;
	}

	iter++;

	Clock::time_point t1 = Clock::now();
	milliseconds ms = std::chrono::duration_cast<milliseconds>(t1-t0);

	if((iter % 100) == 0 && !verbose)
		std::cout 	<< std::setw(12) << std::left << iter 
				<< std::setw(13) << std::left << chisq 
				<< std::setw(7)  << std::left << NDF
				<< std::setw(13) << std::left << chisq/(double)NDF 
				<< std::setw(12) << std::left << litchisq 
				<< std::setw(24) << std::left << ms.count() 
				<< "\r" << std::flush;

	return chisq;	

}

void GOSIASimMinFCN::ClearAll(){

	parameters.clear();			
	
	ME_Beam.clear();			
	exptData_Beam.clear();			
	litLifetimes_Beam.clear();			
	litBranchingRatios_Beam.clear();		
	litMixingRatios_Beam.clear();		
	EffectiveCrossSection_Beam.clear();

	ME_Target.clear();			
	exptData_Target.clear();			
	litLifetimes_Target.clear();			
	litBranchingRatios_Target.clear();		
	litMixingRatios_Target.clear();		
	EffectiveCrossSection_Target.clear();

}
