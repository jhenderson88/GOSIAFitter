#include "MatrixElement.h"


MatrixElement& MatrixElement::operator = (const MatrixElement& m){

	index			= m.index;
	lambda			= m.lambda;
	initialstate		= m.initialstate;
	finalstate		= m.finalstate;
	matrixElement		= m.matrixElement;
	matrixElement_ll	= m.matrixElement_ll;
	matrixElement_ul 	= m.matrixElement_ul;
  fixed = m.fixed;

	return *this;

}
MatrixElement::MatrixElement(const MatrixElement& m){

	index			= m.index;
	lambda			= m.lambda;
	initialstate		= m.initialstate;
	finalstate		= m.finalstate;
	matrixElement		= m.matrixElement;
	matrixElement_ll	= m.matrixElement_ll;
	matrixElement_ul 	= m.matrixElement_ul;
  fixed = m.fixed;

}
void MatrixElement::Print() const {

	std::cout 	<< std::setw(10) << std::left << "Index:"
			<< std::setw(10) << std::left << "Lambda:"
			<< std::setw(12) << std::left << "Initial:"
			<< std::setw(12) << std::left << "Final:" 
			<< std::setw(10) << std::left << "ME:"
			<< std::setw(10) << std::left << "LL:"
			<< std::setw(10) << std::left << "UL:"
			<< std::endl;
	std::cout 	<< std::setw(10) << std::left << index
			<< std::setw(10) << std::left << lambda
			<< std::setw(12) << std::left << initialstate
			<< std::setw(12) << std::left << finalstate
			<< std::setw(10) << std::left << matrixElement
			<< std::setw(10) << std::left << matrixElement_ll
			<< std::setw(10) << std::left << matrixElement_ul
			<< std::endl;

}

RelativeMatrixElement& RelativeMatrixElement::operator = (const RelativeMatrixElement& m){

	index			= m.index;
  lambda			= m.lambda;
	initialstate		= m.initialstate;
	finalstate		= m.finalstate;
  lambdaRel			= m.lambdaRel;
  initialstateRel		= m.initialstateRel;
	finalstateRel		= m.finalstateRel;
  rel = m.rel;
  relll = m.relll;
  relul = m.relul;
  fixed = m.fixed;

	return *this;

}
RelativeMatrixElement::RelativeMatrixElement(const RelativeMatrixElement& m){

  index			= m.index;
  lambda			= m.lambda;
	initialstate		= m.initialstate;
	finalstate		= m.finalstate;
  lambdaRel			= m.lambdaRel;
  initialstateRel		= m.initialstateRel;
	finalstateRel		= m.finalstateRel;
  rel = m.rel;
  relll = m.relll;
  relul = m.relul;
  fixed = m.fixed;


}
void RelativeMatrixElement::Print() const {

	std::cout 	<< std::setw(10) << std::left << "Index:"
			<< std::setw(10) << std::left << "Lambda:"
			<< std::setw(12) << std::left << "Initial:"
			<< std::setw(12) << std::left << "Final:"
      << std::setw(10) << std::left << "Lambda (Rel):"
			<< std::setw(12) << std::left << "Initial (Rel):"
			<< std::setw(12) << std::left << "Final (Rel):"
      << std::setw(10) << std::left << "Value (Rel):" 
			<< std::endl;
	std::cout 	<< std::setw(10) << std::left << index
			<< std::setw(10) << std::left << lambda
			<< std::setw(12) << std::left << initialstate
			<< std::setw(12) << std::left << finalstate
			<< std::setw(10) << std::left << lambdaRel
			<< std::setw(12) << std::left << initialstateRel
			<< std::setw(12) << std::left << finalstateRel
      << std::setw(10) << std::left << rel     
			<< std::endl;

}
