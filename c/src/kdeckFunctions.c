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
void calDiv_temp (int k, int n)
{
	int filecnt_old[128] = {4194561,4195715,4191955,4195790,4194999,4194943,4191879,
		4198411,4195880,4198314,4193539,4194466,4195425,4192449,4197746,4196359,
		4192660,4196408,4197060,4192867,4195395,4191534,4192325,4194195,4195164,
		4195118,4190119,4193371,4191994,4194001,4194030,4194035,4195013,4194448,
		4195366,4194696,4196197,4195023,4193363,4194070,4198265,4196153,4196221,
		4194833,4194120,4193316,4195113,4194721,4189606,4195580,4195908,4190260,
		4191322,4193874,4193314,4194228,4190969,4192715,4194759,4194007,4197514,
		4193245,4193958,4194662,4192812,4194699,4193433,4194625,4194896,4193035,
		4195035,4193113,4193251,4191862,4194870,4195552,4198318,4194983,4196926,
		4195462,4194346,4195838,4191661,4196046,4193754,4193049,4191320,4195248,
		4195097,4193886,4195208,4191951,4193186,4193460,4191466,4196398,4193078,
		4192810,4192985,4194711,4193578,4193390,4192893,4200483,4195937,4193971,
		4194160,4191397,4194892,4195945,4192358,4197787,4192610,4194437,4195367,
		4191618,4194635,4197471,4191725,4193824,4195876,4193225,4197344,4191927,
		4193219,4192681,4194262,4195984};

	int len = (1 << k);			// Length of k-deck

	// Calculate transition matrix
	int **M = initTransitionMatrix (k, k-1);

	// Print the title
	printf("--------------------------------------------------\n");
	printf("                    %d-DECK, 29-30                \n",k);
	printf("--------------------------------------------------\n");

	// Record time
	printf("n = %d\n", 30);
	time_t start = time(0);

	// Initialize
	int file_number = 128;
	int *filecnt_new = (int *) malloc (file_number * sizeof(int));
	printf("file_number = %d\n", file_number);

	calDeckFileFile(file_number, file_number, filecnt_old, filecnt_new, M, 29, k);

	// Print
	printf("K-Deck Calculation Time (to files): "); printTime(start,time(0)); printf("\n");
	printf("Number of k-decks in each file:");
	for (int j = 0; j < file_number; j++)
		printf("%d,", filecnt_new[j]);
	printf("\n");

	// Check duplicates in each file
	start = time(0);
	checkDuplicatesinFiles (file_number, filecnt_new, 30, len);
	printf("Total time for checking duplicates"); printTime(start,time(0)); printf("\n");

	// Free the space
	free(filecnt_new);
	freeTransitionMatrix (M, k-1);
}

// Calculate the divergence number of k-deck
void calDiv (int k, int n)
{
	if (k > n || k <= 1)
	{
		printf("Error: k = %d, n = %d in function.c: ", k, n);
		printf("calDiv, require 1 < k <= n.\n");
	}

	// Create a folder
	int status = mkdir("KDECK_PROCESS",0777);
	if (status)
	{
		printf("Warning: in kdeckFunctions.c");
		printf("error creating folder 'KDECK_PROCESS'\n");
	}
	int *filecnt_new;		// count the # of k-decks in each file
	int *filecnt_old;

	// Initialize 2^k strings
	int len = (1 << k);			// Length of k-deck
	int num = (1 << k);			// Number of different k-decks
	// int NUM = (1 << 22);			// the ave number of k-decks in a file
	int NUM = (1 << 6);			// the ave number of k-decks in a file
	int **L = (int **) malloc (len * sizeof(int *));
	uchar L_isfreed = 0;
	for (int i = 0; i < len; i++)
	{
		L[i] = (int *) malloc (len * sizeof(int));
		for (int j = 0; j < len ; j++)
			L[i][j] = (i == j)?1:0;
	}

	// Calculate transition matrix
	int **M = initTransitionMatrix (k, k-1);

	// Print the title
	printf("--------------------------------------------------\n");
	printf("                    %d-DECK                       \n",k);
	printf("--------------------------------------------------\n");

	// Main Loop
	for (int i = 29; i < n; i++)
	{
		// Record time
		printf("n = %d\n", i+1);
		time_t start = time(0);

		if (num < NUM)
		{
			// Allocate space for new list
			int **L_new = (int **) malloc (2 * num * sizeof(int *));
			for (int j = 0; j < 2 * num; j++)
				L_new[j] = (int *) malloc (len * sizeof(int));

			// Calculate new list
			calDeckListList(L, L_new, num, M, i, k);

			// Free the old array
			for (int i = 0; i < num; i++)
				free(L[i]);
			free(L);
	
			// Update
			L = L_new;
			num *= 2;
	
			// Check duplicates
			printf("K-Deck Calculation Time: ");
			printTime(start,time(0));
			printf("\n");
			checkDuplicates(L, num, len);
		}
		else
		{
			// Initialize
			int file_number = 2*num/NUM;
			filecnt_new = (int *) malloc (file_number * sizeof(int));
			printf("file_number = %d\n", file_number);

			if (file_number == 2)
			{
				calDeckListFile(L, num, file_number, filecnt_new, M, i, k);

				// Free L
				for (int i = 0; i < num; i++)
					free(L[i]);
				free(L);
				L_isfreed = 1;

			}
			else
			{
				calDeckFileFile(file_number/2, file_number, filecnt_old, filecnt_new, M, i, k);

				// Free the space
				free(filecnt_old);
			}

			// Print
			printf("K-Deck Calculation Time (to files): "); printTime(start,time(0)); printf("\n");
			printf("Number of k-decks in each file:");
			for (int j = 0; j < file_number; j++)
				printf("%d ", filecnt_new[j]);
			printf("\n");

			// Check duplicates in each file
			start = time(0);
			checkDuplicatesinFiles (file_number, filecnt_new, i+1, len);
			printf("Total time for checking duplicates"); printTime(start,time(0)); printf("\n");

			// Update
			num *= 2;
			filecnt_old = filecnt_new;
			if (i == n-1)
				free(filecnt_new);
		}
	}

	// Free the new array
	if (L_isfreed == 0)
	{
		for (int i = 0; i < num; i++)
			free(L[i]);
		free(L);
	}

	// Free transition matrix
	freeTransitionMatrix (M, k-1);
}

// Print the k-deck from deck to fptr
void printDecktoFile(FILE *fptr, int *deck, int len)
{
	for (int i = 0; i < len; i++)
		fprintf (fptr, "%d ", deck[i]);
	fprintf(fptr, "\n");
}

// Read the k-deck from fprt to deck
void readDeckfromFile(FILE *fptr, int *deck, int len)
{
	for (int i = 0; i < len; i++)
		fscanf (fptr, "%d", deck+i);
}

// Calculate the new k-deck from list L of length n to new list
// Assume L_new has 2 * num_old spaces
// Need to pre-allocate space for L_old and L_new
void calDeckListList(int **L_old, int **L_new, int num_old, int **M, int n, int k)
{
	for (int i = 0; i < num_old; i++)
	{
		extKdeck (L_old[i], L_new[i], M, n, k, 0);				// Extend a 0
		extKdeck (L_old[i], L_new[i + num_old], M, n, k, 1);	// Extend a 1
	}
}

// Calculate the new k-deck from list L of length n to files
// file_count count is a list of integers of length file_number
// it counts the number of k-decks in each file
void calDeckListFile(int **L, int num, int file_number, int *file_count, int **M, int n, int k)
{
	// Calculate length of k-deck
	int len = (1 << k);

	// Allocate space
	FILE **file_array = (FILE **) malloc (file_number * sizeof (FILE*));
	char filename[100];

	// Make a new directory
	sprintf(filename, "KDECK_PROCESS/%d", n+1);
	mkdir(filename, 0777);

	// Open files
	for (int t = 0; t < file_number; t++)
	{
		sprintf(filename, "KDECK_PROCESS/%d/%d.txt", n+1, t);
		file_array[t] = fopen(filename, "w");
	}

	// Initialize file_count
	for (int i = 0; i < file_number; i++)
		file_count[i] = 0;

	// Distribute k-decks
	int *deck = (int *) malloc (len * sizeof(int));
	uint mask = file_number - 1;		// Take file_number bits
	uint idx;

	for (int j = 0; j < num; j++)		// From L
	{
		for (int b = 0; b < 2; b++)
		{
			extKdeck(L[j], deck, M, n, k, b);
			idx = hashKey2 (deck, len) & mask;

			printDecktoFile(file_array[idx], deck, len);
			file_count[idx] += 1;
		}
	}

	// Free the space
	for (int t = 0; t < file_number; t++)
		fclose(file_array[t]);
	free(file_array); 
	free(deck);
}

// Calculate the new k-deck of length n from files to files
// file_count count is a list of integers of length file_number
// it counts the number of k-decks in each file
void calDeckFileFile(int filenum_old, int filenum_new, 
		int *filecnt_old, int *filecnt_new, int **M, int n, int k)
{
	// Calculate length of k-deck
	int len = (1 << k);

	// Allocate space
	FILE **file_array = (FILE **) malloc (filenum_new * sizeof (FILE*));
	char filename[100];

	// Make a new directory
	sprintf(filename, "KDECK_PROCESS/%d", n+1);
	mkdir(filename, 0777);

	// Open files
	for (int t = 0; t < filenum_new; t++)
	{
		sprintf(filename, "KDECK_PROCESS/%d/%d.txt", n+1, t);
		file_array[t] = fopen(filename, "w");
	}

	// Initialize filecnt_new
	for (int i = 0; i < filenum_new; i++)
		filecnt_new[i] = 0;

	// Distribute k-decks
	int *deck_old = (int *) malloc (len * sizeof(int));
	int *deck_new = (int *) malloc (len * sizeof(int));
	uint mask = filenum_new - 1;		// Take file_number bits
	uint idx;


	for (int f = 0; f < filenum_old; f++)
	{
		// Record time
		printf("From file %d: %d; time:", f, filecnt_old[f]);
		time_t start = time(0);

		sprintf(filename, "KDECK_PROCESS/%d/%d.txt", n, f);
		FILE *fptr = fopen(filename, "r");

		for (int j = 0; j < filecnt_old[f]; j++)		// From file
		{
			// Read the k-deck
			readDeckfromFile(fptr, deck_old, len);

			// Calculate new k-deck
			for (int b = 0; b < 2; b++)
			{
				extKdeck(deck_old, deck_new, M, n, k, b);
				idx = hashKey2(deck_new, len) & mask;
	
				printDecktoFile(file_array[idx], deck_new, len);
				filecnt_new[idx] += 1;
			}
		}

		fclose(fptr);
		printTime(start,time(0)); printf("\n");
		remove(filename);
	}

	// Free the space
	for (int t = 0; t < filenum_new; t++)
		fclose(file_array[t]);
	free(file_array); 
	free(deck_old);
	free(deck_new);
}

void checkDuplicatesinFiles (int filenum, int *filecnt, int n, int len)
{
	char filename[100];

	// Open files
	for (int t = 0; t < filenum; t++)
	{
		printf("File %d:\n", t);
		sprintf(filename, "KDECK_PROCESS/%d/%d.txt", n, t);
		FILE *fptr = fopen(filename, "r");

		// Allocate space for L
	    int **L = (int **) malloc (filecnt[t] * sizeof(int*));
	    for (int i = 0; i < filecnt[t]; i++)
	    {
	        L[i] = (int *) malloc (len * sizeof(int));
	        readDeckfromFile (fptr, L[i], len);
	    }
	
	    // Check
	    checkDuplicates( L, filecnt[t], len);

	    // Free the space
	    for (int i = 0; i < filecnt[t]; i++)
	        free(L[i]);
	    free(L);
		fclose(fptr);
	}
}
