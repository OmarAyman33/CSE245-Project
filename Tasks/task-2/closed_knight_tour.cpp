#include <iostream>
#include <vector>
#include <string>
#include <set>
#include <cstdint>
#include <iomanip>
using namespace std;

/*
instead of the chess board having rows as letter and columns as nums,
for easier representation we'll be using rows and letters as 0 indexed nums

- example 8x8 board:
_ _ _ _ _ _ _ _
7 |_ _ _ _ _ _ _ _|
6 |_ _ _ _ _ _ _ _|
5 |_ _ _ _ _ _ _ _|
4 |_ _ _ _ _ _ _ _|
3 |_ _ _ _ _ _ _ _|
2 |_ _ _ _ _ _ _ _|
1 |_ _ _ _ _ _ _ _|
0 |_ _ _ _ _ _ _ _|
0 1 2 3 4 5 6 7
*/

const vector<pair<int, int>> possible_move_vectors = {
    {-2, -1}, {-2, 1},
    {-1, -2}, {-1, 2},
    { 1, -2}, { 1, 2},
    { 2, -1}, { 2, 1}
};


bool isValidKnightMove(pair<int, int> from, pair<int, int> to) {
    int dr = abs(from.first - to.first);
    int dc = abs(from.second - to.second);
    return (dr == 2 && dc == 1) || (dr == 1 && dc == 2);
}

void printBoard(vector<vector<int>>& board){
    if (board.empty()) return;

    for (const auto& row : board) {
        std::cout << "[ ";
        for (size_t i = 0; i < row.size(); ++i) {
            std::cout << std::setw(8) << row[i];
            if (i < row.size() - 1) std::cout << " ";
        }
        std::cout << " ]\n";
    }
}



bool isValidCell(int n, int row, int col) {
    // checks if a cell lies in the bounds of a board.
    return (row < n && row >= 0) && (col < n && col >= 0);
}


vector<pair<int, int>> getValidMoves(int n, int row, int col) {
    // returns all possible positions from start row,col
    vector<pair<int, int>> valid_moves;

    for (pair<int, int> possible_move : possible_move_vectors) {
        if (isValidCell(n, possible_move.first + row, possible_move.second + col))
            valid_moves.push_back({ possible_move.first + row,possible_move.second + col });
    }

    return valid_moves;
}

vector<pair<int, int>> getUnvisitedMoves(int n, int row, int col, vector<vector<int>>& board) {
    // returns all unvisited valid moves from start row & col. 
    vector<pair<int, int>> valid_moves = getValidMoves(n, row, col);

    vector<pair<int, int>> result;

    for (pair<int, int> valid_move : valid_moves) {
        if (board[valid_move.first][valid_move.second] == 0)
            result.push_back(valid_move);
    }

    return result;
}


int getCellDegree(int n, pair<int, int> pos, vector<vector<int>>& board) {
    /*
    A Cell's "Degree" is how many unvisited squares can be reached from a cell.
    */
    return getUnvisitedMoves(n, pos.first, pos.second, board).size();
}


double distToCenter(int n, pair<int, int> pos) {
    double cr = (n - 1) / 2.0, cc = (n - 1) / 2.0;
    return (pos.first - cr) * (pos.first - cr) + (pos.second - cc) * (pos.second - cc);
}

pair<int, int> getBestStep(int n, pair<int, int> currPos, vector<vector<int>>& board) {
    vector<pair<int, int>> unvisited_neighbours = getUnvisitedMoves(n, currPos.first, currPos.second, board);
    /*
        from all the unvisited neighbours, Warnsdorff's algorithm selects the neighbour
        with the lowest cell degree.
        If two or more neighbours share the same cell degree, then the tiebreaker is
        the distance of the neighbour to the center.
    */
    int minCellDegree = INT32_MAX;
    double maxCellDistanceToCenter = -1.0;
    pair<int, int> currBest = { -1,-1 };

    for (pair<int, int> neighbour : unvisited_neighbours) {
        int neighbour_degree = getCellDegree(n, neighbour, board);
        if (neighbour_degree < minCellDegree) {
            currBest = neighbour;
            maxCellDistanceToCenter = distToCenter(n, neighbour);
            minCellDegree = neighbour_degree;
        }
        else if (neighbour_degree == minCellDegree) {
            // tiebreaker
            if (distToCenter(n, neighbour) > maxCellDistanceToCenter) {
                maxCellDistanceToCenter = distToCenter(n, neighbour);
                currBest = neighbour;
                minCellDegree = neighbour_degree;
            }
        }
    }

    return currBest;
}

bool nextStep(int n, pair<int, int> &currPos, vector<vector<int>>& board) {
    pair<int, int> next_step = getBestStep(n, currPos, board);
    if (next_step.first == -1 || board[next_step.first][next_step.second] != 0) {
        return false;
    }
    board[next_step.first][next_step.second] = board[currPos.first][currPos.second] + 1;
    currPos = next_step;
    return true;
}

bool closedKnightsTour(int n,int sx,int sy) {
    /*
        this function contains the greedy algorithm that solves the closed Knights
        tour problem. It uses Warnsdorff's Algorithm that uses the degree of
        closeness of unvisited tiles in terms of knight moves as the heuristic.

        Arguments:
            - n : the size of the board. (n*n)
            - sx, sy : the starting x & y positions of the knight.
        returns:
            - string : empty if no possible tour is found, else, it'll contain
            the moves needed for the tour in chess notation*.

        *note: the chess notation won't work if inputted into a chess engine as
        this function solves the problem for any nxn board, the notation won't
        be in the form of Nf6, but instead N66 for the same move as both rows
        & columns will be following the same notation.
    */

    // check if the starting position is valid
    if(!isValidCell(n, sx, sy)) {
        return false;
    }

    // board where each cell is labeled by step no. 
    vector<vector<int>> board(n, vector<int>(n, 0));
    pair<int, int> startPos = { sx, sy };
    board[sx][sy] = 1;
    pair<int, int> currPos = startPos;
    for(int i = 0; i < n * n - 1; i++) {
        if (!nextStep(n, currPos, board)) {
            return false;
        }
    }

    if(isValidKnightMove(currPos, startPos)) {
        // validate that it's a closed tour.
        printBoard(board);
        return true;
    }
    else {
        return false;
    }
}

bool closedKnightsTour(int n){
    // wrapper function of closed Knight Tour
    // attempts all possible start-x & y positions till a valid closed tour is found.
    for(int x = 0 ; x < n ; x++){
        for(int y = 0; y < n; y++){
            if(closedKnightsTour(n,x,y)) return true;
        }
    }
    return false;
}



// testing helpers: (AI generated 3shan ana t3bt)

int main(){
    // TEST CASES
    cout << "Test case 1: 5x5 board" << endl;
    cout << "expected output: no possible path." << endl;
    if(closedKnightsTour(5)) cout << "Failed Test case 1, found impossible path for 5x5 board\n";
    else cout << "Test Case 1 passed \n";

    cout << "Test case 2: 6x6 board" << endl;
    cout << "expected output: possible path." << endl;
    if(closedKnightsTour(6)) cout << "Test Case 2 passed \n";
    else cout << "Failed Test case 2, no path found for 6x6 board\n";

    cout << "Test case 3: 8x8 board" << endl;
    cout << "expected output: possible path." << endl;
    if(closedKnightsTour(8)) cout << "Test Case 3 passed \n";
    else cout << "Failed Test case 3, no path found for 8x8 board\n";

    cout << "Test case 4: 10x10 board" << endl;
    cout << "expected output: possible path." << endl;
    if(closedKnightsTour(10)) cout << "Test Case 4 passed \n";
    else cout << "Failed Test case 4, no path found for 10x10 board\n";

    cout << "Test case 5: 12x12 board" << endl;
    cout << "expected output: possible path." << endl;
    if(closedKnightsTour(12)) cout << "Test Case 5 passed \n";
    else cout << "Failed Test case 5, no path found for 12x12 board\n"; 

    cout << "Test case 6: 20x20 board" << endl;
    cout << "expected output: possible path." << endl;  
    if(closedKnightsTour(20)) cout << "Test Case 6 passed \n";
    else cout << "Failed Test case 6, no path found for 20x20 board\n";

    system("pause");
    return 0;
}