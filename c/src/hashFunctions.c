/* ------------------------------------------------------------------------ */
/*																			*/
/*						Hash Functions for main.c							*/
/*						Author: Hanwen Yao									*/
/*						Last Updated: 05/20/2019							*/
/*																			*/
/* ------------------------------------------------------------------------ */

#include "main.h"

//// from one-at-a-time hash from
//// http://www.burtleburtle.net/bob/hash/doobs.html
//uint hashKey (int *array, int len)
//{
//	uint hash = 0;
//	for (int i = 0; i < len; i++)
//	{
//		hash += array[i];
//    	hash += (hash << 10);
//    	hash ^= (hash >> 6);
//	}
//  	hash += (hash << 3);
//  	hash ^= (hash >> 11);
//  	hash += (hash << 15);
//
//	return hash;
//}
//
//// from one-at-a-time hash from
//// http://www.burtleburtle.net/bob/hash/doobs.html
//uint hashKey2 (int *array, int len)
//{
//	uint hash = 0;
//	for (int i = len - 1; i >= 0; i--)
//	{
//		hash += array[i];
//    	hash += (hash << 10);
//    	hash ^= (hash >> 6);
//	}
//  	hash += (hash << 3);
//  	hash ^= (hash >> 11);
//  	hash += (hash << 15);
//
//	return hash;
//}
//
//int stringIsSame(int *array1, int *array2, int len)
//{
//	for (int i = 0; i < len; i++)
//		if (array1[i] != array2[i])
//			return 0;
//	return 1;
//}
//
///* ------------------------------------------------------------------------ */
///*						Hash Functions										*/
///* ------------------------------------------------------------------------ */
//
//void checkDuplicatesFile (FILE *fptr, int num, int len)
//{
//	int **L = (int **) malloc (num * sizeof(int*));
//	for (int i = 0; i < num; i++)
//	{
//		L[i] = (int *) malloc (len * sizeof(int));
//		readDeckfromFile (fptr, L[i], len);
//	}
//
//	// Check
//	checkDuplicates( L, num, len);
//
//	// Free the space
//	for (int i = 0; i < num; i++)
//		free(L[i]);
//	free(L);
//}
//
//void checkDuplicates (int **L, int num, int len)
//{
//	if (num > 0.1 * HASHSIZE)
//	{
//		printf("Warning: num = %d, HASHSIZE = %d in hashFunctions.c:", num, HASHSIZE);
//		printf("checkDuplicates: num > 0.1 * HASHSIZE.\n");
//	}
//
//	// Record the time
//	time_t start = time(0);
//
//	// Initialize hash table
//	int *hashTable = (int *) malloc (HASHSIZE * sizeof(int));
//	for (int i = 0; i < HASHSIZE; i++)
//		hashTable[i] = -1;
//
//	// Hashing
//	int duplicates = 0;
//	int collisions = 0;
//	int firstcollisions = 0;
//	int flag = 0;
//
//	for (int i = 0; i < num; i++)
//	{
//		uint key = hashKey (L[i], len) & HASHMASK;
//		flag = 0;
//		while(1)
//		{
//			if (hashTable[key] == -1)		// slot is available
//			{
//				hashTable[key] = i;			// means the number appears first time
//				break;
//			}
//			else
//			{
//				if (stringIsSame (L[hashTable[key]], L[i], len))
//				{
//					duplicates ++;				// Duplicates
//
//					// Print out duplicates
//					printf("Duplicates: ");
//					for (int j = 0; j < len; j++)
//						printf("%d ", L[i][j]);
//					printf("\n");
//
//					break;
//				}
//				else
//				{
//					collisions ++;
//					firstcollisions += (flag)?0:1;
//					key = (key + 1) & HASHMASK;
//					flag = 1;
//				}
//			}
//		}
//	}
//
//	// Print the hashtable report
//	printf("----------------------\n");
//	printf("---HASH TABLE REPORT:\n");
//	printf("---Hash table size: %d; Array size: %d; ", HASHSIZE, num);
//	printf("Firstcollisions: %d; Collisions: %d; Duplicates: %d\n", firstcollisions, collisions, duplicates);
//	printf("---Time used: "); printTime(start,time(0)); printf("\n");
//	if ((HASHSIZE >> 20)*4 < 1)
//		printf("---Memory used: <1MB\n");
//	else
//		printf("---Memory used: %dMB\n", (HASHSIZE >> 20)*4);
//	printf("----------------------\n");
//
//	// Free the space
//	free(hashTable);
//
//	return;
//}
