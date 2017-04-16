// nqueens.c
// a solution to N-Queens 
// along long with Mqueens on an 8x8 Board

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

#define QUEEN 1
#define PAWN 2
#define BOARDSIZE 8

// BASIC BOARD FUNCTIONS

int** make_board(int n) {
	int **array = (int **) calloc(n, sizeof(int *));
	int i; 
	for (i = 0; i < n; i++) {
		array[i] = (int *) calloc(n, sizeof(int)); 	// also zeroes array out
	}
	return array;
}


void print_board(int **b, int size) {

	int i,j;
	for(i = 0; i< size; i++) {
		fprintf(stderr, "{");
		for (j = 0; j < size; j++) {
			if(b[i][j] == QUEEN) fprintf(stderr, " Q");
			else if(b[i][j] == PAWN) fprintf(stderr, " P");
			else fprintf(stderr, " -");
		}
		fprintf( stderr, " }\n");
	}
} 

void insert_queen(int **board, int dim, int xcoord,int ycoord) {
	if(xcoord > dim | ycoord > dim )
		printf("tried to insert q at [%d][%d]\n", xcoord, ycoord);
	else board[xcoord-1][ycoord-1] = QUEEN; 
} 

void insert_pawn(int **board, int dim, int xcoord,int ycoord) {
	if(xcoord > dim | ycoord > dim )
		printf("tried to insert p at [%d][%d]\n", xcoord, ycoord);
	else board[xcoord-1][ycoord-1] = PAWN; 
} 

int** insert_and_print(int **board, int m, int pq, int *ylist) {
	for(int i = 1; i<= m; i++) {
		// printf("%d\n", ylist[i]);
		insert_queen(board, m, i, ylist[i]);		
		}

	print_board(board,m);
	return board;

}

int** clear_board(int m, int **board) {
	for(int i = 0; i < m ; i++) {
		for(int j = 0; j < m; j++)
			board[i][j] = 0;
	}
	return board;
}

void destroy_board(int **board, int size){
	for(int i = 0; i < size; i++) {
		free(board[i]);
	}
	free(board);
}

int* create_box_solution(int m, int *ylist) {
	int n, i, y1;
	if( m & 1) {			// if m is odd
		ylist[m] = m;			
		n = m-1;
	} else n = m;

	if ((n % 6) -2)	{			// n != 6k -2 for k in N 
		for(i = 1; i <= n/2; i++) {
			y1 = i<<1; 
			ylist[i] = y1;
			ylist[i+(n/2)] = y1-1; //-1 ? 
		}

	} else if (n % 6) {		// n != 6k
		for(i = 1; i <= n/2; i++) {
			ylist[1 +((2*i + n/2 - 3)% n)] = i;
			ylist[n -((2*i + n/2 - 3)% n)] = n+1-i;	
		}
	}
	return ylist; 	
}

int* invert(int size, int *list) {
	int *ret = calloc(size+1, sizeof(int));
	for(int i = 1; i <= size; i++) {
		ret[list[i]] = i;
	}

	return ret;
}

bool isvalidboard(int** board, int n) {
	int queenAllowed = 1;

	// Check rows for valid queen(s)

	for(int i = 0; i<n; i++) {
		queenAllowed = 1;
		for(int j = 0; j < n; j++) {
			if(board[i][j] == QUEEN) {
				if(!queenAllowed) {
					printf("Board fails: row %d has multiple queens\n\n", j);
					return 0;
				} 
				queenAllowed = 0;

			} else if(board[i][j] == PAWN) {
				queenAllowed = 1;
			}
		}
	}

// each column has precisely one queen or pawns in-between
	for(int j = 0; j<n; j++) {
		queenAllowed = 1;
		for(int i = 0; i < n; i++) {
			if(board[i][j] == QUEEN) {
				if(!queenAllowed) {
					printf("Board fails: column %d has multiple queens\n\n", i);
					return 0;
				} 
				queenAllowed = 0;

			} else if(board[i][j] == PAWN) {
				queenAllowed = 1;
			}
		}
	}

// Diagonals have 1 queen or less (without pawns in between)

	for (int x = 0; x < n; x++) {
		queenAllowed = 1;
		for (int y = 0; y <= x; y++) {
 			if( board[y][x-y]== QUEEN) {
				if(!queenAllowed) {
					printf("Board fails: diagonal1 %d has multiple queens\n\n", x);
					return 0;
				} 
				queenAllowed = 0;

			} else if(board[y][x-y] == PAWN) {
				queenAllowed = 1;
			} 
		}
	}

	for (int x = 0; x < n; x++) {
		queenAllowed = 1;
		for (int y = 0; y < n-x; y++) {

 			if( board[y][x+y]== QUEEN) {
				if(!queenAllowed) {
					printf("Board fails: diagonal2 %d has multiple queens\n\n", x);
					return 0;
				} 
				queenAllowed = 0;

			} else if(board[y][x+y] == PAWN) {
				queenAllowed = 1;
			} 
		}
	}

	for (int x = 0; x < n ; x++) {
		queenAllowed = 1;
		for (int y = 0; y < n-x; y++) {
 			if( board[n-1-y][x+y]== QUEEN) {
				if(!queenAllowed) {
					printf("Board fails: diagonal3 %d has multiple queens\n\n", x);
					return 0;
				} 
				queenAllowed = 0;

			} else if(board[n-1-y][x+y] == PAWN) {
				queenAllowed = 1;
			} 
		}
	}

	for (int x = 0; x < n ; x++) {
		queenAllowed = 1;
		for (int y = 0; y <= x; y++) {
 			if( board[n-1-y][x-y]== 1) {
				if(!queenAllowed) {
					printf("Board fails: diagonal4 %d has multiple queens\n\n", x);
					return 0;
				} 
				queenAllowed = 0;

			} else if(board[n-1-y][x-y] == 2) {
				queenAllowed = 1;
			} 
		}
	}

	return 1; // queenAllowed; // should be 1 

}

	int** insert_and_print_if_valid(int **board, int m, int sum, int *ylist) {
		printf("in insert_and_print_if_valid\n");
	for(int i = 1; i<= sum; i++)
		insert_queen(board, m, i % m, ylist[i]);		
	if(isvalidboard (board, m))
		print_board(board,m);
	return board;
}


int solved_n_queens() {

	int i = 0;
	char *c = NULL; 
	size_t inputsize = 256; 
	getline(&c, &inputsize, stdin);  
	int m = atoi(c);
	if(m == -1)	return 0;
	int *ylist = (int *) calloc(m+1, sizeof(int));
	int n, y1;
	if( m & 1) {			// if m is odd
		ylist[m] = m;			
		n = m-1;
	} else n = m;

	if ((n % 6) -2)	{			// n != 6k -2 for k in N 
		for(i = 1; i <= n/2; i++) {
			y1 = i<<1; 
			ylist[i] = y1;
			ylist[i+(n/2)] = y1-1; //-1 ? 
		}

	} else if (n % 6) {		// n != 6k
		for(i = 1; i <= n/2; i++) {
			ylist[1 +((2*i + n/2 - 3)% n)] = i;
			ylist[n -((2*i + n/2 - 3)% n)] = n+1-i;	
		}
	}

	if(m <1000) {
		printf("List of position pairs\n");
		for(i = 1; i <= m; i++)
		printf("(%d, %d) ",i, ylist[i]);	  
		printf("\n");
	}

	if(m >= 30) printf("Board too large to look pretty...");

	else {
			int **board = make_board(m);
			insert_and_print(board, m, m, ylist);
			int *altlist = invert(m, ylist);
			printf("\n(Or, alternatively: \n\n");
			clear_board(m, board);
			insert_and_print(board, m, m, altlist);
			bool good = isvalidboard(board,m);	
			free(ylist);
			free(altlist);
			destroy_board(board,m);
	}

return 1;
}	

void swap(uint8_t *fir, uint8_t *sec)
{
    uint8_t temp = *fir;
    *fir = *sec;
    *sec = temp;
}

void permute(uint8_t *array, int i, int length, uint8_t **arrays, unsigned long long int *arraysLength, unsigned long long  int *arraysSize, unsigned long long int maxtimes) { 
   if((*arraysLength) >= maxtimes) return;

  if (length == i){  	 
    if((*arraysLength) >= (*arraysSize)) {
    	(*arraysSize) *= 2;
    	arrays = realloc(arrays, (*arraysSize));

    }

    printf("About to add an array to arrays.\n");

    arrays[*arraysLength] = (uint8_t *) calloc(64, sizeof(uint8_t)); 	

	memcpy(arrays[*arraysLength], array, length*sizeof(uint8_t));
	for(int u=0; u< 64; u++) printf("arrays[arraysLength][%d] is %d\n", u, (int) arrays[(*arraysLength)][u]);    

    (*arraysLength)++;

    return;
  }

  int j = i;
 
  for (j = i; j <= length; j++) { 
     swap(array+i,array+j);
     permute(array,i+1,length, arrays, arraysLength, arraysSize, maxtimes);
     swap(array+i,array+j);
  }

  return;
}

int* create_ylist(uint8_t *array, int *ret, int m, int sum) {
	for(int i = 0; i <= (BOARDSIZE*BOARDSIZE)-1; i++) {
		int index = i%BOARDSIZE;
		if(array[i] != 0) {		// is a pawn or queen
			for(int s = 0; s < sum/BOARDSIZE +1; s++) {
				if(ret[index] == 0) { // if not yet filled,
					ret[i%BOARDSIZE +(s*BOARDSIZE)] = array[i]; // fill it
					printf("array[i] is %d\n", array[i]);
					break; 
				} 
			}
		} else fprintf(stderr, "array[i] was 0\n");
	}

	return ret;
}


int main (int argc, char ** argv) {
	
	printf("Welcome to NQueens!\n"); 
	
	int cont = 1;
	while(cont) {
		printf("Enter an integer >=4 to see a solution on a board of that size, or enter -1 to exit.\n");
	cont = solved_n_queens();
	}

	
	printf("How many queens would you like on the 8x8 Board? \n");
	char *c = NULL; 
	size_t inputsize = 256; 
	getline(&c, &inputsize, stdin);  
	int q = atoi(c);

	printf("How many pawns would you like to try for %d queens?\n",q);
	c = NULL; 
	inputsize = 256; 
	getline(&c, &inputsize, stdin);  
	int p = atoi(c);

	// generate the array of 1's, 2's, 0's
	uint8_t *qplist = (uint8_t *) calloc(BOARDSIZE*BOARDSIZE, sizeof(uint8_t));

	for(uint8_t i = 0; i < q; i++) 		// q 1's
		qplist[i] = 1;

	for(uint8_t i = q; i < q+p; i++)   // p 2's
		qplist[i] = 2;

	// call permute on it
	int initial_i = 0;
	int arraymax = (BOARDSIZE*BOARDSIZE);  // boardsize -1
	unsigned long long int *arraysLength = malloc(sizeof(unsigned long long int));
	*arraysLength = 0;
	unsigned long long int *arraysSize = malloc(sizeof(unsigned long long int));
	*arraysSize = 11; // (&arraymax);
	uint8_t **arrays = (uint8_t **) calloc(*arraysSize, sizeof(uint8_t *));
	

	printf("Made vars; about to permute. arraymax = %d\n",arraymax);
//	permute(qplist, initial_i, arraymax-1, arrays, arraysLength, arraysSize, 1024);
	printf("Left permute\n");
	
	printf("arraysLength is %lld\n", *arraysLength);

	// reconstruct the boards from the permuted arrays
	// check the boards for validity

	int *ylist = (int *) calloc(p+q+1, sizeof(int));
	int **board = make_board(BOARDSIZE);

	// for(int j = 0; j < (*arraysLength); j++) {
	// 	fprintf(stderr, "%d\n", j);
	// 	ylist = create_ylist(arrays[j], ylist, BOARDSIZE, p+q+1);
	// 	printf("About to check for validity\n");
	// 	insert_and_print_if_valid(board, BOARDSIZE, p+q, ylist);
	// 	printf("board %d was valid\n", j);
	// 	clear_board(BOARDSIZE, board);
	// }

	destroy_board(board, BOARDSIZE);
	free(ylist);

	exit(0);
}
