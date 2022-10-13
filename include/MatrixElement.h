#ifndef MatrixElement_h
#define MatrixElement_h

#include <iostream>
#include <iomanip>

///
///	\class MatrixElement
///
///	\brief Holder class for matrix element information for use in fitting routines
///


class MatrixElement{

	public:
		MatrixElement()	{;}
		MatrixElement(int i, int l, int s1, int s2, double me, double mell, double meul){
									 	index 			= i; 
										lambda 			= l; 
										initialstate 		= s1; 
										finalstate 		= s2; 
										matrixElement 		= me;	
										matrixElement_ll 	= mell;
										matrixElement_ul 	= meul;
                    fixed = false;
		}							/*!< Construct matrix element me, of index i, multipolarity l, between states s1 and s2 with lower and upper limits of mell and meul  */
		~MatrixElement() {;}
		MatrixElement& operator = (const MatrixElement& m);	/*!< Assignment operator */
		MatrixElement(const MatrixElement& m);			/*!< Copy constructor */

		void	SetupME(int i, int l, int s1, int s2, double me, double mell, double meul){
									 	index 			= i; 
										lambda 			= l; 
										initialstate 		= s1; 
										finalstate 		= s2; 
										matrixElement 		= me;	
										matrixElement_ll 	= mell;
										matrixElement_ul 	= meul;
		}							/*!< Define matrix element me, of index i, multipolarity l, between states s1 and s2 with lower and upper limits of mell and meul  */
		void	SetMatrixElement(double ME)				{ matrixElement = ME;		}	/*!< Define matrix element value */
		void	SetMatrixElementLowerLimit(double ME_LL)		{ matrixElement_ll = ME_LL;	}	/*!< Define matrix element lower limit */
		void	SetMatrixElementUpperLimit(double ME_UL)		{ matrixElement_ul = ME_UL;	}	/*!< Define matrix element upper limit */
    void SetFixed(bool f) { fixed = f; }
		double 	GetMatrixElement() const				{ return matrixElement;		}	/*!< Return matrix element value */
		double	GetMatrixElementLowerLimit() const			{ return matrixElement_ll;	}	/*!< Return matrix element lower limit */
		double 	GetMatrixElementUpperLimit() const			{ return matrixElement_ul;	}	/*!< Return matrix element upper limit */
		int	GetIndex() const					{ return index;			}	/*!< Return matrix element index */
		int 	GetLambda() const 					{ return lambda;		}	/*!< Return matrix element mulitpolarity */
		int 	GetInitialState() const					{ return initialstate;		}	/*!< Return initial state index */
		int	GetFinalState()	const					{ return finalstate;		}	/*!< Return final state index */
  bool GetFixed() const { return fixed; }

		void	Print() const;		/*!< Print matrix element information */

	private:
		int 		index;			/*!< Matrix element index */
		int 		lambda;			/*!< Matrix element multipolarity */
		int 		initialstate;		/*!< Initial state (initial and final are arbitrary) */
		int 		finalstate;		/*!< Final state (initial and final are arbitrary)*/
		double		matrixElement;		/*!< Matrix element value */
		double		matrixElement_ll;	/*!< Matrix element lower limit */
		double		matrixElement_ul;	/*!< Matrix element upper limit */
  bool fixed; 

};

///
///	\class RelativeMatrixElement
///
///	\brief Holder class for matrix element information for use in fitting routines, where this matrix element is fixed relative to some other matrix element
///


class RelativeMatrixElement{

	public:
		RelativeMatrixElement()	{;}
    RelativeMatrixElement(int i, int l, int s1, int s2, int l2, int s12, int s22, double r, double rll, double rul, bool fx=false){
      index 			= i; 
      lambda 			= l; 
      initialstate 		= s1; 
      finalstate 		= s2;
      lambdaRel 			= l2; 
      initialstateRel 		= s12; 
      finalstateRel 		= s22; 
      rel = r;
      relll = rll;
      relul = rul;
      fixed = fx;
		}							/*!< Construct matrix element me, of index i, multipolarity l, between states s1 and s2 with lower and upper limits of mell and meul  */
		~RelativeMatrixElement() {;}
		RelativeMatrixElement& operator = (const RelativeMatrixElement& m);	/*!< Assignment operator */
		RelativeMatrixElement(const RelativeMatrixElement& m);			/*!< Copy constructor */

    void	SetupME(int i, int l, int s1, int s2, int l2, int s12, int s22, double r, double rll, double rul, bool fx=false){
      index 			= i; 
      lambda 			= l; 
      initialstate 		= s1; 
      finalstate 		= s2;
      lambdaRel 			= l2; 
      initialstateRel 		= s12; 
      finalstateRel 		= s22; 
      rel = r;
      relll = rll;
      relul = rul;
      fixed = fx;
		}							/*!< Define matrix element me, of index i, multipolarity l, between states s1 and s2 with lower and upper limits of mell and meul  */
		void	SetRelativeElement(double r)				{ rel = r;		}	/*!< Define matrix element value */
    void	SetRelativeElementLowerLimit(double ME_LL)		{ relll = ME_LL;	}	/*!< Define matrix element lower limit */
		void	SetRelativeElementUpperLimit(double ME_UL)		{ relul = ME_UL;	}	/*!< Define matrix element upper limit */

		double 	GetRelativeElement() const				{ return rel;		}	/*!< Return matrix element value */
    double 	GetRelativeElementLowerLimit() const				{ return relll;		}	/*!< Return matrix element value */
    double 	GetRelativeElementUpperLimit() const				{ return relul;		}	/*!< Return matrix element value */
		int	    GetIndex() const					{ return index;			}	/*!< Return matrix element index */
		int 	  GetLambda() const 					{ return lambda;		}	/*!< Return matrix element mulitpolarity */
		int 	  GetInitialState() const					{ return initialstate;		}	/*!< Return initial state index */
		int	    GetFinalState()	const					{ return finalstate;		}	/*!< Return final state index */
  	int 	  GetLambdaRel() const 					{ return lambdaRel;		}	/*!< Return matrix element mulitpolarity */
		int 	  GetInitialStateRel() const					{ return initialstateRel;		}	/*!< Return initial state index */
		int	    GetFinalStateRel()	const					{ return finalstateRel;		}	/*!< Return final state index */
    void    SetFixed(bool fx) { fixed = fx; }
    bool    GetFixed() const { return fixed; };
		void	  Print() const;		/*!< Print matrix element information */

	private:
		int 		  index;			/*!< Matrix element index */
		int 		  lambda;			/*!< Matrix element multipolarity */
		int 		  initialstate;		/*!< Initial state (initial and final are arbitrary) */
		int 		  finalstate;		/*!< Final state (initial and final are arbitrary)*/
    int 		  lambdaRel;			/*!< Matrix element multipolarity */
		int 		  initialstateRel;		/*!< Initial state (initial and final are arbitrary) */
		int 		  finalstateRel;		/*!< Final state (initial and final are arbitrary)*/
		double		rel;		/*!< Matrix element value */
    double		relll;		/*!< Matrix element value */
    double		relul;		/*!< Matrix element value */
    bool      fixed;

};

#endif
