#include "algo_x.h"
#include <stdio.h>
#include <stdlib.h>

#define DEFAULT_PROBLEM_SIZE 7

int main(int argc, char** argv)
{
	int num_queens = DEFAULT_PROBLEM_SIZE;
	if (argc == 2) {
		num_queens = atoi(argv[1]);
	}

	printf("Board size per dimension / # queens: %d\n", num_queens);
	int result = algo_x(num_queens);
	printf("Result: %d\n", result);

	return EXIT_SUCCESS;
}
