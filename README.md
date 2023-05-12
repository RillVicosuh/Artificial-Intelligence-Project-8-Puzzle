# 8-Puzzle Solver

This project is an 8-puzzle solver implemented in C++. The solver uses Uniform Cost Search, A* with Misplaced Tile heuristic, and A* with Manhattan distance heuristic to find a solution to the given 8-puzzle.

## Dependencies

- C++11 or later

## How to Use

1. Compile the program using a C++ compiler.
2. Run the compiled program.
3. Input the initial state of the 8-puzzle, using a zero (0) to represent the blank space. Ensure that your values are separated by a space.
4. Select your algorithm of choice:
   1. Uniform Cost Search
   2. A* with Misplaced Tile heuristic
   3. A* with Manhattan distance heuristic
5. The program will display the best state to expand at each step, and once the solution is found, it will display the solution depth, the number of nodes expanded, and the max queue size.

## Code Structure

- `main()`: The main function that takes user input for the initial puzzle state and algorithm choice, then calls the `uniform_cost_search()` function.
- `uniform_cost_search()`: Implements the chosen search algorithm and outputs the progress of the search.
- `expand()`: Expands a given node by generating its child nodes and pushing them into the priority queue.
- `goalTest()`: Determines if the current puzzle state matches the goal state.
- `misplaced_tile()`: Calculates the number of misplaced tiles in the current puzzle state.
- `manhattan_distance()`: Calculates the sum of the Manhattan distances of each tile from its position in the current puzzle state to its position in the goal state.

### Custom Comparator Class

- `compare`: A custom comparator class used to create a min-heap with the priority queue, comparing the heuristics and costs of the nodes.

## Notes

- This solver assumes that the input puzzle is solvable. Unsolvable puzzles will result in an infinite loop.
