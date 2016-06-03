#pragma once

typedef struct User {
	char ID[20], signUpDate[40], screenName[20];
} User;

typedef struct Adj_User {
	char ID[20];
	struct Adj * next;
} Adj_User;

typedef struct Vertex_User {
	Adj_User * first; //friend
	struct Vertex * left;
	struct Vertex * right;
	struct Vertex * parent;
	struct User * user;
} Vertex_User;

typedef struct BST_User {
	Vertex_User * root;
} BST_User;

void init_Vertex_User(Vertex_User * self) {
	self = malloc(sizeof(Vertex_User));
	self->first = NULL;
	self->left = NULL;
	self->right = NULL;
	self->parent = NULL;
}

void init_Adj(Adj_User * adj) {
	adj = malloc(sizeof(Adj_User));
	strcpy(adj->ID, "");
	adj->next = NULL;
}

void add_Vertex_User(BST_User * tree, Vertex_User * b, char * ID) {
	User * user = malloc(sizeof(User));
	Vertex_User * a = tree->root;
	Vertex_User * tmp;

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