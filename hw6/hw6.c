#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <math.h>
#include "ClassErrors.h"
#include "poly.h"



int main (int argc, char* argv[]) {

	polynomial p = {0, NULL};
	initPoly(&p, 4);
	p.polyCoef[0] = 1909;
	p.polyCoef[1] = -52.2;
	p.polyCoef[2] = -0.75;
	p.polyCoef[3] = 0.02;

	printPoly(&p);
	
	roots(&p, 0.0, 0);
	//double complex *results = evalDerivs(&p, (double complex)1.0);
	//printComplex(results[0]);
	//printf("\n");
	//printComplex(results[1]);
	//printf("\n");
	//printComplex(results[2]);
	//printf("\n");

	return PGM_SUCCESS;
}
