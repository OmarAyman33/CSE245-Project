# Task 2 - Closed Knight’s Tour on an 𝑛×𝑛 Board

## Problem Description

Is it possible for a chess knight to visit all the cells of an 8 × 8 chessboard exactly once, ending at a cell one knight’s move away from the starting cell? (Such a tour is called closed or re-entrant. Note that a cell is considered visited only when the knight lands on it, not just passes over it on its move.) What if the chessboard is n × n? is all n where n>8 is possible for a chess knight to visit all the cells? If not, what is the cases?

design a greedy algorithm to find the minimum number of moves the chess knight needs. And study the different cases of n

## Problem Analysis and Understanding

### Subtask Breakdown

#### Subtask 1: Closed Knight's Tour on an 8 x 8 Board

**Question:** Can a knight complete a closed tour on the standard 8x8 chessboard?

**Answer:** **Yes.** Closed knight's tours on the 8x8 board have been known for centuries. Euler himself studied this problem in 1759. There are approximately 26,534,728,821,064 distinct closed tours on the 8x8 board.

Since the board has 64 squares, a closed tour consists of exactly 64 moves (visiting all 64 squares and returning to the start).

---

#### Subtask 2: For Which n x n Boards Does a Closed Tour Exist?

**Question:** For which values of n does a closed knight's tour exist on an n x n board?

**Key Results:**

| n | Closed Tour? | Reason |
|---|-------------|--------|
| 1 | No | Only 1 cell, no moves possible |
| 2 | No | Board too small, knight cannot reach all cells |
| 3 | No | Board too small, knight gets stuck |
| 4 | No | Proven impossible (exhaustive search) |
| 5 | No | n is odd -> impossible (coloring argument) |
| 6 | Yes | First board size where closed tour is possible |
| 7 | No | n is odd -> impossible |
| 8 | Yes | Classic problem, many solutions exist |
| n >= 6, even | Yes | Constructive proofs exist |
| n odd | **Never** | Impossible by the coloring argument |

**Conclusion:** A closed knight's tour on an n x n board exists **if and only if n is even and n >= 6**.

---

#### Subtask 3: Why Odd n is Always Impossible (Coloring Argument)

Color the board like a standard chessboard (alternating black and white). A knight **always** moves from a black square to a white square and vice versa. Therefore:

1. A knight's tour on n x n visits n^2 squares.
2. A closed tour forms a **Hamiltonian cycle** on the knight graph.
3. The knight graph is **bipartite** (black squares vs. white squares).
4. A Hamiltonian cycle in a bipartite graph requires **equal-sized** partitions.
5. When n is odd, n^2 is odd, so one color has (n^2+1)/2 squares and the other has (n^2-1)/2 squares -- **unequal**.
6. Therefore, **no closed tour exists for any odd n**.

---

#### Subtask 4: Why n = 4 is Impossible

Even though 4 is even and 16 is even (equal partitions of 8 black and 8 white), the 4x4 board is too constrained. The corner squares have only 2 possible knight moves, and the overall connectivity of the knight graph on a 4x4 board does not admit a Hamiltonian cycle. This has been verified by exhaustive computer search.

---

#### Subtask 5: Design a Greedy Algorithm

**Goal:** Design a greedy algorithm to find a closed knight's tour.

**Approach:** Use **Warnsdorff's Rule** (1823), a classic greedy heuristic:

- From the current square, move to the neighbor that has the **fewest onward moves** (i.e., the fewest unvisited neighbors).
- Ties are broken by preferring squares closer to the board center (or by trying all tied options).

For **closed** tours specifically, we must also ensure the last square is a knight's move away from the starting square. This may require backtracking on the final steps.

## References

Knight's Tour Explanation: https://www.chess.com/terms/knights-tour-chess

another source: https://www.youtube.com/watch?v=ab_dY3dZFHM

WarnsDorffs Algorithm: https://www.geeksforgeeks.org/dsa/warnsdorffs-algorithm-knights-tour-problem/ 
