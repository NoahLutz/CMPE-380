/*********************************************************************
 * Example: Using the GSL (Gnu Scientific Library) to solve a system 
 * of algebraic equations Ax=b via permuted LU factorization. 
 * No error checking
 *
 * Compile: gcc -ansi -lgsl -lgslcblas gsl_example.c -o lu
 *
 * Revision : Juan C. Cockburn, 1/13/13
 * Reference: Golub and Vanloan Algorithms 3.4.1
 * 05/25/2016 - Added P and LU debug info
 *********************************************************************/
#include <stdio.h>
#include <gsl/gsl_linalg.h>

int main (int argc, char* argv[]) {
	/* Local variables */
	int i, j, k, l; /* Indices and counters	*/
	int s;/* Sign of the permutation		*/
	int nr = 3;/* Matrix dimensions, rows		*/
	int nc = 3;/* Matrix dimensions, columns */
	
	/* Declare and allocate matrix and vector variables */
	gsl_matrix *A = gsl_matrix_calloc (nr,nc) ; /* A */
	gsl_vector *b = gsl_vector_calloc (nr) ;		/* b */
	gsl_vector *x = gsl_vector_calloc (nc);/* x */
	
	gsl_permutation *p = gsl_permutation_alloc (nr); /* Permutation Vector	for LU */

	/* Simple Example */
	/* A 3x3 matrix */
	double a_data[] = { 2.00, 1.00, 1.00,
						4.00,-6.00, 0.00,
 						-2.00, 7.00, 2.00 } ;
 
	/* b 3-vector */
	double b_data[] = { 5.00, -2.00, 9.00 };

	/* Initialize coefficient matrix A and vector b */
	/* use gsl_matrix_set	and gsl_vector_set */
	k = 0 ; l = 0 ; /* set counters	to zero */
	for (i=0;i<nr;i++) {
		for (j=0;j<nc;j++) {
			gsl_matrix_set(A,i,j,a_data[k]) ; 
			k++ ;
		} /* for j */
		
		gsl_vector_set(b,i,b_data[l]) ; 
		l++ ;
	} /* for i */

	/* Print entries of A use gsl_matrix_get and printf */
	/* do not use gsl_matrix_fprintf */
	printf("Solution of the system Ax=b via PLU factorizations\n");
	printf("Matrix A:\n");
	for (i = 0; i < nr; i++) {
		for (j = 0; j < nc; j++)
 			printf ("%7.2g ", gsl_matrix_get (A, i, j));
			putchar('\n');
	} /* for i */
	
	/* Print entries of vector b */
	printf("Vector b:\n");
	gsl_vector_fprintf (stdout, b,"%7.2g") ;

	/* Perform (in place) PLU factorization */
	gsl_linalg_LU_decomp (A, p, &s); /* A is overwritten, p is permutation*/
	
	/* Print out the P and LU matrix */
	fprintf(stdout, "\nP = [");
	gsl_permutation_fprintf (stdout, p, " %u");
	fprintf(stdout, " ] \n");
	
	fprintf(stdout, "LU matrix = \n");
	printf("Matrix LU:\n");
	for (i = 0; i < nr; i++) {
		for (j = 0; j < nc; j++) {
	 		printf ("%7.2g ", gsl_matrix_get (A, i, j));
		} 
		putchar('\n');
	} /* End for i */
	
	/* Find solution using the PLU factors found above and b */
	gsl_linalg_LU_solve (A, p, b, x);

	/* Print solution x */
	printf("Solution x:\n");
	gsl_vector_fprintf (stdout, x, "%7.2g");

	/* Clean up - free heap memory */
	gsl_matrix_free (A);
	gsl_vector_free (b);
	gsl_permutation_free (p);
	gsl_vector_free (x);

	return 0;
} /* main */
