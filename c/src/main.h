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

/* Transition Matrix M */
int ** initTransitionMatrix (int k, int s);
void printTransitionMatrix(int **M, int k, int s);
void freeTransitionMatrix (int **M, int s);
int substringOccurrence (uchar *A, int n, uchar *B, int k);

/* K-Deck */
void extKdeck (int *arr_old, int *arr_new, int **M, int n, int k, int bit);
void calDiv (int k, int n);

void printDecktoFile(FILE *fptr, int *deck, int len);
void readDeckfromFile(FILE *fptr, int *deck, int len);
void calDeckListList(int **L_old, int **L_new, int num_old, int **M, int n, int k);
void calDeckListFile(int **L, int num, int file_number, int *file_count, int **M, int n, int k);
void calDeckFileFile(int filenum_old, int filenum_new, int *filecnt_old, int *filecnt_new, int **M, int n, int k);
void checkDuplicatesinFiles (int filenum, int *filecnt, int n, int len);

/* Hash Function */ 
void checkDuplicates(int **L, int num, int len); 
uint hashKey (int *array, int len);
uint hashKey2 (int *array, int len);

/* Print Time */
void printTime(time_t start_t, time_t end_t);

#endif
