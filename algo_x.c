#include "algo_x.h"
#include <stdio.h>
#include <stdlib.h>

// ------------------------------------------------------------------------------------------ Getters

Matrix* get_matrix(int num_primaries, int num_secondaries)
{
	int num_columns = num_primaries + num_secondaries;
	Matrix* matrix  = malloc(sizeof(Matrix));
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
void add_constraints(struct constraint* constraint_row, Matrix* matrix)
{
	int length       = constraint_row->length;
	int* constraints = constraint_row->array;

	// Sanity check if now invalid columns are passed
	for (int i = 0; i < length; i++) {
		if (constraints[i] > matrix->num_columns)
			return;
	}

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

int algorithm_x(Matrix* matrix)
{
	return search(matrix, 0);
}
