#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define BLACK 0
#define RED 1

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
	int color;
	int friend;
} Vertex_User;

typedef struct RBT_User {
	Vertex_User * root;
} RBT_User;

typedef struct Word {
	char word[300];
	int count;
} Word;

typedef struct Adj_Word {
	char ID[20];
	int count;
	struct Adj_Word * next;
} Adj_Word;

typedef struct Vertex_Word {
	Adj_Word * first; //who mentioned
	struct Vertex_Word * left;
	struct Vertex_Word * right;
	struct Vertex_Word * parent;
	struct Word * word;
	int color;
} Vertex_Word;

typedef struct RBT_Word {
	Vertex_Word * root;
} RBT_Word;

void init_Vertex_User(Vertex_User * self) {
	self->first = NULL;
	self->left = NULL;
	self->right = NULL;
	self->parent = NULL;
	self->color = BLACK;
	self->friend = 0;
}

void init_Adj_User(Adj_User * adj) {
	adj = malloc(sizeof(Adj_User));
	strcpy(adj->ID, "");
	adj->next = NULL;
}

void LRotate_User(RBT_User * tree, Vertex_User * x) {
	Vertex_User * y;
	y = x->right;
	x->right = y->left;
	if (y->left != NULL)
		y->left->parent = x;
	y->parent = x->parent;
	if (x->parent == NULL)
		tree->root = y;
	else if (x == x->parent->left)
		x->parent->left = y;
	else
		x->parent->right = y;
	y->left = x;
	x->parent = y;
}

void RRotate_User(RBT_User * tree, Vertex_User * x) {
	Vertex_User * y;
	y = x->left;
	x->left = y->right;
	if (y->right != NULL)
		y->right->parent = x;
	y->parent = x->parent;
	if (x->parent == NULL)
		tree->root = y;
	else if (x == x->parent->left)
		x->parent->left = y;
	else
		x->parent->right = y;
	y->right = x;
	x->parent = y;
}

void LRotate_Word(RBT_Word * tree, Vertex_Word * x) {
	Vertex_Word * y;
	y = x->right;
	x->right = y->left;
	if (y->left != NULL)
		y->left->parent = x;
	y->parent = x->parent;
	if (x->parent == NULL)
		tree->root = y;
	else if (x == x->parent->left)
		x->parent->left = y;
	else
		x->parent->right = y;
	y->left = x;
	x->parent = y;
}

void RRotate_Word(RBT_Word * tree, Vertex_Word * x) {
	Vertex_Word * y;
	y = x->left;
	x->left = y->right;
	if (y->right != NULL)
		y->right->parent = x;
	y->parent = x->parent;
	if (x->parent == NULL)
		tree->root = y;
	else if (x == x->parent->left)
		x->parent->left = y;
	else
		x->parent->right = y;
	y->right = x;
	x->parent = y;
}

void RBinsertFixup_User(RBT_User * tree, Vertex_User * a) {
	Vertex_User *b;
	while (a->parent != NULL && a->parent->color == RED) {
		if (a->parent == a->parent->parent->left) {
			b = a->parent->parent->right;
			if (b != NULL && b->color == RED) {
				a->parent->color = BLACK;
				b->color = BLACK;
				a->parent->parent->color = RED;
				a = a->parent->parent;
				continue;
			}
			else if (a == a->parent->right) {
				a = a->parent;
				LRotate_User(tree, a);
			}
			a->parent->color = BLACK;
			a->parent->parent->color = RED;
			RRotate_User(tree, a->parent->parent);
		}
		else {
			b = a->parent->parent->left;
			if (b != NULL && b->color == RED) {
				a->parent->color = BLACK;
				b->color = BLACK;
				a->parent->parent->color = RED;
				a = a->parent->parent;
				continue;
			}
			else if (a == a->parent->left) {
				a = a->parent;
				RRotate_User(tree, a);
			}
			a->parent->color = BLACK;
			a->parent->parent->color = RED;
			LRotate_User(tree, a->parent->parent);
		}
	}
	tree->root->color = BLACK;
}

void RBinsertFixup_Word(RBT_Word * tree, Vertex_Word * a) {
	Vertex_Word *b;
	while (a->parent != NULL && a->parent->color == RED) {
		if (a->parent == a->parent->parent->left) {
			b = a->parent->parent->right;
			if (b != NULL && b->color == RED) {
				a->parent->color = BLACK;
				b->color = BLACK;
				a->parent->parent->color = RED;
				a = a->parent->parent;
				continue;
			}
			else if (a == a->parent->right) {
				a = a->parent;
				LRotate_Word(tree, a);
			}
			a->parent->color = BLACK;
			a->parent->parent->color = RED;
			RRotate_Word(tree, a->parent->parent);
		}
		else {
			b = a->parent->parent->left;
			if (b != NULL && b->color == RED) {
				a->parent->color = BLACK;
				b->color = BLACK;
				a->parent->parent->color = RED;
				a = a->parent->parent;
				continue;
			}
			else if (a == a->parent->left) {
				a = a->parent;
				RRotate_Word(tree, a);
			}
			a->parent->color = BLACK;
			a->parent->parent->color = RED;
			LRotate_Word(tree, a->parent->parent);
		}
	}
	tree->root->color = BLACK;
}


void add_Vertex_User(RBT_User * tree, Vertex_User * b, char * ID) {
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
			if (cmpID(a->user->ID, b->user->ID) > 0)
				a = a->left;
			else
				a = a->right;
		}
		b->parent = tmp;
		if (cmpID(b->user->ID, tmp->user->ID) > 0)
			tmp->right = b;
		else
			tmp->left = b;
	}
	b->color = RED;
	RBinsertFixup_User(tree, b);
}

int cmpID(char * a, char * b) {
	if (strlen(a) > strlen(b))
		return 1;
	else if (strlen(a) < strlen(b))
		return -1;
	else
		return strcmp(a, b);
}



Vertex_User * RBTsearchID(RBT_User * tree, char* ID) {
	Vertex_User * a = tree->root;
	while (a != NULL) {
		if (cmpID(a->user->ID, ID) > 0)
			a = a->left;
		else if (cmpID(a->user->ID, ID) < 0)
			a = a->right;
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
	self->color = BLACK;
}

void init_Adj_Word(Adj_Word * adj) {
	strcpy(adj->ID, "");
	adj->count = 0;
	adj->next = NULL;
}

void add_Vertex_Word(RBT_Word * tree, Vertex_Word * b) {
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
			if (strcmp(a->word->word, b->word->word) > 0)
				a = a->left;
			else
				a = a->right;
		}
		b->parent = tmp;
		if (strcmp(b->word->word, tmp->word->word) > 0)
			tmp->right = b;
		else
			tmp->left = b;
	}
	b->color = RED;
	RBinsertFixup_Word(tree, b);
}

Vertex_Word * RBTsearchWord(RBT_Word* tree, char* word) {
	Vertex_Word * a = tree->root;
	while (a != NULL) {
		if (strcmp(a->word->word, word) > 0)
			a = a->left;
		else if (strcmp(a->word->word, word) < 0)
			a = a->right;
		else
			break;
	}
	return a;
}

void TransPlant_User(RBT_User * tree, Vertex_User * vertex, Vertex_User * vertex2) {
	if (vertex->parent == NULL)
		tree->root = vertex2;
	else if (vertex == vertex->parent->left)
		vertex->parent->left = vertex2;
	else
		vertex->parent->right = vertex2;
	if (vertex2 != NULL)
		vertex2->parent = vertex->parent;
}

void RB_Del_FixUp_User(RBT_User * tree, Vertex_User * vertex) {
	Vertex_User * vertex2;
	if (vertex == NULL)
		return;
	while (vertex != tree->root && vertex->color == BLACK) {
		if (vertex == vertex->parent->left) {
			vertex2 = vertex->parent->right;
			if (vertex2->color == RED) {
				vertex2->color = BLACK;
				vertex->parent->color = RED;
				LRotate_User(tree, vertex->parent);
				vertex2 = vertex->parent->right;
			}
			if (vertex2->left->color == BLACK && vertex2->right->color == BLACK) {
				vertex2->color = RED;
				vertex = vertex->parent;
			}
			else if (vertex2->right->color == BLACK) {
				vertex2->left->color = BLACK;
				vertex2->color = RED;
				RRotate_User(tree, vertex2);
				vertex2 = vertex->parent->right;
			}
			vertex2->color = vertex->parent->color;
			vertex->parent->color = BLACK;
			vertex2->right->color = BLACK;
			LRotate_User(tree, vertex->parent);
			vertex = tree->root;
		}
		else {
			vertex2 = vertex->parent->left;
			if (vertex2->color == RED) {
				vertex2->color = BLACK;
				vertex->parent->color = RED;
				RRotate_User(tree, vertex->parent);
				vertex2 = vertex->parent->left;
			}
			if (vertex2->right->color == BLACK && vertex2->left->color == BLACK) {
				vertex2->color = RED;
				vertex = vertex->parent;
			}
			else if (vertex2->left->color == BLACK) {
				vertex2->right->color = BLACK;
				vertex2->color = RED;
				LRotate_User(tree, vertex2);
				vertex2 = vertex->parent->left;
			}
			vertex2->color = vertex->parent->color;
			vertex->parent->color = BLACK;
			vertex2->left->color = BLACK;
			RRotate_User(tree, vertex->parent);
			vertex = tree->root;
		}
	}
}

void RBT_Delete_User(RBT_User * tree, Vertex_User * vertex) {
	Vertex_User * vertex2 = vertex, *vertex3;
	int origin_color = vertex->color;
	if (vertex == NULL)
		return;
	if (vertex->left == NULL) {
		vertex3 = vertex->right;
		TransPlant_User(tree, vertex, vertex->right);
	}
	else if (vertex->right == NULL) {
		vertex3 = vertex->left;
		TransPlant_User(tree, vertex, vertex->left);
	}
	else {
		vertex2 = vertex->right;
		while (vertex2->left != NULL) {
			vertex2 = vertex2->left;
		}
		origin_color = vertex2->color;
		vertex3 = vertex2->right;
		if (vertex2->parent == vertex)
			vertex3->parent = vertex2;
		else {
			TransPlant_User(tree, vertex2, vertex2->right);
			vertex2->right = vertex->right;
			vertex2->right->parent = vertex2;
		}
		TransPlant_User(tree, vertex, vertex2);
		vertex2->left = vertex->left;
		vertex2->left->parent = vertex2;
		vertex2->color = vertex->color;
	}
	free(vertex->user);
	free(vertex);
	if (origin_color == BLACK)
		RB_Del_FixUp_User(tree, vertex3);
}

void FindMostTweet(RBT_Word * tree, Vertex_Word * vertices[], Vertex_Word * vertex) {
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

void TransPlant_Word(RBT_Word * tree, Vertex_Word * vertex, Vertex_Word * vertex2) {
	if (vertex->parent == NULL)
		tree->root = vertex2;
	else if (vertex == vertex->parent->left)
		vertex->parent->left = vertex2;
	else
		vertex->parent->right = vertex2;
	if (vertex2 != NULL)
		vertex2->parent = vertex->parent;
}

void RB_Del_FixUp_Word(RBT_Word * tree, Vertex_Word * vertex) {
	Vertex_Word * vertex2;
	if (vertex == NULL)
		return;
	while (vertex != tree->root && vertex->color == BLACK) {
		if (vertex == vertex->parent->left) {
			vertex2 = vertex->parent->right;
			if (vertex2->color == RED) {
				vertex2->color = BLACK;
				vertex->parent->color = RED;
				LRotate_Word(tree, vertex->parent);
				vertex2 = vertex->parent->right;
			}
			if (vertex2->left->color == BLACK && vertex2->right->color == BLACK) {
				vertex2->color = RED;
				vertex = vertex->parent;
			}
			else if (vertex2->right->color == BLACK) {
				vertex2->left->color = BLACK;
				vertex2->color = RED;
				RRotate_Word(tree, vertex2);
				vertex2 = vertex->parent->right;
			}
			vertex2->color = vertex->parent->color;
			vertex->parent->color = BLACK;
			vertex2->right->color = BLACK;
			LRotate_Word(tree, vertex->parent);
			vertex = tree->root;
		}
		else {
			vertex2 = vertex->parent->left;
			if (vertex2->color == RED) {
				vertex2->color = BLACK;
				vertex->parent->color = RED;
				RRotate_Word(tree, vertex->parent);
				vertex2 = vertex->parent->left;
			}
			if (vertex2->right->color == BLACK && vertex2->left->color == BLACK) {
				vertex2->color = RED;
				vertex = vertex->parent;
			}
			else if (vertex2->left->color == BLACK) {
				vertex2->right->color = BLACK;
				vertex2->color = RED;
				LRotate_Word(tree, vertex2);
				vertex2 = vertex->parent->left;
			}
			vertex2->color = vertex->parent->color;
			vertex->parent->color = BLACK;
			vertex2->left->color = BLACK;
			RRotate_Word(tree, vertex->parent);
			vertex = tree->root;
		}
	}
}

void RBT_Delete_Word(RBT_Word * tree, Vertex_Word * vertex) {
	Vertex_Word * vertex2 = vertex, *vertex3;
	int origin_color = vertex->color;
	if (vertex == NULL)
		return;
	if (vertex->left == NULL) {
		vertex3 = vertex->right;
		TransPlant_Word(tree, vertex, vertex->right);
	}
	else if (vertex->right == NULL) {
		vertex3 = vertex->left;
		TransPlant_Word(tree, vertex, vertex->left);
	}
	else {
		vertex2 = vertex->right;
		while (vertex2->left != NULL) {
			vertex2 = vertex2->left;
		}
		origin_color = vertex2->color;
		vertex3 = vertex2->right;
		if (vertex2->parent == vertex)
			vertex3->parent = vertex2;
		else {
			TransPlant_Word(tree, vertex2, vertex2->right);
			vertex2->right = vertex->right;
			vertex2->right->parent = vertex2;
		}
		TransPlant_Word(tree, vertex, vertex2);
		vertex2->left = vertex->left;
		vertex2->left->parent = vertex2;
		vertex2->color = vertex->color;
	}
	free(vertex);
	if (origin_color == BLACK)
		RB_Del_FixUp_Word(tree, vertex3);
}

void FindMostOrLeastUser(RBT_User * tree, Vertex_User* vertices[], Vertex_User * vertex, int sz, int option) {
	int i, isSame = 0;
	if (vertex != NULL) {
		if (option == 0) {
			for (i = 0; vertices[i] && i < sz; ++i) {
				if (vertex->user->tweets < vertices[i]->user->tweets) {
					for (int j = sz; j > i; --j) {
						vertices[j] = vertices[j - 1];
					}
					vertices[i] = vertex;
					break;
				}
			}
		}
		else {
			for (i = 0; vertices[i] && i < sz; ++i) {
				if (vertex->user->tweets > vertices[i]->user->tweets) {
					for (int j = sz; j > i; --j) {
						vertices[j] = vertices[j - 1];
					}
					vertices[i] = vertex;
					break;
				}
			}
		}
		if (i < sz && !isSame) {
			vertices[i] = vertex;
		}
		FindMostOrLeastUser(tree, vertices, vertex->left, sz, option);
		FindMostOrLeastUser(tree, vertices, vertex->right, sz, option);
	}
}

void FindMinOrMaxFriend(RBT_User * tree, int count, Vertex_User * vertex, Vertex_User * foundVertex[], int option) {
	if (vertex != NULL) {
		if (option == 0) { //findMin
			if (vertex->friend < count) {
				count = vertex->friend;
				foundVertex[0] = vertex;
			}
		}
		else { //findMax
			if (vertex->friend > count) {
				count = vertex->friend;
				foundVertex[0] = vertex;
			}
		}
		FindMinOrMaxFriend(tree, count, vertex->left, foundVertex, option);
		FindMinOrMaxFriend(tree, count, vertex->right, foundVertex, option);
	}
}

void deleteFriendship(Vertex_User * delVertex, Vertex_User * curVertex, int * friendcount) {
	Adj_User * adj, * prev = NULL;
	if (curVertex != NULL) {
		for (adj = curVertex->first; adj; prev = adj, adj = adj->next) {
			if (strcmp(delVertex->user->ID, adj->ID) == 0) {
				--curVertex->friend;
				--(*friendcount);
				if (prev == NULL) {
					curVertex->first = adj->next;
					free(adj);
					break;
				}
				else {
					prev->next = adj->next;
					free(adj);
					break;
				}
			}
		}
		deleteFriendship(delVertex, curVertex->left, friendcount);
		deleteFriendship(delVertex, curVertex->right, friendcount);
	}
}

int main() {
	RBT_User * UserTree = malloc(sizeof(RBT_User));
	RBT_Word * WordTree = malloc(sizeof(RBT_Word));
	Vertex_Word * mostTweeted[5] = { 0 };
	Vertex_User * vertices[5] = { 0 }, * vertex[1];
	UserTree->root = WordTree->root = NULL;
	while (1) {
		printf("0. Read data files\n1. display statistics\n2. Top 5 most tweeted words\n");
		printf("3. Top 5 most tweeted users\n4. Find users who tweeted a word\n5. Find all people who are friends of the above users\n");
		printf("6. Delete all mentions of a word\n7. Delete all users who mentioned a word\n8. Find strongly connected components\n");
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
			fp = fopen("user2.txt", "r");
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
			fp = fopen("friend2.txt", "r");
			//friend.txt
			while (fscanf(fp,"%s %s", ID, fID) != -1) {
				Vertex_User * self, *friend;
				Adj_User * tmp, *adj = malloc(sizeof(Adj_User));
				init_Adj_User(adj);
				self = RBTsearchID(UserTree, ID);
				friend = RBTsearchID(UserTree, fID);
				if (self == NULL || friend == NULL) {
					continue;
					free(adj);
				}
				tmp = self->first;
				strcpy(adj->ID, fID);
				self->first = adj;
				self->first->next = tmp;
				++self->friend;
				++friendcount;
			}
			fclose(fp);
			printf("done!\n");

			printf("fetching data from 'word.txt'...");
			fp = fopen("word2.txt", "r");
			while (fscanf(fp,"%s", ID) != -1) {
				int isSame = 0;
				fgets(dummy, sizeof(dummy),fp);
				fgets(signUpDate, sizeof(signUpDate),fp);
				fscanf(fp, "%s", tweet);

				Vertex_User * userVertex = RBTsearchID(UserTree, ID);
				if (userVertex == NULL)
					continue;
				Vertex_Word * wordVertex = RBTsearchWord(WordTree, tweet);
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
					adj->next = wordVertex->first;
					wordVertex->first = adj;
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
		case 1: {
			printf("\nStatistics :\n");
			printf("Average number of friends : %.2lf\n", friendcount / (double)usercount);
			for (int i = 0; i < 1; ++i)
				vertex[i] = NULL;
			FindMinOrMaxFriend(UserTree, usercount, UserTree->root, vertex, 0);
			printf("Minimum friends : %s %d\n", vertex[0]->user->screenName, vertex[0]->friend);
			FindMinOrMaxFriend(UserTree, -1, UserTree->root, vertex, 1);
			printf("Maximum number of friends : %d\n\n", vertex[0]->friend);
			printf("Average tweets per user : %.2lf\n", tweetcount / (double)usercount);
			for (int i = 0; i < 1; ++i)
				vertex[i] = NULL;
			FindMostOrLeastUser(UserTree, vertex, UserTree->root, 1, 0);
			printf("Minimum tweets per user : %d\n", vertex[0]->user->tweets);
			for (int i = 0; i < 5; ++i)
				vertex[i] = NULL;
			FindMostOrLeastUser(UserTree, vertex, UserTree->root, 1, 1);
			printf("Maximum tweets per user : %d\n\n", vertex[0]->user->tweets);
			break;
		}
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
				vertices[i] = NULL;
			FindMostOrLeastUser(UserTree, vertices, UserTree->root, 5, 1);
			printf("\nTop 5 most tweeted users :\n");
			for (int i = 0; vertices[i] && i < 5; ++i)
				printf("%d. %s %d\n", i + 1, vertices[i]->user->screenName, vertices[i]->user->tweets);
			printf("\n");
			break;
		case 4:
			printf("\nType a word : ");
			scanf("%s", tweet);
			printf("\n");
			VertexWord = RBTsearchWord(WordTree, tweet);
			if (VertexWord == NULL || VertexWord->first == NULL) {
				printf("There is no one that tweets that word\n\n");
				break;
			}
			for (Adj_Word * adj = VertexWord->first; adj; adj = adj->next) {
				printf("%s %d\n", RBTsearchID(UserTree, adj->ID)->user->ID,adj->count);
			}
			printf("\n");
			break;
		case 5:
			for (int i = 0; i < 5; ++i) {
				if (vertices[i] != NULL) {
					Vertex_User * user = RBTsearchID(UserTree, vertices[i]->user->ID);
					if (user->first == NULL) {
						printf("There is no friend from %s\n\n", user->user->screenName);
						continue;
					}
					printf("Friends of %s :\n", user->user->screenName);
					for (Adj_User * adj = user->first; adj; adj = adj->next) {
						printf("%s\n", RBTsearchID(UserTree, adj->ID)->user->screenName);
					}
				}
			}
			break;
		case 6: {
			printf("\nType a word :");
			scanf("%s", tweet);
			VertexWord = RBTsearchWord(WordTree, tweet);

			printf("These users are deleted :\n");
			Adj_Word * adj, *tmp = VertexWord->first;
			Vertex_User * user;
			for (adj = tmp; adj; adj = tmp) {
				tmp = tmp->next;
				printf("%s\n", adj->ID);
				--RBTsearchID(UserTree, adj->ID)->user->tweets;
				free(adj);

				--VertexWord->word->count;
			}
			VertexWord->first = NULL;
		}
			break;
		case 7: {
			Vertex_User * user;
			printf("\nType a word :");
			scanf("%s", tweet);
			VertexWord = RBTsearchWord(WordTree, tweet);

			printf("These users are deleted :\n");
			Adj_Word * adj, *tmp = VertexWord->first;
			for (adj = tmp; adj; adj = tmp) {
				user = RBTsearchID(UserTree, adj->ID);
				tmp = tmp->next;
				printf("%s\n", adj->ID);
				tweetcount -= user->user->tweets;
				deleteFriendship(user, UserTree->root, &friendcount);
				RBT_Delete_User(UserTree, user);
				free(adj);
				--usercount;

				--VertexWord->word->count;
			}
			VertexWord->first = NULL;
		}
			break;
		case 8:
			break;
		case 9:
			break;
		}

	}
	return 0;
}