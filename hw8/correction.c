/******************************************************************
 * Program to correct the data for the sensor
 * Note: Each student gets unique data, so this exact equation will
 * differ for each students solution
 * Be sure to use Honer's factorization  
 * ***************************************************************/

#include <stdio.h>
#include <stdlib.h>

/* Runs the data through the fitting line */

int main(int argc, char *argv[])
{
    int res, real, ideal;
    
    while(scanf("%d %d", &ideal, &real) != EOF)
    {
     /* Insert your polynomial here, be sure to round properly */
 
 
        printf("%d %d\n", ideal, res);
    }
    return 0;
}
