// Scaffold.cpp
#include <iostream>
#include <vector>
#include <cstdlib>
#include <stack>

#include "provided.h"
using namespace std;

class ScaffoldImpl
{
public:
    ScaffoldImpl(int nColumns, int nLevels);
    int cols() const;
    int levels() const;
    int numberEmpty() const;
    int checkerAt(int column, int level) const;
    void display() const;
    bool makeMove(int column, int color);
    int undoMove();
private:
    int m_cols;     // Cols and levels
    int m_levels;
    vector<vector<int>> m_board; // A vector of vectors to simulate the board
    struct movedata {   
        int* pos;   // A structure that contains all the moves and the columns they were made in
        int column;
    };
    stack<movedata> m_moves;    // A stack of the movedata for the purpose of undoing
    int m_movenumber = 0;   // A counter of movenumber to determine emptiness

};

ScaffoldImpl::ScaffoldImpl(int nColumns, int nLevels)
{
    if (nColumns <= 0 || nLevels <= 0) {    // Invalid parameters checker
        cerr << "Cols and Levels gotta be greater than 0";
        exit(1);
    }
    m_cols = nColumns;  // Assign the cols and levels
    m_levels = nLevels;
    m_board.resize(nLevels);    // Resize the levels appropriately for the vector of vectors
    for (int i = 0; i < nLevels; i++) {
        m_board[i].resize(nColumns);    // Resize each vector for the correct column size
    }
    for (int i = 0; i < nLevels; i++)       // Fill the entire board with -1 to simulate vacancy
        for (int j = 0; j < nColumns; j++)
            m_board[i][j] = -1;
}

int ScaffoldImpl::cols() const
{
    return m_cols;  // Return the cols
}

int ScaffoldImpl::levels() const
{
    return m_levels;  // Return the levels
}

int ScaffoldImpl::numberEmpty() const
{
    return (m_cols * m_levels) - m_movenumber;  //  Calculate the # of empty
}

int ScaffoldImpl::checkerAt(int column, int level) const
{

    if (column > 0 && level > 0 && column <= m_cols && level <= m_levels ) { // Check for validity of position
        if (m_board[level - 1][column - 1] == RED) {    // Just check the value at the correct board pos
            return RED;
        }
        if (m_board[level - 1][column - 1] == BLACK) {
            return BLACK;
        }
    } 
    return VACANT; // Return vacant if it didn't match red/black
}

void ScaffoldImpl::display() const {
    for (int k = m_levels - 1; k >= 0; k--) {   // Start at the top level, work downward
        for (int j = 0; j < m_cols; j++) {  // Start at the left column, work "rightward"
            cout << "|";    // The left wall of the board
            if (m_board[k][j] == 1) {
                cout << "B";    
            }
            else if (m_board[k][j] == 0) {      // Print out the appropriate thing for value at pos
                cout << "R";
            }
            else {
                cout << " ";
            }
        }
        cout << "|";    // The right wall of the board
        cout << endl;   // Go to the next line
    }

    for (int i = 0; i < 2 * m_cols + 1; i++) {
        if (i % 2 == 0) {
            cout << "+";    // This just makes the bottom of the display
        }                   // By alternating between + and - the appropriate number of times
        else {
            cout << "-";
        }
    }
    cout << endl;
}

bool ScaffoldImpl::makeMove(int column, int color)
{
    if (color != 0 && color != 1) {
        return false;
    }
    if (column > m_cols) {
        return false;           // A few validity checks for the inputs
    }                           // to prevent undefined behavior
    if (column <= 0) {
        return false;
    }
    column = column - 1;    // Correct the column index for convenience

    int j = 0;
    while (j < m_levels) {  // While within the height limit
        if (m_board[j][column] == -1) { // Check if there's an empty space where we're at
            m_board[j][column] = color;  // Make the move
            movedata added; // Add this new move to the move data
            added.column = column;  // with its column
            added.pos = &m_board[j][column];  // and position
            m_moves.push(added);    // Push it onto our movedata stack
            m_movenumber++; // Increment move counter
            return true;
        }
        j++;    // If there wasn't an empty space, move up and check again
    }
    return false;   // If we went beyond the height limit, that was not a valid move
}

int ScaffoldImpl::undoMove()
{
    if (!m_moves.empty()) { // Make sure moves have actually been made
        int* undone = m_moves.top().pos;    // Point to the move we're undoing
        int column = m_moves.top().column;  // and save its column
        m_moves.pop();  // Get that off the stack
        *undone = -1;  // Set the position to vacant
        m_movenumber--; // Decrement the move counter
        return column;  // Return its column
}
    return 0;
}

//******************** Scaffold functions *******************************

//  These functions simply delegate to ScaffoldImpl's functions.
//  You probably don't want to change any of this code. 

Scaffold::Scaffold(int nColumns, int nLevels)
{
    m_impl = new ScaffoldImpl(nColumns, nLevels);
}

Scaffold::~Scaffold()
{
    delete m_impl;
}

Scaffold::Scaffold(const Scaffold& other)
{
    m_impl = new ScaffoldImpl(*other.m_impl);
}

Scaffold& Scaffold::operator=(const Scaffold& rhs)
{
    if (this != &rhs)
    {
        Scaffold temp(rhs);
        swap(m_impl, temp.m_impl);
    }
    return *this;
}

int Scaffold::cols() const
{
    return m_impl->cols();
}

int Scaffold::levels() const
{
    return m_impl->levels();
}

int Scaffold::numberEmpty() const
{
    return m_impl->numberEmpty();
}

int Scaffold::checkerAt(int column, int level) const
{
    return m_impl->checkerAt(column, level);
}

void Scaffold::display() const
{
    m_impl->display();
}

bool Scaffold::makeMove(int column, int color)
{
    return m_impl->makeMove(column, color);
}

int Scaffold::undoMove()
{
    return m_impl->undoMove();
}
