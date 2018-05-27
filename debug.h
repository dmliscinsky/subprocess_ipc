/*
 * 
 * 
 * Author: dmliscinsky
 */



#define DEBUG

#ifdef DEBUG

#include <stdio.h>


#define DEBUG_perror(str) (perror(str));
#else
#define DEBUG_perror(str) ;
#endif


