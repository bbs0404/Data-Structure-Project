#pragma once

typedef struct Word {
	char word[600];
	int count;
} Word;

typedef struct Adj_Word {
	char ID[20];
	struct Adj * next;
} Adj_Word;

typedef struct Vertex_Word {
	Adj_Word * first; //friend
	struct Vertex * left;
	struct Vertex * right;
	struct Vertex * parent;
	struct Word * word;
} Vertex_Word;

typedef struct BST_Word {
	Vertex_Word * root;
} BST_Word;

void init_Vertex_Word(Vertex_Word * self) {
	self = malloc(sizeof(Vertex_Word));
	self->first = NULL;
	self->left = NULL;
	self->right = NULL;
	self->parent = NULL;
}

void init_Adj(Adj_Word * adj) {
	adj = malloc(sizeof(Adj_Word));
	strcpy(adj->ID, "");
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
			if (cmpID(a->word->word, b->word->word) < 0)
				a = a->left;
			else
				a = a->right;
		}
		b->parent = tmp;
		if (cmpID(b->word->word, tmp->word->word) < 0)
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

Vertex_Word * BSTsearchID(BST_Word* tree, char* ID) {
	Vertex_Word * a = tree->root;
	while (a != NULL) {
		if (cmpID(a->word->word, ID) > 0)
			a = a->right;
		else if (cmpID(a->word->word, ID) < 0)
			a = a->left;
		else
			break;
	}
	return a;
}