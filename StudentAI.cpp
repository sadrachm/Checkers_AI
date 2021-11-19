#include "StudentAI.h"
#include <random>

//Works only if our program is Player 2

//The following part should be completed by students.
//The students can modify anything except the class name and exisiting functions and varibles.
StudentAI::StudentAI(int col,int row,int p)
        :AI(col, row, p)
{
    board = Board(col,row,p);
    board.initializeGame();
    player = 2;
}
int moveCount = 0;
int turn = -1;
Move StudentAI::GetMove(Move move)
{
    if (move.seq.empty())
    {
        player = 1;
    } else{
        board.makeMove(move,player == 1?2:1);
    }
    turn += 1;
    if (player == 1) {
        vector<vector<Move>> x = board.getAllPossibleMoves(player);
        int i = rand() % (x.size());
        vector<Move> checker_moves = x[i];
        int j = rand() % (checker_moves.size());
        Move res = checker_moves[j];
        board.makeMove(res,player);
        return res;

    }
    Move bestMove = MCTS(move);
    board.makeMove(bestMove,player);
    return bestMove;


}

/* -------------Ideas------------------
 * Mark the node as explored to not repeat

*/

int StudentAI::Simulate(bool mainPlayer) {
    int numOfMoves = 0;
    int win = 0;
    while (!board.isWin(player)) {
        vector<vector<Move>> x = board.getAllPossibleMoves(mainPlayer ? 1 : 2);
        int i = rand() % (x.size());
        vector<Move> checker_moves = x[i];
        int j = rand() % (checker_moves.size());
        Move res = checker_moves[j];
        board.makeMove(res,mainPlayer ? 1 : 2);
        mainPlayer = !mainPlayer;
        numOfMoves++;
    }
    if (goodPieces(true) == 0) {
        win = 1;
    }
    for (int a = 0; a < numOfMoves; a++) {
        board.Undo();
    }
    return win;
}


Move StudentAI::MCTS (Move move) {
    vector<vector<Move> > moves = board.getAllPossibleMoves(player);
    Move bestMove;
    int count = 0;
    vector<int> values;
    while (count < 600) {
        int position = 0;
        for (int a = 0; a < moves.size(); a++) {
            for (int b = 0; b < moves[a].size(); b++) {
                board.makeMove(moves[a][b], 2);
                int newValue = Simulate(true);
                if (values.size() < position+1) {
                    values.push_back(newValue);
                } else {
                    values[position] += newValue;
                }
                board.Undo();
                position++;
                count++;
            }
        }
    }
    int max = 0;
    int position = 0;
    for (int a = 0; a < values.size(); a++) {
        if (values[a] > max) {
            max = values[a];
            position = a;
        } else if (values[a] == max) {
            int i = rand() % 10;
            if (i > 5) {
                max = values[a];
                position = a;
            }
        }
    }
    count = 0;
    for (int a = 0; a < moves.size(); a++) {
        for (int b = 0; b < moves[a].size(); b++) {
            if (count == position) {
                bestMove = moves[a][b];
            }
            count++;
        }
    }
    return bestMove;
}

int StudentAI::goodPieces (bool mainPlayer) {
    if (mainPlayer) {
        return board.blackCount;

    } else {
        return board.whiteCount;
    }
}

int StudentAI::MiniMax (Move move, bool mainPlayer) {
    board.makeMove(move, mainPlayer ? 2 : 1);
    moveCount += 1;
    vector<vector<Move>> x = board.getAllPossibleMoves(mainPlayer ? 1 : 2);
    if (mainPlayer) {
        int check;
        int maxima = -100;
        if (moveCount - turn >= 2) {
            return goodPieces(mainPlayer) - goodPieces(!mainPlayer);
        }
        if (x.size() == 0) {
            return goodPieces(!mainPlayer) * -1;
        }
        for (int a = 0; a < x.size(); a++) {
            for (int b = 0; b < x[a].size(); b++) {
                check = MiniMax(x[a][b], false);
                board.Undo();
                moveCount -= 1;
                maxima = max(maxima, check);
            }
        }
        return maxima;
    } else {
        int check;
        int minima = 100;
        if (moveCount - turn >= 2) {
            return goodPieces(!mainPlayer) - goodPieces(mainPlayer);
        }
        if (x.size() == 0) {
            return goodPieces(mainPlayer) * 1;
        }
        for (int a = 0; a < x.size(); a++) {
            for (int b = 0; b < x[a].size(); b++) {
                check = MiniMax(x[a][b], true);
                board.Undo();
                moveCount -= 1;
                minima = min(minima, check);
            }
        }
        return minima;
    }
    return 0;
}
