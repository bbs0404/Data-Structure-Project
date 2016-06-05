#include <stdio.h>
#include <string.h>
#include <stdlib.h>

typedef struct User {
	char ID[20], signUpDate[40], screenName[20];
	int tweets;
} User;

typedef struct Adj_User {
	char ID[20];
	struct Adj_User * next;
} Adj_User;

typedef struct Vertex_User {
	Adj_User * first; //friend
	struct Vertex_User * left;
	struct Vertex_User * right;
	struct Vertex_User * parent;
	struct User * user;
} Vertex_User;

typedef struct BST_User {
	Vertex_User * root;
} BST_User;

typedef struct Word {
	char word[600];
	int count;
} Word;

typedef struct Adj_Word {
	char ID[20];
	int count;
	struct Adj * next;
} Adj_Word;

typedef struct Vertex_Word {
	Adj_Word * first; //who mentioned
	struct Vertex_Word * left;
	struct Vertex_Word * right;
	struct Vertex_Word * parent;
	struct Word * word;
} Vertex_Word;

typedef struct BST_Word {
	Vertex_Word * root;
} BST_Word;

void init_Vertex_User(Vertex_User * self) {
	self->first = NULL;
	self->left = NULL;
	self->right = NULL;
	self->parent = NULL;
}

void init_Adj_User(Adj_User * adj) {
	adj = malloc(sizeof(Adj_User));
	strcpy(adj->ID, "");
	adj->next = NULL;
}

void add_Vertex_User(BST_User * tree, Vertex_User * b, char * ID) {
	User * user = malloc(sizeof(User));
	Vertex_User * a = tree->root;
	Vertex_User * tmp;

	user->tweets = 0;
	b->user = user;
	strcpy(user->ID, ID);
	if (tree->root == NULL) {
		tree->root = b;
		return;
	}
	else {
		while (a != NULL)
		{
			tmp = a;
			if (cmpID(a->user->ID, b->user->ID) < 0)
				a = a->left;
			else
				a = a->right;
		}
		b->parent = tmp;
		if (cmpID(b->user->ID, tmp->user->ID) < 0)
			tmp->right = b;
		else
			tmp->left = b;
	}
}

int cmpID(char * a, char * b) {
	if (strlen(a) > strlen(b))
		return 1;
	else if (strlen(a) < strlen(b))
		return -1;
	else
		return strcmp(a, b);
}



Vertex_User * BSTsearchID(BST_User * tree, char* ID) {
	Vertex_User * a = tree->root;
	while (a != NULL) {
		if (cmpID(a->user->ID, ID) > 0)
			a = a->right;
		else if (cmpID(a->user->ID, ID) < 0)
			a = a->left;
		else
			break;
	}
	return a;
}

void init_Vertex_Word(Vertex_Word * self) {
	self->first = NULL;
	self->left = NULL;
	self->right = NULL;
	self->parent = NULL;
}

void init_Adj_Word(Adj_Word * adj) {
	strcpy(adj->ID, "");
	adj->count = 0;
	adj->next = NULL;
}

void add_Vertex_Word(BST_Word * tree, Vertex_Word * b, char * tweet) {
	Word * word = malloc(sizeof(Word));
	Vertex_Word * a = tree->root;
	Vertex_Word * tmp;

	b->word = word;
	strcpy(word->word, tweet);
	if (tree->root == NULL) {
		tree->root = b;
		return;
	}
	else {
		while (a != NULL)
		{
			tmp = a;
			if (strcmp(a->word->word, b->word->word) < 0)
				a = a->left;
			else
				a = a->right;
		}
		b->parent = tmp;
		if (strcmp(b->word->word, tmp->word->word) < 0)
			tmp->right = b;
		else
			tmp->left = b;
	}
}

Vertex_Word * BSTsearchWord(BST_Word* tree, char* word) {
	Vertex_Word * a = tree->root;
	while (a != NULL) {
		if (strcmp(a->word->word, word) > 0)
			a = a->right;
		else if (strcmp(a->word->word, word) < 0)
			a = a->left;
		else
			break;
	}
	return a;
}

int main() {
	BST_User * UserTree = malloc(sizeof(BST_User));
	BST_Word * WordTree = malloc(sizeof(BST_Word));
	UserTree->root = WordTree->root = NULL;
	while (1) {
		printf("0. Read data files\n1. display statistics\n2. Top 5 most tweeted words");
		printf("3. Top 5 most tweeted users\n4. Find users who tweeted a word\n5. Find all people who are friends of the above users");
		printf("6. Delete all mentions of a word\n7. Delete all users who mentioned a word\n8. Find strongly connected components");
		printf("9. Find shortest path from a given user\n\99. Quit\nSelect Menu: ");
		int menu, usercount, friendcount, tweetcount;
		char str[30];
		FILE * fp;
		scanf("%d", &menu);
		gets_s(str, sizeof(str));
		if (menu == 99)
			break;
		switch (menu) {
		case 0:
			usercount = friendcount = tweetcount = 0;
			printf("fetching data from 'user.txt'...");
			fp = fopen("user.txt", "r");
			char ID[10], signUpDate[40], screenName[20], fID[10], dummy[10], tweet[500];
			while (fscanf(fp,"%s", ID) != -1) {
				fgets(dummy, sizeof(dummy),fp);
				fgets(signUpDate, sizeof(signUpDate),fp);
				fgets(screenName, sizeof(screenName),fp);

				Vertex_User * vertex = malloc(sizeof(Vertex_User));
				init_Vertex_User(vertex);
				add_Vertex_User(UserTree, vertex, ID);
				strcpy(vertex->user->signUpDate, signUpDate);
				strcpy(vertex->user->screenName, screenName);
				++usercount;
			}
			fclose(fp);
			printf("done!\n");
			printf("fetching data from 'friend.txt'...");
			fp = fopen("friend.txt", "r");
			//friend.txt
			while (fscanf(fp,"%s %s", ID, fID) != -1) {
				Vertex_User * self, *friend;
				Adj_User * tmp, *adj = malloc(sizeof(Adj_User));
				init_Adj_User(adj);
				self = BSTsearchID(UserTree, ID);
				friend = BSTsearchID(UserTree, fID);
				if (self == NULL || friend == NULL) {
					continue;
					free(adj);
				}
				tmp = self->first;
				strcpy(adj->ID, fID);
				self->first = adj;
				self->first->next = tmp;
				++friendcount;
			}
			fclose(fp);
			printf("done!\n");
			printf("fetching data from 'word.txt'...");
			fp = fopen("word.txt", "r");
			while (fscanf(fp,"%s", ID) != -1) {
				fgets(dummy, sizeof(dummy),fp);
				fgets(signUpDate, sizeof(signUpDate),fp);
				fgets(tweet, sizeof(tweet),fp);
				Vertex_User * userVertex = BSTsearchID(UserTree, ID);
				if (userVertex == NULL)
					continue;
				Vertex_Word * wordVertex = malloc(sizeof(Vertex_Word));
				Adj_Word * adj = malloc(sizeof(Adj_Word)), * tmp;
				init_Vertex_Word(wordVertex);
				init_Adj_Word(adj);

				tmp = wordVertex->first;
				wordVertex->first = adj;
				adj->next = tmp;

				++userVertex->user->tweets;
				add_Vertex_Word(WordTree, wordVertex, tweet);
				++tweetcount;
			}
			printf("done!\n");
			fclose(fp);
			printf("Total users : %d\n",usercount);
			printf("Total friendship records : %d\n", friendcount);
			printf("Total tweets : %d\n", tweetcount);
			break;
		case 1:
			break;
		case 2:
			break;
		case 3:
			break;
		case 4:
			break;
		case 5:
			break;
		case 6:
			break;
		case 7:
			break;
		case 8:
			break;
		case 9:
			break;
		}
		//ready step
		

	}
	return 0;
}