// Player.cpp

#include "provided.h"
#include <string>
#include <iostream>
#include <vector>
using namespace std;

struct bestmovestuff {
    int column;     // Structure that contains a column choice and its corresponding score
    int score;
};

bestmovestuff determineBestRedMove(Scaffold& s, int N, int depth, AlarmClock& ac);
bestmovestuff determineBestBlackMove(Scaffold& s, int N, int depth, AlarmClock& ac);
int timedSmartPlayer(AlarmClock& ac, const Scaffold& s, int N, int color);
int evaluator(Scaffold& board, int n, int depth);

class HumanPlayerImpl
{
public:
    int chooseMove(const Scaffold& s, int N, int color);
};

class BadPlayerImpl
{
public:
    int chooseMove(const Scaffold& s, int N, int color);
};

class SmartPlayerImpl
{
public:
    int chooseMove(const Scaffold& s, int N, int color);
};

int HumanPlayerImpl::chooseMove(const Scaffold& s, int N, int color)
{
    int column = 0;
    int level = 0;  // Initialize column and level to 0
    while (column < 1 || level < 1 || column > s.cols() || level > s.levels() || s.checkerAt(column, level) != -1) {    // While not at a valid spot
        while (column < 1 || column > s.cols()) {   // While input column is invalid
            if (color == RED) {
                cout << "Red";
            }
            else {      // Check the correct color to cout
                cout << "Black";
            }
            cout << " please enter a valid column for your move: "/* << endl*/;
            cin >> column;  // Prompt the user for a column
            cout << endl;
        } 
        level++;    // Increment level to the first level on the grid
        if (level > s.levels()) {
            column = -1;    // If we go too high and a spot was never open
            level = 0;      // the column is not valid
        }
    }
    return column;  // If we're at a vacant valid spot, send it in
}

int BadPlayerImpl::chooseMove(const Scaffold& s, int N, int color)
{
    for (int j = 1; j <= s.cols(); j++) {
        if (s.checkerAt(j, s.levels()) == -1) { // Just return the first nonfull column
            return j;
        }
    }
    return 0;  
}

int SmartPlayerImpl::chooseMove(const Scaffold& s, int N, int color)
{
    if (s.numberEmpty() == s.cols() * s.levels()) { // If smart player is going first, just put a checker in the middle
        return (s.cols() / 2) + 1;
    }
    if (s.numberEmpty() == 0) {
        return 0;   // If the board is full, you can't make a move
    }
    AlarmClock ac(9000);
    return timedSmartPlayer(ac, s, N, color);
}

int timedSmartPlayer(AlarmClock& ac, const Scaffold& s, int N, int color) {
    Scaffold s_copy(s); // Make a copy of the const scaffold
    bestmovestuff answer;   // Make a move choice structure for our final choice
    if (color == RED) {
        answer = determineBestRedMove(s_copy, N, RED, ac);  // If we're red, determine best red move
    }
    else {
        answer = determineBestBlackMove(s_copy, N, BLACK, ac);  // If we're black, determine best black mvoe
    }

    return answer.column;  // Return the column of our final choice
}

bestmovestuff determineBestRedMove(Scaffold& s, int N, int depth, AlarmClock& ac) { // determineBestComputerMove from the spec
    int j = 0; // A counter for our evaluations
    vector<bestmovestuff> evals;    // A vector of evaluations
    evals.resize(s.numberEmpty());      // We won't need more evaluations than there are moves
    for (int i = 1; i <= s.cols(); i++) { // For each possible move
        if (s.makeMove(i, RED)) {   // If we can make the move
            int evaluation = evaluator(s, N, depth);    // Evaluate the position after making the move
            if (evaluation != -1) {
                evals[j].column = i;    // If the game is now over, save the column and score in our vector
                evals[j].score = evaluation;
                j++;
            }
            else {  // If inconclusive, continue with black's best move recursively
                evals[j].score = determineBestBlackMove(s, N, depth + 1, ac).score;
                evals[j].column = i;    // Save the corresponding score and column
                j++;
            }
            s.undoMove();   // Undo the move, loop again
        } 
        if (ac.timedOut()) {
            break;
        }
    }
    bestmovestuff optimal; // Our optimal move in this position
    if (j == 0) {
        for (int k = 1; k <= s.cols(); k++) {
            if (s.checkerAt(k, s.levels())) {       // If we haven't stored any evals yet and ran out of time
                optimal.column = k;                 // just to be safe, make optimal point
                optimal.score = -999;               // towards the first valid column, and give it a very
                return optimal;                     // unfavorable score to show a lack of confidence
            }
        }
    }
    optimal = evals[0]; // Initialize it to our first eval
    for (int k = 1; k < j; k++) {   // For every eval
        if (optimal.score < evals[k].score) {
            optimal = evals[k]; // If optimal is currently worse than that eval, replace optimal
        }
    }
    return optimal; // Return optimal
}

bestmovestuff determineBestBlackMove(Scaffold& s, int N, int depth, AlarmClock& ac) { // determineBestHumanMove from the spec
    int j = 0;
    vector<bestmovestuff> evals;
    evals.resize(s.numberEmpty());
    for (int i = 1; i <= s.cols(); i++) {
        if (s.makeMove(i, BLACK)) {
            int evaluation = evaluator(s, N, depth);        // The same as determineBestRedMove, but the recursive call
            if (evaluation != -1) {                         // is to determineBestRedMove instead
                evals[j].column = i;
                evals[j].score = evaluation;
                j++;
            }
            else {
                evals[j].score = determineBestRedMove(s, N, depth + 1, ac).score;
                evals[j].column = i;
                j++;
            }
            s.undoMove();
        } 
        if (ac.timedOut()) {
            break;
        }
    }
    bestmovestuff optimal;
    if (j == 0) {
        for (int k = 1; k <= s.cols(); k++) {
            if (s.checkerAt(k, s.levels())) {
                optimal.column = k;
                optimal.score = 999;
                return optimal;
            }
        }
    }
    optimal = evals[0];
    for (int k = 1; k < j; k++) {
        if (optimal.score > evals[k].score) {   // We want negative evaluations for black
            optimal = evals[k];
        }
    }
    return optimal;
}




int evaluator(Scaffold& board, int n, int depth) {
    for (int column = 1; column <= board.cols(); column++) {    // Increment through every position on the board
        for (int level = 1; level <= board.levels(); level++) {
            if (board.checkerAt(column, level) == RED) {   // If we're looking at a RED checker
                int horizontals = 1;
                int verticals = 1;
                int forwardslashes = 1;
                int backslashes = 1;        // The directions of victory that exist each have a counter

                int ogLevel = level;
                int ogCol = column; // Save the starting points

                level++; // Look up
                while (board.checkerAt(column, level) == RED) {
                    level++;            // If we're within bounds and looking at another red, increment the vertical counter and move up
                    verticals++;
                    if (verticals == n) {
                        return 300 - depth; // BIGNUMBER - depth
                    }
                }

                level = ogLevel; // Go back to the start
                level--; // Look down
                while (board.checkerAt(column, level) == RED) {
                    level--;            // If we're within bounds and looking at another red, increment the vertical counter and move down
                    verticals++;
                    if (verticals == n) {
                        return 300 - depth;
                    }
                }

                level = ogLevel; // Go back to the start
                column++; // Look right
                while (board.checkerAt(column, level) == RED) {
                    column++;       // If we're within bounds, increment and look further right
                    horizontals++;
                    if (horizontals == n) {
                        return 300 - depth;
                    }
                }

                column = ogCol; // Go back to the start
                column--; // Look left
                while (board.checkerAt(column, level) == RED) {
                    column--;   // If we're within bounds, increment and look further left
                    horizontals++;
                    if (horizontals == n) {
                        return 300 - depth;
                    }
                }

                column = ogCol; // Go back to start
                level--;
                column--; // Look down and left
                while (board.checkerAt(column, level) == RED) {
                    level--;        // If we're within bounds, increment and continue along the diagonal
                    column--;
                    forwardslashes++;
                    if (forwardslashes == n) {
                        return 300 - depth;
                    }
                }

                column = ogCol;
                level = ogLevel; // Go back to start
                level++;
                column++;
                while (board.checkerAt(column, level) == RED) {
                    level++;
                    column++;       // If we're within bounds, increment and continue along the diagonal
                    forwardslashes++;
                    if (forwardslashes == n) {
                        return 300 - depth;
                    }
                }

                column = ogCol;
                level = ogLevel; // Go back to start
                level++;
                column--; // Look up and left
                while (board.checkerAt(column, level) == RED) {
                    level++;
                    column--;       // If we're within bounds, increment and continue along the diagonal
                    backslashes++;
                    if (backslashes == n) {
                        return 300 - depth;
                    }
                }

                column = ogCol;
                level = ogLevel; // Go back to start
                level--;
                column++; // Look down and right
                while (board.checkerAt(column, level) == RED) {
                    level--;
                    column++;
                    backslashes++;
                    if (backslashes == n) {
                        return 300 - depth;
                    }
                }
                column = ogCol;
                level = ogLevel;
            }

            // Now we have the alternate line if we're looking at a black checker

            if (board.checkerAt(column, level) == BLACK) {   // If we're looking at a BLACK checker
                int horizontals = 1;
                int verticals = 1;
                int forwardslashes = 1;
                int backslashes = 1;        // The directions of victory that exist each have a counter

                int ogLevel = level;
                int ogCol = column; // Save the starting points

                level++; // Look up
                while (board.checkerAt(column, level) == BLACK) {
                    level++;            // If we're within bounds and looking at another red, increment the vertical counter and move up
                    verticals++;
                    if (verticals == n) {
                        return -(300 - depth); // -(BIGNUMBER - depth)
                    }
                }

                level = ogLevel; // Go back to the start
                level--; // Look down
                while (board.checkerAt(column, level) == BLACK) {
                    level--;            // If we're within bounds and looking at another red, increment the vertical counter and move down
                    verticals++;
                    if (verticals == n) {
                        return -(300 - depth);
                    }
                }

                level = ogLevel; // Go back to the start
                column++; // Look right
                while (board.checkerAt(column, level) == BLACK) {
                    column++;       // If we're within bounds, increment and look further right
                    horizontals++;
                    if (horizontals == n) {
                        return -(300 - depth);
                    }
                }

                column = ogCol; // Go back to the start
                column--; // Look left
                while (board.checkerAt(column, level) == BLACK) {
                    column--;   // If we're within bounds, increment and look further left
                    horizontals++;
                    if (horizontals == n) {
                        return -(300 - depth);
                    }
                }

                column = ogCol; // Go back to start
                level--;
                column--; // Look down and left
                while (board.checkerAt(column, level) == BLACK) {
                    level--;        // If we're within bounds, increment and continue along the diagonal
                    column--;
                    forwardslashes++;
                    if (forwardslashes == n) {
                        return -(300 - depth);
                    }
                }

                column = ogCol;
                level = ogLevel; // Go back to start
                level++;
                column++;
                while (board.checkerAt(column, level) == BLACK) {
                    level++;
                    column++;       // If we're within bounds, increment and continue along the diagonal
                    forwardslashes++;
                    if (forwardslashes == n) {
                        return -(300 - depth);
                    }
                }

                column = ogCol;
                level = ogLevel; // Go back to start
                level++;
                column--; // Look up and left
                while (board.checkerAt(column, level) == BLACK) {
                    level++;
                    column--;       // If we're within bounds, increment and continue along the diagonal
                    backslashes++;
                    if (backslashes == n) {
                        return -(300 - depth);
                    }
                }

                column = ogCol;
                level = ogLevel; // Go back to start
                level--;
                column++; // Look down and right
                while (board.checkerAt(column, level) == BLACK) {
                    level--;
                    column++;
                    backslashes++;
                    if (backslashes == n) {
                        return -(300 - depth);
                    }
                }
                column = ogCol;
                level = ogLevel;
            }

        }
        //level = 1;
    }

    if (board.numberEmpty() == 0) {
        // If the board is full but no winner was found , it's tied
        return 0;
    }
    return -1;  //  If the game isn't even over
}






//******************** Player derived class functions *************************

//  These functions simply delegate to the Impl classes' functions.
//  You probably don't want to change any of this code. 

HumanPlayer::HumanPlayer(string nm)
    : Player(nm)
{
    m_impl = new HumanPlayerImpl;
}

HumanPlayer::~HumanPlayer()
{
    delete m_impl;
}

int HumanPlayer::chooseMove(const Scaffold& s, int N, int color)
{
    return m_impl->chooseMove(s, N, color);
}

BadPlayer::BadPlayer(string nm)
    : Player(nm)
{
    m_impl = new BadPlayerImpl;
}

BadPlayer::~BadPlayer()
{
    delete m_impl;
}

int BadPlayer::chooseMove(const Scaffold& s, int N, int color)
{
    return m_impl->chooseMove(s, N, color);
}

SmartPlayer::SmartPlayer(string nm)
    : Player(nm)
{
    m_impl = new SmartPlayerImpl;
}

SmartPlayer::~SmartPlayer()
{
    delete m_impl;
}

int SmartPlayer::chooseMove(const Scaffold& s, int N, int color)
{
    return m_impl->chooseMove(s, N, color);
}
