/***************************************************************************
  Student framework for root finding algorithms for bisection, newton, and secant.
   
  11/10/2015   R. Repka - Initial version
  12/28/2015 - R. Repka - added integer solution features
***************************************************************************/
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include "ClassErrors.h"
#include "rootfinding.h"


/******************************************************************************
 Purpose: Finds a root of scalar, nonlinear function f using the bisection  
 method. a and b make up the initial bracket to start bisecting from.
 
 Where: func1arg f  - function whose root is to be determined
                      must take a single argument of type double and return
        double a    - initial root bracket guess
        double b    - initial root bracket guess
        double atol - absolute error termination tolerance,
        int verb    - verbose flag, 1 = TRUE, 0 = FALSE
        
Returns: double - the root refined to the desired tolerance or NAN
Errors:  prints a message and returns with NAN                    
******************************************************************************/
double bisection(func1arg f, double a, double b, double atol, int verb) {
	double x1 = a;
	double x2 = b;
	double x3;
	double val3;
	int iter = 0;

	double val1 = (*f)(x1);

	if (fabs(val1) <= atol) {
		return x1;
	}

	double val2 = (*f)(x2);

	if (fabs(val2) <= atol) {
		return x2;
	}

	if (SIGN(val1) == SIGN(val2)){
		fprintf(stderr, "No root inbetween brackets");
		return NAN;
	}
	
	int n = ceil((log(x2-x1)/atol)/log(2.0));
	printf("N:%d\n", n);
	for (int i = 0; i<n; i++) {
		x3 = (x1 + x2) / 2;
		val3 = (*f)(x3);
		
		if(verb) {
			printf("iter:%d\ta:%f\tb:%f\tx:%f\terr:%f\n", iter, x1, x2, x3, 0.0);
		}

		if(fabs(val3) <= atol) {
			return x3;
		}

		if(SIGN(val1) != SIGN(val3)) {
			x2 = x3;
			val2 = val3;
		}
		else {
			x1 = x3;
			val1 = val3;
		}
		iter++;
	}

	return (double) ((x1 + x2)/2.0);
}


/******************************************************************************
 Purpose: Finds a root of function f using the newton method. x0 is the initial 
 guess,  df is the derivative of function f , Nmax is the maximum number
  of iterations, atol is the tolerance, and verb will turn a verbose print* out

  Where: func1arg f  - function whose root is to be determined
                       must take single argument of type double and return
                       single value of type double
         func1arg df - derivative of function whose root is to be determined
                       must take single argument of type double and return
                       single value of type double
         double x0   - initial guess for location of root
         int Nmax    - maximum number of iterations to achieve convergence
         double atol - absolute error convergence tolerance
         int verb    - verbose flag, 1 = TRUE, 0 = FALSE

Returns: double - the root refined to the desired tolerance or NAN
Errors:  prints a message and returns with NAN      
******************************************************************************/
double newton(func1arg f, func1arg df, double x0, int Nmax, double atol, int verb) {
	int i = 0;
	double xn = x0;
	double val = 0;
	double derivVal = 0;

	while (i<Nmax) {
		double nextX = 0;
		val = (*f)(xn);

		if(fabs(val) <= atol) {
			return xn;
		}

		derivVal = (*df)(xn);

		if (fabs(derivVal) <= atol) {
			fprintf(stderr, "Failed f'(xn) is too close to zero\n");
			return NAN;
		}

		nextX = xn - (val/derivVal);

		if(verb) {
			printf("iter:%d\tx0:%f\tx1:%f\terr:%f\n", i, xn, nextX, 0.0);
		}
		xn = nextX;
		i++;
	}
	fprintf(stderr, "Failed, exceeded %d iterations without finding root", Nmax);
	return NAN;
}


/******************************************************************************
 Purpose: Finds a root of function f using the secant method. x0 and x1 are 
 the two initial guesses used to find the first tangent.
 
 Where: func1arg f  - function whose root is to be determined
                       must take single argument of type double and return
                       single value of type double
         double x0   - initial guess for location of root
         double x1   - another initial guess for location of root
         int Nmax    - maximum number of iterations to achieve convergence
         double atol - absolute error convergence tolerance
         int verb    - verbose flag, 1 = TRUE, 0 = FALSE

Returns: double - the root refined to the desired tolerance or NAN
Errors:  prints a message and returns with NAN        
******************************************************************************/
double secant(func1arg f, double x0, double x1, int Nmax, double atol, int verb) {
	int i = 0;
	double xa = x0;
	double xb = x1;
	double val1 = 0;
	double val2 = 0;

	while (i < Nmax) {
		val1 = (*f)(xa);
		val2 = (*f)(xb);
		double slope = (val2 - val1)/(xb - xa);
		double b = val1 - (slope*xa);
		double x_int = -b/slope;

		if(verb) {
			printf("iter:%d\tx0:%f\tx1:%f\terr:%f\n", i, xa, xb, 0.0);
			printf("x_int:%f\tvalXint:%f\n", x_int, (*f)(x_int));
		}

		if(fabs((*f)(x_int)) <= atol) {
			return x_int;
		}
		

		xb = xa;
		xa = x_int;
		i++;
	}
	fprintf(stderr, "Failed, exceeded %d iterations without finding root", Nmax);
	return NAN;
}
