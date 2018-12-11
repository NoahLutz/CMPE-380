/******************************************************************
 * Program to correct the data for the sensor
 * Note: Each student gets unique data, so this exact equation will
 * differ for each students solution
 * Be sure to use Honer's factorization  
 * ***************************************************************/

#include <stdio.h>
#include <stdlib.h>

#define order1(x) ((8.102579e-2 * x) + 9.019905)
#define order2(x) ((((-3.893806e-5 * x) + 2.560093e-1) * x) + -4.699772)
#define order3(x) ((((((-1.723046e-8 * x) + 7.515106e-5) * x) + 4.998119e-2 ) * x) + 3.701614)
#define order4(x) ((((((((-2.623831e-12 * x) + -1.015584e-9) * x) + 5.918942e-5) * x) + 1.040951e-2) * x) + 8.103289)
#define order5(x) ((((((((((-4.154367e-15 * x) + 2.917955e-11) * x) + -5.160138e-8) * x) + -4.506462e-5) * x) + 3.001885e-1) * x) + -5.829405e1)
#define order6(x) ((((((((((((-1.980393e-19 * x) + -4.345028e-15) * x) + 4.199799e-11) * x) + -9.280652e-8) * x) + -4.829757e-5) * x) + 4.277614e-1) * x) + -1.529549e2)


#define ROUND(x) (x>=0?(int)(x+0.5):(int)(x-0.5))
/* Runs the data through the fitting line */

int main(int argc, char *argv[]) {
	int res, real, ideal;
    
	while(scanf("%d %d", &ideal, &real) != EOF){
		/* Insert your polynomial here, be sure to round properly */
 		
		double corr = order5(real);
		//res = real - corr >= 0 ? (int)(corr+0.5) : (int)(corr-0.5);
		res = real - ROUND(corr);
 
		printf("%d %d\n", ideal, res);
	}
	return 0;
}

