#ifndef STUDENTAI_H
#define STUDENTAI_H
#include "AI.h"
#include "Board.h"
#pragma once

//The following part should be completed by students.
//Students can modify anything except the class name and exisiting functions and varibles.
class StudentAI :public AI
{
public:
    Board board;
    StudentAI(int col, int row, int p);
    virtual Move GetMove(Move board);
    virtual int goodPieces(bool mainPlayer);
    virtual int MiniMax(Move board, bool mainPlayer);
    virtual double MCTS(Move board, bool mainPlayer);
};

#endif //STUDENTAI_H
