/* ------------------------------------------------------------------------ */
/*																			*/
/*						Functions of k-decks for main.c						*/
/*						Author: Hanwen Yao									*/
/*						Last Updated: 05/18/2019							*/
/*																			*/
/* ------------------------------------------------------------------------ */

#include "main.h"

/* ------------------------------------------------------------------------ */
/*						Transition Matrix									*/
/* ------------------------------------------------------------------------ */

// Initialize the transition matrix from k-deck to s-deck
// Return a 2^s x 2^k matrix
// Need to free the space
int ** initTransitionMatrix (int k, int s)
{
	if (k < 2 || k >= 10 || s < 1 || s >= k)
	{
		printf("Error: k = %d, s = %d in function.c: ", k, s);
		printf("initTransitionMatrix, require 1 < k < 10, 0 < s < k.\n");
		return NULL;
	}

	// Calculate size of the matrix M
	int col = (1 << k);
	int row = (1 << s);

	// Initialize space for M
	int **M = (int **) malloc (row * sizeof(int *));
	for (int i = 0; i < row; i++)
		M[i] = (int *) malloc (col * sizeof(int));
	
	// Allocate space for kdeck, sdeck and indice for enumeration
	uchar *kdeck = (uchar *) malloc (k * sizeof(uchar));
	uchar *sdeck = (uchar *) malloc (s * sizeof(uchar));
	
	// Main loop to populate M
	// Sweep the rows
	for (int i = 0; i < row; i++)
	{
		// Initialize sdeck
		int temp = i;
		for (int t = 0; t < s; t++)
		{
			sdeck[t] = (temp % 2)?1:0;
			temp >>= 1;
		}

		// Sweep the columns
		for (int j = 0; j < col; j++)
		{
			// Initialize kdeck
			int temp = j;
			for (int t = 0; t < k; t++)
			{
				kdeck[t] = (temp % 2)?1:0;
				temp >>= 1;
			}

			// Calculate M[i][j], the occurrence of sdeck in kdeck
			M[i][j] = substringOccurrence (kdeck, k, sdeck, s);
		}
	}

	// Free the space
	free(kdeck);
	free(sdeck);

	return M;
}

// Print the transition matrix M
void printTransitionMatrix(int **M, int k, int s)
{
	// Calculate size of the matrix M
	int col = (1 << k);
	int row = (1 << s);

	// Print title:
	printf("Transition matrix M:\n");
	for (int i = 0; i < s; i++)
		printf(" ");
	printf("  ");
	for (int i = 0; i < col; i++)
	{
		int temp = i;
		for (int t = 0; t < k; t++)
		{
			printf("%u",(temp % 2)?1:0);
			temp >>= 1;
		}
		printf(" ");
	}
	printf("\n");

	for (int i = 0; i < row; i++)
	{
		int temp = i;
		for (int t = 0; t < s; t++)
		{
			printf("%u", (temp % 2)?1:0);
			temp >>= 1;
		}
		printf(": ");

		// Sweep the columns
		for (int j = 0; j < col; j++)
		{
			for (int t = 0; t < k-2; t++)
				printf(" ");
			printf("%2d ", M[i][j]);
		}
		printf("\n");
	}
	printf("\n");

}

// Free the transition matrix M from k-deck to s-deck
void freeTransitionMatrix (int **M, int s)
{
	// Calculate rows of the matrix M
	int row = (1 << s);

	for (int i = 0; i < row; i++)
		free(M[i]);
	free(M);

	return;
}

// Calculate occurrence of B string in A string
// B has length k and A has length n
int substringOccurrence (uchar *A, int n, uchar *B, int k)
{
	if (k >= n || k < 1)
	{
		printf("Warning: n = %d, k = %d in function.c: ",n, k);
		printf("substringOccurrence, require 0 < k < n.\n");
		return 0;
	}

	// Initialize indice for enumeration of n choose k
	int *indice = (int *) malloc (k * sizeof(int));
	for (int i = 0; i < k; i++)
		indice[i] = i;

	// Count the occurrence of B
	int cnt = 0;
	int flag, j;

	do
	{
		// Check if substring of A match B
		flag = 0;
		for (int i = 0; i < k; i++)
//			printf("%u ", A[indice[i]]);
//		printf("\n");
			if (A[indice[i]] != B[i])
			{
				flag = 1;
				break;
			}

		cnt += (flag)?0:1;

		// Increment indice by one
		flag = 0;
		for (int i = k-1; i >= 0; i--)
		{
			if (indice[i] < (n - 1) - (k - 1) + i)
			{
				j = i;
			flag = 1;
			break;
			}
		}

		indice[j] += 1;
		if (flag)
			for (int i = j+1; i < k; i++)
				indice[i] = indice[i-1] + 1;

	}while(flag);

	// Free the space
	free(indice);

	return cnt;
}	

/* ------------------------------------------------------------------------ */
/*						K-Deck												*/
/* ------------------------------------------------------------------------ */

// extend the k-deck of a length n string by a 0 or 1 bit at the end
void extKdeck (int *arr_old, int *arr_new, int **M, int n, int k, int bit)
{
	if (k < 2)
	{
		printf("Error: k = %d in function.c: ", k);
		printf("extKdeck, require 2 <= k.\n");
	}

	int len = (1 << k);			// Length of k-deck
	for (int i = 0; i < len; i++)
		arr_new[i] = arr_old[i];

	int inc = (bit)?(len/2):0;	// Increment index depending on bit
	int temp;
	for (int i = 0; i < len/2; i++)
	{
		temp = 0;
		for (int j = 0; j < len; j++)
			temp += arr_old[j] * M[i][j];
		arr_new[i + inc] += temp/(n - k + 1);
	}
}

// Calculate the divergence number of k-deck
void calDiv (int k, int n)
{
	if (k > n || k <= 1)
	{
		printf("Error: k = %d, n = %d in function.c: ", k, n);
		printf("calDiv, require 1 < k <= n.\n");
	}

	// Initialization
	int len = (1 << k);			// Length of k-deck
	int num = (1 << k);			// Number of different k-decks
	int NUM = (1 << 24);
	int **L = (int **) malloc (len * sizeof(int *));
	for (int i = 0; i < len; i++)
	{
		L[i] = (int *) malloc (len * sizeof(int));
		for (int j = 0; j < len ; j++)
			L[i][j] = (i == j)?1:0;
	}

	// Indicator
	int tofile = 1;				// Indicate list to file
	int filenum, filenum_new;
	int *filecnt, *filecnt_new;

	// Calculate transition matrix
	int **M = initTransitionMatrix (k, k-1);

	// Print the title
	printf("--------------------------------------------------\n");
	printf("                    %d-DECK                       \n",k);
	printf("--------------------------------------------------\n");
	time_t START = time(0);

	// Main Loop
	for (int i = k; i < n; i++)
	{
		// Record time
		time_t start = time(0);

		// Calculate new list
		if (num < NUM)
		{
			int **L_new = (int **) malloc (2 * num * sizeof(int *));
			calDeckListList(L, L_new, &num, M, i, k);
			L = L_new;
		}
		else
		{
			printf("To file...\n");
			if (tofile)
			{
				filenum = (num * 2) / NUM + 1;
				filecnt = (int *) malloc (filenum * sizeof(int));
				printf("filenum = %d\n", filenum);
				calDeckListFile(L, &num, filenum, filecnt, M, i, k);
				tofile = 0;
			}
			else
			{
				filenum_new = (num * 2) / NUM + 1;
				filecnt_new = (int *) malloc (filenum_new * sizeof(int));
				printf("filenum = %d\n", filenum_new);
				calDeckFileFile(filenum, filecnt, filenum_new, filecnt_new, &num, M, i, k);

				free(filecnt);
				filecnt = filecnt_new;
				filenum = filenum_new;
			}
		}

		// Print the result
		printf("Total time: ");
		printTime(start,time(0)); printf(": ");
		printf("D[%d,%d] = %d\n", k, i+1, num);
	}
	printf("TOTAL TIME: ");
	printTime(START,time(0)); printf("\n");

	// Free the space
	if (tofile)
	{
		for (int i = 0; i < num; i++)
			free(L[i]);
		free(L);
	}
	else
		free(filecnt);

	// Free the transition matrix
	freeTransitionMatrix (M, k-1);
}

/* ------------------------------------------------------------------------ */
/*						Hash Functions										*/
/* ------------------------------------------------------------------------ */

// from one-at-a-time hash from
// http://www.burtleburtle.net/bob/hash/doobs.html
uint hashKey (int *array, int len)
{
	uint hash = 0;
	for (int i = 0; i < len; i++)
	{
		hash += array[i];
    	hash += (hash << 10);
    	hash ^= (hash >> 6);
	}
  	hash += (hash << 3);
  	hash ^= (hash >> 11);
  	hash += (hash << 15);

	return hash;
}

// from one-at-a-time hash from
// http://www.burtleburtle.net/bob/hash/doobs.html
uint hashKey2 (int *array, int len)
{
	uint hash = 0;
	for (int i = len - 1; i >= 0; i--)
	{
		hash += array[i];
    	hash += (hash << 10);
    	hash ^= (hash >> 6);
	}
  	hash += (hash << 3);
  	hash ^= (hash >> 11);
  	hash += (hash << 15);

	return hash;
}

int stringIsSame(int *array1, int *array2, int len)
{
	for (int i = 0; i < len; i++)
		if (array1[i] != array2[i])
			return 0;
	return 1;
}

// Use a hash table for calculating new k-decks
// L_old has length n
void calDeckListList(int **L_old, int **L_new, int *num, int **M, int n, int k)
{
	// Set global parameter for hash tables
	uint HASHSIZE;
	int hash_print = 0;
	if ((1 << 30) * 0.01 < (*num))
	{
		HASHSIZE = (1 << 30);  // 32 * 4 = 128 MB
		hash_print = 1;
	}
	else
	{
		HASHSIZE = 1;
		while (HASHSIZE < (*num) * 100)
			HASHSIZE <<= 1;
	}
	uint HASHMASK = HASHSIZE - 1;

	// Record the time
	time_t start = time(0);

	// Initialize hash table
	int *hashTable = (int *) malloc (HASHSIZE * sizeof(int));
	for (int i = 0; i < HASHSIZE; i++)
		hashTable[i] = -1;

	// Hashing
	int duplicates = 0;
	int collisions = 0;
	int firstcollisions = 0;
	int flag = 0;
	int rep = 0;

	// Initialize a temp k-deck
	int len = (1 << k);
	int *kdeck = (int *) malloc (len * sizeof(int));
	int j = 0;

	for (int i = 0; i < *num; i++)
	{
		for (int b = 0; b < 2; b++)
		{
			// Calculate new k-deck
			extKdeck ( L_old[i], kdeck, M, n, k, b);
	
			// Hashing
			uint key = hashKey (kdeck, len) & HASHMASK;
			flag = 0;
			rep = 0;
	
			while(1)
			{
				if (hashTable[key] == -1)		// slot is available
				{
					hashTable[key] = j;			// means the number appears first time
					break;
				}
				else
				{
					if (stringIsSame (L_new[hashTable[key]], kdeck, len))
					{
						duplicates ++;				// Duplicates
						rep = 1;
						break;
					}
					else
					{
						collisions ++;
						firstcollisions += (flag)?0:1;
						key = (key + 1) & HASHMASK;
						flag = 1;
					}
				}
			}
	
			// Add kdeck into L_new
			if (rep == 0)
			{
				L_new[j] = (int *) malloc (len * sizeof(int));
				for (int t = 0; t < len; t++)
					L_new[j][t] = kdeck[t];
				j++;
			}
		}
		
		// Free the space
		free(L_old[i]);
	}

	// Print the hashtable report
	if (hash_print)
	{
		printf("----------------------\n");
		printf("---HASH TABLE REPORT:\n");
		printf("---Hash table size: %d; Array size: %d; ", HASHSIZE, *num * 2);
		printf("Firstcollisions: %d; Collisions: %d; Duplicates: %d\n", firstcollisions, collisions, duplicates);
		printf("---Time used: "); printTime(start,time(0)); printf("\n");
		if ((HASHSIZE >> 20)*4 < 1)
			printf("---Memory used: <1MB\n");
		else
			printf("---Memory used: %dMB\n", (HASHSIZE >> 20)*4);
		printf("----------------------\n");
	}

	// Free the space and update
	free(hashTable);
	free(L_old);
	*num = j;

	return;
}

/* ------------------------------------------------------------------------ */
/*							To File											*/
/* ------------------------------------------------------------------------ */

// Calculate the new k-deck from list L of length n to files
// filecnt is a list of integers of length file_number
// it counts the number of k-decks in each file
void calDeckListFile(int **L, int *num, int filenum, int *filecnt, int **M, int n, int k)
{
	time_t start = time(0);

	// Calculate length of k-deck
	int len = (1 << k);

	// Allocate space
	FILE **file_array = (FILE **) malloc (filenum * sizeof (FILE*));
	char filename[100];

	// Make a new directory
	sprintf(filename, "KDECK_PROCESS");
	mkdir(filename, 0777);
	sprintf(filename, "KDECK_PROCESS/%d", n+1);
	mkdir(filename, 0777);

	// Open files
	for (int t = 0; t < filenum; t++)
	{
		sprintf(filename, "KDECK_PROCESS/%d/%d", n+1, t);
		file_array[t] = fopen(filename, "w");
	}

	// Initialize filecnt 
	for (int i = 0; i < filenum; i++)
		filecnt[i] = 0;

	// Distribute k-decks
	int *kdeck = (int *) malloc (len * sizeof(int));
	int idx;

	for (int j = 0; j < *num; j++)		// From L
	{
		for (int b = 0; b < 2; b++)
		{
			extKdeck( L[j], kdeck, M, n, k, b);
			idx = hashKey2( kdeck, len) % filenum;

			printDecktoFile(file_array[idx], kdeck, len);
			filecnt[idx] += 1;
		}
		free(L[j]);
	}
	free(L);
	
	printf("Filecnt: ");
	for (int t = 0; t < filenum; t++)
		printf("%d ", filecnt[t]);
	printf("\nTime used for calculating to files: "); 
	printTime(start, time(0)); printf("\n");

	// Close the files
	for (int t = 0; t < filenum; t++)
		fclose(file_array[t]);
	free(file_array); 

	// Remove duplicates and update sum
	printf("Remove duplicates...\n");
	start = time(0);
	*num = 0;
	for (int t = 0; t < filenum; t++)
	{
		time_t s = time(0);
		removeDuplicateFile(t, filecnt + t, n+1, k);
		*num += filecnt[t];
		printf("file %d, time: ", t); printTime(s,time(0)); printf("\n");
	}
	printf("Total time for removing duplicates: "); 
	printTime(start,time(0)); printf("\n");

	// Free the space
	free(kdeck);
}

// Calculate the new k-deck of length n from files to files
// filecnt is a list of integers of length file_number
// it counts the number of k-decks in each file
void calDeckFileFile(int filenum, int *filecnt, int filenum_new, int *filecnt_new, 
		int *num, int **M, int n, int k)
{
	// Calculate length of k-deck
	int len = (1 << k);
	time_t start = time(0);

	// Allocate space
	FILE **file_array = (FILE **) malloc (filenum_new * sizeof (FILE*));
	char filename[100];

	// Make a new directory
	sprintf(filename, "KDECK_PROCESS/%d", n+1);
	mkdir(filename, 0777);

	// Open files
	for (int t = 0; t < filenum_new; t++)
	{
		sprintf(filename, "KDECK_PROCESS/%d/%d", n+1, t);
		file_array[t] = fopen(filename, "w");
	}

	// Initialize filecnt_new
	for (int i = 0; i < filenum_new; i++)
		filecnt_new[i] = 0;

	// Distribute k-decks
	int *kdeck = (int *) malloc (len * sizeof(int));
	int *kdeck_new = (int *) malloc (len * sizeof(int));
	int idx;

	for (int f = 0; f < filenum; f++)
	{
		sprintf(filename, "KDECK_PROCESS/%d/%d", n, f);
		FILE *fptr = fopen(filename, "r");

		for (int j = 0; j < filecnt[f]; j++)		// From file
		{
			// Read the k-deck
			readDeckfromFile(fptr, kdeck, len);

			// Calculate new k-deck
			for (int b = 0; b < 2; b++)
			{
				extKdeck(kdeck, kdeck_new, M, n, k, b);
				idx = hashKey2(kdeck_new, len) % filenum_new;
	
				printDecktoFile(file_array[idx], kdeck_new, len);
				filecnt_new[idx] += 1;
			}
		}

		fclose(fptr);
		remove(filename);
	}
	// Delete folder
	sprintf(filename, "KDECK_PROCESS/%d", n);
	remove(filename);

	// Close the file
	for (int t = 0; t < filenum_new; t++)
		fclose(file_array[t]);
	free(file_array); 

	printf("Filecnt: ");
	for (int t = 0; t < filenum_new; t++)
		printf("%d ", filecnt_new[t]);
	printf("\nTime used for calculating to files: "); 
	printTime(start, time(0)); printf("\n");

	// Remove duplicates and update sum
	*num = 0;
	printf("Remove duplicates...\n");
	start = time(0);
	for (int t = 0; t < filenum_new; t++)
	{
		time_t s = time(0);
		removeDuplicateFile(t, filecnt_new + t, n+1, k);
		*num += filecnt_new[t];
		printf("file %d, time: ", t); printTime(s,time(0)); printf("\n");
	}
	printf("Total time for removing duplicates: "); 
	printTime(start,time(0)); printf("\n");

	// Free the space
	free(kdeck);
	free(kdeck_new);
}


// Remove k-deck duplicates in a file using hash table
void removeDuplicateFile(int t, int *cnt, int n, int k)
{

	// Record the time
	time_t start = time(0);

	char filename[100], filename_new[100];
	sprintf(filename, "KDECK_PROCESS/%d/%d", n, t);
	sprintf(filename_new, "KDECK_PROCESS/%d/%d_new", n, t);
	FILE *fptr = fopen(filename, "r");
	FILE *fptr_new = fopen(filename_new, "w");

	int len = (1 << k);
	int **L = (int **) malloc ((*cnt) * sizeof(int*));
	int *kdeck = (int *) malloc (len * sizeof(int));
	int j = 0;

	// Set global parameter for hash tables
	uint HASHSIZE;
	int hash_print = 0;
	if ((1 << 30) * 0.01 < (*cnt))
	{
		HASHSIZE = (1 << 30);  // 32 * 4 = 128 MB
		hash_print = 1;
	}
	else
	{
		HASHSIZE = 1;
		while (HASHSIZE < (*cnt) * 100)
			HASHSIZE <<= 1;
	}
	uint HASHMASK = HASHSIZE - 1;

	// Initialize hash table
	int *hashTable = (int *) malloc (HASHSIZE * sizeof(int));
	for (int i = 0; i < HASHSIZE; i++)
		hashTable[i] = -1;

	// Hashing
	int duplicates = 0;
	int collisions = 0;
	int firstcollisions = 0;
	int flag = 0;
	int rep = 0;

	for (int i = 0; i < *cnt; i++)
	{
		readDeckfromFile(fptr, kdeck, len);
		uint key = hashKey (kdeck, len) & HASHMASK;		// Hashing
		flag = 0;
		rep = 0;

		while(1)
		{
			if (hashTable[key] == -1)		// slot is available
			{
				hashTable[key] = j;			// means the number appears first time
				break;
			}
			else
			{
				if (stringIsSame (L[hashTable[key]], kdeck, len))
				{
					duplicates ++;				// Duplicates
					rep = 1;
					break;
				}
				else
				{
					collisions ++;
					firstcollisions += (flag)?0:1;
					key = (key + 1) & HASHMASK;
					flag = 1;
				}
			}
		}

		if (rep == 0)			// Add kdeck into L_new
		{
			L[j] = (int *) malloc (len * sizeof(int));
			for (int t = 0; t < len; t++)
				L[j][t] = kdeck[t];
			printDecktoFile(fptr_new, kdeck, len);
			j++;
		}
	}

	// Print the hashtable report
	if (hash_print)
	{
		printf("----------------------\n");
		printf("---HASH TABLE REPORT:\n");
		printf("---Hash table size: %d; Array size: %d; ", HASHSIZE, *cnt);
		printf("Firstcollisions: %d; Collisions: %d; Duplicates: %d\n", firstcollisions, collisions, duplicates);
		printf("---Time used: "); printTime(start,time(0)); printf("\n");
		if ((HASHSIZE >> 20)*4 < 1)
			printf("---Memory used: <1MB\n");
		else
			printf("---Memory used: %dMB\n", (HASHSIZE >> 20)*4);
		printf("----------------------\n");
	}

	// Free the space and update
	*cnt = j;

	fclose(fptr);
	fclose(fptr_new);
	remove(filename);
	rename(filename_new,filename);
	
	free(hashTable);
	for (int i = 0; i < j; i++)
		free(L[i]);
	free(L);

	return;
}

// Print the k-deck from deck to fptr
void printDecktoFile(FILE *fptr, int *kdeck, int len)
{
	for (int i = 0; i < len; i++)
		fprintf (fptr, "%d ", kdeck[i]);
	fprintf(fptr, "\n");
}

// Read the k-deck from fprt to deck
void readDeckfromFile(FILE *fptr, int *kdeck, int len)
{
	for (int i = 0; i < len; i++)
		fscanf (fptr, "%d", kdeck+i);
}
