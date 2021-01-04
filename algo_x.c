#include "algo_x.h"
#include <stdio.h>
#include <stdlib.h>

// ------------------------------------------------------------------------------------------ Getters

Matrix* get_matrix(int num_queens)
{
	int num_primaries   = 2 * num_queens;
	int num_secondaries = 2 * (2 * num_queens - 3);
	int num_columns     = num_primaries + num_secondaries;
	Matrix* matrix      = malloc(sizeof(Matrix));
	if (!matrix)
		return NULL;
	matrix->num_columns = num_columns;
	matrix->headers     = malloc(sizeof(Column) * num_columns);
	if (!matrix->headers) {
		destroy_matrix(matrix);
		return NULL;
	}
	matrix->head = malloc(sizeof(Node));
	if (!matrix->head) {
		destroy_matrix(matrix);
		return NULL;
	}
	for (int i = 0; i < num_columns; i++) {
		matrix->headers[i].node = malloc(sizeof(Node));
		if (!matrix->headers[i].node) {
			destroy_matrix(matrix);
			return NULL;
		}
	}
	matrix->head->left        = matrix->headers[num_primaries - 1].node;
	matrix->head->left->right = matrix->head;

	Node* prev = matrix->head;
	for (int i = 0; i < num_columns; i++) {
		matrix->headers[i].node->col    = &matrix->headers[i];
		matrix->headers[i].node->top    = matrix->headers[i].node;
		matrix->headers[i].node->bottom = matrix->headers[i].node;
		matrix->headers[i].size         = 0;
		if (i < num_primaries) {
			matrix->headers[i].node->left = prev;
			prev->right                   = matrix->headers[i].node;
			prev                          = matrix->headers[i].node;
		} else {
			matrix->headers[i].node->left  = matrix->headers[i].node;
			matrix->headers[i].node->right = matrix->headers[i].node;
		}
	}
	return matrix;
}

void destroy_matrix(Matrix* matrix)
{
	if (!matrix)
		return;
	if (matrix->head)
		free(matrix->head);
	if (matrix->headers) {
		for (int i = 0; i < matrix->num_columns; i++) {
			Node* node = matrix->headers[i].node;
			Node* top  = node;
			node       = node->bottom;
			Node* tmp  = node;
			while (node != top) {
				node = node->bottom;
				free(tmp);
				tmp = node;
			}
			free(top);
		}
		free(matrix->headers);
	}
	free(matrix);
}

// ------------------------------------------------------------------------------------------ Algo X

static void cover(Column* col)
{
	col->node->right->left = col->node->left;
	col->node->left->right = col->node->right;
	for (Node* i = col->node->bottom; i != col->node; i = i->bottom) {
		for (Node* j = i->right; j != i; j = j->right) {
			j->bottom->top = j->top;
			j->top->bottom = j->bottom;
			j->col->size--;
		}
	}
}

static void uncover(Column* col)
{
	for (Node* i = col->node->top; i != col->node; i = i->top) {
		for (Node* j = i->left; j != i; j = j->left) {
			j->col->size++;
			j->bottom->top = j;
			j->top->bottom = j;
		}
	}
	col->node->right->left = col->node;
	col->node->left->right = col->node;
}

// The actual recursive algorithm
int search(Matrix* matrix, int solutions)
{
	// If matrix is empty -> solution found
	Node* j = matrix->head->right;
	if (j == matrix->head) {
		return solutions + 1;
	}

	// Look up column with smallest size
	Column* column = j->col;
	for (j = j->right; j != matrix->head; j = j->right) {
		if (j->col->size < column->size) {
			column = j->col;
		}
	}

	cover(column);
	for (Node* r = column->node->bottom; r != column->node; r = r->bottom) {
		for (Node* j = r->right; j != r; j = j->right) {
			cover(j->col);
		}
		solutions = search(matrix, solutions);
		for (Node* j = r->left; j != r; j = j->left) {
			uncover(j->col);
		}
	}
	uncover(column);
	return solutions;
}

// Add constraint rows to existing matrix
static void add_constraints(int constraints[], int length, Matrix* matrix)
{
	Node* first    = malloc(sizeof(Node));
	Node* current  = first;
	Node* next     = NULL;
	current->right = next;
	for (int i = 0; i < length - 1; i++) {
		next           = malloc(sizeof(Node));
		current->right = next;
		current        = next;
	}
	Node* last = current;
	Node* x    = first;
	for (int i = 0; i < length; i++) {
		Column* col = &matrix->headers[constraints[i]];
		col->size++;

		x->top         = col->node->top;
		x->bottom      = col->node;
		x->left        = last;
		x->col         = col;
		x->top->bottom = x;
		x->bottom->top = x;

		last->right = x;
		last        = x;
		x           = x->right;
	}
}

struct constraint {
	int* array;
	int length;
};

// Get constraints for queen on field (x,y) with size 'size'
static struct constraint get_constraints(int x, int y, int size)
{
	struct constraint constraint;
	constraint.length = 4;
	int row           = x;
	int col           = y + size;
	int diagonal      = x + y + (2 * size - 1);
	int rev_diag      = (size - 1 - x + y) + (4 * size - 4);

	bool drop_diagonal = (diagonal <= (2 * size - 1) || diagonal > 4 * size - 4);
	bool drop_rev      = (rev_diag < (4 * size - 3) || rev_diag > (6 * size - 7));

	if (drop_rev) {
		constraint.length--;
	}
	if (drop_diagonal) {
		constraint.length--;
	}

	constraint.array    = malloc(sizeof(int) * constraint.length);
	constraint.array[0] = row;
	constraint.array[1] = col;

	if (drop_rev) {
		if (!drop_diagonal)
			constraint.array[2] = diagonal;
	} else {
		if (drop_diagonal) {
			constraint.array[2] = rev_diag;
		} else {
			constraint.array[2] = diagonal;
			constraint.array[3] = rev_diag;
		}
	}
	return constraint;
}

// Add all relevant constraints to the matrix
void init_matrix(Matrix* matrix, int num_queens)
{
	for (int i = 0; i < num_queens; i++) {
		for (int j = 0; j < num_queens; j++) {
			struct constraint constraint = get_constraints(i, j, num_queens);
			add_constraints(constraint.array, constraint.length, matrix);
			free(constraint.array);
		}
	}
}

int algo_x(int num_queens)
{
	Matrix* matrix = get_matrix(num_queens);
	init_matrix(matrix, num_queens);
	int solutions = search(matrix, 0);
	destroy_matrix(matrix);
	return solutions;
}
