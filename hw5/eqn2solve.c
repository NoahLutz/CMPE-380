/*---------------------------------------------------------------------------
  05/08/2017    R. Repka    Make derivative clearer
---------------------------------------------------------------------------*/
#include <math.h>

/*---------------------------------------------------------------------------
  Student framework equations to be solved to help with the root finder 
---------------------------------------------------------------------------*/
#include "rootfinding.h"

/*---------------------------------------------------------------------------
  This function implements the equation to be solved.  
  
  Where: double x - the value to evaluate
  Returns: double - the value of the function at the point
  Errors:  none
---------------------------------------------------------------------------*/
double func1(double x){
	return (double)((0.76*x*sin((30.0/52.0)*x)*tan((10.0/47.0)*x)) + (double)(2.9*cos(x+2.5)*sin(0.39*(1.5+x))));
}


/*---------------------------------------------------------------------------
  This function implements the first derivative equation, which is calculated
  via any off-line process you wish (e.g  Wolfram Alpha)
  There is NO requirement to programmatically generate the derivative.
  
  Where: double x - the value to evaluate
  Returns: double - the value of the function at the point
  Errors:  none
---------------------------------------------------------------------------*/
double func1Deriv(double x){
	return NAN;
}
