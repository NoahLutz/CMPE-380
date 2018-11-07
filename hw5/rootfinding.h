#ifndef _ROOTFINDING_H_
#define _ROOTFINDING_H_
/*****************************************************************************
 Header file for root-finding C-module
 Course: Applied Programming
 Adapted to ANSI C by Juan C. Cockburn
 
 Module provides: bisection, newton, secant
 12/28/2015 - R. Repka - added integer solution features
 06/05/2017 - R. Repka - added iNAN
 07/21/2017 - R. Repka - Switched to ClassError.h
*****************************************************************************/
 
#include <math.h>

/****************************************************************************
  This defines the number of Qn fractional digits for the integer implementation
  Change this based on your calculations and defend your decision in the
  analysis.txt file
 ***************************************************************************/
#define QN 26

/* Macros */
#define SIGN(x) ((x) < 0 ? -1 :1)



/****************************************************************************
  These simplified Qn format macros, assume a constant Qn factor 
 ***************************************************************************/
#define QBASE long
#define FLOAT_TO_FIX(x) ( (QBASE) (x * (QBASE) ((QBASE)1 << QN)) )
#define FIX_TO_FLOAT(X) ((double) (X) / (double) ((QBASE)1 << QN))
#define Qn_DIVIDE(A,B)  ( (QBASE) ( ((QBASE)(A)<<QN)/(B) ))
#define MUL_FIX(A,B)    ((A) * (B) >> QN)


/* Typemark for a scalar function of one variable */
typedef double (*func1arg) (double);
typedef long (*ifunc1arg) (long);

/* Function prototypes */
double func1(double x);
double rline(double x);
long iline(long x);
double func1Deriv(double x);
double bisection ( func1arg f, double a, double b, double atol , int verb);
long ibisection ( ifunc1arg f, long a, long b, long atol , int verb);
double newton ( func1arg f, func1arg df, double x0, int Nmax, double atol, int verb);
double secant ( func1arg f, double x0, double x1, int Nmax, double atol, int verb);

#endif /* _ROOTFINDING_H_ */
