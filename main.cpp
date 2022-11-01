#include <iostream>
#include <string>
#include <math.h>
#include <queue>
#include <vector>
#include <algorithm>
#include <tuple>

using namespace std;

//Creating a comparator class which will be used to create a min-heap with the priority queue
//Used to compare the heuristics and costs of the nodes for the priority queue
class compare{
	public:
		bool operator()(tuple<int, int, vector<vector<int>>> node1, tuple<int, int, vector<vector<int>>> node2){
			return get<0>(node1) + get<1>(node1) > get<0>(node2) + get<1>(node2);
		}
};

int totalNodes;
void uniform_cost_search(vector< vector<int> > puzzle, int choice);
int misplaced_tile(vector<vector<int>> puzzle);
int manhattan_distance(vector<vector<int>> puzzle);
bool goalTest(vector< vector<int> > puzzle);
void expand(tuple<int, int, vector<vector<int>>> node, priority_queue< tuple<int, int, vector<vector<int>>>, vector<tuple<int, int, vector<vector<int>>>>, compare > &nodes, int choice);

int main(){
	vector< vector<int> > puzzle;//The puzzle that will be solved
	cout << "Welcome to my 8-puzzle solver. Enter your puzzle, using a zero to represent the blank. Ensure that your values are separated by a space. " << endl;
	cout << "Enter the first row: ";
	int num1, num2, num3;
	cin >> num1 >> num2 >> num3;
	vector<int> puzzleRow;
  
  	//Inserting the users numbers into the row of the puzzle
	puzzleRow.push_back(num1);
	puzzleRow.push_back(num2);
	puzzleRow.push_back(num3);
  	//Inserting the row into the overall puzzle created by the user
	puzzle.push_back(puzzleRow);
	puzzleRow.clear();

  	//Repeating the above process for the second row
	cout << "Enter the second row: ";
	cin >> num1 >> num2 >> num3;
	puzzleRow.push_back(num1);
	puzzleRow.push_back(num2);
	puzzleRow.push_back(num3);
	puzzle.push_back(puzzleRow);
	puzzleRow.clear();

  	//Repeating for the third row
	cout << "Enter the third row: ";
	cin >> num1 >> num2 >> num3;
	puzzleRow.push_back(num1);
	puzzleRow.push_back(num2);
	puzzleRow.push_back(num3);
	puzzle.push_back(puzzleRow);
	puzzleRow.clear();

	cout << "Select your algorithm" << endl;
	cout << "1. Uniform Cost Search" << endl;
	cout << "2. A* with Misplaced Tile heuristic" << endl;
	cout << "3. A* with Manhattan distance heuristic" << endl;
	int choice = 0;
	cin >> choice;

	uniform_cost_search(puzzle, choice);

	return 0;
}

void uniform_cost_search(vector< vector<int> > puzzle, int choice){
	int heuristic = 0;
  	//Based on the user's choice we get h(n)
	if (choice == 1){
		heuristic = 0;
	}
	else if (choice == 2){
		heuristic = misplaced_tile(puzzle);
	}
	else if (choice == 3){
		heuristic = manhattan_distance(puzzle);
	}
  
  	//Creating the starting node using the tuple library
  	//The node will consist of the value of h(n), the cost g(n), and the puzzle state
	tuple< int, int, vector<vector<int>> > node (heuristic, 0, puzzle);
	//Creating the queue that will store the nodes using the comparator class
	priority_queue< tuple<int, int, vector<vector<int>>>, vector<tuple<int, int, vector<vector<int>>>>, compare > nodes;
  	//Pushing the first node into the priority queue
	nodes.push(node);

	
  	//Creating the temporary node that we will use to store the first node of the priority queue
	tuple<int, int, vector<vector<int>>> tempNode = nodes.top();
  	//Getting the third element of the tuple, which is the puzzle of that node
	vector<vector<int>> tempPuzzle = get<2>(tempNode);

  	int maxNodes = 0;
	int tempCount = 0;
	while (tempCount != 1 && tempCount != 2){
		//Before checking for the goal state or expanding the node we need to determine if the 
    		//priority queue is empty
    		//If it's empty before reaching the goal state, that means the algorithm failed to find a 
    		//solution
		if (nodes.empty()){
			cout << "No solution could be found." << endl;
			tempCount = 2;
			return;
		}

    		//Store the next node in the queue, get the puzzle state of that node and make it the puzzle 
    		//state of the current node, then pop the node from the queue
		tempNode = nodes.top();
		get<2>(node) = get<2>(tempNode);
    		tempPuzzle = get<2>(node);
		nodes.pop();
    
		//Checking to see if the current puzzle state matches the goal state
		if (goalTest(tempPuzzle) == true){
			cout << "Goal State!" << endl;
      			cout << "Solution depth was " << get<1>(tempNode) << endl;
			cout << "Number of nodes expanded: " << totalNodes << endl;
			cout << "Max queue size: " << maxNodes << endl;
			tempCount = 1;
			return;
		}

    		//If we have not reached the goal state, then we can expand the node and update the queue
		expand(tempNode, nodes, choice);
		int queueSize = nodes.size();
		maxNodes = max(maxNodes, queueSize);

    		//Similar process before checking goal state. Get next node in queue and get the puzzle state
		tempNode = nodes.top();
		tempPuzzle = get<2>(tempNode);
		cout << "The best state to expand with a g(n) = " << get<1>(tempNode) << " and h(n) = " << get<0>(tempNode) << " is..." << endl;
    		//Printing out the current puzzle state
		for (int i = 0; i < 3; i++){
			for (int j = 0; j < 3; j++){
				cout << tempPuzzle[i][j] << " ";
      			}
			cout << endl;
		}
	}
}

//Passing the node, priority queue, and the algorithm choice
void expand(tuple<int, int, vector<vector<int>>> node, priority_queue< tuple<int, int, vector<vector<int>>>, vector<tuple<int, int, vector<vector<int>>>>, compare > &nodes, int choice){
	int heuristic = 0;
  	//Getting the position of the blank tile
	vector<vector<int>> tempPuzzle = get<2>(node);
  	//These will be the position of the blank tile
  	int x = 0;
	int y = 0;
	for (int i = 0; i < 3; i++){
		for (int j = 0; j < 3; j++){
      		//If the value is 0 that means the blank has been found
			if (tempPuzzle[i][j] == 0){
				x = j;
				y = i;
			}
		}
	}

	//Creating a new node by moving the blank tile up
  	//Creating a new puzzle state for the child nodes
	vector<vector<int>> tempPuzzle1 = tempPuzzle; 
	tuple< int, int, vector<vector<int>> > childNode;
  	//We can only move the blank tile up if its not in the first row
	if (y != 0){
		swap(tempPuzzle1[y][x], tempPuzzle1[y - 1][x]);
    		//Calculating the new h(n) of the updated puzzle state/chile node
		if (choice == 1){
			heuristic = 0;
		}
		else if (choice == 2){
			heuristic = misplaced_tile(tempPuzzle1);
		}
		else if (choice == 3){
			heuristic = manhattan_distance(tempPuzzle1);
		}
    		//Updating h(n)
		get<0>(childNode) = heuristic;
    		//Updating g(n)
		get<1>(childNode) = get<1>(node) + 1;
		//Updating the puzzle state of the childNode
		get<2>(childNode) = tempPuzzle1;
    		//Updating the total nodes that have been expanded
		totalNodes++;
    		//After creating the child node it is pushed into the priority queue
		nodes.push(childNode);
	}

	//Creating a new node by moving the blank tile down
  	//Similar to the process above
	tempPuzzle1 = tempPuzzle;
  	//Blank tile can only move up if it's not in the last row
	if (y != 2){
		swap(tempPuzzle1[y][x], tempPuzzle1[y + 1][x]);
		if (choice == 1){
			heuristic = 0;
		}
		else if (choice == 2){
			heuristic = misplaced_tile(tempPuzzle1);
		}
		else if (choice == 3){
			heuristic = manhattan_distance(tempPuzzle1);
		}
    		//Updating h(n)
		get<0>(childNode) = heuristic;
    		//Updating g(n)
		get<1>(childNode) = get<1>(node) + 1;
   		//Updating the puzzle state of the childNode
		get<2>(childNode) = tempPuzzle1;
    		//Updating the total nodes that have been expanded
		totalNodes++;
    		//After creating the child node it is pushed into the priority queue
		nodes.push(childNode);
	}

	//Creating a new node by moving the blank tile to the right
	tempPuzzle1 = tempPuzzle;
  	//We can only move the blank tile to the right if it's not in the last column
	if (x != 2){
		swap(tempPuzzle1[y][x], tempPuzzle1[y][x + 1]);
		if (choice == 1){
			heuristic = 0;
		}
		else if (choice == 2){
			heuristic = misplaced_tile(tempPuzzle1);
		}
		else if (choice == 3){
			heuristic = manhattan_distance(tempPuzzle1);
		}
		//Updating h(n)
		get<0>(childNode) = heuristic;
    		//Updating g(n)
		get<1>(childNode) = get<1>(node) + 1;
    		//Updating the puzzle state of the childNode
		get<2>(childNode) = tempPuzzle1;
    		//Updating the total nodes that have been expanded
		totalNodes++;
    		//After creating the child node it is pushed into the priority queue
		nodes.push(childNode);
	}
  
	//Creating a new node by moving the blank tile to the left
	tempPuzzle1 = tempPuzzle;
  	//We can only move the blank tile to the left if it's not in the first column
	if (x != 0){
		swap(tempPuzzle1[y][x], tempPuzzle1[y][x - 1]);
		if (choice == 1){
			heuristic = 0;
		}
		else if (choice == 2){
			heuristic = misplaced_tile(tempPuzzle1);
		}
		else if (choice == 3){
			heuristic = manhattan_distance(tempPuzzle1);
		}
		//Updating h(n)
		get<0>(childNode) = heuristic;
    		//Updating g(n)
		get<1>(childNode) = get<1>(node) + 1;
    		//Updating the puzzle state of the childNode
		get<2>(childNode) = tempPuzzle1;
    		//Updating the total nodes that have been expanded
		totalNodes++;
    		//After creating the child node it is pushed into the priority queue
		nodes.push(childNode);
	}
}

bool goalTest(vector< vector<int> > puzzle){
	vector< vector<int> > goal;
	int column = 3;
	int row = 3;
	int num = 0;
	goal.resize(column, vector<int>(row, num));
  	//Creating the goal state puzzle
  	for (int i = 0; i < 3; i++){
		for (int j = 0; j < 3; j++){
        	num += 1;
		goal[i][j] = num;
      		}		
	}
  	//The last tile will be given a value of 9, so we want to change that to 0
  	goal[2][2] = 0;
  	//Checking if current puzzle state matches the goal state
	if (puzzle == goal){
		return true;
	}
  	return false;
}

int misplaced_tile(vector<vector<int>> puzzle){
	int sum = 0;
  	int num = 0;

  	//Going through the puzzle that was given and determining if there are any misplaced tiles
  	for (int i = 0; i < 3; i++){
		for (int j = 0; j < 3; j++){
      			num += 1;
      			//We don't want to count the blank space
      			if(i != 2 && j != 2){
				if(puzzle[i][j] != num){
          				sum++;
        			}
      			}
    		}
	}
	return sum;
}

int manhattan_distance(vector<vector<int>> puzzle){
	int sum = 0;
  	int x = 0;
  	int y = 0;
  	int num = 0;
  	//For each tile on the puzzle, we calculate the distance to the same tile in the goal state
	for (int i = 0; i < 3; i++){
		for (int j = 0; j < 3; j++){
			num = puzzle[i][j];
      			x = 0;
      			y = 0;
      			//Again, we don't want to count the blank space
			if (num != 0){
				x = (num - 1) % 3;
				y = (num - 1) / 3;
        			sum += abs(j - x) + abs(i - y);
			}
		}
	}
	return sum;
}
