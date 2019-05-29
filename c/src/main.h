/* ------------------------------------------------------------------------ */
/*                                                                          */
/*                      Header File for main.c								*/
/*                      Author: Hanwen Yao                                  */
/*                      Last Updated: 05/18/2019                            */
/*                                                                          */
/* ------------------------------------------------------------------------ */

#ifndef K_DECK
#define	K_DECK

#include <stdio.h>
#include <stdlib.h> // rand()
#include <time.h>   // time()

#include <sys/stat.h>	// mkdir

typedef unsigned char uchar;
typedef unsigned int uint;

/* ---------- kdeckFunctios.c ---------- */

/* Transition Matrix M */
int ** initTransitionMatrix (int k, int s);
void printTransitionMatrix(int **M, int k, int s);
void freeTransitionMatrix (int **M, int s);
int substringOccurrence (uchar *A, int n, uchar *B, int k);

/* K-Deck */
void extKdeck (int *arr_old, int *arr_new, int **M, int n, int k, int bit);
void calDiv (int k, int n);

/* Hash Functions */ 
uint hashKey (int *array, int len);
uint hashKey2 (int *array, int len);
int stringIsSame(int *array1, int *array2, int len);
void calDeckListList(int **L_old, int **L_new, int *num, int **M, int n, int k);


/* To File */
void calDeckListFile(int **L, int *num, int filenum, int *filecnt, int **M, int n, int k);
void calDeckFileFile(int filenum, int *filecnt, int filenum_new, int *filecnt_new,
		int *num, int **M, int n, int k);
void removeDuplicateFile(int t, int *cnt, int n, int k);
void printDecktoFile(FILE *fptr, int *kdeck, int len);
void readDeckfromFile(FILE *fptr, int *kdeck, int len);

/* ---------- msc.c ---------- */

/* Msc */
void printTime(time_t start_t, time_t end_t);

#endif
