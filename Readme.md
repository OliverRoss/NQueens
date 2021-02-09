# Knuth's Algorithm X 

This repository implements [Knuth's Algorithm X](https://arxiv.org/pdf/cs/0011047.pdf), which solves the exact cover 
problem as a C library.
Using a technique called "dancing links" (see the linked paper), the algorithm is quite efficient.
The implementation is a C port from [this blog post's](http://www.nohuddleoffense.de/2019/01/20/dancing-links-algorithm-x-and-the-n-queens-puzzle/) Go code. I added some logic to generalize it to arbitrary problem sizes.

## Using the library

`get_matrix` takes the number of secondary and primary columns as arguments and constructs the data structure. It
transfers ownership of the matrix to the caller by returning the pointer. 
Add constraints (aka rows), by calling `add_constraints`. You have to construct the specific constraints for your 
problem, see the 'main.c' file for an example how these look for the n-queens problem.

Call `algorithm_x` on the constructed matrix to get the number of possible solutions and use `destroy_matrix` to release
the resources.

## N-Queens problem

The n-queens problem can be seen as a generalized version of an exact cover problem and is used to demonstrate the
functionality of the library.
The challenge is to place `n` queens on an `n x n` chessboard, without any queen attacking another queen.
The file 'main.c' demonstrates how to use the library to solve that problem. See the blog post linked above for some
explanation how the constraints are constructed.
For compilation run `make all` and execute with `build/main [<number_of_queens>]` (Default number of queens is 7).
