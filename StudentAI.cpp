#include "StudentAI.h"
#include <random>

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
    vector<vector<Move> > moves = board.getAllPossibleMoves(player);
    Move bestMove;
//    int maxHeuristic = -100;
//    int minHeuristic = 100;
//    int temp;
    double value = 0;
    for (int a = 0; a < moves.size(); a++) {
        for (int b = 0; b < moves[a].size(); b++) {
            double newValue = MCTS(moves[a][b], player == 2 ? true : false);
            if (newValue >= value) {
                //cout << newValue << endl;
                value = newValue;
                bestMove = moves[a][b];
            }
//            moveCount = turn;
//            temp = MiniMax(moves[a][b], player==2 ? true : false);
//            if (temp > maxHeuristic && player == 1) {
//                bestMove = moves[a][b];
//                maxHeuristic = temp;
//            } else if (temp < minHeuristic && player == 2){
//                bestMove = moves[a][b];
//                minHeuristic = temp;
//            }
//            board.Undo();
        }
    }
    //cout << "WE Went with " << value << endl;
    board.makeMove(bestMove,player);
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

/* -------------Ideas------------------
 * Mark the node as explored to not repeat

*/

double StudentAI::MCTS (Move move, bool mainPlayer) {
    bool goodplayer = mainPlayer;
    int count = 0;
    double value = 0;
    double total = 0;
    board.makeMove(move, mainPlayer ? 2 : 1);
    while (count < 100) {
        int numOfMoves = 0;
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
        int x = board.isWin(goodplayer ? 2 : 1);
        if (x == player) {
            value += 1;
            total += 1;
        } else {
            total += 1;
        }
        for (int a = 0; a < numOfMoves; a++) {
            board.Undo();
        }

        count++;
    }
    //cout << value << " " << total << endl;
    board.Undo();
    return value/total;
}