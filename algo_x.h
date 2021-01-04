#ifndef ALGO_X_H
#define ALGO_X_H

#include <stdbool.h>

// ------------------------------------------------------------------------------------------  Data structures

struct node {
	struct node* left;
	struct node* right;
	struct node* top;
	struct node* bottom;
	struct column* col;
};

typedef struct node Node;

struct column {
	struct node* node;
	int size;
};

typedef struct column Column;

struct matrix {
	struct node* head;
	struct column* headers;
	int num_columns;
};

typedef struct matrix Matrix;

// ------------------------------------------------------------------------------------------  Getters

Matrix* get_matrix(int num_queens);

void destroy_matrix(Matrix* matrix);

// ------------------------------------------------------------------------------------------  Algo X

int algo_x(int num_queens);

#endif // ALGO_X_H
