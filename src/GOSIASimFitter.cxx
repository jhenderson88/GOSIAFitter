#include "GOSIASimFitter.h"

#include "TString.h"
#include "TGraph.h"
#include "TGraphErrors.h"
#include "TMultiGraph.h"

GOSIASimFitter::GOSIASimFitter()
{

	ClearAll();

	first		= true;

	fLikelihood	= false;

	fDoFullUnc	= false;

	maxIter		= 500;
	maxCalls	= 500;
	fitTolerance	= 0.001;
	nThreads	= 1;

	verbose		= false;

	chisq		= -1;

  	workingDir = "./";

}

GOSIASimFitter::GOSIASimFitter(const GOSIASimFitter& g) {
  	workingDir = g.workingDir;

	index				= g.index;

	chisq				= g.chisq;
	parameters			= g.parameters;			
	par_LL				= g.par_LL;				
	par_UL				= g.par_UL;				
	matrixElements_Beam		= g.matrixElements_Beam;		
	matrixElements_Target		= g.matrixElements_Target;		
	scalingParameters		= g.scalingParameters;		

	correctionFactors_Beam		= g.correctionFactors_Beam;		
	correctionFactors_Target	= g.correctionFactors_Target;	

	exptData_Beam			= g.exptData_Beam;			
	exptData_Target			= g.exptData_Target;		

	litLifetimes_Beam		= g.litLifetimes_Beam;		
	litLifetimes_Target		= g.litLifetimes_Target;		

	litBranchingRatios_Beam		= g.litBranchingRatios_Beam;	
	litBranchingRatios_Target	= g.litBranchingRatios_Target;	

	litMixingRatios_Beam		= g.litMixingRatios_Beam;		
	litMixingRatios_Target		= g.litMixingRatios_Target;		

	litMatrixElements_Beam		= g.litMatrixElements_Beam;		
	litMatrixElements_Target	= g.litMatrixElements_Target;	

	theErrorDef			= g.theErrorDef;

	EffectiveCrossSection_Beam	= g.EffectiveCrossSection_Beam;
	EffectiveCrossSection_Target	= g.EffectiveCrossSection_Target;

	fNucleus_Beam			= g.fNucleus_Beam;
	fNucleus_Target			= g.fNucleus_Target;
	fNucleus_Beam_Base		= g.fNucleus_Beam_Base;
	fNucleus_Target_Base		= g.fNucleus_Target_Base;

	maxIter				= g.maxIter;
	maxCalls			= g.maxCalls;
	fitTolerance			= g.fitTolerance;

	nThreads			= g.nThreads;

	first				= g.first;
	verbose				= g.verbose;

	covMat				= g.covMat;
	corMat				= g.corMat;

	fDoFullUnc			= g.fDoFullUnc;

	fLikelihood			= g.fLikelihood;

	beamGOSIAFile_inp		= g.beamGOSIAFile_inp;
	targetGOSIAFile_inp		= g.targetGOSIAFile_inp;

	beamGOSIAFile_out		= g.beamGOSIAFile_out;
	targetGOSIAFile_out		= g.targetGOSIAFile_out;

	beamBSTFile			= g.beamBSTFile;
	targetBSTFile			= g.targetBSTFile;

	beamMapping_i			= g.beamMapping_i;
	beamMapping_f			= g.beamMapping_f;
	beamMapping_l			= g.beamMapping_l;

	targetMapping_i			= g.targetMapping_i;
	targetMapping_f			= g.targetMapping_f;
	targetMapping_l			= g.targetMapping_l;

	expt_weights			= g.expt_weights;

}
GOSIASimFitter& GOSIASimFitter::operator = (const GOSIASimFitter& g){

  	workingDir = g.workingDir;
	index				= g.index;

	chisq				= g.chisq;
	parameters			= g.parameters;			
	par_LL				= g.par_LL;				
	par_UL				= g.par_UL;				
	matrixElements_Beam		= g.matrixElements_Beam;		
	matrixElements_Target		= g.matrixElements_Target;		
	scalingParameters		= g.scalingParameters;		

	correctionFactors_Beam		= g.correctionFactors_Beam;		
	correctionFactors_Target	= g.correctionFactors_Target;	

	exptData_Beam			= g.exptData_Beam;			
	exptData_Target			= g.exptData_Target;		

	litLifetimes_Beam		= g.litLifetimes_Beam;		
	litLifetimes_Target		= g.litLifetimes_Target;		

	litBranchingRatios_Beam		= g.litBranchingRatios_Beam;	
	litBranchingRatios_Target	= g.litBranchingRatios_Target;	

	litMixingRatios_Beam		= g.litMixingRatios_Beam;		
	litMixingRatios_Target		= g.litMixingRatios_Target;		

	litMatrixElements_Beam		= g.litMatrixElements_Beam;		
	litMatrixElements_Target	= g.litMatrixElements_Target;	

	theErrorDef			= g.theErrorDef;

	EffectiveCrossSection_Beam	= g.EffectiveCrossSection_Beam;
	EffectiveCrossSection_Target	= g.EffectiveCrossSection_Target;

	fNucleus_Beam			= g.fNucleus_Beam;
	fNucleus_Target			= g.fNucleus_Target;
	fNucleus_Beam_Base		= g.fNucleus_Beam_Base;
	fNucleus_Target_Base		= g.fNucleus_Target_Base;

	maxIter				= g.maxIter;
	maxCalls			= g.maxCalls;
	fitTolerance			= g.fitTolerance;

	nThreads			= g.nThreads;

	first				= g.first;
	verbose				= g.verbose;

	covMat				= g.covMat;
	corMat				= g.corMat;

	fDoFullUnc			= g.fDoFullUnc;

	fLikelihood			= g.fLikelihood;

	beamGOSIAFile_inp		= g.beamGOSIAFile_inp;
	targetGOSIAFile_inp		= g.targetGOSIAFile_inp;

	beamGOSIAFile_out		= g.beamGOSIAFile_out;
	targetGOSIAFile_out		= g.targetGOSIAFile_out;

	beamBSTFile			= g.beamBSTFile;
	targetBSTFile			= g.targetBSTFile;

	beamMapping_i			= g.beamMapping_i;
	beamMapping_f			= g.beamMapping_f;
	beamMapping_l			= g.beamMapping_l;

	targetMapping_i			= g.targetMapping_i;
	targetMapping_f			= g.targetMapping_f;
	targetMapping_l			= g.targetMapping_l;

	expt_weights			= g.expt_weights;

	return *this;

}

void GOSIASimFitter::UpdateMEs() {
  	for(unsigned int i=0;i<matrixElements_Beam.size();i++){
    		fNucleus_Beam.SetMatrixElement(matrixElements_Beam.at(i).GetLambda(),matrixElements_Beam.at(i).GetInitialState(),matrixElements_Beam.at(i).GetFinalState(),matrixElements_Beam.at(i).GetMatrixElement());
	}
  	for(unsigned int i=0;i<relativeElements_Beam.size();i++){
    		double me = fNucleus_Beam.GetMatrixElements().at(relativeElements_Beam.at(i).GetLambdaRel())[relativeElements_Beam.at(i).GetInitialStateRel()][relativeElements_Beam.at(i).GetFinalStateRel()];
    		me = me * relativeElements_Beam.at(i).GetRelativeElement();
		fNucleus_Beam.SetMatrixElement(relativeElements_Beam.at(i).GetLambda(),relativeElements_Beam.at(i).GetInitialState(),relativeElements_Beam.at(i).GetFinalState(),me);
	}
  	for(unsigned int i=0;i<matrixElements_Target.size();i++){
	  	fNucleus_Target.SetMatrixElement(matrixElements_Target.at(i).GetLambda(),matrixElements_Target.at(i).GetInitialState(),matrixElements_Target.at(i).GetFinalState(),matrixElements_Target.at(i).GetMatrixElement());
	}
  	for(unsigned int i=0;i<relativeElements_Target.size();i++){
    		double me = fNucleus_Target.GetMatrixElements().at(relativeElements_Target.at(i).GetLambdaRel())[relativeElements_Target.at(i).GetInitialStateRel()][relativeElements_Target.at(i).GetFinalStateRel()];
    		me = me * relativeElements_Target.at(i).GetRelativeElement();
		fNucleus_Target.SetMatrixElement(relativeElements_Target.at(i).GetLambda(),relativeElements_Target.at(i).GetInitialState(),relativeElements_Target.at(i).GetFinalState(),me);
	}
}

void GOSIASimFitter::WriteBST() {
  	std::ofstream	beam_bst(workingDir+"/"+beamBSTFile);
	for(size_t i=0;i<beamMapping_i.size();i++){
		beam_bst << fNucleus_Beam.GetMatrixElements().at(beamMapping_l.at(i))[beamMapping_f.at(i)][beamMapping_i.at(i)] << "\n";
	}
	beam_bst.close();
  	std::ofstream	target_bst(workingDir+"/"+targetBSTFile);
	for(size_t i=0;i<targetMapping_i.size();i++){
		target_bst << fNucleus_Target.GetMatrixElements().at(targetMapping_l.at(i))[targetMapping_f.at(i)][targetMapping_i.at(i)] << "\n";
	}
	target_bst.close();
}

void GOSIASimFitter::SetupParameters(){

	names.clear();
	parameters.clear();
	par_LL.clear();
	par_UL.clear();
	int nBeamMatrixElements = 0;
	for(unsigned int i=0;i<matrixElements_Beam.size();i++){
		if (!matrixElements_Beam.at(i).GetFixed()) {
			parameters.push_back(matrixElements_Beam.at(i).GetMatrixElement());
			par_LL.push_back(matrixElements_Beam.at(i).GetMatrixElementLowerLimit());
			par_UL.push_back(matrixElements_Beam.at(i).GetMatrixElementUpperLimit());
			names.push_back("Beam-ME-"+std::to_string(i));
			++nBeamMatrixElements;
		}
	}
	int nRelativeBeam = 0;
	for(unsigned int i=0;i<relativeElements_Beam.size();i++){
		if (!relativeElements_Beam.at(i).GetFixed()) {
			++nRelativeBeam;
			parameters.push_back(relativeElements_Beam.at(i).GetRelativeElement());
			par_LL.push_back(relativeElements_Beam.at(i).GetRelativeElementLowerLimit());
			par_UL.push_back(relativeElements_Beam.at(i).GetRelativeElementUpperLimit());
			names.push_back("Beam-RelME-"+std::to_string(i));
		}
	}
	int nTargetMatrixElements = 0;
	for(unsigned int i=0;i<matrixElements_Target.size();i++){
		if (!matrixElements_Target.at(i).GetFixed()) {
			parameters.push_back(matrixElements_Target.at(i).GetMatrixElement());
			par_LL.push_back(matrixElements_Target.at(i).GetMatrixElementLowerLimit());
			par_UL.push_back(matrixElements_Target.at(i).GetMatrixElementUpperLimit());
			names.push_back("Target-ME-"+std::to_string(i));
		}
	}
	int nRelativeTarget = 0;
	for(unsigned int i=0;i<relativeElements_Target.size();i++){
		if (!relativeElements_Target.at(i).GetFixed()) {
			++nRelativeTarget;
			parameters.push_back(relativeElements_Target.at(i).GetRelativeElement());
			par_LL.push_back(relativeElements_Target.at(i).GetRelativeElementLowerLimit());
			par_UL.push_back(relativeElements_Target.at(i).GetRelativeElementUpperLimit());
			names.push_back("Target-RelME-"+std::to_string(i));
		}
	}  

	std::cout 	<< std::setw(12) << std::left << "Parameters:" 
                	<< std::endl;
	for(unsigned int i=0;i<nBeamMatrixElements;i++){
		std::cout	<< std::setw(13) << std::left << "Beam ME:" 
				<< std::setw(4) << std::left << i+1;
	}
  	for(unsigned int i=0;i<nRelativeBeam;i++){
		std::cout	<< std::setw(13) << std::left << "Rel Beam ME:" 
				<< std::setw(4) << std::left << i+1;
	}
	for(unsigned int i=0;i<nTargetMatrixElements;i++){
		std::cout	<< std::setw(13) << std::left << "Target ME:" 
              			<< std::setw(4) << std::left << i+1;
	}
 	for(unsigned int i=0;i<nRelativeTarget;i++){
		std::cout	<< std::setw(13) << std::left << "Rel Beam ME:" 
				<< std::setw(4) << std::left << i+1;
	}
	std::cout	<< std::endl;
	for(unsigned int i=0;i<parameters.size();i++){
		std::cout 	<< std::setw(13) << std::left << parameters.at(i) 
				<< std::setw(4) << std::left << "";
	}
	std::cout << std::endl;

	Nexpts = 0;
	if(exptData_Beam.size() > exptData_Target.size())
		Nexpts = exptData_Beam.size();
	else
		Nexpts = exptData_Target.size();

	if(expt_weights.size() != Nexpts){
		expt_weights.resize(Nexpts);
		std::fill(expt_weights.begin(),expt_weights.end(),1);
	}

}

void GOSIASimFitter::DoFit(const char* method, const char *algorithm){

	GOSIASimMinFCN theFCN(exptData_Beam,exptData_Target);

  	theFCN.SetWorkingDir(workingDir);
	theFCN.SetBeamGOSIAInput(GetBeamGOSIAInput());
	theFCN.SetBeamGOSIAOutput(GetBeamGOSIAOutput());
	theFCN.SetTargetGOSIAInput(GetTargetGOSIAInput());
	theFCN.SetTargetGOSIAOutput(GetTargetGOSIAOutput());
	theFCN.SetBeamBST(GetBeamBST());
	theFCN.SetTargetBST(GetTargetBST());
	theFCN.SetBeamMapping(beamMapping_i,beamMapping_f,beamMapping_l);
	theFCN.SetTargetMapping(targetMapping_i,targetMapping_f,targetMapping_l);

	theFCN.SetBeamMatrixElements(matrixElements_Beam);
  	theFCN.SetBeamRelativeElements(relativeElements_Beam);
	theFCN.SetTargetMatrixElements(matrixElements_Target);
  	theFCN.SetTargetRelativeElements(relativeElements_Target);
	theFCN.SetScalingParameters(scalingParameters);

	theFCN.SetBeamLitLifetimes(litLifetimes_Beam);
	theFCN.SetBeamLitBranching(litBranchingRatios_Beam);
	theFCN.SetBeamLitMixing(litMixingRatios_Beam);	
	theFCN.SetBeamLitMatrixElements(litMatrixElements_Beam);	
	theFCN.SetTargetLitLifetimes(litLifetimes_Target);
	theFCN.SetTargetLitBranching(litBranchingRatios_Target);
	theFCN.SetTargetLitMixing(litMixingRatios_Target);	
	theFCN.SetTargetLitMatrixElements(litMatrixElements_Target);	

	theFCN.SetBeamNucleus(&fNucleus_Beam);
	theFCN.SetTargetNucleus(&fNucleus_Target);

	theFCN.SetBeamCorrectionFactors(correctionFactors_Beam);
	theFCN.SetTargetCorrectionFactors(correctionFactors_Target);

	theFCN.SetIter(maxIter);
	theFCN.SetCalls(maxCalls);

	theFCN.SetNthreads(nThreads);

	theFCN.SetVerbose(verbose);

	theFCN.SetupCalculation();

	theFCN.SetLikelihoodFit(fLikelihood);

	if (verbose) {
		for(size_t m=0;m<matrixElements_Beam.size();m++)
			matrixElements_Beam.at(m).Print();
		for(size_t m=0;m<matrixElements_Target.size();m++)
			matrixElements_Target.at(m).Print();
	}

	SetupParameters();

	theFCN.SetWeights(expt_weights);

	theFCN.SetNpar(parameters.size());

	ROOT::Math::Minimizer *min =
			ROOT::Math::Factory::CreateMinimizer(method, algorithm);
	//		ROOT::Math::Factory::CreateMinimizer("Minuit2","Migrad");
	//		ROOT::Math::Factory::CreateMinimizer("Minuit2","Simplex");
	//		ROOT::Math::Factory::CreateMinimizer("Minuit2","Combined");
	//		ROOT::Math::Factory::CreateMinimizer("Minuit2","Scan");
	//		ROOT::Math::Factory::CreateMinimizer("Minuit2","Fumili");
	//		ROOT::Math::Factory::CreateMinimizer("GSLMultiMin", "ConjugateFR");
	//		ROOT::Math::Factory::CreateMinimizer("GSLMultiMin", "ConjugatePR");
	//		ROOT::Math::Factory::CreateMinimizer("GSLMultiMin", "BFGS");
	//		ROOT::Math::Factory::CreateMinimizer("GSLMultiMin", "BFGS2");
	//		ROOT::Math::Factory::CreateMinimizer("GSLMultiMin", "SteepestDescent");
	ROOT::Math::Functor f_init(theFCN,parameters.size());

	min->SetErrorDef(1.);
	//if(fLikelihood)
	if(false)
		min->SetErrorDef(0.5);

	std::cout 	<< "Iterations: " 
			<< maxIter << std::endl;
	std::cout 	<< "Calls: " 
			<< maxCalls << std::endl;

	min->SetMaxFunctionCalls(maxCalls);
	min->SetMaxIterations(maxIter);
	min->SetTolerance(fitTolerance);
	min->SetFunction(f_init);

	std::cout	<< "Tolerance: " << min->Tolerance()
			<< std::endl;

	for(unsigned int i=0; i<parameters.size(); i++){
    		min->SetLimitedVariable(i,names.at(i),parameters.at(i),0.0001,par_LL.at(i),par_UL.at(i));
	   	std::cout	<< names.at(i) 
				<< std::setw(4) << " " 
				<< std::setw(10) << std::left << parameters.at(i) 
				<< std::endl;
	}

	std::cout << std::endl;


	min->SetPrecision(1e-8);

	std::cout	<< "************************************ INITIAL MINIMIZATION ************************************"
			<< std::endl;


	if(!verbose && !fLikelihood){
		std::cout 	<< std::setw(12) << std::left << "Iteration:" 
				<< std::setw(13) << std::left << "Chi2 value:" 
				<< std::setw(7)  << std::left << "NDF:"
				<< std::setw(13) << std::left << "Red. Chi2:"
				<< std::setw(12) << std::left << "Lit. Chi2:" 
				<< std::setw(24) << std::left << "Processing time: (ms)" 
				<< std::endl;
	}
	else{
		std::cout 	<< std::setw(12) << std::left << "Iteration:" 
				<< std::setw(13) << std::left << "-Ln(L) value:" 
				<< std::setw(7)  << std::left << "NDF:"
				<< std::setw(13) << std::left << "Red. Ln(L):"
				<< std::setw(12) << std::left << "Lit. -Ln(L):" 
				<< std::setw(24) << std::left << "Processing time: (ms)" 
				<< std::endl;
	}
	
	typedef std::chrono::high_resolution_clock Clock;
	typedef std::chrono::milliseconds milliseconds;
	Clock::time_point t0 = Clock::now();

	min->Minimize();

	std::cout	<< std::endl;

	int 	counter = 0;
	int	status = min->Status();

	while(counter < 5 && status == 3){
		min->Minimize();
		status	= min->Status();
		counter++;
	}

	std::cout << std::endl;
	Clock::time_point t1 = Clock::now();
	milliseconds ms = std::chrono::duration_cast<milliseconds>(t1-t0);

	std::cout	<< "**************************************** FIT COMPLETE ****************************************"
			<< std::endl;

	//min->PrintResults();

	chisq	= min->MinValue();

	const double	*res = min->X();
	const double	*unc = min->Errors();

	std::ofstream	resultFile("FitResult.txt");

	resultFile	<< "Beam matrix elements:"
			<< std::endl;
	std::cout	<< "Beam matrix elements:"
			<< std::endl;
  	int parct = 0;
	for(size_t i=0;i<matrixElements_Beam.size();i++){
    		if (!matrixElements_Beam.at(i).GetFixed()) {
			matrixElements_Beam.at(i).SetMatrixElement(res[parct]);
			fNucleus_Beam.SetMatrixElement(matrixElements_Beam.at(i).GetLambda(),
							matrixElements_Beam.at(i).GetInitialState(),
							matrixElements_Beam.at(i).GetFinalState(),
							res[parct]);
			std::cout	<< std::setw(10) << std::left << matrixElements_Beam.at(i).GetInitialState()
					<< std::setw(10) << std::left << matrixElements_Beam.at(i).GetFinalState()
		//			<< std::setw(15) << std::left << fNucleus_Beam.GetMatrixElements().at(matrixElements_Beam.at(i).GetLambda())[matrixElements_Beam.at(i).GetInitialState()][matrixElements_Beam.at(i).GetFinalState()]
					<< std::setw(15) << std::left << res[parct] 
					<< std::setw(15) << std::left << unc[parct]
					<< std::endl;
			resultFile	<< std::setw(10) << std::left << matrixElements_Beam.at(i).GetInitialState()
					<< std::setw(10) << std::left << matrixElements_Beam.at(i).GetFinalState()
		//			<< std::setw(15) << std::left << fNucleus_Beam.GetMatrixElements().at(matrixElements_Beam.at(i).GetLambda())[matrixElements_Beam.at(i).GetInitialState()][matrixElements_Beam.at(i).GetFinalState()]
					<< std::setw(15) << std::left << res[parct] 
					<< std::setw(15) << std::left << unc[parct]
					<< std::endl;
      			++parct;
    		}
		else {
			fNucleus_Beam.SetMatrixElement(matrixElements_Beam.at(i).GetLambda(),
                                     matrixElements_Beam.at(i).GetInitialState(),
                                     matrixElements_Beam.at(i).GetFinalState(),
                                     matrixElements_Beam.at(i).GetMatrixElement());
			std::cout	<< std::setw(10) << std::left << matrixElements_Beam.at(i).GetInitialState()
					<< std::setw(10) << std::left << matrixElements_Beam.at(i).GetFinalState()
					<< std::setw(15) << std::left << fNucleus_Beam.GetMatrixElements().at(matrixElements_Beam.at(i).GetLambda())[matrixElements_Beam.at(i).GetInitialState()][matrixElements_Beam.at(i).GetFinalState()] << std::endl;
		}
	}
	std::cout	<< "Relative Beam matrix elements:"
            		<< std::endl;
	for(size_t i=0;i<relativeElements_Beam.size();i++){
		std::cout	<< std::setw(10) << std::left << relativeElements_Beam.at(i).GetInitialState()
				<< std::setw(10) << std::left << relativeElements_Beam.at(i).GetFinalState();
		if (!relativeElements_Beam.at(i).GetFixed()) {
			relativeElements_Beam.at(i).SetRelativeElement(res[parct]);
			fNucleus_Beam.SetMatrixElement(relativeElements_Beam.at(i).GetLambda(),
							relativeElements_Beam.at(i).GetInitialState(),
							relativeElements_Beam.at(i).GetFinalState(),
							res[parct]*fNucleus_Beam.GetMatrixElements().at(relativeElements_Beam.at(i).GetLambdaRel())[relativeElements_Beam.at(i).GetInitialStateRel()][relativeElements_Beam.at(i).GetFinalStateRel()]);
			std::cout 	<< std::setw(15) << std::left << res[parct]*fNucleus_Beam.GetMatrixElements().at(relativeElements_Beam.at(i).GetLambdaRel())[relativeElements_Beam.at(i).GetInitialStateRel()][relativeElements_Beam.at(i).GetFinalStateRel()]
					<< std::setw(15) << std::left << res[parct]
					<< std::setw(15) << std::left << unc[parct]
					<< std::endl;
			++parct;
		}
		else {
			fNucleus_Beam.SetMatrixElement(relativeElements_Beam.at(i).GetLambda(),
							relativeElements_Beam.at(i).GetInitialState(),
							relativeElements_Beam.at(i).GetFinalState(),
							relativeElements_Beam.at(i).GetRelativeElement()*fNucleus_Beam.GetMatrixElements().at(relativeElements_Beam.at(i).GetLambdaRel())[relativeElements_Beam.at(i).GetInitialStateRel()][relativeElements_Beam.at(i).GetFinalStateRel()]);
			std::cout 	<< std::setw(15) << std::left << relativeElements_Beam.at(i).GetRelativeElement()*fNucleus_Beam.GetMatrixElements().at(relativeElements_Beam.at(i).GetLambdaRel())[relativeElements_Beam.at(i).GetInitialStateRel()][relativeElements_Beam.at(i).GetFinalStateRel()]
					<< std::setw(15) << std::left << relativeElements_Beam.at(i).GetRelativeElement()
					<< std::setw(15) << std::left << 0.0
					<< std::endl;

		}
	} 
	if(exptData_Target.size()>0){ 
		resultFile	<< "Target matrix elements:"
				<< std::endl;
		std::cout	<< "Target matrix elements:"
				<< std::endl;
	}
	for(size_t i=0;i<matrixElements_Target.size();i++){
    		if (!matrixElements_Target.at(i).GetFixed()) {
      			matrixElements_Target.at(i).SetMatrixElement(res[parct]);
      			fNucleus_Target.SetMatrixElement(matrixElements_Target.at(i).GetLambda(),
                        	matrixElements_Target.at(i).GetInitialState(),
				matrixElements_Target.at(i).GetFinalState(),
				res[parct]);
			std::cout	<< std::setw(10) << std::left << matrixElements_Target.at(i).GetInitialState()
					<< std::setw(10) << std::left << matrixElements_Target.at(i).GetFinalState()
					//<< std::setw(15) << std::left << fNucleus_Target.GetMatrixElements().at(matrixElements_Target.at(i).GetLambda())[matrixElements_Target.at(i).GetInitialState()][matrixElements_Target.at(i).GetFinalState()]
					<< std::setw(15) << std::left << res[parct] 
					<< std::setw(15) << std::left << unc[parct]
			                << std::endl;
			resultFile	<< std::setw(10) << std::left << matrixElements_Target.at(i).GetInitialState()
					<< std::setw(10) << std::left << matrixElements_Target.at(i).GetFinalState()
					//<< std::setw(15) << std::left << fNucleus_Target.GetMatrixElements().at(matrixElements_Target.at(i).GetLambda())[matrixElements_Target.at(i).GetInitialState()][matrixElements_Target.at(i).GetFinalState()]
					<< std::setw(15) << std::left << res[parct] 
					<< std::setw(15) << std::left << unc[parct]
			                << std::endl;
			++parct;
		}
		else {
			fNucleus_Target.SetMatrixElement(matrixElements_Target.at(i).GetLambda(),
							matrixElements_Target.at(i).GetInitialState(),
							matrixElements_Target.at(i).GetFinalState(),
							matrixElements_Target.at(i).GetMatrixElement());
			std::cout	<< std::setw(10) << std::left << matrixElements_Target.at(i).GetInitialState()
					<< std::setw(10) << std::left << matrixElements_Target.at(i).GetFinalState()
					<< std::setw(15) << std::left << fNucleus_Target.GetMatrixElements().at(matrixElements_Target.at(i).GetLambda())[matrixElements_Target.at(i).GetInitialState()][matrixElements_Target.at(i).GetFinalState()] << std::endl;
		}
	}
	std::cout	<< "Relative Target matrix elements:"
			<< std::endl;
	for(size_t i=0;i<relativeElements_Target.size();i++){
		std::cout	<< std::setw(10) << std::left << relativeElements_Target.at(i).GetInitialState()
				<< std::setw(10) << std::left << relativeElements_Target.at(i).GetFinalState();
		if (!relativeElements_Target.at(i).GetFixed()) {
			relativeElements_Target.at(i).SetRelativeElement(res[parct]);
			fNucleus_Target.SetMatrixElement(relativeElements_Target.at(i).GetLambda(),
							relativeElements_Target.at(i).GetInitialState(),
							relativeElements_Target.at(i).GetFinalState(),
							res[parct]*fNucleus_Target.GetMatrixElements().at(relativeElements_Target.at(i).GetLambdaRel())[relativeElements_Target.at(i).GetInitialStateRel()][relativeElements_Target.at(i).GetFinalStateRel()]);
			std::cout 	<< std::setw(15) << std::left << res[parct]*fNucleus_Target.GetMatrixElements().at(relativeElements_Target.at(i).GetLambdaRel())[relativeElements_Target.at(i).GetInitialStateRel()][relativeElements_Target.at(i).GetFinalStateRel()]
					<< std::setw(15) << std::left << res[parct]
					<< std::setw(15) << std::left << unc[parct]
					<< std::endl;
			++parct;
		}
		else {
			fNucleus_Target.SetMatrixElement(relativeElements_Target.at(i).GetLambda(),
							relativeElements_Target.at(i).GetInitialState(),
							relativeElements_Target.at(i).GetFinalState(),
							relativeElements_Target.at(i).GetRelativeElement()*fNucleus_Target.GetMatrixElements().at(relativeElements_Target.at(i).GetLambdaRel())[relativeElements_Target.at(i).GetInitialStateRel()][relativeElements_Target.at(i).GetFinalStateRel()]);
			std::cout 	<< std::setw(15) << std::left << relativeElements_Target.at(i).GetRelativeElement()*fNucleus_Target.GetMatrixElements().at(relativeElements_Target.at(i).GetLambdaRel())[relativeElements_Target.at(i).GetInitialStateRel()][relativeElements_Target.at(i).GetFinalStateRel()]
					<< std::setw(15) << std::left << relativeElements_Target.at(i).GetRelativeElement()
					<< std::setw(15) << std::left << 0.0
					<< std::endl;

		}
	}   

	std::cout	<< std::endl;

	for(unsigned int i=0;i<parameters.size();i++)
		parameters[i] = res[i];

	covMat.ResizeTo(parameters.size(),parameters.size());
	corMat.ResizeTo(parameters.size(),parameters.size());
	for(unsigned int i=0;i<parameters.size();i++){
		for(unsigned int j=i;j<parameters.size();j++){
			covMat[i][j] = min->CovMatrix(i,j);
			covMat[j][i] = min->CovMatrix(j,i);
			corMat[i][j] = min->Correlation(i,j);
			corMat[j][i] = min->Correlation(j,i);
		}
	}

	if(DoFullUncertainty()){

		//min->SetTolerance(0.01);

		std::cout	<< "************************************** UNCERTAINTY EVAL. **************************************"
				<< std::endl;
		std::cout	<< "MINOS uncertainties (asymmetric):"
				<< std::endl;
		std::vector<double> errLowVec, errUpVec;
		for(unsigned int i=0;i<parameters.size();i++){
			double errLow, errUp;
			min->GetMinosError(i,errLow,errUp);
			errLowVec.push_back(errLow);
			errUpVec.push_back(errUp);
		}
		std::cout	<< "Correlated uncertainty calculation completed"
				<< std::endl;
		std::cout	<< std::setw(14) << std::left << "Parameter" 
				<< std::setw(14) << std::left << "Value" 
				<< std::setw(14) << "+" 
				<< std::setw(3) << "/"
				<< std::setw(14) << "-" 
				<< std::endl;
		for(unsigned int i=0;i<parameters.size();i++){
			std::cout	<< std::setw(14) << std::left << min->VariableName(i)
					<< std::setw(14) << std::left << parameters[i]
					<< std::setw(14) << errUpVec[i] 
					<< std::setw(3) << ""
					<< std::setw(14) << errLowVec[i]
					<< std::endl;
		}

		for(unsigned int i=0;i<parameters.size();i++){
			for(unsigned int j=i;j<parameters.size();j++){
				covMat[i][j] = min->CovMatrix(i,j);
				covMat[j][i] = min->CovMatrix(j,i);
				corMat[i][j] = min->Correlation(i,j);
				corMat[j][i] = min->Correlation(j,i);
			}
		}

	}

	std::cout	<< "\n"
			<< "******** Covariance matrix **********\n"
			<< std::endl;

	covMat.Print();

	std::cout	<< "\n"
			<< "******** Correlation matrix **********\n"
			<< std::endl;

	corMat.Print();


	std::cout	<< "\n"
			<< "******** Final Chi-Squared ***********\n"
			<< std::endl;

	std::cout	<< chisq
			<< std::endl;

	std::cout	<< "********     Complete      ***********\n"
			<< std::endl;

	resultFile	<< "\n"
			<< "******** Covariance matrix **********\n"
			<< std::endl;

	for(int i=0;i<covMat.GetNcols();i++){
		for(int j=0;j<covMat.GetNrows();j++){
			resultFile	<< std::setw(14) << std::left << covMat[i][j];
		}
		resultFile	<< std::endl;
	}

	resultFile	<< "\n"
			<< "******** Correlation matrix **********\n"
			<< std::endl;


	for(int i=0;i<corMat.GetNcols();i++){
		for(int j=0;j<corMat.GetNrows();j++){
			resultFile	<< std::setw(14) << std::left << corMat[i][j];
		}
		resultFile	<< std::endl;
	}


	resultFile	<< "\n"
			<< "******** Final Chi-Squared ***********\n"
			<< std::endl;

	resultFile	<< chisq
			<< std::endl;

	resultFile	<< "********     Complete      ***********\n"
			<< std::endl;

	resultFile.close();
}

void GOSIASimFitter::CreateScalingParameter(std::vector<int> expnum){

	std::vector<float>	expNRM(expnum.size(),1);

	ScalingParameter tmpScaling;
	tmpScaling.SetExperimentVector(expnum,expNRM);

	scalingParameters.push_back(tmpScaling);

}

void GOSIASimFitter::CreateScalingParameter(std::vector<int> expnum, std::vector<float> expNRM){

	ScalingParameter tmpScaling;
	tmpScaling.SetExperimentVector(expnum,expNRM);

	scalingParameters.push_back(tmpScaling);

}

void GOSIASimFitter::AddBeamFittingMatrixElement(int lambda, int init, int fin, double ME, double LL, double UL){
	MatrixElement tmpME(matrixElements_Beam.size(),lambda,init,fin,ME,LL,UL);
	matrixElements_Beam.push_back(tmpME);
}

void GOSIASimFitter::SetBeamFittingMatrixElement(int lambda, int init, int fin, double ME, double LL, double UL){
  	for (int j=0; j<matrixElements_Beam.size(); ++j) {
    		if (matrixElements_Beam[j].GetLambda() == lambda &&
        	matrixElements_Beam[j].GetInitialState() == init &&
        	matrixElements_Beam[j].GetFinalState() == fin) {
      			matrixElements_Beam[j].SetMatrixElement(ME);
      			matrixElements_Beam[j].SetMatrixElementLowerLimit(LL);
      			matrixElements_Beam[j].SetMatrixElementLowerLimit(UL);
    		}
  	}
}

void GOSIASimFitter::AddBeamRelativeMatrixElement(int lambda, int init,int fin,int lambda2,int init2,int fin2 ,double ME, double ME_LL, double ME_UL, bool fixed, bool relative) {
	double rel = 1.0;
  	double rel_ll = 1.0; 
  	double rel_ul = 1.0;
  	if (relative == true ) {
    		rel = ME;
    		rel_ll = ME_LL;
    		rel_ul = ME_UL;
  	}
  	else {
    		rel = ME/fNucleus_Beam.GetMatrixElements().at(lambda2 )[init2][fin2];
    		rel_ll = ME_LL/fNucleus_Beam.GetMatrixElements().at(lambda2)[init2][fin2];
    		rel_ul = ME_UL/fNucleus_Beam.GetMatrixElements().at(lambda2)[init2][fin2];
	}    
  	RelativeMatrixElement tmpME(relativeElements_Beam.size(),lambda,init,fin,lambda2,init2,fin2,rel,rel_ll,rel_ul,fixed);
  	relativeElements_Beam.push_back(tmpME);
}

void GOSIASimFitter::AddTargetFittingMatrixElement(int lambda, int init, int fin, double ME, double LL, double UL){
	MatrixElement tmpME(matrixElements_Target.size(),lambda,init,fin,ME,LL,UL);
	matrixElements_Target.push_back(tmpME);
}

void GOSIASimFitter::AddTargetRelativeMatrixElement(int lambda, int init,int fin,int lambda2,int init2,int fin2 ,double ME, double ME_LL, double ME_UL, bool fixed, bool relative) {
  	double rel = 1.0;
  	double rel_ll = 1.0; 
  	double rel_ul = 1.0;
  	if (relative == true ) {
    		rel = ME;
    		rel_ll = ME_LL;
    		rel_ul = ME_UL;
  	}
  	else {
    		rel = ME/fNucleus_Target.GetMatrixElements().at(lambda2)[init2][fin2];
    		rel_ll = ME_LL/fNucleus_Target.GetMatrixElements().at(lambda2)[init2][fin2];
    		rel_ul = ME_UL/fNucleus_Target.GetMatrixElements().at(lambda2)[init2][fin2];
  	}    
  	RelativeMatrixElement tmpME(relativeElements_Target.size(),lambda,init,fin,lambda2,init2,fin2,rel,rel_ll,rel_ul,fixed);
  	relativeElements_Target.push_back(tmpME);
}


//void GOSIASimFitter::AddBeamCorrectionFactor(TVectorD corrFac){
//	correctionFactors_Beam.push_back(corrFac);
//}
//void GOSIASimFitter::SetBeamCorrectionFactor(int i, TVectorD corrFac){
//	if((i < (int)correctionFactors_Beam.size()))
//		correctionFactors_Beam.at(i) = corrFac;
//	else
//		std::cout << "Outside vector range" << std::endl;
//}
//void GOSIASimFitter::AddTargetCorrectionFactor(TVectorD corrFac){
//	correctionFactors_Target.push_back(corrFac);
//}
//void GOSIASimFitter::SetTargetCorrectionFactor(int i, TVectorD corrFac){
//	if((i < (int)correctionFactors_Target.size()))
//		correctionFactors_Target.at(i) = corrFac;
//	else
//		std::cout << "Outside vector range" << std::endl;
//}

void GOSIASimFitter::AddWeightingFactor(float f){
	expt_weights.push_back(f);
}
void GOSIASimFitter::SetWeightingFactor(int i, float f){
	if(i < (int)expt_weights.size())
		expt_weights.at(i) = f;
	else
		std::cout << "Outside vector range" << std::endl;
}

void GOSIASimFitter::AddBeamCorrectionFactor(TMatrixD corrFac){
	correctionFactors_Beam.push_back(corrFac);
}
void GOSIASimFitter::SetBeamCorrectionFactor(int i, TMatrixD corrFac){
	if((i < (int)correctionFactors_Beam.size()))
		correctionFactors_Beam.at(i) = corrFac;
	else
		std::cout << "Outside vector range" << std::endl;
}
void GOSIASimFitter::AddTargetCorrectionFactor(TMatrixD corrFac){
	correctionFactors_Target.push_back(corrFac);
}
void GOSIASimFitter::SetTargetCorrectionFactor(int i, TMatrixD corrFac){
	if((i < (int)correctionFactors_Target.size()))
		correctionFactors_Target.at(i) = corrFac;
	else
		std::cout << "Outside vector range" << std::endl;
}

void GOSIASimFitter::DefineExperiment(double thetacm){
	ExperimentData tmpExp;
	tmpExp.SetThetaCM(thetacm);
	exptData_Beam.push_back(tmpExp);		
	exptData_Target.push_back(tmpExp);		
}
void GOSIASimFitter::AddBeamData(int nExpt, int init, int fin, double counts, double unc){
	exptData_Beam.at(nExpt).AddData(init,fin,counts,unc);
} 
void GOSIASimFitter::AddTargetData(int nExpt, int init, int fin, double counts, double unc){
	exptData_Target.at(nExpt).AddData(init,fin,counts,unc);
}   

void GOSIASimFitter::AddBeamLifetime(int index, double lifetime, double unc){
	LitLifetime tmpLifetime(index,lifetime,unc);
	litLifetimes_Beam.push_back(tmpLifetime);  
}

void GOSIASimFitter::AddBeamHalfLife(int index, double halflife, double unc){
	LitLifetime tmpLifetime(index,halflife/TMath::Log(2),unc/TMath::Log(2));
	litLifetimes_Beam.push_back(tmpLifetime);  
}
void GOSIASimFitter::AddBeamBranchingRatio(int index_I1, int index_F1, int index_F2, double br, double unc){
	LitBranchingRatio tmpBR(index_I1,index_F1,index_F2,br,unc);
	litBranchingRatios_Beam.push_back(tmpBR);	
}  
void GOSIASimFitter::AddBeamMixingRatio(int index_I, int index_F, double delta, double unc){
	LitMixingRatio tmpMR(index_I,index_F,delta,unc);
	litMixingRatios_Beam.push_back(tmpMR);	
}
void GOSIASimFitter::AddBeamMatrixElement(int mult, int index_I, int index_F, double me, double unc){
	LitMatrixElement tmpME(mult,index_I,index_F,me,unc);
	litMatrixElements_Beam.push_back(tmpME);	
}
void GOSIASimFitter::AddTargetLifetime(int index, double lifetime, double unc){
	LitLifetime tmpLifetime(index,lifetime,unc);
	litLifetimes_Target.push_back(tmpLifetime);	
}
void GOSIASimFitter::AddTargetHalfLife(int index, double halflife, double unc){
	LitLifetime tmpLifetime(index,halflife/TMath::Log(2),unc/TMath::Log(2));
	litLifetimes_Target.push_back(tmpLifetime);  
}
void GOSIASimFitter::AddTargetBranchingRatio(int index_I1, int index_F1, int index_F2, double br, double unc){
	LitBranchingRatio tmpBR(index_I1,index_F1,index_F2,br,unc);
	litBranchingRatios_Target.push_back(tmpBR);	
}  
void GOSIASimFitter::AddTargetMixingRatio(int index_I, int index_F, double delta, double unc){
	LitMixingRatio tmpMR(index_I,index_F,delta,unc);
	litMixingRatios_Target.push_back(tmpMR);	
}    
void GOSIASimFitter::AddTargetMatrixElement(int mult, int index_I, int index_F, double me, double unc){
	LitMatrixElement tmpME(mult, index_I,index_F,me,unc);
	litMatrixElements_Target.push_back(tmpME);	
}    

void GOSIASimFitter::ClearAll(){

	index.clear();
	parameters.clear();
			
	matrixElements_Beam.clear();		
	exptData_Beam.clear();			
	litLifetimes_Beam.clear();			
	litBranchingRatios_Beam.clear();		
	litMixingRatios_Beam.clear();		
	litMatrixElements_Beam.clear();		
	EffectiveCrossSection_Beam.clear();
			
	matrixElements_Target.clear();		
	exptData_Target.clear();			
	litLifetimes_Target.clear();			
	litBranchingRatios_Target.clear();		
	litMixingRatios_Target.clear();		
	litMatrixElements_Target.clear();		
	EffectiveCrossSection_Target.clear();

}

void GOSIASimFitter::Print() const{

	if(exptData_Beam.size()>0){
		std::cout 	<< "\n\n"
				<< "Experimental data (beam):" << std::endl;
		
		std::cout 	<< exptData_Beam.size() << " experiments" << std::endl;
		
		for(unsigned int i=0;i<exptData_Beam.size();i++){
			std::cout	<< "Experiment " << i+1 << std::endl;
			std::cout	<< "Theta [CM]: " << exptData_Beam.at(i).GetThetaCM() << std::endl;
			std::cout 	<< std::setw(15) << std::left << "Init. index:" 
					<< std::setw(15) << std::left << "Final index:"
					<< std::setw(15) << std::left << "Init. J:" 
					<< std::setw(15) << std::left << "Final J:"
					<< std::setw(10) << std::left << "Counts:"
					<< std::setw(10) << std::left << "Unc:"
					<< std::endl;
			for(unsigned int t=0;t<exptData_Beam.at(i).GetData().size();t++){
				std::cout 	<< std::setw(15) << std::left << exptData_Beam.at(i).GetDataPoint(t).GetInitialIndex()
						<< std::setw(15) << std::left << exptData_Beam.at(i).GetDataPoint(t).GetFinalIndex()
						<< std::setw(15) << std::left << fNucleus_Beam.GetLevelJ().at(exptData_Beam.at(i).GetDataPoint(t).GetInitialIndex())
						<< std::setw(15) << std::left << fNucleus_Beam.GetLevelJ().at(exptData_Beam.at(i).GetDataPoint(t).GetFinalIndex())
						<< std::setw(10) << std::left << exptData_Beam.at(i).GetDataPoint(t).GetCounts()
						<< std::setw(10) << std::left << exptData_Beam.at(i).GetDataPoint(t).GetUpUnc()
						<< std::endl;
			}			
		}
	}
	else	
		std::cout << "No experimental data declared" << std::endl;

	if(litLifetimes_Beam.size()>0){
		std::cout	<< "\n\n"
				<< "Literature lifetimes (beam):"
				<< std::endl;
		std::cout	<< std::setw(8)  << std::left << "Index"
				<< std::setw(6)  << std::left << "J:"
				<< std::setw(15) << std::left << "Lifetime (ps)" 
				<< std::setw(15) << std::left << "Uncertainty:"
				<< std::endl;
		for(unsigned int i=0;i<litLifetimes_Beam.size();i++){
			std::cout 	<< std::setw(8)  << std::left << litLifetimes_Beam.at(i).GetIndex()
					<< std::setw(6)  << std::left << fNucleus_Beam.GetLevelJ().at(litLifetimes_Beam.at(i).GetIndex())
					<< std::setw(15) << std::left << litLifetimes_Beam.at(i).GetLifetime()
					<< std::setw(15) << std::left << litLifetimes_Beam.at(i).GetUpUnc()
					<< std::endl;
		}
	}

	if(litBranchingRatios_Beam.size()>0){
		std::cout	<< "\n\n"
				<< "Literature Branching Ratios (beam):"
				<< std::endl;
		std::cout	<< std::setw(15) << std::left << "Init. Index"
				<< std::setw(15) << std::left << "Final Index 1"
				<< std::setw(15) << std::left << "Final Index 2"
				<< std::setw(10) << std::left << "J init:"
				<< std::setw(12) << std::left << "J final 1:"
				<< std::setw(12) << std::left << "J final 2:"
				<< std::setw(17) << std::left << "Branching Ratio" 
				<< std::setw(15) << std::left << "Uncertainty:"
				<< std::endl;
		for(unsigned int i=0;i<litBranchingRatios_Beam.size();i++){
			std::cout 	<< std::setw(15) << std::left << litBranchingRatios_Beam.at(i).GetInitialIndex()
					<< std::setw(15) << std::left << litBranchingRatios_Beam.at(i).GetFinalIndex_1()
					<< std::setw(15) << std::left << litBranchingRatios_Beam.at(i).GetFinalIndex_2()
					<< std::setw(10) << std::left << fNucleus_Beam.GetLevelJ().at(litBranchingRatios_Beam.at(i).GetInitialIndex())
					<< std::setw(12) << std::left << fNucleus_Beam.GetLevelJ().at(litBranchingRatios_Beam.at(i).GetFinalIndex_1())
					<< std::setw(12) << std::left << fNucleus_Beam.GetLevelJ().at(litBranchingRatios_Beam.at(i).GetFinalIndex_2())
					<< std::setw(17) << std::left << litBranchingRatios_Beam.at(i).GetBranchingRatio()
					<< std::setw(15) << std::left << litBranchingRatios_Beam.at(i).GetUpUnc()
					<< std::endl;
		}
	}
	
	if(litMixingRatios_Beam.size()>0){
		std::cout	<< "\n\n"
				<< "Literature Mixing Ratios (beam):"
				<< std::endl;
		std::cout	<< std::setw(15) << std::left << "Init. Index"
				<< std::setw(15) << std::left << "Final Index"
				<< std::setw(10) << std::left << "J init:"
				<< std::setw(10) << std::left << "J final:"
				<< std::setw(14) << std::left << "Mixing Ratio" 
				<< std::setw(15) << std::left << "Uncertainty:"
				<< std::endl;
		for(unsigned int i=0;i<litMixingRatios_Beam.size();i++){
			std::cout 	<< std::setw(15) << std::left << litMixingRatios_Beam.at(i).GetInitialIndex()
					<< std::setw(15) << std::left << litMixingRatios_Beam.at(i).GetFinalIndex()
					<< std::setw(10) << std::left << fNucleus_Beam.GetLevelJ().at(litMixingRatios_Beam.at(i).GetInitialIndex())
					<< std::setw(10) << std::left << fNucleus_Beam.GetLevelJ().at(litMixingRatios_Beam.at(i).GetFinalIndex())
					<< std::setw(14) << std::left << litMixingRatios_Beam.at(i).GetMixingRatio()
					<< std::setw(15) << std::left << litMixingRatios_Beam.at(i).GetUpUnc()
					<< std::endl;
		}
	}

	std::string	mult[8] = {"E1","E2","E3","E4","E5","E6","M1","M2"};

	std::cout 	<< "\n\n"
			<< "Starting matrix elements (beam):"
			<< std::endl;
	for(unsigned int l = 0; l < fNucleus_Beam.GetMatrixElements().size(); l++){
		if(MiscFunctions::GetMaxMatrix(fNucleus_Beam.GetMatrixElements().at(l)) > 0){
			std::cout	<< mult[l] << " matrix elements"
					<< std::endl;	
			TMatrixD	tmpMat;
			tmpMat.ResizeTo(fNucleus_Beam.GetMatrixElements().at(l).GetNrows(),fNucleus_Beam.GetMatrixElements().at(l).GetNcols());
			tmpMat =	fNucleus_Beam.GetMatrixElements().at(l);
			for(unsigned int s = 0; s < matrixElements_Beam.size(); s++){
				if(matrixElements_Beam.at(s).GetLambda() == (int)l){
					tmpMat[matrixElements_Beam.at(s).GetInitialState()][matrixElements_Beam.at(s).GetFinalState()] = matrixElements_Beam.at(s).GetMatrixElement();
					tmpMat[matrixElements_Beam.at(s).GetFinalState()][matrixElements_Beam.at(s).GetInitialState()] = matrixElements_Beam.at(s).GetMatrixElement();
				}
			}
			MiscFunctions::PrintMatrixNucleus(tmpMat,fNucleus_Beam);	

		}
	}	

	if(exptData_Target.size()>0){
		std::cout 	<< "\n\n"
				<< "Experimental data (target):" << std::endl;
		
		std::cout 	<< exptData_Target.size() << " experiments" << std::endl;
		
		for(unsigned int i=0;i<exptData_Target.size();i++){
			std::cout	<< "Experiment " << i+1 << std::endl;
			std::cout	<< "Theta [CM]: " << exptData_Target.at(i).GetThetaCM() << std::endl;
			std::cout 	<< std::setw(15) << std::left << "Init. index:" 
					<< std::setw(15) << std::left << "Final index:"
					<< std::setw(15) << std::left << "Init. J:" 
					<< std::setw(15) << std::left << "Final J:"
					<< std::setw(10) << std::left << "Counts:"
					<< std::setw(10) << std::left << "Unc:"
					<< std::endl;
			for(unsigned int t=0;t<exptData_Target.at(i).GetData().size();t++){
				std::cout 	<< std::setw(15) << std::left << exptData_Target.at(i).GetDataPoint(t).GetInitialIndex()
						<< std::setw(15) << std::left << exptData_Target.at(i).GetDataPoint(t).GetFinalIndex()
						<< std::setw(15) << std::left << fNucleus_Target.GetLevelJ().at(exptData_Target.at(i).GetDataPoint(t).GetInitialIndex())
						<< std::setw(15) << std::left << fNucleus_Target.GetLevelJ().at(exptData_Target.at(i).GetDataPoint(t).GetFinalIndex())
						<< std::setw(10) << std::left << exptData_Target.at(i).GetDataPoint(t).GetCounts()
						<< std::setw(10) << std::left << exptData_Target.at(i).GetDataPoint(t).GetUpUnc()
						<< std::endl;
			}			
		}
	}
	else	
		std::cout << "No experimental data declared" << std::endl;

	if(litLifetimes_Target.size()>0){
		std::cout	<< "\n\n"
				<< "Literature lifetimes (target):"
				<< std::endl;
		std::cout	<< std::setw(8)  << std::left << "Index"
				<< std::setw(6)  << std::left << "J:"
				<< std::setw(15) << std::left << "Lifetime (ps)" 
				<< std::setw(15) << std::left << "Uncertainty:"
				<< std::endl;
		for(unsigned int i=0;i<litLifetimes_Target.size();i++){
			std::cout 	<< std::setw(8)  << std::left << litLifetimes_Target.at(i).GetIndex()
					<< std::setw(6)  << std::left << fNucleus_Target.GetLevelJ().at(litLifetimes_Target.at(i).GetIndex())
					<< std::setw(15) << std::left << litLifetimes_Target.at(i).GetLifetime()
					<< std::setw(15) << std::left << litLifetimes_Target.at(i).GetUpUnc()
					<< std::endl;
		}
	}

	if(litBranchingRatios_Target.size()>0){
		std::cout	<< "\n\n"
				<< "Literature Branching Ratios (target):"
				<< std::endl;
		std::cout	<< std::setw(15) << std::left << "Init. Index"
				<< std::setw(15) << std::left << "Final Index 1"
				<< std::setw(15) << std::left << "Final Index 2"
				<< std::setw(10) << std::left << "J init:"
				<< std::setw(12) << std::left << "J final 1:"
				<< std::setw(12) << std::left << "J final 2:"
				<< std::setw(17) << std::left << "Branching Ratio" 
				<< std::setw(15) << std::left << "Uncertainty:"
				<< std::endl;
		for(unsigned int i=0;i<litBranchingRatios_Target.size();i++){
			std::cout 	<< std::setw(15) << std::left << litBranchingRatios_Target.at(i).GetInitialIndex()
					<< std::setw(15) << std::left << litBranchingRatios_Target.at(i).GetFinalIndex_1()
					<< std::setw(15) << std::left << litBranchingRatios_Target.at(i).GetFinalIndex_2()
					<< std::setw(10) << std::left << fNucleus_Target.GetLevelJ().at(litBranchingRatios_Target.at(i).GetInitialIndex())
					<< std::setw(12) << std::left << fNucleus_Target.GetLevelJ().at(litBranchingRatios_Target.at(i).GetFinalIndex_1())
					<< std::setw(12) << std::left << fNucleus_Target.GetLevelJ().at(litBranchingRatios_Target.at(i).GetFinalIndex_2())
					<< std::setw(17) << std::left << litBranchingRatios_Target.at(i).GetBranchingRatio()
					<< std::setw(15) << std::left << litBranchingRatios_Target.at(i).GetUpUnc()
					<< std::endl;
		}
	}
	
	if(litMixingRatios_Target.size()>0){
		std::cout	<< "\n\n"
				<< "Literature Mixing Ratios (target):"
				<< std::endl;
		std::cout	<< std::setw(15) << std::left << "Init. Index"
				<< std::setw(15) << std::left << "Final Index"
				<< std::setw(10) << std::left << "J init:"
				<< std::setw(10) << std::left << "J final:"
				<< std::setw(14) << std::left << "Mixing Ratio" 
				<< std::setw(15) << std::left << "Uncertainty:"
				<< std::endl;
		for(unsigned int i=0;i<litMixingRatios_Target.size();i++){
			std::cout 	<< std::setw(15) << std::left << litMixingRatios_Target.at(i).GetInitialIndex()
					<< std::setw(15) << std::left << litMixingRatios_Target.at(i).GetFinalIndex()
					<< std::setw(10) << std::left << fNucleus_Target.GetLevelJ().at(litMixingRatios_Target.at(i).GetInitialIndex())
					<< std::setw(10) << std::left << fNucleus_Target.GetLevelJ().at(litMixingRatios_Target.at(i).GetFinalIndex())
					<< std::setw(14) << std::left << litMixingRatios_Target.at(i).GetMixingRatio()
					<< std::setw(15) << std::left << litMixingRatios_Target.at(i).GetUpUnc()
					<< std::endl;
		}
	}

	std::cout 	<< "\n\n"
			<< "Starting matrix elements (target):"
			<< std::endl;
	for(unsigned int l = 0; l < fNucleus_Target.GetMatrixElements().size(); l++){
		if(MiscFunctions::GetMaxMatrix(fNucleus_Target.GetMatrixElements().at(l)) > 0){
			std::cout	<< mult[l] << " matrix elements"
					<< std::endl;	
			TMatrixD	tmpMat;
			tmpMat.ResizeTo(fNucleus_Target.GetMatrixElements().at(l).GetNrows(),fNucleus_Target.GetMatrixElements().at(l).GetNcols());
			tmpMat =	fNucleus_Target.GetMatrixElements().at(l);
			for(unsigned int s = 0; s < matrixElements_Target.size(); s++){
				if(matrixElements_Target.at(s).GetLambda() == (int)l){
					tmpMat[matrixElements_Target.at(s).GetInitialState()][matrixElements_Target.at(s).GetFinalState()] = matrixElements_Target.at(s).GetMatrixElement();
					tmpMat[matrixElements_Target.at(s).GetFinalState()][matrixElements_Target.at(s).GetInitialState()] = matrixElements_Target.at(s).GetMatrixElement();
				}
			}
			MiscFunctions::PrintMatrixNucleus(tmpMat,fNucleus_Target);	

		}
	}

}

void GOSIASimFitter::WriteYieldGraphs(TFile *file, std::vector<double> angles, std::vector<double> norms) {
  	file->cd();
  	TransitionRates rates_b(&fNucleus_Beam);

	const char	*b_out = beamGOSIAFile_out.c_str();
	GOSIAReader	beam_gosiaReader(&fNucleus_Beam,b_out);	//	Grab the GOSIA yields
  
	std::vector<ExperimentData>	beamCalc	= beam_gosiaReader.GetGOSIAData();
	EffectiveCrossSection_Beam.clear();	

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

  	int			counter = 0;


  	std::map<std::pair<int, int>, TGraph* > calcGraphs;
  	std::map<std::pair<int, int>, TGraphErrors* > expGraphs;
  	std::map<std::pair<int, int>, TMultiGraph* > graphs;

  	std::map<std::pair<int, int>, TGraph* > normCalcGraphs;
  	std::map<std::pair<int, int>, TGraphErrors* > normExpGraphs;
  	std::map<std::pair<int, int>, TMultiGraph* > normGraphs;
  
  	std::map<std::pair<int, int>, int > write;

	for(unsigned int i=0;i<exptData_Beam.size();i++){
    
		double	exptchisq	= 0;
		if(expt_weights.at(i) == 0) 
			continue;
    
		for(unsigned int t=0;t<exptData_Beam.at(i).GetData().size();++t){
			double 	tmp 		= 0;
			int	index_init 	= exptData_Beam.at(i).GetData().at(t).GetInitialIndex();
			int	index_final 	= exptData_Beam.at(i).GetData().at(t).GetFinalIndex();

      	TGraph *calcGraph;
      	TGraphErrors *expGraph;
      	TMultiGraph *graph;
      
      	TGraph *normCalcGraph;
      	TGraphErrors *normExpGraph;
      	TMultiGraph *normGraph;
      
      	if (calcGraphs.find({index_init, index_final}) == calcGraphs.end()) {
        	calcGraph = new TGraph();
        	expGraph = new TGraphErrors();
        	graph = new TMultiGraph();

		normCalcGraph = new TGraph();
		normExpGraph = new TGraphErrors();
		normGraph = new TMultiGraph();
		
		calcGraphs[{index_init, index_final}] = calcGraph;
		expGraphs[{index_init, index_final}] = expGraph;
		graphs[{index_init, index_final}] = graph;

		normCalcGraphs[{index_init, index_final}] = normCalcGraph;
		normExpGraphs[{index_init, index_final}] = normExpGraph;
		normGraphs[{index_init, index_final}] = normGraph;

		TString calcname;
		calcname.Form("calcYields_%i_%i", index_init, index_final);
		TString expname;
		expname.Form("expYields_%i_%i", index_init, index_final);

		TString normcalcname;
		normcalcname.Form("calcYieldsNrm_%i_%i", index_init, index_final);
		TString normexpname;
		normexpname.Form("expYieldsNrm_%i_%i", index_init, index_final);
		
		calcGraph->SetName(calcname.Data());
		calcGraph->SetLineWidth(2);
		calcGraph->SetLineColor(kRed);

		expGraph->SetMarkerStyle(kFullCircle);
		expGraph->SetName(expname.Data());

		normCalcGraph->SetName(normcalcname.Data());
		normCalcGraph->SetLineWidth(2);
		normCalcGraph->SetLineColor(kRed);

		normExpGraph->SetMarkerStyle(kFullCircle);
		normExpGraph->SetName(normexpname.Data());
		
		TString name;
		name.Form("yields_%i_%i", index_init, index_final);
		graph->SetName(name.Data());
		graph->Add(calcGraph, "L");
		graph->Add(expGraph, "P");

		TString normname;
		normname.Form("yieldsNrm_%i_%i", index_init, index_final);
		normGraph->SetName(normname.Data());
		normGraph->Add(normCalcGraph, "L");
		normGraph->Add(normExpGraph, "P");        
      	}
      	else {
		calcGraph =         calcGraphs[{index_init, index_final}];
		expGraph =           expGraphs[{index_init, index_final}];
		graph =           graphs[{index_init, index_final}];

		normCalcGraph =         normCalcGraphs[{index_init, index_final}];
		normExpGraph =           normExpGraphs[{index_init, index_final}];
		normGraph =           normGraphs[{index_init, index_final}];       
      	}
     
	double 	calcCounts 	= scaling.at(i) * EffectiveCrossSection_Beam.at(i)[index_final][index_init];
     double 	exptCounts 	= exptData_Beam.at(i).GetData().at(t).GetCounts();
     double	sigma		= exptData_Beam.at(i).GetData().at(t).GetUpUnc() * exptData_Beam.at(i).GetData().at(t).GetDnUnc();
     double	sigma_prime	= (exptData_Beam.at(i).GetData().at(t).GetUpUnc() - exptData_Beam.at(i).GetData().at(t).GetDnUnc());
     sigma			/= expt_weights.at(i);
      sigma_prime	/= expt_weights.at(i);

      calcGraph->AddPoint(i+1, calcCounts);
      expGraph->AddPoint(i+1, exptCounts);
      expGraph->SetPointError(expGraph->GetN()-1, 0, exptData_Beam.at(i).GetData().at(t).GetUpUnc());

      normCalcGraph->AddPoint(angles[i], calcCounts/norms[i]);
      normExpGraph->AddPoint(angles[i], exptCounts/norms[i]);
      normExpGraph->SetPointError(expGraph->GetN()-1, 0, exptData_Beam.at(i).GetData().at(t).GetUpUnc()/norms[i]);
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
		}
		counter++;
	}

  
  for(unsigned int i=0;i<exptData_Beam.size();i++){
    
		double	exptchisq	= 0;
		if(expt_weights.at(i) == 0) 
			continue;
    
		for(unsigned int t=0;t<exptData_Beam.at(i).GetData().size();++t){
			double 	tmp 		= 0;
			int	index_init 	= exptData_Beam.at(i).GetData().at(t).GetInitialIndex();
			int	index_final 	= exptData_Beam.at(i).GetData().at(t).GetFinalIndex();

      if (calcGraphs.find({index_init, index_final}) == calcGraphs.end()) {
      }
      else {
        if (write.find({index_init, index_final}) == write.end()) {
          calcGraphs[{index_init, index_final}]->Write();
          expGraphs[{index_init, index_final}]->Write();
          graphs[{index_init, index_final}]->Write();

          normCalcGraphs[{index_init, index_final}]->Write();
          normExpGraphs[{index_init, index_final}]->Write();
          normGraphs[{index_init, index_final}]->Write();
        }
        else {
          write[{index_init, index_final}] = 1;
        }
      }
    }
  }
  file->Write();  
}


void GOSIASimFitter::FixAllBeamMatrixElements() {
  for (int j=0; j<matrixElements_Beam.size(); ++j) {
    matrixElements_Beam[j].SetFixed(true);
  }

  for (int j=0; j<relativeElements_Beam.size(); ++j) {
    relativeElements_Beam[j].SetFixed(true);
  }
}

void GOSIASimFitter::UnFixAllBeamMatrixElements() {
  for (int j=0; j<matrixElements_Beam.size(); ++j) {
    matrixElements_Beam[j].SetFixed(false);
  }

  for (int j=0; j<relativeElements_Beam.size(); ++j) {
    relativeElements_Beam[j].SetFixed(false);
  }
}


void GOSIASimFitter::FixBeamMatrixElements(std::vector<std::vector<int> > indices) {
  for (int i = 0; i<indices.size(); ++i) {
    int lambda = indices[i][0];
    int init = indices[i][1];
    int fin = indices[i][2];

    for (int j=0; j<matrixElements_Beam.size(); ++j) {
      if (matrixElements_Beam[j].GetLambda() == lambda &&
          matrixElements_Beam[j].GetInitialState() == init &&
          matrixElements_Beam[j].GetFinalState() == fin) {
        matrixElements_Beam[j].SetFixed(true);
      }
    }

    for (int j=0; j<relativeElements_Beam.size(); ++j) {
      if (relativeElements_Beam[j].GetLambda() == lambda &&
          relativeElements_Beam[j].GetInitialState() == init &&
          relativeElements_Beam[j].GetFinalState() == fin) {
        relativeElements_Beam[j].SetFixed(true);
      }
    }
  }
}

void GOSIASimFitter::UnFixBeamMatrixElements(std::vector<std::vector<int> > indices) {
  for (int i = 0; i<indices.size(); ++i) {
    int lambda = indices[i][0];
    int init = indices[i][1];
    int fin = indices[i][2];

    for (int j=0; j<matrixElements_Beam.size(); ++j) {
      if (matrixElements_Beam[j].GetLambda() == lambda &&
          matrixElements_Beam[j].GetInitialState() == init &&
          matrixElements_Beam[j].GetFinalState() == fin) {
        matrixElements_Beam[j].SetFixed(false);
      }
    }

    for (int j=0; j<relativeElements_Beam.size(); ++j) {
      if (relativeElements_Beam[j].GetLambda() == lambda &&
          relativeElements_Beam[j].GetInitialState() == init &&
          relativeElements_Beam[j].GetFinalState() == fin) {
        relativeElements_Beam[j].SetFixed(false);
      }
    }
  }
}
