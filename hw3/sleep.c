/*---------------------------------------------------------------------------
  sleep.c - student file
  01/01/2016    R. Repka    This code is only used to verify your timing macros
  02/202016     R. Repka    Added include file hint
---------------------------------------------------------------------------*/
#include <unistd.h>
#include <stdio.h>
/* add other include files as necessary */

int main()
   {
   clock_t end_t;
   int delay;

   printf("Start\n");
   /* Your macro stuff here */
   
   /* wait for 60 seconds */
   end_t = clock() + 60 * CLOCKS_PER_SEC;
   while (end_t > clock())
      {
      /* Consume CPU time */
      delay = 1<<19;
      while (delay) 
         {
         delay--;
         }
      }
   
   /* more of your macro stuff */   
   
   printf("End\n");
   return 0;
}
