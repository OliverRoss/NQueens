# Knuth's Algorithm X to solve the n-queens problem

## Problem

The n-queens problem is to place `n` queens on an `n x n` chessboard, without any queen attacking another queen.

## Knuth's Algorithm X

[Knuth's Algorithm X](https://arxiv.org/pdf/cs/0011047.pdf) solves exact cover problem. 
The n-queens problem can be seen as a generalized version of an exact cover problem.
Using a technique called "dancing links" (see the linked paper), the algorithm is quite efficient.

## Implementation

The implementation is a C port from [this blog post's](http://www.nohuddleoffense.de/2019/01/20/dancing-links-algorithm-x-and-the-n-queens-puzzle/) Go code.
I added some logic to generalize it to arbitrary chessboard sizes.

## Building

Run `make all` and execute with `build/main <number_of_queens>` (Default number of queens is 7).
