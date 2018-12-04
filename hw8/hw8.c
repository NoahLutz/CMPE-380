/********************************************************************
 * Applied Programming:															
 *	 Solution of Overdetermined System of Equations Ax=b arising	
 *	 in least square problems via QR factorizations using the GSL																 *																						
 * Compilation:  gcc -ansi -g -lgsl -lgslcblas  hw8.c DynamicArrays.c -o  hw8
 *																						
 * Tested in Ubuntu 12.04 LTS													
 * Revised: Juan C. Cockburn, April 9, 2015 (juan.cockburn@rit.edu) 
 * 10/10/2015 R. Repka - Minor clean up
 * 10/26/2016 R. Repka - Major re-write, added QR and norm solution modes
 * 11/12/2017 R. Repka - Minor comment change for norm of residuals, removed
 *							  "c" in GE_FindPoint
 * 11/28/2016 R. Repka - Added Pearson function
 * 07/11/2017 R. Repka - Switched to getopt_long_only
 * 04/20/2018 R. Repka - removed QR solution, added better problem
 *							  description
 ********************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <string.h>
#include <getopt.h>
#include <math.h>
#include <gsl/gsl_math.h>
#include <gsl/gsl_blas.h>
#include <gsl/gsl_vector.h>
#include <gsl/gsl_matrix.h>
#include <gsl/gsl_linalg.h>
#include "ClassErrors.h"
#include "DynamicArrays.h"

/*****************************************************************************
 Function prototypes 
*****************************************************************************/
void readPoints (FILE *file, DArray *DArrayPtr);
void Norm_FindPoint(int nr, int nc, const DArray *points, gsl_vector *x_ls, 
																					  int verbose);
double RSquareError(int nr, int nc, const DArray *points, 
																		const gsl_vector *x_ls);
double normOfResiduals(int nr, int nc, const DArray *points, 
																		const gsl_vector *x_ls);
double pearson_correl(int nr, int nc, const DArray *points,
																		const gsl_vector *x_ls);
double evalPoly(int nc, double x, const gsl_vector *x_ls);


/*****************************************************************************
 This program uses least squares to generate approximate functions
	 usage: hw8  -norm -order  num	-points  file  [-verbose] \n");
 
 Returns: 0 for success, non-zero for errors
 *****************************************************************************/
int main(int argc, char *argv[]) {
	int verbose = 0;
	int norm = 0;
	int order = 0;
	int rc;
	int option_index = 0;
	char *getoptOptions = "nvo:p:";
	char *input_file = NULL;

	DArray points;
	FILE *pointsFile;
	gsl_vector *x_ls; /* least squares solution	*/
  
	struct option long_options[] = {
		{"verbose",		no_argument,		0,	'v'},
		{"verb",		no_argument,		0,	'v'},
		{"norm",		no_argument,		0,	'n'},
		{"order",		required_argument,	0,	'o'},
		{"point",		required_argument,	0,	'o'}
	};
	
	while((rc = getopt_long_only(argc, argv, getoptOptions, long_options,
					&option_index)) != -1) {
		switch(rc) {
			case 'v':
				verbose = 1;
				break;
			case 'n':
				norm = 1;
				break;
			case 'o':
				order = atoi(optarg);
				break;
			case 'p':
				input_file = optarg;
				break;
			case '?':
				fprintf(stderr, "Encountered error parsing options.\n");
				return PGM_INTERNAL_ERROR;
			default:
				fprintf(stderr, "Internal Error: undefined option %0xX\n", rc);
				return PGM_INTERNAL_ERROR;
		}
	}
 	
	if(input_file != NULL) {
		pointsFile = fopen(input_file, "r");
	}
	else {
		pointsFile = stdin;
	}

	if(order <= 0) {
		fprintf(stderr, "Please specify order > 0\n");
		return PGM_INTERNAL_ERROR;
	}
	
	CreateDArray(&points, 0);
	readPoints(pointsFile, &points);

	x_ls = gsl_vector_calloc(order);
	
	Norm_FindPoint(points.EntriesUsed, order, &points, x_ls, verbose);

 	/* Clean up */
	gsl_vector_free(x_ls);  
	DestroyDArray(&points);
	
	return PGM_SUCCESS; /* main */
}

/*---------------------------------------------------------------------------
  Find the least squares approximation to data "points" of order "nc" using
  the "Normal equations" approach.
		  
								A'Az = A'b
  
  Where: int nr			  - The number of points (rows) of the input file
			int nc			  - The number of columns (order) of the solution
			DArray *points	- Pointer to the x,y data
			gsl_vector *x_ls - The solution is returned here
			int verbose		- Verbose flag

  Returns: nothing
  Errors: Assumes the standard GSL error handler
---------------------------------------------------------------------------*/
void Norm_FindPoint(int nr, int nc, const DArray *points, gsl_vector *x_ls, int verbose) {
	double x;
	int i, j;			/* counters					  */
	gsl_matrix *A;	 /* coefficient matrix A	  */
	gsl_matrix *AT;	/* coefficient matrix A'	 */
	gsl_matrix *ATA;  /* coefficient matrix A'A	*/
	gsl_vector *b;	 /* coefficient vector b	  */
	gsl_vector *ATB;  /* coefficient vector A'b	*/
	gsl_vector *tau;  /* Householder coefficients */
	gsl_vector *res;  /* vector of residuals		*/

	/* Allocate space for Matrices and vectors */
	ATA  = gsl_matrix_alloc(nc, nc); /* Data matrix */
	AT	= gsl_matrix_alloc(nc, nr); /* Data matrix */
	A	 = gsl_matrix_alloc(nr, nc); /* Data matrix */
	b	 = gsl_vector_alloc(nr);	  /* Data vector */
	ATB  = gsl_vector_alloc(nc);	  /* Data vector */
	tau  = gsl_vector_alloc(nc);	  /* required place holder for GSL */
	res  = gsl_vector_alloc(nc);	  /* Contains the residual */
	
  
	
	/* your code here  */
	for(int i = 0; i<nr; i++) {
		for(int j = 0; j<nc; j++) {
			if(j == 0) {
				gsl_matrix_set(A,i,j,1.0);
			}
			else {
				gsl_matrix_set(A,i,j, pow(points->Payload[i].xval, j));
			}
		}
		gsl_vector_set(b,i, points->Payload[i].yval);
	}


	printf("A=\n");
	for(int i = 0; i<nr; i++) {
		for(int j = 0; j<nc; j++) {
			printf("%7.2lf", gsl_matrix_get(A, i,j));
		}
		putchar('\n');
	}
	printf("b=\n");
	for(int i = 0; i<nr; i++) {
		printf("%7.2lf\n", gsl_vector_get(b, i));
	}
 
	/* Transpose A matrix */
	gsl_matrix_transpose_memcpy(AT, A);

	/* Calculate ATA */
	gsl_blas_dgemm(CblasNoTrans, CblasNoTrans, 1.0, AT, A, 0.0, ATA);
	
	printf("ATA=\n");
	for(int i = 0; i<nc; i++) {
		for(int j = 0; j<nc; j++) {
			printf("%7.2lf", gsl_matrix_get(ATA, i,j));
		}
		putchar('\n');
	}

	/* Calculate ATB */
	gsl_blas_dgemv(CblasNoTrans, 1.0, AT, b, 0.0, ATB);
	
	printf("ATb=\n");
	for(int i = 0; i<nc; i++) {
		printf("%7.4lf\n", gsl_vector_get(ATB, i));
	}

	/* Solve for x */
	gsl_permutation *p = gsl_permutation_alloc(nc);
	int s;
	gsl_linalg_LU_decomp(ATA, p, &s);
	gsl_linalg_LU_solve(ATA, p, ATB, x_ls);


	printf("x_ls=\n");
	for(int i = 0; i<nc; i++) {
		printf("%7.4lf\n", gsl_vector_get(x_ls, i));
	}
 
	/* Free memory  */
	gsl_matrix_free(A);
	gsl_matrix_free(AT);
	gsl_matrix_free(ATA);
	gsl_vector_free(b);
	gsl_vector_free(ATB);
	gsl_vector_free(tau); 
	gsl_vector_free(res);
} /* end Norm_FindPoint() */



/****************************************************************************
  This calculate the norm of residuals given the points and the solution
  Note: You must apply the correction polynomial f(x) to the input data to 
		  simulate the correction effect 
  
						 normR = squareRoot [sum {( yi - f(xi))**2} ]

  Where: int nr			  - The number of points (rows) of the input file
			int nc			  - The number of columns (order) of the solution
			DArray *points	- Pointer to the x,y data
			gsl_vector *x_ls - The solution vector, small power first

  Errors: Assumes the standard GSL error handler
  
  Returns: double norm of residuals
****************************************************************************/
double normOfResiduals(int nr, int nc, const DArray *points, const gsl_vector *x_ls) {
  
} /* normOfResiduals */


/****************************************************************************
  This calculate the R2 coefficient of Determination error between the points  
  and the solution.
  Note: You must apply the correction polynomial f(x) to the input data to 
		  simulate the correction effect 
						 
  Where: int nr			  - The number of points (rows) of the input file
			int nc			  - The number of columns (order) of the solution
			DArray *points	- Pointer to the x,y data
			gsl_vector *x_ls - The solution vector, small power first
 
  Errors: Assumes the standard GSL error handler
  
  Returns: R squared error
****************************************************************************/
double RSquareError(int nr, int nc, const DArray *points, const gsl_vector *x_ls) {
  
} /* End RSquareError */


/*****************************************************************************
 This calculates the Pearson's Correlation, or the excel function correl()
  Note: You must apply the correction polynomial f(x) to the input data to 
		  simulate the correction effect 
						  
  Where: int nr			  - The number of points (rows) of the input file
			int nc			  - The number of columns (order) of the solution
			 DArray *points	- Pointer to the x,y data
			gsl_vector *x_ls - The solution vector, small power first

  Errors: Assumes the standard GSL error handler		 
		 
 Returns: double pearson_srq 
*****************************************************************************/
double pearson_correl(int nr, int nc, const DArray *points,
															 const gsl_vector *x_ls) {
 
} /* End pearson_correl */




/***************************************************************************************
 Evaluates a polynomial at a point, assumes low order term first.  
 Must use Horner's factorization 
 
 Where: int nc			  - The number of columns in the solution
		  double x			- Point at which splines should be evaluated
		  gsl_vector *x_ls - The solution vector, small power first
			 
 Returns: double - The value at the desired point
 Errors:  none
*****************************************************************************************/
double evalPoly(int nc, double x, const gsl_vector *x_ls) {
  
	
  
} /* End evalPoly */


/***************************************************************************************
 Reads the points data from file and returns it in a Darray
 
 Where: FILE *file -	open handle to read from
						of the form:	22.0		6.7
										23.4		18.8
		DArray *DArrayPtr - Pointer to a dynamic array to store the data
  Returns: nothing
  Errors:  none
*****************************************************************************************/
void readPoints(FILE *file, DArray *DArrayPtr) {
	double x = 0.0;
	double y = 0.0;
	int i = 0;
	
	while(fscanf(file, "%lf %lf", &x, &y) != EOF) {
		Data *newPoint = malloc(sizeof(Data));

		if(newPoint==NULL) {
			fprintf(stderr, "Failed to allocate memory\n");
			exit(MALLOC_ERROR);
		}

		newPoint->Num = i;
		newPoint->xval = x;
		newPoint->yval = y;

		PushToDArray(DArrayPtr, newPoint);

		i++;
	}

	return;
} /* readPoints */
