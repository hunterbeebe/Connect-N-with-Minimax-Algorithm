// Game.cpp

#include "provided.h"
#include <iostream>

class GameImpl
{
public:
    GameImpl(int nColumns, int nLevels, int N, Player* red, Player* black);
    ~GameImpl();
    bool completed(int& winner) const;
    bool takeTurn();
    void play();
    int checkerAt(int c, int r) const;
private:
    Scaffold* board;
    int m_goal = 0;
    Player* m_red;
    Player* m_black;
    bool m_redsturn = 1;
};

GameImpl::GameImpl(int nColumns, int nLevels, int N, Player* red, Player* black)
{
    board = new Scaffold(nColumns, nLevels);
    m_goal = N;
    m_red = red;
    m_black = black;
}

GameImpl::~GameImpl() {
    delete board;
}

bool GameImpl::completed(int& winner) const
{
    if (m_goal == 1) {
        for (int column = 1; column <= board->cols(); column++) {    // Increment through every position on the board
            for (int level = 1; level <= board->levels(); level++) {
                if (board->checkerAt(column, level) == RED) {
                    winner = RED;
                    return true;
                }
                if (board->checkerAt(column, level) == BLACK) {
                    winner = BLACK;
                    return true;
                }
            }
        }
    } 
    for (int column = 1; column <= board->cols(); column++) {    // Increment through every position on the board
        for (int level = 1; level <= board->levels(); level++) {
            if (board->checkerAt(column, level) == RED) {   // If we're looking at a RED checker
                int horizontals = 1;
                int verticals = 1;
                int forwardslashes = 1;
                int backslashes = 1;        // The directions of victory that exist each have a counter

                int ogLevel = level;
                int ogCol = column; // Save the starting points

                level++; // Look up
                while (board->checkerAt(column, level) == RED) {
                    level++;            // If we're within bounds and looking at another red, increment the vertical counter and move up
                    verticals++;
                    if (verticals == m_goal) {
                        winner = RED;
                        return true;
                    }
                }

                level = ogLevel; // Go back to the start
                level--; // Look down
                while (board->checkerAt(column, level) == RED) {
                    level--;            // If we're within bounds and looking at another red, increment the vertical counter and move down
                    verticals++;
                    if (verticals == m_goal) {
                        winner = RED;
                        return true;
                    }
                }

                level = ogLevel; // Go back to the start
                column++; // Look right
                while (board->checkerAt(column, level) == RED) {
                    column++;       // If we're within bounds, increment and look further right
                    horizontals++;
                    if (horizontals == m_goal) {
                        winner = RED;
                        return true;
                    }
                }

                column = ogCol; // Go back to the start
                column--; // Look left
                while (board->checkerAt(column, level) == RED) {
                    column--;   // If we're within bounds, increment and look further left
                    horizontals++;
                    if (horizontals == m_goal) {
                        winner = RED;
                        return true;
                    }
                }

                column = ogCol; // Go back to start
                level--;
                column--; // Look down and left
                while (board->checkerAt(column, level) == RED) {
                    level--;        // If we're within bounds, increment and continue along the diagonal
                    column--;
                    forwardslashes++;
                    if (forwardslashes == m_goal) {
                        winner = RED;
                        return true;
                    }
                }

                column = ogCol;
                level = ogLevel; // Go back to start
                level++;
                column++;
                while (board->checkerAt(column, level) == RED) {
                    level++;
                    column++;       // If we're within bounds, increment and continue along the diagonal
                    forwardslashes++;
                    if (forwardslashes == m_goal) {
                        winner = RED;
                        return true;
                    }
                }

                column = ogCol;
                level = ogLevel; // Go back to start
                level++;
                column--; // Look up and left
                while (board->checkerAt(column, level) == RED) {
                    level++;
                    column--;       // If we're within bounds, increment and continue along the diagonal
                    backslashes++;
                    if (backslashes == m_goal) {
                        winner = RED;
                        return true;
                    }
                }

                column = ogCol;
                level = ogLevel; // Go back to start
                level--;
                column++; // Look down and right
                while (board->checkerAt(column, level) == RED) {
                    level--;
                    column++;
                    backslashes++;
                    if (backslashes == m_goal) {
                        winner = RED;
                        return true;
                    }
                }
                column = ogCol;
                level = ogLevel;
            }

                // Now we have the alternate line if we're looking at a black checker

                if (board->checkerAt(column, level) == BLACK) {   // If we're looking at a BLACK checker
                    int horizontals = 1;
                    int verticals = 1;
                    int forwardslashes = 1;
                    int backslashes = 1;        // The directions of victory that exist each have a counter

                    int ogLevel = level;
                    int ogCol = column; // Save the starting points

                    level++; // Look up
                    while (board->checkerAt(column, level) == BLACK) {
                        level++;            // If we're within bounds and looking at another red, increment the vertical counter and move up
                        verticals++;
                        if (verticals == m_goal) {
                            winner = BLACK;
                            return true;
                        }
                    }

                    level = ogLevel; // Go back to the start
                    level--; // Look down
                    while (board->checkerAt(column, level) == BLACK) {
                        level--;            // If we're within bounds and looking at another red, increment the vertical counter and move down
                        verticals++;
                        if (verticals == m_goal) {
                            winner = BLACK;
                            return true;
                        }
                    }

                    level = ogLevel; // Go back to the start
                    column++; // Look right
                    while (board->checkerAt(column, level) == BLACK) {
                        column++;       // If we're within bounds, increment and look further right
                        horizontals++;
                        if (horizontals == m_goal) {
                            winner = BLACK;
                            return true;
                        }
                    }

                    column = ogCol; // Go back to the start
                    column--; // Look left
                    while (board->checkerAt(column, level) == BLACK) {
                        column--;   // If we're within bounds, increment and look further left
                        horizontals++;
                        if (horizontals == m_goal) {
                            winner = BLACK;
                            return true;
                        }
                    }

                    column = ogCol; // Go back to start
                    level--;
                    column--; // Look down and left
                    while (board->checkerAt(column, level) == BLACK) {
                        level--;        // If we're within bounds, increment and continue along the diagonal
                        column--;
                        forwardslashes++;
                        if (forwardslashes == m_goal) {
                            winner = BLACK;
                            return true;
                        }
                    }

                    column = ogCol;
                    level = ogLevel; // Go back to start
                    level++;
                    column++;
                    while (board->checkerAt(column, level) == BLACK) {
                        level++;
                        column++;       // If we're within bounds, increment and continue along the diagonal
                        forwardslashes++;
                        if (forwardslashes == m_goal) {
                            winner = BLACK;
                            return true;
                        }
                    }

                    column = ogCol;
                    level = ogLevel; // Go back to start
                    level++;
                    column--; // Look up and left
                    while (board->checkerAt(column, level) == BLACK) {
                        level++;
                        column--;       // If we're within bounds, increment and continue along the diagonal
                        backslashes++;
                        if (backslashes == m_goal) {
                            winner = BLACK;
                            return true;
                        }
                    }

                    column = ogCol;
                    level = ogLevel; // Go back to start
                    level--;
                    column++; // Look down and right
                    while (board->checkerAt(column, level) == BLACK) {
                        level--;
                        column++;
                        backslashes++;
                        if (backslashes == m_goal) {
                            winner = BLACK;
                            return true;
                        }
                    }
                    column = ogCol;
                    level = ogLevel;
                } 
           
        }
        //level = 1;
    }

    if (board->numberEmpty() == 0) {
        winner = TIE_GAME;  // If the board is full but no winner was found , it's tied
        return true;
    }
    return false;  //  If no winner was found, return false
}

bool GameImpl::takeTurn()
{
    int winner;
    if (completed(winner)) {    // If the game is already over return false
        return false;
    }
    if (m_redsturn) {   // If it's red turn
        int col = m_red->chooseMove(*board, m_goal, RED); // Pick the column
        board->makeMove(col, RED);  // Make the move on the board
        m_redsturn = false; // Flip whose turn it is
        return true;
    }
    else {      // The process if it's black's turn
        int col = m_black->chooseMove(*board, m_goal, BLACK);
        board->makeMove(col, BLACK);
        m_redsturn = true;
        return true;
    }

}

void GameImpl::play()
{
    board->display();       // Display the initial board
    std::cout << "Begin the game." << std::endl;
    bool robots;    
    robots = (!m_red->isInteractive() && !m_black->isInteractive());    // Check if there's a human player
    int winner;
    while (!completed(winner)) {        // While the game isn't over
        takeTurn();     // Take a player's turn
        board->display();   // Display the new board
        std::cout << "A move has been made." << std::endl;  // Announce that a move was made
        if (robots) {       // If there's no human player, make user press enter to continue so game doesn't flash by
            std::cout << "Press enter to continue.";
            std::cin.ignore(1000, '\n');
        }
    }
    if (winner == 0) {  // Depending on the outcome, comment out the appropriate finishing sentence
        std::cout << m_red->name() << " has won the game." << std::endl;
    }
    else if (winner == 1) {
        std::cout << m_black->name() << " has won the game." << std::endl;
    }
    else {
        std::cout << "Tied game." << std::endl;
    }
}

int GameImpl::checkerAt(int c, int r) const
{
    return board->checkerAt(c, r);  
}

//******************** Game functions *******************************

//  These functions simply delegate to GameImpl's functions.
//  You probably don't want to change any of this code. 

Game::Game(int nColumns, int nLevels, int N, Player* red, Player* black)
{
    m_impl = new GameImpl(nColumns, nLevels, N, red, black);
}

Game::~Game()
{
    delete m_impl;
}

bool Game::completed(int& winner) const
{
    return m_impl->completed(winner);
}

bool Game::takeTurn()
{
    return m_impl->takeTurn();
}

void Game::play()
{
    m_impl->play();
}

int Game::checkerAt(int c, int r) const
{
    return m_impl->checkerAt(c, r);
}
