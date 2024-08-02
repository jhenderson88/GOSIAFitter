#include "ExperimentalInput.h"

ExptMultiplet::ExptMultiplet(const ExptMultiplet& e){

	StateIndex_I.clear();
	StateIndex_F.clear();
	for(size_t i=0;i<e.StateIndex_I.size();i++)
		StateIndex_I.push_back(e.StateIndex_I.at(i));
	for(size_t i=0;i<e.StateIndex_F.size();i++)
		StateIndex_F.push_back(e.StateIndex_F.at(i));
	Counts		= e.Counts;
	Uncertainty 	= e.Uncertainty;

}
ExptMultiplet& ExptMultiplet::operator = (const ExptMultiplet& e){

	StateIndex_I.clear();
	StateIndex_F.clear();
	for(size_t i=0;i<e.StateIndex_I.size();i++)
		StateIndex_I.push_back(e.StateIndex_I.at(i));
	for(size_t i=0;i<e.StateIndex_F.size();i++)
		StateIndex_F.push_back(e.StateIndex_F.at(i));
	Counts		= e.Counts;
	Uncertainty 	= e.Uncertainty;

	return *this;

}

ExptDoublet::ExptDoublet(const ExptDoublet& e){

 	StateIndex_I1	= e.StateIndex_I1;
  	StateIndex_F1	= e.StateIndex_F1;
 	StateIndex_I2	= e.StateIndex_I2;
  	StateIndex_F2	= e.StateIndex_F2;
	Counts		= e.Counts;
	UpUncertainty 	= e.UpUncertainty;
	DnUncertainty	= e.DnUncertainty;
	DetEff		= e.DetEff;	

}
ExptDoublet& ExptDoublet::operator = (const ExptDoublet& e){

 	StateIndex_I1	= e.StateIndex_I1;
  	StateIndex_F1	= e.StateIndex_F1;
 	StateIndex_I2	= e.StateIndex_I2;
  	StateIndex_F2	= e.StateIndex_F2;
	Counts		= e.Counts;
	UpUncertainty 	= e.UpUncertainty;
	DnUncertainty	= e.DnUncertainty;
	DetEff		= e.DetEff;

	return *this;

}

ExptData::ExptData(const ExptData& e){

 	StateIndex_I	= e.StateIndex_I;
  	StateIndex_F	= e.StateIndex_F;
	Counts		= e.Counts;
	UpUncertainty 	= e.UpUncertainty;
	DnUncertainty	= e.DnUncertainty;
	DetEff		= e.DetEff;	


}
ExptData& ExptData::operator = (const ExptData& e){

 	StateIndex_I	= e.StateIndex_I;
  	StateIndex_F	= e.StateIndex_F;
	Counts		= e.Counts;
	UpUncertainty 	= e.UpUncertainty;
	DnUncertainty	= e.DnUncertainty;
	DetEff		= e.DetEff;

	return *this;

}
ExperimentData::ExperimentData(const ExperimentData& e){

	Data.resize(e.Data.size());
	for(size_t d=0;d<e.Data.size();d++)
		Data.at(d) = e.Data.at(d);		
	Doublet.resize(e.Doublet.size());
	for(size_t d=0;d<e.Doublet.size();d++)
		Doublet.at(d) = e.Doublet.at(d);		
	Multiplet.resize(e.Multiplet.size());
	for(size_t d=0;d<e.Multiplet.size();d++)
		Multiplet.at(d) = e.Multiplet.at(d);		
	thetaCM		= e.thetaCM;
	

}
ExperimentData& ExperimentData::operator = (const ExperimentData& e){

	Data.resize(e.Data.size());
	for(size_t d=0;d<e.Data.size();d++)
		Data.at(d) = e.Data.at(d);		
	Doublet.resize(e.Doublet.size());
	for(size_t d=0;d<e.Doublet.size();d++)
		Doublet.at(d) = e.Doublet.at(d);		
	Multiplet.resize(e.Multiplet.size());
	for(size_t d=0;d<e.Multiplet.size();d++)
		Multiplet.at(d) = e.Multiplet.at(d);		
	thetaCM		= e.thetaCM;

	return *this;

}

void ExperimentData::Print() const {

	if(thetaCM < 0)
		std::cout << "Theta undefined" << std::endl;
	else
		std::cout << "Theta [CM]: " << thetaCM << " degrees" << std::endl;

	std::cout	<< std::setw(14) << std::left << "Singlets: "  
			<< std::setw(9)  << std::left << Data.size() 
			<< std::endl;
	std::cout 	<< std::setw(9)  << std::left << "Initial"
			<< std::setw(7)  << std::left << "Final"
			<< std::setw(14) << std::left << "Counts"
			<< std::setw(14) << std::left << "UpUncert."
			<< std::setw(14) << std::left << "DnUncert."
			<< std::endl;
	for(unsigned int i=0;i<Data.size();i++)
		Data.at(i).Print();
	std::cout	<< std::setw(14) << std::left << "Multiplets: "  
			<< std::setw(9)  << std::left << Multiplet.size() 
			<< std::endl;
	std::cout 	<< std::setw(9)  << std::left << "Initial xN"
			<< std::setw(7)  << std::left << "Final xN"
			<< std::setw(14) << std::left << "Counts"
			<< std::setw(14) << std::left << "UpUncert."
			<< std::setw(14) << std::left << "DnUncert."
			<< std::endl;
	for(unsigned int i=0;i<Multiplet.size();i++)
		Multiplet.at(i).Print();

}

void ExptData::Print() const {
	std::cout 	<< std::setw(9) << std::left << StateIndex_I 
			<< std::setw(7) << std::left << StateIndex_F 
			<< std::setw(14) << std::left << Counts 
			<< std::setw(14) << UpUncertainty 
			<< std::setw(14) << DnUncertainty 
			<< std::endl; 
}

void ExptMultiplet::Print() const {
	for(int i=0;i<StateIndex_I.size();i++){
		std::cout 	<< std::setw(9) << std::left << StateIndex_I.at(i)
				<< std::setw(7) << std::left << StateIndex_F.at(i);
	}
	std::cout 	<< std::setw(14) << std::left << Counts 
			<< std::setw(14) << std::left << Uncertainty 
			<< std::endl; 
}
