 /*********************************************************
 * APMATRIX Module - student file
 *
 * 10/24/2013,	Juan C. Cockburn							 
 * 09/13/2015,	R. Repka - added error checking
 * 11/15/2015	 R. Repka - changed Real to double
 * 04/10/2016	 R. Repka - Minor change to headers
 * 05/25/2016	 R. Repka - Added feature to m_free to 
 *												 detect pointer rearrangement. 
 * 11/01/2016	 R. Repka - Removed m_ident(), and made
 *												 minor comment changes
 * 04/06/2017	 R. Repka		Minor commend modification G to A
 *********************************************************/

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <math.h>
#include "ClassErrors.h"
#include "apmatrix.h"

Matrix* extract_L(Matrix* A);
Matrix* extract_U(Matrix* A);
		

/*---------------------------------------------------------------------------
	Implements in place Gaussian elimination.	
	PLU_Factor takes an A matrix (not G, no b included) and does a Gaussian 
	factorization in place.	It returns the factorized A matrix 
	(made of L&U in place) and a 'p' vector which recorded the row swaps for 
	future 'b' manipulations.

	
	Where: Matrix	*A - Pointers to the matrix to solve
				 iVector *p - Pointer to the permutation vector
	Errors: none
	Returns: int - 0 = success
							 - 3 = No pivot point found, not solvable
---------------------------------------------------------------------------*/
int PLU_factor(Matrix *A, iVector *p){
	//Find non-zero pivot
	if(A->mat[0][0] <= ZERO){
		// Loop through all rows if first has zero pivot
		for(int i = 1; i< A->nr; i++) {
			if(A->mat[i][0] > ZERO) {
				//If non-zero pivot found, swap rows
				double* row0 = A->mat[0];
				A->mat[0] = A->mat[i];
				A->mat[i] = row0;

				//Adjust p vector as well
				int p0 = p->ivec[0];
				p->ivec[0] = p->ivec[i];
				p->ivec[i] = p0;

				//Break the loop once pivot is found
				break;
			}
		}
		if(A->mat[0][0] <= ZERO) {
			// If unable to find pivot, return status code 3
			return 3;
		}
	}
			
	//Loop through all rows
	for(int i = 0; i< A->nr-1; i++){
		//Get the pivot
		double pivot = A->mat[i][i];

		//Put in values for L vector
		for(int j = i+1; j<A->nc; j++) {
			A->mat[j][i] = A->mat[j][i]/pivot;
		}

		//Adjust next rows
		for(int j = i+1; j<A->nr; j++) {
			double multiplier = A->mat[j][i];
			for(int k = i+1; k<A->nc; k++){
				A->mat[j][k] = A->mat[j][k] - multiplier * A->mat[i][k];
			}
		}
	}
	return 0;
}


/*---------------------------------------------------------------------------
	Solves Ax=b using PLU factorization
	
	Where: Matrix	*A - Pointer to the PLU factored matrix
				 iVector *p - Pointer to the permutation vector 
				 rVector *b - Pointer to the b matrix
				 rVector *x - Pointer to the vector to hold the answer
	Errors: none
	Returns: nothing
---------------------------------------------------------------------------*/
void PLU_solve(Matrix *A, iVector *p, rVector *b, rVector *x){
	rVector *c = NULL;
	Matrix *L = NULL;
	Matrix *U = NULL;
	
	//Sort b according to p
	for(int i = 0; i<p->n; i++) {
		if(p->ivec[i] != i) {
			double temp = b->vec[i];
			b->vec[i] = b->vec[p->ivec[i]];
			b->vec[p->ivec[i]] = temp;
		}
	}

	c = rv_alloc(p->n);

	L = extract_L(A);
	U = extract_U(A);

	//Solve for C using forward subst.
	c->vec[0] = b->vec[0];
	for(int i = 1; i<c->n; i++) {
		double sum = 0;
		for(int j = 0; j<A->nc; j++){
			if(i>j) {
				sum += L->mat[i][j] * c->vec[j];
			}
		}
		c->vec[i] = (b->vec[i] - sum);
	}

	//Solve for x using backwards subst.
	x->vec[x->n-1] = c->vec[x->n-1]/U->mat[x->n-1][x->n-1];
	for(int i = x->n-2; i>=0; i--) {
		double sum = 0;
		for(int j = U->nc-1; j > i; j--){
			sum += U->mat[i][j] * x->vec[j];
		}
		x->vec[i] = (c->vec[i] - sum)/U->mat[i][i];
	}
	rv_free(c);
	m_free(L);
	m_free(U);
}

/*---------------------------------------------------------------------------
	Extracts L matrix from PLU factored matrix
	
	Where: Matrix	*A - Pointer to the PLU factored matrix
	
	Errors: none
	Returns: L Matrix
---------------------------------------------------------------------------*/
Matrix* extract_L(Matrix *A){
	Matrix *L = m_alloc(A->nr, A->nc);

	for(int i = 0; i<A->nr; i++){
		for(int j = 0; j<A->nc; j++) {
			if(i == j) {
				L->mat[i][j] = 1.0;
			}
			if(i>j) {
				L->mat[i][j] = A->mat[i][j];
			}
		}
	}
	return L;
}

/*---------------------------------------------------------------------------
	Extracts U matrix from PLU factored matrix
	
	Where: Matrix	*A - Pointer to the PLU factored matrix
	
	Errors: none
	Returns: U Matrix
---------------------------------------------------------------------------*/
Matrix* extract_U(Matrix *A){
	Matrix *U = m_alloc(A->nr, A->nc);

	for(int i = 0; i<A->nr; i++) {
		for(int j = 0; j<A->nc; j++) {
			if(i<=j) {
				U->mat[i][j] = A->mat[i][j];
			}
		}
	}
	return U;
}

/*---------------------------------------------------------------------------
	Allocate memory space for matrix, initialized to zero.	
	Note: This allocates one big block of RAM and then adjusts the pointer table
	
	Where: int nr - The number of rows and columns in the matrix
				 int nc -
	Errors: exits
	Returns Matrix * - Pointer to the resulting matrix space
---------------------------------------------------------------------------*/
Matrix* m_alloc(int nr, int nc) {
	int i;
	double* ptr; /* pointer to head memory */
	Matrix* M;
	
	/* Allocate memory for matrix "header" structure */
	M = malloc(sizeof(Matrix));
	if (NULL == M)
	{
			printf("Error: Malloc() failed at line %d of %s\n", __LINE__, __FILE__);
			exit(MALLOC_ERROR);
	}
	M->nr = nr;
	M->nc = nc;
	
	/* Allocate space for matrix data */
	M->mat = malloc( nr * sizeof(double *)); /* row pointers */
	if (NULL == M->mat)
	{
			printf("Error: Malloc() failed at line %d of %s\n", __LINE__, __FILE__);
			exit(MALLOC_ERROR);
	}
	
	/* The data needs to be set to zero */
	ptr = calloc( nr*nc, sizeof(double) );	 /* matrix data	 */
	if (NULL == ptr)
	{
			printf("Error: calloc() failed at line %d of %s\n", __LINE__, __FILE__);
			exit(MALLOC_ERROR);
	}
	
	for (i=0; i<nr; i++){								/* set row pointers */
		M->mat[i] = ptr + nc*i;
	}
	return M;
}


/*---------------------------------------------------------------------------
	Release memory space used by a matrix	
	
	Where: Matrix *m - The matrix to free
	Errors: none
	Returns nothing
---------------------------------------------------------------------------*/
void m_free(Matrix* M) {

	int i;
	void *smallP;

	/* The pointer might have been rearranged, so find the smallest */
	smallP = (void *)M->mat[0];
	for (i=1; i<M->nr; i++) {
		if (smallP > (void *)M->mat[i]) {
			smallP = (void *)M->mat[i];
		}
	}

	free(smallP);		/* free data */
	free(M->mat);		/* free row pointers */
	free(M);				 /* free matrix header */
 

}

 
 
/*---------------------------------------------------------------------------
	Print the matrix elements 
					e.g	m_print(mat, " %8.4f ");

	Where: Matrix *m - The matrix to print
				 char *fs	- pointer to a printf format string
	Errors: none
	Returns nothing
---------------------------------------------------------------------------*/
void m_print(const Matrix* M, const char* fs) {
	int i,j;
	for (i=0; i<M->nr; ++i) 
	{ 
		for (j=0; j<M->nc; ++j)
		{				
				printf(fs, M->mat[i][j]);
		}
		putchar('\n');
	}
	putchar('\n');
}


/*---------------------------------------------------------------------------
	Allocates a real vector of size n, uninitialized
	
	Where: int n - The size of the vector to create
	Errors: none
	Returns rVector * - Pointer to the resulting reals vector
---------------------------------------------------------------------------*/
rVector* rv_alloc(int n) {
	rVector* r = malloc(sizeof(rVector));

	if(r == NULL) {
		fprintf(stderr, "Failed to alloc memory for rVector. Line: %d File: %s\n", 
				__LINE__, __FILE__);
		exit(MALLOC_ERROR);
	}

	r->vec = calloc(n, sizeof(double));
	if(r->vec == NULL) {
		fprintf(stderr, "Failed to alloc memory for rVector. Line: %d File: %s\n", 
				__LINE__, __FILE__);
		exit(CALLOC_ERROR);
	}
	
	r->n = n;
	return r;
}


/*---------------------------------------------------------------------------
	Allocates an integer vector of size n
	
	Where: int n - The size of the integer vector to create
	Errors: exits
	Returns iVector * - Pointer to the resulting integer vector
---------------------------------------------------------------------------*/
iVector* iv_alloc(int n) {
	iVector *i = malloc(sizeof(iVector));

	if(i == NULL) {
		fprintf(stderr, "Failed to alloc memory for rVector. Line: %d File: %s\n", 
				__LINE__, __FILE__);
		exit(MALLOC_ERROR);
	}

	i->ivec = calloc(n, sizeof(int));
	
	if(i->ivec == NULL) {
		fprintf(stderr, "Failed to alloc memory for rVector. Line: %d File: %s\n", 
				__LINE__, __FILE__);
		exit(CALLOC_ERROR);
	}
	
	i->n = n;
	return i;

}


/*---------------------------------------------------------------------------
	Frees a reals vector 
	
	Where: rVector *V - The real vector to release
	Errors: none
	Returns: nothing
---------------------------------------------------------------------------*/
void rv_free(rVector* V){
	free(V->vec);
	free(V);
}


/*---------------------------------------------------------------------------
	Frees an integer vector 
	
	Where: iVector *V - The integer vector to release
	Errors: none
	Returns: nothing
---------------------------------------------------------------------------*/
void iv_free(iVector* V){
	free(V->ivec);
	free(V);
}


/*---------------------------------------------------------------------------
	Prints a real vector 
				e.g rv_print(b, "	 %8.4f \n");
	
	Where: rVector *V - The reals vector to print
				 char *fs	- Pointer to a printf format string
	Errors: none
	Returns: nothing
---------------------------------------------------------------------------*/
void rv_print(const rVector* V, const char* fs){
	for(int i = 0; i< V->n; i++) {
		printf(fs, V->vec[i]);
	}
	putchar('\n');
}

/*---------------------------------------------------------------------------
	Prints an integer vector 
	
	Where: iVector *V - The integer vector to print
				 char *fs	- Pointer to a printf format string
	Errors: none
	Returns: nothing
---------------------------------------------------------------------------*/
/* Prints an ivector */
void iv_print(const iVector* V, const char* fs){
	for(int i = 0; i< V->n; i++) {
		printf(fs, V->ivec[i]);
	}
}



/*---------------------------------------------------------------------------
	Multiplies a matrix by a vector.	May not be needed
	
	Where: Matrix *M - Pointers to the matrix to multiply
				 rVector *V - Pointers to the vector to multiply
	Errors: none
	Returns: rVector * - Pointer to the resulting vector
---------------------------------------------------------------------------*/
rVector* MtimesV(Matrix* M, rVector* V){
	return NULL;
}
