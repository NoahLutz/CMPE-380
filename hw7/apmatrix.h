#ifndef _APMATRIX_H_
#define _APMATRIX_H_
/* Applied Programming: Matrix Module *
 *
 * Revised: JCCK 4/6/2015
 *          RHR  11/06/2015 
 *          RHR  double Real to double, Vector to rVector
 *          RHR  11/01/2016 - Removed m_ident(), and made
 *                            minor comment changes
 * 04/06/2017   R. Repka    Minor commend modification G to A
 */
 
#include <stdio.h>
#include <stdlib.h>

/* integer vector definition */
typedef struct {
    unsigned int n;
    int *ivec;
    } iVector;

/* vector data type definition */
typedef struct {
    unsigned int n;
    double *vec;
    } rVector;

/* matrix data type definition */
typedef struct { /* matrix data structure */
   unsigned int nr;   /* number or rows     */
   unsigned int nc;   /* number of columns  */   
   double **mat;      /* Pointer to array that holds data  */
  } Matrix;


/* Function Prototypes */
/* Implements in place Gaussian elimination */
int PLU_factor(Matrix* A, iVector* p);
void PLU_solve(Matrix* A, iVector* p, rVector* b, rVector* x);

/* Matrices */
Matrix* m_alloc(int nr,int nc);
void m_free(Matrix* M);
void m_print(const Matrix* M, const char* fs);

/* Vectors */
rVector* rv_alloc(int n);
iVector* iv_alloc(int n);
void rv_free(rVector* V);
void iv_free(iVector* V);
void rv_print(const rVector* V, const char* fs);
void iv_print(const iVector* V, const char* fs);
#endif
