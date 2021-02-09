#include "algo_x.h"
#include <stdio.h>
#include <stdlib.h>

// An example program using the library to solve the N queens problem

#define DEFAULT_PROBLEM_SIZE 7

// Get constraints for placing queen on field (x,y) with boardsize 'size'
struct constraint* get_queens_constraints(int x, int y, int size);
void destroy_constraint(struct constraint* constraint);

// Add all relevant constraints to the matrix
void init_matrix(Matrix* matrix, int num_queens);

int solve_nqueens(int num_queens);

int main(int argc, char** argv)
{
	int num_queens = DEFAULT_PROBLEM_SIZE;
	if (argc == 2) {
		num_queens = atoi(argv[1]);
	}
	if (num_queens < 2) {
		printf("num_queens must be at least 2.\n");
		return EXIT_FAILURE;
	}

	printf("Board size per dimension / # queens: %d\n", num_queens);
	int result = solve_nqueens(num_queens);
	printf("Result: %d\n", result);

	return EXIT_SUCCESS;
}

struct constraint* get_queens_constraints(int x, int y, int size)
{
	struct constraint* constraint = malloc(sizeof(struct constraint));
	if (!constraint)
		return NULL;
	constraint->length = 4;
	int row            = x;
	int col            = y + size;
	int diagonal       = x + y + (2 * size - 1);
	int rev_diag       = (size - 1 - x + y) + (4 * size - 4);

	bool drop_diagonal = (diagonal <= (2 * size - 1) || diagonal > 4 * size - 4);
	bool drop_rev      = (rev_diag < (4 * size - 3) || rev_diag > (6 * size - 7));

	if (drop_rev) {
		constraint->length--;
	}
	if (drop_diagonal) {
		constraint->length--;
	}

	constraint->array = malloc(sizeof(int) * constraint->length);
	if (!constraint->array) {
		free(constraint);
		return NULL;
	}
	constraint->array[0] = row;
	constraint->array[1] = col;

	if (drop_rev) {
		if (!drop_diagonal)
			constraint->array[2] = diagonal;
	} else {
		if (drop_diagonal) {
			constraint->array[2] = rev_diag;
		} else {
			constraint->array[2] = diagonal;
			constraint->array[3] = rev_diag;
		}
	}
	return constraint;
}

void destroy_constraint(struct constraint* constraint)
{
	if (constraint)
		free(constraint->array);
	free(constraint);
}

void init_matrix(Matrix* matrix, int num_queens)
{
	for (int i = 0; i < num_queens; i++) {
		for (int j = 0; j < num_queens; j++) {
			struct constraint* constraint = get_queens_constraints(i, j, num_queens);
			if (!constraint)
				continue;
			add_constraints(constraint, matrix);
			destroy_constraint(constraint);
		}
	}
}

int solve_nqueens(int num_queens)
{
	// columns && rows
	int num_primaries = 2 * num_queens;
	// diagonals && reverse diagonals
	int num_secondaries = 2 * (2 * num_queens - 3);

	// construct matrix
	Matrix* matrix = get_matrix(num_primaries, num_secondaries);
	init_matrix(matrix, num_queens);

	int solutions = algorithm_x(matrix);
	destroy_matrix(matrix);
	return solutions;
}
