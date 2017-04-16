// animals.c

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct treeNode;

typedef struct treeNode {
    char* line;
    struct treeNode *no;
    struct treeNode *yes;
} treeNode;


treeNode* treeNode_alloc(){
	treeNode* n = (treeNode*) malloc(sizeof(treeNode));
	if(n == NULL) {
   		return NULL;
   	} 
   	return n;
}

treeNode* create_treeNode (char* l, treeNode* yes, treeNode* no) { 
	treeNode* n = treeNode_alloc();
   	n->line = l;    
   	n->yes = yes;
   	n->no = no;
   	return n; 
}

treeNode* win_Leaf() {
	return create_treeNode("I win!\n", NULL, NULL);
}

treeNode* loss_Leaf() {
	return create_treeNode("I lose.\n", NULL, NULL);
}


treeNode* create_TreeRoot() {	
	treeNode* w = win_Leaf();
	treeNode* l = loss_Leaf();

  	treeNode* y = create_treeNode("a horse", w, l);
   	treeNode* n = create_treeNode("a chicken", w, l);
   	treeNode* ret = create_treeNode("Does your animal have four legs?", y, n);

   	return ret; 
}


void win_seq(treeNode* t) {
	if(!(strncmp(t->line, "I win!\n", strlen(t->line)))) {
		printf("%s", t->line);
	}
}

treeNode* loss_seq(treeNode* t, char* guess, char* answer, char* question, size_t sq) {

	printf("%s", t->no->line);
	printf("Your animal is > ");     	// Get the real answer
	getline(&answer, &sq, stdin);
	answer[strlen(answer)-1]='\0';
	printf("Please state a question that is true of %s but false of %s. > ", answer, guess); 

	getline(&question, &sq, stdin);
	question[strlen(question)-1]='\0';

	t->line = strdup(question);
	t->yes = create_treeNode(strdup(answer), win_Leaf(), loss_Leaf());
	t->no = create_treeNode(strdup(guess), win_Leaf(), loss_Leaf());

	return t;
}

int numspaces(char* s) {
	int count = 0;
	int i = 0;
	for(i=0; s[i]!='\0'; i++) {
	  if(s[i] == ' ') count++;
	}
	return count;
}

int main(int argc, char **argv) {
	
	printf("Welcome to the Animal Game!\n \n");
	treeNode* gametree = create_TreeRoot();
	treeNode* t = gametree;
	treeNode* done = t;
	
	char* ans = (char *) malloc(4 * sizeof(char));    // just for y/n. 
	char* ques = (char *) malloc(256 * sizeof(char));
	char* g = (char *) malloc(256 * sizeof(char));				// what if question is too large??

	size_t sizea = 5;
	size_t sizeq = 257;
	int not_over = 0;

	do { 
		while(t != NULL) {                        // nested while loop while within tree
			if(numspaces(t->line) > 1) {
				printf("%s (y/n) ", t->line);
			} else {
				printf("Is your animal %s? (y/n) ", t->line);
			}
			getline(&ans, &sizea, stdin);

			if(!(strcmp(ans, "y\n"))) {          // if ans is yes
				t = t->yes;
				if(!(strcmp(t->line, "I win!\n"))) {
					win_seq(t);
					break;
				}
			} else if (!(strcmp(ans, "n\n"))) {
				if(!(strcmp(t->no->line, "I lose.\n"))) {
					t = loss_seq(t, t->line, g, ques, sizeq);
					break;
				}

				t = t->no;

			} else {    //t->no = NULL 
				printf("Invalid input!\n");
			}
		}
				// After a game... t is null or reached break statement

		printf("\nWould you like to play again? (y/n) ");  
		getline(&ans, &sizea, stdin);
		done = t;
		
		if(!(strcmp(ans, "y\n"))) {  // set up for another round!
			t = gametree;
			not_over = 1;

		} else if (!(strcmp(ans, "n\n"))) { // guarantee break out of outer loop
			not_over = 0;
			done = NULL;
		}
	} while(done != NULL || not_over);

	printf("Goodbye.\n");

	free(ans);
	free(ques);
	free(g);
	exit(0);
}

// Sample Interaction: 

// $ gcc animals.c -o animals
// $ ./animals
// Welcome to the Animal Game!
 
// Does your animal have four legs? (y/n) n
// Is your animal a chicken? (y/n) n
// I lose.
// Your animal is > a human
// Please state a question that is true of a human but false of a chicken. > Does your animal program?

// Would you like to play again? (y/n) y
// Does your animal have four legs? (y/n) n
// Does your animal program? (y/n) n
// Is your animal a chicken? (y/n) y
// I win!

// Would you like to play again? (y/n) y
// Does your animal have four legs? (y/n) y
// Is your animal a horse? (y/n) y
// I win!

// Would you like to play again? (y/n) y
// Does your animal have four legs? (y/n) y
// Is your animal a horse? (y/n) n
// I lose.
// Your animal is > a dog
// Please state a question that is true of a dog but false of a horse. > Does your animal bark?

// Would you like to play again? (y/n) y
// Does your animal have four legs? (y/n) y
// Does your animal bark? (y/n) y
// Is your animal a dog? (y/n) n
// I lose.
// Your animal is > a seal
// Please state a question that is true of a seal but false of a dog. > Does your animal live in the sea?

// Would you like to play again? (y/n) y
// Does your animal have four legs? (y/n) y
// Does your animal bark? (y/n) y
// Does your animal live in the sea? (y/n) y
// Is your animal a seal? (y/n) y
// I win!

// Would you like to play again? (y/n) y
// Does your animal have four legs? (y/n) n
// Does your animal program? (y/n) y
// Is your animal a human? (y/n) y
// I win!

// Would you like to play again? (y/n) n
// Goodbye.
// $ 

