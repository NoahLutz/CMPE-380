/*---------------------------------------------------------------------------
  val.c - This program demonstrats VALGRIND returning programming errors
  Student copy
---------------------------------------------------------------------------*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define NUM_ELEMS (5)

int main(){

    int i = 0;
    int *num;
    
    num = malloc(sizeof(int)*NUM_ELEMS);
	memset(num, 0, sizeof(int)*NUM_ELEMS);
    num [0] = 0;

    printf("The first numbers are %d %d\n", num[0], num[1]);

    /* Init the number */
    for (i = 2; i < NUM_ELEMS; i++) {
       num[i] = i;
    }
    
    
    printf("The next numbers are %d %d\n", num[2], num[3]);
    free(num);
    


return(0);
}
