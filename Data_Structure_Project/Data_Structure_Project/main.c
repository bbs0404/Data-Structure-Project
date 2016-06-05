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

void add_Vertex_Word(BST_Word * tree, Vertex_Word * b) {
	Vertex_Word * a = tree->root;
	Vertex_Word * tmp;
;
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

void DeleteVertex_User(BST_User * tree, Vertex_User * vertex) {
	if (vertex->left == NULL && vertex->right == NULL) {
		if (vertex->parent->left == vertex)
			vertex->parent->left = NULL;
		else
			vertex->parent->right = NULL;
	}
	else if (vertex->left == NULL) {
		if (vertex->parent->left == vertex)
			vertex->parent->left = vertex->right;
		else
			vertex->parent->right = vertex->right;
	}
	else if (vertex->right == NULL) {
		if (vertex->parent->left == vertex)
			vertex->parent->left = vertex->left;
		else
			vertex->parent->right = vertex->left;
	}
	else {

	}
	Adj_User * tmp, * tmp2 = vertex->first;
	for (tmp = tmp2; tmp; tmp = tmp2) {
		tmp2 = tmp2->next;
		free(tmp);
	}
	free(vertex);
}

void FindMostTweet(BST_Word * tree, Vertex_Word * vertices[5], Vertex_Word * vertex) {
	int i, isSame = 0;
	if (vertex != NULL) {
		for (i = 0; vertices[i] && i < 5; ++i) {
			if (strcmp(vertices[i]->word->word, vertex->word->word) == 0) {
				isSame = 1;
				break;
			}
			if (vertex->word->count > vertices[i]->word->count) {
				for (int j = 5; j > i; --j) {
					vertices[j] = vertices[j - 1];
				}
				vertices[i] = vertex;
				break;
			}
		}
		if (i < 5 && !isSame) {
			vertices[i] = vertex;
		}
		FindMostTweet(tree, vertices, vertex->left);
		FindMostTweet(tree, vertices, vertex->right);
	}
}

void FindMostUser(BST_User * tree, Vertex_User* vertices[5], Vertex_User * vertex) {
	int i, isSame = 0;
	if (vertex != NULL) {
		for (i = 0; vertices[i] && i < 5; ++i) {
			if (vertex->user->tweets > vertices[i]->user->tweets) {
				for (int j = 5; j > i; --j) {
					vertices[j] = vertices[j - 1];
				}
				vertices[i] = vertex;
				break;
			}
		}
		if (i < 5 && !isSame) {
			vertices[i] = vertex;
		}
		FindMostUser(tree, vertices, vertex->left);
		FindMostUser(tree, vertices, vertex->right);
	}
}

int main() {
	BST_User * UserTree = malloc(sizeof(BST_User));
	BST_Word * WordTree = malloc(sizeof(BST_Word));
	Vertex_Word * mostTweeted[5] = { 0 };
	Vertex_User * mostUsers[5] = { 0 };
	UserTree->root = WordTree->root = NULL;
	while (1) {
		printf("0. Read data files\n1. display statistics\n2. Top 5 most tweeted words");
		printf("3. Top 5 most tweeted users\n4. Find users who tweeted a word\n5. Find all people who are friends of the above users");
		printf("6. Delete all mentions of a word\n7. Delete all users who mentioned a word\n8. Find strongly connected components");
		printf("9. Find shortest path from a given user\n\99. Quit\nSelect Menu: ");
		int menu, usercount, friendcount, tweetcount;
		Vertex_Word * VertexWord;
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
				fscanf(fp, "%s", screenName);
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
				int isSame = 0;
				fgets(dummy, sizeof(dummy),fp);
				fgets(signUpDate, sizeof(signUpDate),fp);
				fscanf(fp, "%s", tweet);

				Vertex_User * userVertex = BSTsearchID(UserTree, ID);
				if (userVertex == NULL)
					continue;
				Vertex_Word * wordVertex = BSTsearchWord(WordTree, tweet);
				Adj_Word * adj, *tmp;
				if (wordVertex == NULL) {
					wordVertex = malloc(sizeof(Vertex_Word));
					init_Vertex_Word(wordVertex);
					Word * word = malloc(sizeof(Word));
					wordVertex->word = word;
					word->count = 0;
					strcpy(word->word, tweet);
					add_Vertex_Word(WordTree, wordVertex);
				}
				else {
					adj = wordVertex->first;
					for (tmp = adj; tmp; tmp = tmp->next) {
						if (strcmp(tmp->ID, ID) == 0) {
							isSame = 1;
							break;
						}
					}
				}
				if (!isSame) {
					adj = malloc(sizeof(Adj_Word));
					init_Adj_Word(adj);
					tmp = wordVertex->first;
					wordVertex->first = adj;
					adj->next = tmp;
					strcpy(wordVertex->first->ID, ID);
				}
				++userVertex->user->tweets;
				++adj->count;
				++tweetcount;
				++wordVertex->word->count;
			}
			printf("done!\n");
			fclose(fp);
			printf("\nTotal users : %d\n",usercount);
			printf("Total friendship records : %d\n", friendcount);
			printf("Total tweets : %d\n\n", tweetcount);
			break;
		case 1:
			printf("\nStatistics :\n");
			printf("Average number of friends : %.2lf\n", friendcount/(double)usercount);
			printf("Minimum friends : %d\n");
			break;
		case 2:
			for (int i = 0; i < 5; ++i)
				mostTweeted[i] = NULL;
			FindMostTweet(WordTree, mostTweeted, WordTree->root);
			printf("\nTop 5 most tweeted words :\n");
			for (int i = 0; mostTweeted[i] && i < 5; ++i)
				printf("%d. %s %d\n", i + 1, mostTweeted[i]->word->word,mostTweeted[i]->word->count);
			printf("\n");
			break;
		case 3:
			for (int i = 0; i < 5; ++i)
				mostUsers[i] = NULL;
			FindMostUser(UserTree, mostUsers, UserTree->root);
			printf("\nTop 5 most tweeted users :\n");
			for (int i = 0; mostUsers[i] && i < 5; ++i)
				printf("%d. %s %d\n", i + 1, mostUsers[i]->user->screenName, mostUsers[i]->user->tweets);
			printf("\n");
			break;
		case 4:
			printf("\nType a word : ");
			scanf("%s", tweet);
			printf("\n");
			VertexWord = BSTsearchWord(WordTree, tweet);
			if (VertexWord == NULL) {
				printf("There is no one that tweets that word\n\n");
				break;
			}
			for (Adj_Word * adj = VertexWord->first; adj; adj = adj->next) {
				printf("%s %d\n", BSTsearchID(UserTree, adj->ID)->user->screenName,adj->count);
			}
			printf("\n");
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

	}
	return 0;
}