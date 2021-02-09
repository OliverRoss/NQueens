#ifndef ALGO_X_H
#define ALGO_X_H

#include <stdbool.h>

// A small library that implements Knuth's algorithm X with dancing links

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

Matrix* get_matrix(int num_primaries, int num_secondaries);

void destroy_matrix(Matrix* matrix);

// ------------------------------------------------------------------------------------------ Construct matrix

// Use this struct to add constraints to the matrix. It corresponds to adding a row to the matrix.
// The integers in the array denote the column that should have a '1' in that particular row.
struct constraint {
	int* array;
	int length;
};

void add_constraints(struct constraint* constraint_row, Matrix* matrix);

// ------------------------------------------------------------------------------------------  Algo X

int algorithm_x(Matrix* matrix);

#endif // ALGO_X_H
