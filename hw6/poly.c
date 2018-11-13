/***********************************************************************
 * Student C framework to calculate the roots and evaluate polynomials
 * static functions are not REQURED, you are free to implement they way
 * you wish
 * Course: Applied Programming 
 * Revised: 2015
 *
 * 10/16/2015 R. Repka  - Removed evalPoly, extern
 * 04/03/2015 R. Repka  - Added cpow() and Horner comments, 
 * 10/24/2016 R. Repka  - Updated evalDerivs comments
 * 10/26/2016 R. Repka  - Changed createPoly to initPoly
 * 10/09/2017 R, Repka  - Minor comment changes
 ***********************************************************************/ 
#include <complex.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <math.h>
#include "ClassErrors.h"
#include "poly.h"

#define ZERO 0.00000001
#define MAX_ITERATIONS 50000

/*---------------------------------------------------------------------------
  Define local functions
---------------------------------------------------------------------------*/
static double complex* quadraticRoots( polynomial *p);
static double complex laguerre(polynomial *p, double tol, int verb);
static polynomial* deflPoly(polynomial *p, double complex root);
static double complex* evalDerivs(polynomial *p, double complex point);
static void printComplex(double complex x);

/*---------------------------------------------------------------------------
  Initializes a polynomial data structure with nterms.  This allocates storage
  for the actual polynomial.
  
  Where: polynomial *p       - Pointer to polynomial data structure to create
         unsigned int nterms - The number of elements to create
  Returns: nothing
  Errors:  prints an error and exits
---------------------------------------------------------------------------*/
void initPoly(polynomial *p, unsigned int nterms){
	p->nterms = nterms;
	p->polyCoef = malloc(nterms*sizeof(double complex));
	if(p->polyCoef == NULL) {
		fprintf(stderr, "Failed to allocate memory for polynomial. File: %s Line: %d",
				__FILE__, __LINE__);
		exit(MALLOC_ERROR);
	}
}


/*---------------------------------------------------------------------------
  Destroys/frees a polynomial
  
  Where: polynomial *p - Pointer to polynomial data structure to destroy
  Returns: nothing
  Errors:  none
---------------------------------------------------------------------------*/
void freePoly(polynomial *p){
	p->nterms = 0;
	free(p->polyCoef);
}


/*---------------------------------------------------------------------------
  This OPTIONAL function evaluates a polynomial at a complex point z.  
  Don't use the cpow() function, it generates round off errors.  You are 
  required to use Horner's factorization 
   
  Were: polynomial *p    - Pointer to the polynomial to evaluate
        double complex z - Point to evaluate 
  Returns: double complex- Complex solution
  Errors:  none
---------------------------------------------------------------------------*/
double complex cevalPoly(polynomial *p, double complex z){
	double complex sum = p->polyCoef[p->nterms - 1];
	for(int i = p->nterms-2; i>=0; i--){
		sum = ((double complex)sum * (double complex)z) + (double complex) p->polyCoef[i];
	}
	return sum;
}


/*---------------------------------------------------------------------------
 This finds all the roots (real and complex) of a real polynomial.  If there
 is a single root, it solves for it.  If there are two roots then it uses
 the quadratic formula.  If there are more than two roots, it uses Laguerre.
 If a complex root is found then BOTH the + and - roots are deflated from the
 polynomial.
 
 Where: polynomial *poly - Pointer to an array of polynomial coefficients
        double tolerance - Termination tolerance
        int verb         - Verbose flag indicating whether to print intermediate
                           results or not ( 1 = TRUE, 0 = FALSE 
 
 Returns: double complex *roots - A pointer to a complex root array
 Errors:  prints an error and exits
---------------------------------------------------------------------------*/
double complex* roots(polynomial *poly, double tolerance, int verb){
	polynomial *current_equ = poly;
	int arraySize = 10;
	int numRoots = 0;
	double complex *roots = malloc(arraySize * sizeof(double complex));
	
	while (current_equ->nterms > 3) {
		double complex root = laguerre(current_equ, tolerance, verb);

		//Add root to list
		if(numRoots >= arraySize){
			if(cimag(root) > ZERO) {
				arraySize+=2;
			}
			else {
				arraySize++;
			}
			roots = realloc(roots, arraySize * sizeof(double complex));
		}

		polynomial *temp;

		if(cimag(root) > ZERO) {
			if(verb) {
				printf("\tFound two roots: ");
				printComplex(root);
				printComplex(conj(root));
				printf("\n");
			}

			roots[numRoots++] = root;
			roots[numRoots++] = conj(root);

			//Deflate the equation with both roots
			temp = deflPoly(current_equ, root);
			temp = deflPoly(temp, conj(root));
		}
		else {
			if(verb) {
				printf("\tFound root: %f\n", creal(root));
			}
			roots[numRoots++] = root;

			//Deflate the equation with the one root
			temp = deflPoly(current_equ, root);
		}
		
		if(verb) {
			printf("\n\tDeflated equation: ");
			printPoly(temp);
		}
		
		//Free the previous equation (if not the one passed in)
		if(current_equ != poly) {
			freePoly(current_equ);
			free(current_equ);
		}

		//Update current equation
		current_equ = temp;
	}
	
	//TODO: use quadratic for last two roots
	double complex *quad_roots = quadraticRoots(current_equ);

	if(verb){
		printf("\tFinding remaining roots using quadratic formula...\n");
	}

	if(numRoots >= arraySize){
		if(cimag(quad_roots[0]) > ZERO) {
			arraySize+=2;
		}
		else {
			arraySize++;
		}
		if(cimag(quad_roots[1]) > ZERO) {
			arraySize+=2;
		}
		else {
			arraySize++;
		}
		roots = realloc(roots, arraySize * sizeof(double complex));
	}
	if(cabs(cevalPoly(poly, quad_roots[0])) <= tolerance){
		if(verb) {
			printf("\tFound root: ");
			printComplex(quad_roots[0]);
			printf("\n");
		}
		roots[numRoots++] = quad_roots[0];
	}
	if(cabs(cevalPoly(poly, quad_roots[1])) <= tolerance) {
		if(verb) {
			printf("\tFound root: ");
			printComplex(quad_roots[1]);
			printf("\n");
		}
		roots[numRoots++] = quad_roots[1];
	}

	//Terminate array with NAN
	if (numRoots >= arraySize){
		arraySize++;
		roots = realloc(roots, arraySize * sizeof(double complex));
	}
	roots[numRoots] = NAN + 0*I;
	return roots;
}

/*---------------------------------------------------------------------------
  This function evaluates the polynomial derivatives at a point using the
  compact method or another equivalent method.
     
  If you decide not use the compact all-in-one p,p',p'' evaluation code, 
  you can implement this function:
        As separate p, p' and p'' derivatives
        Then evaluate each function separately
        Still returning 3 values
  OR
  You can choose to create functions of your own and not implement this function
  at all
  
  Where: polynomial *p        - Pointer to a polynomial data to evaluate
         double complex point - The point to evaluate
  Returns: double complex*    - Pointer to an array of evaluated 
                                derivatives d, d' and d'' 
  Errors:  prints an error and exits
---------------------------------------------------------------------------*/
static double complex* evalDerivs(polynomial *p, double complex point){
	double complex* results = malloc(3*sizeof(double complex));
	polynomial d;
	polynomial dd;

	initPoly(&d, p->nterms-1);
	initPoly(&dd, p->nterms-2);


	for(int i = 1; i< p->nterms; i++) {
		d.polyCoef[i-1] = i*p->polyCoef[i];
	}

	for(int i = 1; i< d.nterms; i++) {
		dd.polyCoef[i-1] = i*d.polyCoef[i];
	}
	
	results[0] = cevalPoly(p, point);
	results[1] = cevalPoly(&d, point);
	results[2] = cevalPoly(&dd, point);

	return results;
}

/*---------------------------------------------------------------------------
  Returns the two roots from a quadratic polynomial
  
  Where: polynomial *p - Pointer to a 2nd order polynomial
  Returns: double complex* - A pointer to a complex root pair.
  Errors:  prints an error and exits
---------------------------------------------------------------------------*/
static double complex* quadraticRoots( polynomial *p){
	double complex *roots = malloc(2 * sizeof(double complex));
	double complex sqrt_term;
	double complex a = p->polyCoef[2];
	double complex b = p->polyCoef[1];
	double complex c = p->polyCoef[0];
	
	if(p->nterms != 3) {
		fprintf(stderr, "Error: quadraticRoots() called using %d order polynomial\n", p->nterms-1);
		exit(PGM_INTERNAL_ERROR);
	}

	sqrt_term = csqrt(cpow(b,2)- 4 * a * c);

	roots[0] = ((-1 * b) + sqrt_term) / (2*a);
	roots[1] = ((-1 * b) - sqrt_term) / (2*a);

	return roots;
}


/*---------------------------------------------------------------------------
  Uses Laguerre's method to compute a root of a real polynomial
  Note: In addition to verifying that the update value alpha is greater than 
        to, also be sure to verify that polynomial evaluated at the 
        "current guess" is not less than the tolerance.  If p(x) is less than
        tol, you have found a root.
  
  Where: polynomial *p - Pointer to a polynomial structure to evaluate
         double tol    - Termination tolerance 
         int verb      - Verbose flag indicating whether to print intermediate
                         results or not ( 1 = TRUE, 0 = FALSE 
 Returns: double complex - The complex root or (NAN + 0.0*I) if  none is found
 Errors:  none
---------------------------------------------------------------------------*/
static double complex laguerre(polynomial *p, double tol, int verb){
	double complex x = 0.0;
	double complex *evalResults;
	double complex g;
	double complex h;
	double complex alpha;
	int i = 0;
	
	if(verb) {
		printf("\tLaguerre's Algorithm ( tol = %G )\n", tol);
	}
	while (i < MAX_ITERATIONS) {
		if(verb) {
			printf("\t  it: %d x: %f\n", i, cabs(x));
		}
		evalResults = evalDerivs(p, x);

		if(cabs(creal(evalResults[0])) <= ZERO 
				&& cabs(cimag(evalResults[0]) <= ZERO)){
			return x;
		}
		g = evalResults[1]/evalResults[0];
		h = cpow(g, 2) - (evalResults[2]/evalResults[1]);

		

		double complex alpha_denom1 = (g + csqrt((p->nterms-2) * (((p->nterms-1) * h) - cpow(g,2))));
		double complex alpha_denom2 = (g - csqrt((p->nterms-2) * (((p->nterms-1) * h) - cpow(g,2))));


		if(cabs(alpha_denom1) > cabs(alpha_denom2)) {
			alpha = (p->nterms-1) / alpha_denom1;
		}
		else {
			alpha = (p->nterms-1) / alpha_denom2;
		}

		if(verb) {
			printf("\t\tG(x): %f\n", cabs(g));
			printf("\t\tH(x): %f\n", cabs(h));
			printf("\t\tAlpha: %f\n", cabs(alpha));
		}

		if((cabs(creal(alpha)) <= tol) && (cabs(cimag(alpha)) <= tol)){
			return x;
		}

		x = x - alpha;
		i++;
	}
    return NAN;
}

/*---------------------------------------------------------------------------
  Deflates a root from the polynomial, returning the new polynomial 
  
  Where: polynomial *p       - Pointer to the polynomial to deflate
         double complex root - The root to use 
  Returns: polynomial *      - Pointer to the deflated polynomial
  Errors:  none
---------------------------------------------------------------------------*/
static polynomial* deflPoly(polynomial *p, double complex root){
	double complex remainder = p->polyCoef[p->nterms-1];
	polynomial *def_p = malloc(sizeof(polynomial));

	initPoly(def_p, p->nterms-1);

	for(int i = p->nterms-2; i>=0; i--) {
		def_p->polyCoef[i] = remainder;
		remainder = (remainder * root) + p->polyCoef[i];
	}
	
	return def_p;
}


/*---------------------------------------------------------------------------
  The function prints out complex data
  
  Where: double complex x - the complex data to print out
  returns:  nothing
  errors:   none
---------------------------------------------------------------------------*/
static void printComplex(double complex x){
	printf("(%f%+fi)", creal(x), cimag(x));
}

/*---------------------------------------------------------------------------
  Prints a polynomial
  Where: polynomial *p - Pointer to polynomial data structure to print
  Errors: none
  Returns: nothing
---------------------------------------------------------------------------*/
void printPoly (polynomial *p){
	for(int i = p->nterms-1; i>0; i--){
		if(cabs(p->polyCoef[i]) > ZERO){
			printf("(%f)x^%d + ", creal(p->polyCoef[i]), i);
		}
	}
	printf("(%f)\n", creal(p->polyCoef[0]));
}

/*---------------------------------------------------------------------------
  Prints the roots of a polynomial from largest (in magnitude) to smallest
  
  Where: polynomial *p - Pointer to polynomial data structure to print
  Returns: nothing
  Errors:  none
---------------------------------------------------------------------------*/
void printRoots (polynomial *p, double tolerance, int verb){

}


/*---------------------------------------------------------------------------
  Optional helper function to print out x and y value from evaluating 
  polynomials, not required
  
  Where: double complex x - data pair to print
         double complex y - 
  Returns: nothing
  Errors:  none
---------------------------------------------------------------------------*/
void printPoint(double complex x, double complex y){

}