#include "Output.h"

Output::Output(GOSIASimFitter *f) : fitter(f) 
{

	if(f->GetFitParameters().size() == 0)
		f->SetupParameters();
	CalculateScaling();

}

void Output::CalculateScaling(){

	workingDir	= fitter->GetWorkingDir();
	ME_Beam		= fitter->GetBeamMatrixElements();
	ME_Target	= fitter->GetTargetMatrixElements();
	ME_BeamRel	= fitter->GetBeamRelMEs();
	ME_TargetRel	= fitter->GetTargetRelMEs();


	std::vector<int>	beamMapping_i	= fitter->GetBeamMappingInit();	
	std::vector<int>	beamMapping_f	= fitter->GetBeamMappingFinal();	
	std::vector<int>	beamMapping_l	= fitter->GetBeamMappingLambda();	

	std::vector<int>	targetMapping_i	= fitter->GetTargetMappingInit();	
	std::vector<int>	targetMapping_f	= fitter->GetTargetMappingFinal();	
	std::vector<int>	targetMapping_l	= fitter->GetTargetMappingLambda();


	correctionFactors_Beam		= fitter->GetBeamCorrectionFactors();	 
        correctionFactors_Target	= fitter->GetTargetCorrectionFactors();

                                 
        exptData_Beam			= fitter->GetBeamData();		 
        exptData_Target			= fitter->GetTargetData();	 

                                 
        litLifetimes_Beam		= fitter->GetBeamLitLifetimes();	 
        litLifetimes_Target		= fitter->GetTargetLitLifetimes();
	 
                                 
        litBranchingRatios_Beam		= fitter->GetBeamLitBranching(); 
        litBranchingRatios_Target	= fitter->GetTargetLitBranching();

                                 
        litMixingRatios_Beam		= fitter->GetBeamLitMixing();	 
        litMixingRatios_Target		= fitter->GetTargetLitMixing();	 

                                 
        litMatrixElements_Beam		= fitter->GetBeamLitMatrixElement();	 
	litMatrixElements_Target	= fitter->GetTargetLitMatrixElement();

	scalingParameters		= fitter->GetScalingParameters();


	expt_weights			= fitter->GetWeightingFactor();


	beamBSTFile			= fitter->GetBeamBST();
	targetBSTFile			= fitter->GetTargetBST();
	
	Nucleus nucl_b;
        Nucleus nucl_t;

	bool	beam	= false;
	bool	targ	= false;

	if(fitter->GetNTargetExpt() > 0){
		targ	= true;
		nucl_t	= fitter->GetTargetNucleus();
	}
	if(fitter->GetNBeamExpt() > 0){
		beam	= true;
		nucl_b	= fitter->GetBeamNucleus();
	}
	

	std::vector<double>	parameters;
	if(beam){	
		for(size_t i=0;i<fitter->GetBeamMatrixElements().size();i++){	
			nucl_b.SetMatrixElement(fitter->GetBeamMatrixElements().at(i).GetLambda(),fitter->GetBeamMatrixElements().at(i).GetInitialState(),fitter->GetBeamMatrixElements().at(i).GetFinalState(),fitter->GetFitParameters().at(i));
			parameters.push_back(fitter->GetFitParameters().at(i));
		}
	}
	
	if(targ){
		for(size_t i=0;i<fitter->GetTargetMatrixElements().size();i++){
			nucl_t.SetMatrixElement(fitter->GetTargetMatrixElements().at(i).GetLambda(),fitter->GetTargetMatrixElements().at(i).GetInitialState(),fitter->GetTargetMatrixElements().at(i).GetFinalState(),fitter->GetFitParameters().at(i+fitter->GetBeamMatrixElements().size()));
			parameters.push_back(fitter->GetFitParameters().at(i + fitter->GetBeamMatrixElements().size()));
		}
	}
	
	
	if(beam){
		std::ofstream		beam_bst(fitter->GetBeamBST());
		for(size_t i=0;i<fitter->GetBeamMappingInit().size();i++){
			beam_bst << nucl_b.GetMatrixElements().at(fitter->GetBeamMappingLambda().at(i))[fitter->GetBeamMappingFinal().at(i)][fitter->GetBeamMappingInit().at(i)] << "\n";
		}
		beam_bst.close();
	}
	
	
	if(targ){
		std::ofstream		target_bst(fitter->GetTargetBST());
		for(size_t i=0;i<fitter->GetTargetMappingInit().size();i++){
			target_bst 	<< nucl_t.GetMatrixElements().at(fitter->GetTargetMappingLambda().at(i))[fitter->GetTargetMappingFinal().at(i)][fitter->GetTargetMappingInit().at(i)] << "\n";
		}
		target_bst.close();
	}

	std::string	str;

	GOSIAReader	beam_gosiaReader;
	GOSIAReader	target_gosiaReader;
	std::vector<ExperimentData>	beamCalc;
	std::vector<ExperimentData>	targetCalc;
	std::vector<TMatrixD>		EffectiveCrossSection_Beam;	
	std::vector<TMatrixD>		EffectiveCrossSection_Target;
	
	if(beam){
		str = "./gosia < "+ fitter->GetBeamGOSIAInput()+"> /dev/null";
		const char* 	c_b 	= str.c_str();
		system(c_b);		// 	Run the beam GOSIA file
		const char	*b_out	= fitter->GetBeamGOSIAOutput().c_str();
		beam_gosiaReader	= GOSIAReader(&nucl_b,b_out);	//	Grab the GOSIA yields
		beamCalc		= beam_gosiaReader.GetGOSIAData();
	}
	if(targ){
		str = "./gosia < "+ fitter->GetTargetGOSIAInput()+"> /dev/null";
		const char* 	c_t 	= str.c_str();
		system(c_t);	// 	Run the target GOSIA file
		const char	*t_out 	= fitter->GetTargetGOSIAOutput().c_str();
		target_gosiaReader	= GOSIAReader(&nucl_t,t_out);	//	Grab the GOSIA yields
		targetCalc		= target_gosiaReader.GetGOSIAData();
	}
	
	
	TransitionRates rates_b;
	if(beam)
		rates_b	= TransitionRates(&nucl_b);
	TransitionRates rates_t;
	if(targ)
		rates_t	= TransitionRates(&nucl_t);

	
	for(size_t i=0; i<beamCalc.size(); i++){
		TMatrixD	tmpMat;
		tmpMat.ResizeTo(rates_b.GetBranchingRatios().GetNrows(),rates_b.GetBranchingRatios().GetNcols());
		size_t	nRows = beamCalc.at(i).GetData().size();
		for(size_t j=0; j<nRows; j++){
			int	init		= beamCalc.at(i).GetData().at(j).GetInitialIndex();
			int	fina		= beamCalc.at(i).GetData().at(j).GetFinalIndex();
			double 	counts 		= beamCalc.at(i).GetData().at(j).GetCounts();
			tmpMat[fina][init]	= counts * fitter->GetBeamCorrectionFactors().at(i)[init][fina];
			tmpMat[init][fina]	= counts * fitter->GetBeamCorrectionFactors().at(i)[init][fina];
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
			tmpMat[fina][init]	= counts * fitter->GetTargetCorrectionFactors().at(i)[init][fina];
			tmpMat[init][fina]	= counts * fitter->GetTargetCorrectionFactors().at(i)[init][fina];
		}
		EffectiveCrossSection_Target.push_back(tmpMat);
	}
	

	std::vector<double>	scaling;
	scaling.resize(fitter->GetBeamData().size());
	for(size_t s=0;s<fitter->GetScalingParameters().size();s++){
		std::vector<double>	sc_expt;
		std::vector<double>	sc_expt_unc;
		std::vector<double>	sc_calc;
		for(size_t ss=0;ss<scalingParameters.at(s).GetExperimentNumbers().size();ss++){
			size_t i = scalingParameters.at(s).GetExperimentNumbers().at(ss);
			float	ynrm = scalingParameters.at(s).GetExperimentNRMs().at(ss);	// Relative scaling parameter
			if(expt_weights.at(i) == 0) 
				continue;
			if(i < exptData_Beam.size()){
				for(size_t t=0;t<exptData_Beam.at(i).GetData().size();++t){
					int	index_init 	= exptData_Beam.at(i).GetData().at(t).GetInitialIndex();
					int	index_final 	= exptData_Beam.at(i).GetData().at(t).GetFinalIndex();
					double 	calcCounts 	= EffectiveCrossSection_Beam.at(i)[index_final][index_init];
					calcCounts		*= ynrm;
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
					calcCounts		*= ynrm;
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
					calcCounts		*= ynrm;
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
					calcCounts		*= ynrm;
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
			min->SetTolerance(0.000001);
			min->Minimize();

		
			for(size_t ss=0;ss<scalingParameters.at(s).GetExperimentNumbers().size();ss++){
				size_t i 	= scalingParameters.at(s).GetExperimentNumbers().at(ss);
				float ynrm 	= scalingParameters.at(s).GetExperimentNRMs().at(ss);	// Relative scaling parameter
				scaling[i]	= min->X()[0] * ynrm;
			}
		}
		else{
			for(size_t ss=0;ss<scalingParameters.at(s).GetExperimentNumbers().size();ss++){
				size_t i 	= scalingParameters.at(s).GetExperimentNumbers().at(ss);
				scaling[i]	= 0;
			}

		}	
	}

	std::cout	<< std::setw( 8) << std::left << "Expt:"	
			<< std::setw(10) << std::left << "Scaling:"
			<< std::endl;
	for(size_t i=0;i<fitter->GetBeamData().size();i++){
			std::cout	<< std::setw(8) << std::left << i+1	
					<< std::setw(10) << std::left << scaling.at(i)
					<< std::endl;	
	}

}

void Output::Write(const char* outfilename){

	std::ofstream	outfile(outfilename);

	double chisq = 0;
	int 	NDF = 0;

	int	NDF_lit = 0;
	int	NDF_beam = 0;
	int 	NDF_targ = 0;

	
	Nucleus nucl_b;
        Nucleus nucl_t;

	bool	beam	= false;
	bool	targ	= false;

	if(fitter->GetNTargetExpt() > 0){
		targ	= true;
		nucl_t	= fitter->GetTargetNucleus();
	}
	if(fitter->GetNBeamExpt() > 0){
		beam	= true;
		nucl_b	= fitter->GetBeamNucleus();
	}

	if(beam){	
		for(size_t i=0;i<fitter->GetBeamMatrixElements().size();i++){	
			nucl_b.SetMatrixElement(fitter->GetBeamMatrixElements().at(i).GetLambda(),fitter->GetBeamMatrixElements().at(i).GetInitialState(),fitter->GetBeamMatrixElements().at(i).GetFinalState(),fitter->GetFitParameters().at(i));
		}
	}
	
	if(targ){
		for(size_t i=0;i<fitter->GetTargetMatrixElements().size();i++){
			nucl_t.SetMatrixElement(fitter->GetTargetMatrixElements().at(i).GetLambda(),fitter->GetTargetMatrixElements().at(i).GetInitialState(),fitter->GetTargetMatrixElements().at(i).GetFinalState(),fitter->GetFitParameters().at(i+fitter->GetBeamMatrixElements().size()));
		}
	}

	parameters	= fitter->GetFitParameters();
	
	workingDir	= fitter->GetWorkingDir();
	ME_Beam		= fitter->GetBeamMatrixElements();
	ME_Target	= fitter->GetTargetMatrixElements();
	ME_BeamRel	= fitter->GetBeamRelMEs();
	ME_TargetRel	= fitter->GetTargetRelMEs();


	std::vector<int>	beamMapping_i	= fitter->GetBeamMappingInit();	
	std::vector<int>	beamMapping_f	= fitter->GetBeamMappingFinal();	
	std::vector<int>	beamMapping_l	= fitter->GetBeamMappingLambda();	

	std::vector<int>	targetMapping_i	= fitter->GetTargetMappingInit();	
	std::vector<int>	targetMapping_f	= fitter->GetTargetMappingFinal();	
	std::vector<int>	targetMapping_l	= fitter->GetTargetMappingLambda();


	correctionFactors_Beam		= fitter->GetBeamCorrectionFactors();	 
        correctionFactors_Target	= fitter->GetTargetCorrectionFactors();

                                 
        exptData_Beam			= fitter->GetBeamData();		 
        exptData_Target			= fitter->GetTargetData();	 

                                 
        litLifetimes_Beam		= fitter->GetBeamLitLifetimes();	 
        litLifetimes_Target		= fitter->GetTargetLitLifetimes();
	 
                                 
        litBranchingRatios_Beam		= fitter->GetBeamLitBranching(); 
        litBranchingRatios_Target	= fitter->GetTargetLitBranching();

                                 
        litMixingRatios_Beam		= fitter->GetBeamLitMixing();	 
        litMixingRatios_Target		= fitter->GetTargetLitMixing();	 

                                 
        litMatrixElements_Beam		= fitter->GetBeamLitMatrixElement();	 
	litMatrixElements_Target	= fitter->GetTargetLitMatrixElement();

	scalingParameters		= fitter->GetScalingParameters();


	expt_weights			= fitter->GetWeightingFactor();


	beamBSTFile			= fitter->GetBeamBST();
	targetBSTFile			= fitter->GetTargetBST();
	
	int parct 	= 0;
	int nRelBeam 	= 0;
	int nRelTarget 	= 0;
	int nBeamME 	= 0;
	int nTargetME 	= 0;

	// 	COMPARE WITH LITERATURE CONSTRAINTS:
	// 	First, compare with the literature for the beam:
	
	
	TransitionRates rates_b;
	if(beam)
		rates_b	= TransitionRates(&nucl_b);
	TransitionRates rates_t;
	if(targ)
		rates_t	= TransitionRates(&nucl_t);


	double lifetime_chisq = 0;	
	if(litLifetimes_Beam.size()>0)    
		outfile	<< "Lifetimes (Beam):" 
				<< std::endl;
	for(size_t i=0;i<litLifetimes_Beam.size();i++){
		double 	tmp = 0;
		int	index		= litLifetimes_Beam.at(i).GetIndex();
		double	lifetime	= litLifetimes_Beam.at(i).GetLifetime();
		double	calcLifetime	= rates_b.GetLifetimes()[index];
		if(calcLifetime > lifetime)
			tmp = (calcLifetime - lifetime) / litLifetimes_Beam.at(i).GetUpUnc();
		else
			tmp = (calcLifetime - lifetime) / litLifetimes_Beam.at(i).GetDnUnc();
		chisq += tmp * tmp;
		lifetime_chisq += tmp*tmp;
		outfile		<< std::setw(14) << std::left << "Lifetime:"
				<< std::setw(10) << std::left << index 
				<< std::setw(10) << std::left << calcLifetime
				<< std::setw(10) << std::left << lifetime
				<< std::setw(10) << std::left << litLifetimes_Beam.at(i).GetUpUnc()
				<< std::setw(10) << std::left << tmp*tmp 
				<< std::endl;
		NDF++;
		NDF_lit++;
	}

	double br_chisq = 0;
	if(litBranchingRatios_Beam.size()>0)    
		outfile	<< "BR (Beam):" 
				<< std::endl;
	for(size_t i=0;i<litBranchingRatios_Beam.size();i++){
		double 	tmp 		= 0;
		int	index_init 	= litBranchingRatios_Beam.at(i).GetInitialIndex();
		int	index_final1	= litBranchingRatios_Beam.at(i).GetFinalIndex_1();
		int	index_final2	= litBranchingRatios_Beam.at(i).GetFinalIndex_2();
		double	BR		= litBranchingRatios_Beam.at(i).GetBranchingRatio();
		double  calcBR		= rates_b.GetBranchingRatios()[index_final1][index_init] / rates_b.GetBranchingRatios()[index_final2][index_init];
		if(calcBR > BR)
			tmp = (BR - calcBR) / litBranchingRatios_Beam.at(i).GetUpUnc();
		else
			tmp = (BR - calcBR) / litBranchingRatios_Beam.at(i).GetDnUnc();
		chisq += tmp * tmp;
		br_chisq += tmp*tmp;
		outfile		<< std::setw(10) << std::left << index_init 
				<< std::setw(10) << std::left << index_final1
				<< std::setw(10) << std::left << index_final2
				<< std::setw(10) << std::left << calcBR
				<< std::setw(10) << std::left << BR
				<< std::setw(10) << std::left << litBranchingRatios_Beam.at(i).GetUpUnc()
				<< std::setw(10) << std::left << tmp*tmp 
				<< std::endl;
		NDF++;
		NDF_lit++;
	}

	double mr_chisq = 0;
	if(litMixingRatios_Beam.size()>0)
		outfile	<< "Delta (Beam):" 
				<< std::endl;
	for(size_t i=0;i<litMixingRatios_Beam.size();i++){
		double 	tmp;
		int 	index_init	= litMixingRatios_Beam.at(i).GetInitialIndex();
		int	index_final	= litMixingRatios_Beam.at(i).GetFinalIndex();
		double	delta		= litMixingRatios_Beam.at(i).GetMixingRatio();
		double	calcDelta	= rates_b.GetMixingRatios()[index_final][index_init];
		if(calcDelta > delta)
			tmp = (delta - calcDelta) / litMixingRatios_Beam.at(i).GetUpUnc();
		else
			tmp = (delta - calcDelta) / litMixingRatios_Beam.at(i).GetDnUnc();
		chisq += tmp * tmp;		
		mr_chisq += tmp*tmp;
			outfile	<< std::setw(10) << std::left << index_init 
					<< std::setw(10) << std::left << index_final
					<< std::setw(10) << std::left << calcDelta
					<< std::setw(10) << std::left << delta
					<< std::setw(10) << std::left << litMixingRatios_Beam.at(i).GetUpUnc()
					<< std::setw(10) << std::left << tmp*tmp 
					<< std::endl;
		NDF++;
		NDF_lit++;
	}

	double me_chisq = 0;
	if(litMatrixElements_Beam.size()>0)
		outfile 	<< "Matrix Elements (Beam)"
				<< std::endl;
	for(size_t i=0;i<litMatrixElements_Beam.size();i++){
		double tmp;
		int	mult		= litMatrixElements_Beam.at(i).GetMultipolarity();
		int 	index_init	= litMatrixElements_Beam.at(i).GetInitialIndex();
		int	index_final	= litMatrixElements_Beam.at(i).GetFinalIndex();
		double	ME		= litMatrixElements_Beam.at(i).GetMatrixElement();
		double	calcME		= nucl_b.GetMatrixElements().at(mult)[index_init][index_final];
		if(calcME > ME)
			tmp = (ME - calcME) / litMatrixElements_Beam.at(i).GetUpUnc();
		else
			tmp = (ME - calcME) / litMatrixElements_Beam.at(i).GetDnUnc();
		chisq += tmp * tmp;		
		me_chisq += tmp*tmp;
		outfile	<< std::setw(10) << std::left << index_init 
				<< std::setw(10) << std::left << index_final
				<< std::setw(10) << std::left << mult
				<< std::setw(14) << std::left << calcME
				<< std::setw(14) << std::left << ME
				<< std::setw(14) << std::left << litMatrixElements_Beam.at(i).GetUpUnc()
				<< std::setw(14) << std::left << tmp*tmp 
				<< std::endl;
		NDF++;
		NDF_lit++;
	}
	// 	Now, compare with the literature for the target:


	if(litLifetimes_Target.size()>0)    
		outfile		<< "Lifetimes (Target):" 
				<< std::endl;
	for(size_t i=0;i<litLifetimes_Target.size();i++){
		double 	tmp = 0;
		int	index		= litLifetimes_Target.at(i).GetIndex();
		double	lifetime	= litLifetimes_Target.at(i).GetLifetime();
		double	calcLifetime	= rates_t.GetLifetimes()[index];
		if(calcLifetime > lifetime)
			tmp = (calcLifetime - lifetime) / litLifetimes_Target.at(i).GetUpUnc();
		else
			tmp = (calcLifetime - lifetime) / litLifetimes_Target.at(i).GetDnUnc();
		chisq += tmp * tmp;
		lifetime_chisq += tmp*tmp;
		outfile	<< std::setw(10) << std::left << index
				<< std::setw(10) << std::left << calcLifetime
				<< std::setw(10) << std::left << lifetime
				<< std::setw(10) << std::left << litLifetimes_Target.at(i).GetUpUnc()
				<< std::setw(10) << std::left << tmp*tmp 
				<< std::endl;
		NDF++;
		NDF_lit++;
	}

	if(litBranchingRatios_Target.size()>0){
		outfile		<< "BR (Target):" 
				<< std::endl;
	}
	for(size_t i=0;i<litBranchingRatios_Target.size();i++){
		double 	tmp 		= 0;
		int	index_init 	= litBranchingRatios_Target.at(i).GetInitialIndex();
		int	index_final1	= litBranchingRatios_Target.at(i).GetFinalIndex_1();
		int	index_final2	= litBranchingRatios_Target.at(i).GetFinalIndex_2();
		double	BR		= litBranchingRatios_Target.at(i).GetBranchingRatio();
		double  calcBR		= rates_t.GetBranchingRatios()[index_final1][index_init] / rates_t.GetBranchingRatios()[index_final2][index_init];
		if(calcBR > BR)
			tmp = (BR - calcBR) / litBranchingRatios_Target.at(i).GetUpUnc();
		else
			tmp = (BR - calcBR) / litBranchingRatios_Target.at(i).GetDnUnc();
		chisq +=tmp * tmp;
		br_chisq+= tmp*tmp;
		outfile	<< std::setw(10) << std::left << index_init 
			<< std::setw(10) << std::left << index_final1
			<< std::setw(10) << std::left << index_final2
			<< std::setw(10) << std::left << calcBR
			<< std::setw(10) << std::left << BR
			<< std::setw(10) << std::left << litBranchingRatios_Target.at(i).GetUpUnc()
			<< std::setw(10) << std::left << tmp*tmp 
			<< std::endl;
		NDF++;
		NDF_lit++;
	}
	
	if(litMixingRatios_Target.size()>0){
	    	outfile		<< "Delta (Target):" 
   				<< std::endl;
	}
	for(size_t i=0;i<litMixingRatios_Target.size();i++){
		double tmp;
		int 	index_init	= litMixingRatios_Target.at(i).GetInitialIndex();
		int	index_final	= litMixingRatios_Target.at(i).GetFinalIndex();
		double	delta		= litMixingRatios_Target.at(i).GetMixingRatio();
		double	calcDelta	= rates_t.GetMixingRatios()[index_final][index_init];
		if(calcDelta > delta)
			tmp = (delta - calcDelta) / litMixingRatios_Target.at(i).GetUpUnc();
		else
			tmp = (delta - calcDelta) / litMixingRatios_Target.at(i).GetDnUnc();
		chisq += tmp * tmp;		
		mr_chisq += tmp*tmp;
		outfile		<< std::setw(10) << std::left << index_init 
				<< std::setw(10) << std::left << index_final
				<< std::setw(10) << std::left << calcDelta
				<< std::setw(10) << std::left << delta
				<< std::setw(10) << std::left << litMixingRatios_Target.at(i).GetUpUnc()
				<< std::setw(10) << std::left << tmp*tmp 
				<< std::endl;
		NDF++;
		NDF_lit++;
	}
	
	if(litMatrixElements_Target.size()>0){
		outfile 	<< "Matrix Elements (Target)"
				<< std::endl;
	}
	for(size_t i=0;i<litMatrixElements_Target.size();i++){
		double tmp;
		int	mult		= litMatrixElements_Target.at(i).GetMultipolarity();
		int 	index_init	= litMatrixElements_Target.at(i).GetInitialIndex();
		int	index_final	= litMatrixElements_Target.at(i).GetFinalIndex();
		double	ME		= litMatrixElements_Target.at(i).GetMatrixElement();
		double	calcME		= nucl_t.GetMatrixElements().at(mult)[index_init][index_final];
		if(calcME > ME)
			tmp = (ME - calcME) / litMatrixElements_Target.at(i).GetUpUnc();
		else
			tmp = (ME - calcME) / litMatrixElements_Target.at(i).GetDnUnc();
		chisq += tmp * tmp;		
		me_chisq += tmp*tmp;
		outfile		<< std::setw(10) << std::left << index_init 
				<< std::setw(10) << std::left << index_final
				<< std::setw(10) << std::left << mult
				<< std::setw(14) << std::left << calcME
				<< std::setw(14) << std::left << ME
				<< std::setw(14) << std::left << litMatrixElements_Target.at(i).GetUpUnc()
				<< std::setw(14) << std::left << tmp*tmp 
				<< std::endl;
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

	if(exptData_Target.size() > 0){
		std::ofstream		target_bst(targetBSTFile);
		for(size_t i=0;i<targetMapping_i.size();i++){
			target_bst 	<< nucl_t.GetMatrixElements().at(targetMapping_l.at(i))[targetMapping_f.at(i)][targetMapping_i.at(i)] << "\n";
		}
		target_bst.close();
	}

	
	std::string	str;

	GOSIAReader	beam_gosiaReader;
	GOSIAReader	target_gosiaReader;
	std::vector<ExperimentData>	beamCalc;
	std::vector<ExperimentData>	targetCalc;
	std::vector<TMatrixD>		EffectiveCrossSection_Beam;	
	std::vector<TMatrixD>		EffectiveCrossSection_Target;

	if(beam){
		str = "./gosia < "+ fitter->GetBeamGOSIAInput()+"> /dev/null";
		const char* 	c_b 	= str.c_str();
		system(c_b);		// 	Run the beam GOSIA file
		const char	*b_out	= fitter->GetBeamGOSIAOutput().c_str();
		beam_gosiaReader	= GOSIAReader(&nucl_b,b_out);	//	Grab the GOSIA yields
		beamCalc		= beam_gosiaReader.GetGOSIAData();
	}
	if(targ){
		str = "./gosia < "+ fitter->GetTargetGOSIAInput()+"> /dev/null";
		const char* 	c_t 	= str.c_str();
		system(c_t);	// 	Run the target GOSIA file
		const char	*t_out 	= fitter->GetTargetGOSIAOutput().c_str();
		target_gosiaReader	= GOSIAReader(&nucl_t,t_out);	//	Grab the GOSIA yields
		targetCalc		= target_gosiaReader.GetGOSIAData();
	}
	

	for(size_t i=0; i<beamCalc.size(); i++){
		TMatrixD	tmpMat;
		tmpMat.ResizeTo(rates_b.GetBranchingRatios().GetNrows(),rates_b.GetBranchingRatios().GetNcols());
		size_t	nRows = beamCalc.at(i).GetData().size();
		for(size_t j=0; j<nRows; j++){
			int	init		= beamCalc.at(i).GetData().at(j).GetInitialIndex();
			int	fina		= beamCalc.at(i).GetData().at(j).GetFinalIndex();
			double 	counts 		= beamCalc.at(i).GetData().at(j).GetCounts();
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
			tmpMat[fina][init]	= counts * correctionFactors_Target.at(i)[init][fina];
			tmpMat[init][fina]	= counts * correctionFactors_Target.at(i)[init][fina];
		}
		EffectiveCrossSection_Target.push_back(tmpMat);
	}

	
	outfile 	<< std::endl;

	//	Everything needs printing for both beam and target...
	outfile 	<< std::setw( 7) << std::left << "Beam:"
			<< std::endl;
	outfile 	<< std::setw( 4) << std::left << " ";
	outfile		<< std::setw(10) << std::left << "Scaling:";
	outfile 	<< std::setw( 6) << std::left << "Init:"
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
	outfile 	<< std::endl;
	
	std::vector<double>	scaling;
	scaling.resize(exptData_Beam.size());
	for(size_t s=0;s<scalingParameters.size();s++){
		std::vector<double>	sc_expt;
		std::vector<double>	sc_expt_unc;
		std::vector<double>	sc_calc;
		for(size_t ss=0;ss<scalingParameters.at(s).GetExperimentNumbers().size();ss++){
			size_t i = scalingParameters.at(s).GetExperimentNumbers().at(ss);
			float	ynrm = scalingParameters.at(s).GetExperimentNRMs().at(ss);	// Relative scaling parameter
			if(expt_weights.at(i) == 0) 
				continue;
			if(i < exptData_Beam.size()){
				for(size_t t=0;t<exptData_Beam.at(i).GetData().size();++t){
					int	index_init 	= exptData_Beam.at(i).GetData().at(t).GetInitialIndex();
					int	index_final 	= exptData_Beam.at(i).GetData().at(t).GetFinalIndex();
					double 	calcCounts 	= EffectiveCrossSection_Beam.at(i)[index_final][index_init];
					calcCounts		*= ynrm;
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
					calcCounts		*= ynrm;
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
					calcCounts		*= ynrm;
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
					calcCounts		*= ynrm;
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

		
			for(size_t ss=0;ss<scalingParameters.at(s).GetExperimentNumbers().size();ss++){
				size_t i 	= scalingParameters.at(s).GetExperimentNumbers().at(ss);
				float ynrm 	= scalingParameters.at(s).GetExperimentNRMs().at(ss);	// Relative scaling parameter
				scaling[i]	= min->X()[0] * ynrm;
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

	for(size_t i=0;i<exptData_Beam.size();i++){
		double	exptchisq	= 0;
		if(expt_weights.at(i) == 0) 
			continue;
		outfile	<< std::setw(4) << std::left << i+1;
		outfile	<< std::setw(10) << std::left << scaling.at(i);
		for(size_t t=0;t<exptData_Beam.at(i).GetData().size();++t){
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
				outfile 	<< std::setw( 6) << std::left << index_init 
						<< std::setw( 6) << std::left << index_final 
						<< std::setw(14) << std::left << calcCounts 
						<< std::setw(14) << std::left << exptCounts 
						<< std::setw(14) << std::left << exptData_Beam.at(i).GetData().at(t).GetUpUnc()
						<< std::setw(14) << std::left << calcCounts/exptCounts
						<< std::setw(14) << std::left << TMath::Power((calcCounts - exptCounts)/exptData_Beam.at(i).GetData().at(t).GetUpUnc(),2);
				if (t%2 == 0) {
					outfile << std::setw(20) << std::left << " ";
					if (t==exptData_Beam.at(i).GetData().size()-1) { outfile << std::endl; }
				}
				else { 
					outfile << std::endl << std::setw(14) << std::left << " "; 
				}
			
				if(calcCounts > exptCounts)
					tmp 		= (calcCounts - exptCounts) / exptData_Beam.at(i).GetData().at(t).GetUpUnc();
				else
					tmp 		= (calcCounts - exptCounts) / exptData_Beam.at(i).GetData().at(t).GetDnUnc();
				chisq		+= tmp * tmp;
				beamchisq	+= tmp * tmp;
				exptchisq	+= tmp * tmp;
				
				NDF++;
				NDF_beam++;
			}
		}
		for(size_t t=0;t<exptData_Beam.at(i).GetDoublet().size();++t){
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
				outfile 	<< std::setw( 6) << std::left << index_init1*100 + index_init2 
						<< std::setw( 6) << std::left << index_final1*100 + index_final2
						<< std::setw(10) << std::left << calcCounts 
						<< std::setw(10) << std::left << exptCounts 
						<< std::setw(10) << std::left << exptData_Beam.at(i).GetDoublet().at(t).GetUpUnc()
						<< std::setw(10) << std::left << calcCounts/exptCounts
						<< std::setw(12) << std::left << TMath::Power((calcCounts - exptCounts)/exptData_Beam.at(i).GetData().at(t).GetUpUnc(),2);
				if(calcCounts > exptCounts)
					tmp 		= (calcCounts - exptCounts) / exptData_Beam.at(i).GetData().at(t).GetUpUnc();
				else
					tmp 		= (calcCounts - exptCounts) / exptData_Beam.at(i).GetData().at(t).GetDnUnc();
				chisq		+= tmp * tmp;
				beamchisq	+= tmp * tmp;
				exptchisq	+= tmp * tmp;
				
				NDF++;
				NDF_beam++;
			}
		}
		beamdata.push_back(i);
		beamexptchisq.push_back(exptchisq);
		outfile << std::endl;
		counter++;
	}

	outfile 	<< std::setw( 7) << std::left << "Target:"
			<< std::endl;
	outfile 	<< std::setw( 4) << std::left << "";
	outfile	<< std::setw(10) << std::left << "Scaling:";
	if(exptData_Target.size() > 0){
		for(size_t t=0;t<exptData_Target.at(exptData_Target.size()-1).GetData().size();t++){
			outfile	<< std::setw( 6) << std::left << "Init:"
					<< std::setw( 6) << std::left << "Finl:"
					<< std::setw(10) << std::left << "Calc:"
					<< std::setw(10) << std::left << "Expt:"
					<< std::setw(10) << std::left << "Err:"
					<< std::setw(10) << std::left << "C/E:"
					<< std::setw(12) << std::left << "Chisq:";
		}
		outfile 	<< std::endl;
	}
	outfile 	<< std::endl;

	double			targchisq = 0;
	std::vector<int>	targdata;
	std::vector<double>	targexptchisq;
	counter = 0;

	for(size_t i=0;i<exptData_Target.size();i++){
		double	exptchisq	= 0;
		if(expt_weights.at(i) == 0.)
			continue;
		outfile		<< std::setw(4) << std::left << i+1;
		outfile		<< std::setw(10) << std::left << scaling.at(i);
		for(size_t t=0;t<exptData_Target.at(i).GetData().size();++t){
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
				outfile 	<< std::setw( 6) << std::left << index_init 
						<< std::setw( 6) << std::left << index_final 
						<< std::setw(10) << std::left << calcCounts 
						<< std::setw(10) << std::left << exptCounts 
						<< std::setw(10) << std::left << exptData_Target.at(i).GetData().at(t).GetUpUnc() 
						<< std::setw(10) << std::left << calcCounts/exptCounts
						<< std::setw(12) << std::left << TMath::Power((calcCounts - exptCounts)/exptData_Target.at(i).GetData().at(t).GetUpUnc(),2);
				if(calcCounts > exptCounts)
					tmp 		= (calcCounts - exptCounts) / exptData_Target.at(i).GetData().at(t).GetUpUnc();
				else
					tmp 		= (calcCounts - exptCounts) / exptData_Target.at(i).GetData().at(t).GetDnUnc();
				chisq		+= tmp * tmp;
				targchisq 	+= tmp * tmp;
				exptchisq	+= tmp * tmp;
				NDF++;
				NDF_targ++;
			}
		}
		for(size_t t=0;t<exptData_Target.at(i).GetDoublet().size();++t){
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
				outfile 	<< std::setw( 6) << std::left << index_init1*100 + index_init2 
						<< std::setw( 6) << std::left << index_final1*100 + index_final2
						<< std::setw(10) << std::left << calcCounts 
						<< std::setw(10) << std::left << exptCounts 
						<< std::setw(10) << std::left << exptData_Target.at(i).GetDoublet().at(t).GetUpUnc()
						<< std::setw(10) << std::left << calcCounts/exptCounts
						<< std::setw(12) << std::left << TMath::Power((calcCounts - exptCounts)/exptData_Target.at(i).GetData().at(t).GetUpUnc(),2);
				if(calcCounts > exptCounts)
					tmp 		= (calcCounts - exptCounts) / exptData_Target.at(i).GetData().at(t).GetUpUnc();
				else
					tmp 		= (calcCounts - exptCounts) / exptData_Target.at(i).GetData().at(t).GetDnUnc();
				chisq		+= tmp * tmp;
				beamchisq	+= tmp * tmp;
				targchisq	+= tmp * tmp;
				NDF++;
				NDF_targ++;
			}
		}
		targdata.push_back(i);
		targexptchisq.push_back(exptchisq);
		outfile 	<< std::endl;
	}

	outfile		<< std::setw(16) << std::left << "Beam expt.:";
	for(size_t i=0;i<beamdata.size();i++)
		outfile 	<< std::setw(12) << std::left << beamdata.at(i) + 1;
	outfile		<< std::endl;
	outfile		<< std::setw(16) << std::left << "Chi-squared:";
	for(size_t i=0;i<beamexptchisq.size();i++)
		outfile 	<< std::setw(12) << std::left << std::setprecision(3) << beamexptchisq.at(i);
	outfile		<< std::endl;
	outfile		<< std::setw(16) << std::left << "Target expt.:";
	for(size_t i=0;i<targdata.size();i++)
		outfile 	<< std::setw(12) << std::left << targdata.at(i);
	outfile		<< std::endl;
	outfile		<< std::setw(16) << std::left << "Chi-squared:";
	for(size_t i=0;i<targexptchisq.size();i++)
		outfile 	<< std::setw(12) << std::left << std::setprecision(3) << targexptchisq.at(i);
	outfile		<< std::endl;

	outfile 	<< std::setw(26) << std::left << "Literature chi-squared: " 
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
	outfile	<< std::endl;
	outfile	<< std::setw(16) << std::left << "NDF lit: " 
			<< std::setw( 8) << std::left << std::setprecision(6) << NDF_lit
			<< std::setw(16) << std::left << "NDF beam: " 
			<< std::setw( 8) << std::left << std::setprecision(6) << NDF_beam
			<< std::setw(16) << std::left << "NDF target: " 
			<< std::setw( 8) << std::left << std::setprecision(6) << NDF_targ;
	outfile	<< std::endl;

	outfile	<< std::endl;
	outfile	<< parameters.size() << " Parameters:";
	int linect = 0;
	for (size_t i=0; i<parameters.size(); ++i) {
		if (linect%12 == 0) {
			outfile	<< std::endl;
			outfile << std::setw(20) << std::left;      
			if (i == 0) {
				outfile << "Beam: ";
				linect = 0;
			}
			else {
				outfile << " ";
			}
		}
		if (i == fitter->GetBeamMatrixElements().size() && fitter->GetBeamRelMEs().size() > 0) {
			outfile << std::endl;
			outfile << std::setw(20) << std::left << "Relative Beam: ";
			linect = 0;
		}
		else if (i == fitter->GetBeamMatrixElements().size() + fitter->GetBeamRelMEs().size() && fitter->GetTargetMatrixElements().size() > 0) {
			outfile << std::endl;
			outfile << std::setw(20) << std::left << "Target: ";
			linect = 0;
		}
		else if (i == fitter->GetBeamMatrixElements().size() + fitter->GetBeamRelMEs().size() + fitter->GetTargetMatrixElements().size() > 0 && fitter->GetTargetRelMEs().size() > 0) {
			outfile << std::endl;
			outfile << std::setw(20) << std::left << "Relative Target: ";
			linect = 0;
		}
		outfile	<< std::setw(12) << std::left << parameters[i];
		++linect;
	}
	outfile	<< std::endl;

	outfile	<< std::endl;
	outfile	<< std::endl;

	if(beam){
		for(size_t i=0; i<beamCalc.size(); i++){
			TMatrixD	tmpMat;
			outfile		<< "Experiment " << i+1 << ":"
					<< std::endl;
			outfile		<< std::setw( 8) << std::left << "Init:" 
					<< std::setw( 8) << std::left << "Final:"
					<< std::setw(12) << std::left << "Calc:"
					<< std::setw(12) << std::left << "Corr. Fac.:" 
					<< std::setw(12) << std::left << "Corrected Calc:" 
					<< std::endl;
			tmpMat.ResizeTo(rates_b.GetBranchingRatios().GetNrows(),rates_b.GetBranchingRatios().GetNcols());
			size_t	nRows = beamCalc.at(i).GetData().size();
			for(size_t j=0; j<nRows; j++){
				int	init		= beamCalc.at(i).GetData().at(j).GetInitialIndex();
				int	fina		= beamCalc.at(i).GetData().at(j).GetFinalIndex();
				double 	counts 		= beamCalc.at(i).GetData().at(j).GetCounts();
				outfile		<< std::setw( 8) << std::left << init
						<< std::setw( 8) << std::left << fina
						<< std::setw(12) << std::left << counts	
						<< std::setw(12) << std::left << correctionFactors_Beam.at(i)[init][fina]
						<< std::setw(12) << std::left << correctionFactors_Beam.at(i)[init][fina] * counts
						<< std::endl;
				
			}
		}
	}
	if(targ){
		for(size_t i=0; i<targetCalc.size(); i++){
			TMatrixD	tmpMat;
			outfile		<< "Experiment " << i+1 << ":"
					<< std::endl;
			outfile		<< std::setw( 8) << std::left << "Init:" 
					<< std::setw( 8) << std::left << "Final:"
					<< std::setw(12) << std::left << "Calc:"
					<< std::setw(12) << std::left << "Corr. Fac.:" 
					<< std::setw(12) << std::left << "Corrected Calc:" 
					<< std::endl;
			tmpMat.ResizeTo(rates_t.GetBranchingRatios().GetNrows(),rates_t.GetBranchingRatios().GetNcols());
			size_t	nRows = targetCalc.at(i).GetData().size();
			for(size_t j=0; j<nRows; j++){
				int	init		= targetCalc.at(i).GetData().at(j).GetInitialIndex();
				int	fina		= targetCalc.at(i).GetData().at(j).GetFinalIndex();
				double 	counts 		= targetCalc.at(i).GetData().at(j).GetCounts();
				outfile		<< std::setw( 8) << std::left << init
						<< std::setw( 8) << std::left << fina
						<< std::setw(12) << std::left << counts	
						<< std::setw(12) << std::left << correctionFactors_Target.at(i)[init][fina]
						<< std::setw(12) << std::left << correctionFactors_Target.at(i)[init][fina] * counts
						<< std::endl;
				
			}
		}
	}
	outfile.close();

}
