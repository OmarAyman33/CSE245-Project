
void drawCrossing(char grid[GRID_SIZE][GRID_SIZE], int originalN) {
    pair<int,int> startPos = {GRID_SIZE / 2 + 2, GRID_SIZE / 2 - 2};
    pair<int,int> endPos;
    pair<int,int> currentPos = startPos;
    pair<int,int> drawPos = startPos;
    //  FIND START CORNER OF GRID 
    for (int i = 4; i < originalN; i++) {
        if (i % 2 == 0) drawPos.first++;
        else drawPos.second--;
    }
    //  DRAW BASE GRID 
    for (int i = 0; i < originalN; i++) {
        for (int j = 0; j < originalN; j++) {
            grid[drawPos.first - i - 1][drawPos.second + j + 1] = '.';
        }
    }

    displayGrid(grid);
    // BASE CASE (n = 3)
    
    cout << "Starting n = 3\n\n";

    // Line 1
    for (int i = 0; i < 3; i++) {
        currentPos.first--;
        currentPos.second++;
        grid[currentPos.first][currentPos.second] = '+';
    }
    stepCounter++;
    displayGrid(grid);

    // Line 2
    for (int i = 0; i < 3; i++) {
        currentPos.first++;
        grid[currentPos.first][currentPos.second] = '+';
    }
    stepCounter++;
    displayGrid(grid);
    // Line 3
    for (int i = 0; i < 3; i++) {
        currentPos.first--;
        currentPos.second--;
        grid[currentPos.first][currentPos.second] = '+';
    }
    stepCounter++;
    displayGrid(grid);
    // Line 4
    for (int i = 0; i < 4; i++) {
        currentPos.second++;
        grid[currentPos.first][currentPos.second] = '+';
    }
    stepCounter++;
    displayGrid(grid);
    out.close();
    endPos = currentPos;
    cout << "n = 3 finished! Total moves 4\n\n";
    if (originalN == 3) return;
    int sizeNow = 4;
    // INCREMENTAL BUILD
    
    while (sizeNow <= originalN) {

        cout << "Starting n = " << sizeNow << "\n\n";

        // Case 1: go downward then left
        if (grid[currentPos.first][currentPos.second + 1] != '+') {

            for (int i = 0; i < sizeNow - 1; i++) {
                currentPos.first++;
                grid[currentPos.first][currentPos.second] = '+';
            }
            stepCounter++;
            displayGrid(grid);

            for (int i = 0; i < sizeNow; i++) {
                currentPos.second--;
                grid[currentPos.first][currentPos.second] = '+';
            }
            stepCounter++;
            displayGrid(grid);
        }
        // Case 2: go upward then right
        else if (grid[currentPos.first][currentPos.second - 1] != '+') {

            for (int i = 0; i < sizeNow - 1; i++) {
                currentPos.first--;
                grid[currentPos.first][currentPos.second] = '+';
            }
            stepCounter++;
            displayGrid(grid);

            for (int i = 0; i < sizeNow; i++) {
                currentPos.second++;
                grid[currentPos.first][currentPos.second] = '+';
            }
            stepCounter++;
            displayGrid(grid);
        }
        cout << "n = " << sizeNow << " finished! Total moves " << 2 * sizeNow - 2 << "\n\n";
        sizeNow++;
    }
}
// MAIN
int main() {
    char grid[GRID_SIZE][GRID_SIZE];
    // Initialize grid with spaces
    for (int i = 0; i < GRID_SIZE; i++)
        for (int j = 0; j < GRID_SIZE; j++)
            grid[i][j] = ' ';
    int n;
    cout << "Enter n: ";
    cin >> n;
    drawCrossing(grid, n);
    return 0;
}
