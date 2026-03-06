#include <iostream>
#include <vector>
#include <string>
#include <set>
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
    double maxCellDistanceToCenter = 9E10;
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
            if (distToCenter(n, neighbour) < maxCellDistanceToCenter) {
                maxCellDistanceToCenter = distToCenter(n, neighbour);
                currBest = neighbour;
                minCellDegree = neighbour_degree;
            }
        }
    }

    return currBest;
}


string closedKnightsTour(int n) {
    /*
        this function contains the greedy algorithm that solves the closed Knights
        tour problem. It uses Warnsdorff's Algorithm that uses the degree of
        closeness of unvisited tiles in terms of knight moves as the heuristic.

        Arguments:
            - n : the size of the board. (n*n)
        returns:
            - string : empty if no possible tour is found, else, it'll contain
            the moves needed for the tour in chess notation*.

        *note: the chess notation won't work if inputted into a chess engine as
        this function solves the problem for any nxn board, the notation won't
        be in the form of Nf6, but instead N66 for the same move as both rows
        & columns will be following the same notation.
    */

    // board where each cell is labeled by step no. 
    vector<vector<int>> board(n, vector<int>(n, 0));
    bool incompleteFlag;
    string result = "";
    for (int startcol = 0; startcol < n; startcol++) {
        incompleteFlag = false;
        for (int startrow = 0; startrow < n; startrow++) {

            // reset board
            board = vector<vector<int>>(n, vector<int>(n, 0));
            board[startrow][startcol] = 1;

            int row = startrow, col = startcol; incompleteFlag = false;
            result = "";

            for (int step = 2; step <= n * n; step++) {
                pair<int, int> next_step = getBestStep(n, { row,col }, board);

                if (next_step.first == -1 || board[next_step.first][next_step.second] != 0) {
                    incompleteFlag = true;
                    break;
                }

                result += "(" + to_string(row) + "," + to_string(col) + ")->(" + to_string(next_step.first) + "," + to_string(next_step.second) + "),";

                board[next_step.first][next_step.second] = step;
                row = next_step.first; col = next_step.second;
            }
            if (!incompleteFlag) break;
        }
        if (!incompleteFlag) break;
    }

    if (incompleteFlag) return "";
    return result;
}

// testing helpers: (AI generated 3shan ana t3bt)

bool isValidKnightMove(pair<int, int> from, pair<int, int> to) {
    int dr = abs(from.first - to.first);
    int dc = abs(from.second - to.second);
    return (dr == 2 && dc == 1) || (dr == 1 && dc == 2);
}

// Parse the result string into a list of (from, to) pairs
vector<pair<pair<int, int>, pair<int, int>>> parseMoves(const string& result) {
    vector<pair<pair<int, int>, pair<int, int>>> moves;
    int i = 0;
    while (i < result.size()) {
        if (result[i] != '(') { i++; continue; }
        // parse "from"
        i++; // skip '('
        int r1 = 0; while (result[i] != ',') { r1 = r1 * 10 + (result[i] - '0'); i++; }
        i++; // skip ','
        int c1 = 0; while (result[i] != ')') { c1 = c1 * 10 + (result[i] - '0'); i++; }
        i++; // skip ')'
        // skip "->("
        i += 3;
        int r2 = 0; while (result[i] != ',') { r2 = r2 * 10 + (result[i] - '0'); i++; }
        i++; // skip ','
        int c2 = 0; while (result[i] != ')') { c2 = c2 * 10 + (result[i] - '0'); i++; }
        i++; // skip ')'
        moves.push_back({ {r1,c1},{r2,c2} });
    }
    return moves;
}

void validateTour(int n, const string& result, const string& testName) {
    cout << "=== " << testName << " (n=" << n << ") ===" << endl;

    if (result.empty()) {
        cout << "  Result: No tour found." << endl;
        if (n < 5) {
            cout << "  PASS - No knight's tour exists for n=" << n << endl;
        }
        else {
            cout << "  FAIL - Expected a tour for n=" << n << endl;
        }
        cout << endl;
        return;
    }

    auto moves = parseMoves(result);
    cout << "  Moves returned: " << moves.size() << " (expected " << n * n - 1 << ")" << endl;

    // Check move count
    if ((int)moves.size() != n * n - 1) {
        cout << "  FAIL - Wrong number of moves." << endl << endl;
        return;
    }

    // Check all moves are valid knight moves
    bool allValid = true;
    for (auto& m : moves) {
        if (!isValidKnightMove(m.first, m.second)) {
            cout << "  FAIL - Invalid knight move: (" << m.first.first << "," << m.first.second
                << ") -> (" << m.second.first << "," << m.second.second << ")" << endl;
            allValid = false;
            break;
        }
    }

    // Check all cells visited exactly once
    set<pair<int, int>> visited;
    // first cell is the "from" of the first move
    visited.insert(moves[0].first);
    for (auto& m : moves) {
        visited.insert(m.second);
    }

    // Check continuity (each move's "to" == next move's "from")
    bool continuous = true;
    for (int i = 0; i + 1 < (int)moves.size(); i++) {
        if (moves[i].second != moves[i + 1].first) {
            cout << "  FAIL - Discontinuous path at move " << i << endl;
            continuous = false;
            break;
        }
    }

    // Check all cells in bounds
    bool inBounds = true;
    for (auto& cell : visited) {
        if (cell.first < 0 || cell.first >= n || cell.second < 0 || cell.second >= n) {
            cout << "  FAIL - Out of bounds cell: (" << cell.first << "," << cell.second << ")" << endl;
            inBounds = false;
            break;
        }
    }

    int uniqueVisited = visited.size();
    cout << "  Unique cells visited: " << uniqueVisited << " (expected " << n * n << ")" << endl;

    if (allValid && continuous && inBounds && uniqueVisited == n * n) {
        cout << "  PASS" << endl;
    }
    else {
        if (!allValid) cout << "  FAIL - Invalid knight move(s)" << endl;
        if (!continuous) cout << "  FAIL - Path not continuous" << endl;
        if (!inBounds) cout << "  FAIL - Cell out of bounds" << endl;
        if (uniqueVisited != n * n) cout << "  FAIL - Not all cells visited (" << uniqueVisited << "/" << n * n << ")" << endl;
    }
    cout << endl;
}

int main() {
    // Test 1: n=1 (trivial, single cell — no moves needed, but result will be empty)
    cout << "Test 1: n=1 (trivial)" << endl;
    string r1 = closedKnightsTour(1);
    if (r1.empty()) cout << "  PASS - empty result for 1x1 board" << endl;
    else cout << "  FAIL - expected empty for 1x1" << endl;
    cout << endl;

    // Test 2: n=3 (no tour exists)
    cout << "Test 2: n=3 (no tour possible)" << endl;
    string r2 = closedKnightsTour(3);
    if (r2.empty()) cout << "  PASS - no tour for 3x3" << endl;
    else cout << "  Result non-empty, validating..." << endl;
    cout << endl;

    // Test 3: n=5
    validateTour(5, closedKnightsTour(5), "Test 3");

    // Test 4: n=6
    validateTour(6, closedKnightsTour(6), "Test 4");

    // Test 5: n=8 (standard chess board)
    validateTour(8, closedKnightsTour(8), "Test 5");

    // Test 6: n=10
    validateTour(10, closedKnightsTour(10), "Test 6");

    return 0;
}