// Sudoku.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include <string>
#include <fstream>

using namespace std;

#define MAX_SIZE 81   //9x9 numbers
#define GRID_SIZE 3 


struct Coordinates
{
    short Row;
    short Column;
};

struct CorrectNumber
{
    short Row;
    short Column;
    short numberTried;
};

struct SudokuPuzzle
{
    short numberOfRows;
    short numberOfColumns;
    short** puzzleNumbers;
    bool locked[9][9];// position of initially provided puzzle numbers
    short missingNumbers;  //how many numbers are left to fill the boxes-asssigned 0
    Coordinates firstMissingNumber; //position of first missing number in the array
    Coordinates lastMissingNumber; //position of last missing number in the array

};

struct SudokuSolver
{
    SudokuPuzzle* puzzle;
    short numOfSteps = 0;
    short Grid[3][3];
    short numbers[9] = { 1,2,3,4,5,6,7,8,9 };


    bool isRowClear(short row, short num) {
        for (short i = 0; i < 9; i++)
        {
            if (puzzle->puzzleNumbers[row][i] == num)

                return false;
        }
        // cout << "clear Row" << endl;
        return true;
    }




    bool isColumnClear(short column, short num) {
        for (short j = 0; j < puzzle->numberOfRows; j++)
        {
            if (puzzle->puzzleNumbers[j][column] == num)

                return false;

        }
        // cout << "clear Column" << endl;
        return true;
    }

    bool isGridClear(short row, short column, short num) {

        if (row < 0 || row >= 9 || column < 0 || column >= 9)
        {
            cout<<"Invalid Coordinates"<<endl;
            return false;
        }
        short startRow = row - row % GRID_SIZE;
        short startColumn = column - column % GRID_SIZE;

        for (short i = 0; i < GRID_SIZE; i++)
        {
            for (short j = 0; j < GRID_SIZE; j++)
            {
                if (puzzle->puzzleNumbers[i + startRow][j + startColumn] == num)
                {
                    //  cout << "Grid not clear" << endl;
                    return false;
                }
            }
        }

        // cout << "clear Grid" << endl;
        return true;
    }



    bool isValidMove(short row, short column, short num)
    {
        return (isRowClear(row, num) &&
            isColumnClear(column, num) &&
            isGridClear(row, column, num));


    }

};

//load puzzle
SudokuPuzzle* loadPuzzle(string filename)
{
    SudokuPuzzle* puzzle = new SudokuPuzzle();
    ifstream file(filename);
    file >> puzzle->numberOfRows; //the first number of txt is the number of rows
    file >> puzzle->numberOfColumns; //the second number of txt is the number of columns


    puzzle->puzzleNumbers = new short* [puzzle->numberOfRows]; //create the 2D array
    for (short i = 0; i < puzzle->numberOfRows; i++)
    {
        puzzle->puzzleNumbers[i] = new short[puzzle->numberOfColumns];
    }
    for (short i = 0; i < puzzle->numberOfRows; i++)//read the numbers
    {
        for (short j = 0; j < puzzle->numberOfColumns; j++)
        {
            file >> puzzle->puzzleNumbers[i][j];
        }
    }
    for (short i = 0; i < puzzle->numberOfRows; i++)
    {
        for (short j = 0; j < puzzle->numberOfColumns; j++)
        {
            if (puzzle->puzzleNumbers[i][j] != 0)
                puzzle->locked[i][j] = true;
            else
                puzzle->locked[i][j] = false;
        }
    }
    return puzzle;
}

void printPuzzle(SudokuPuzzle* puzzle)
{

    if (puzzle == nullptr)
    {

        cout << "Puzzle is empty";
        return;
    }
    cout << "+-------------------------+" << endl;
    for (short i = 0; i < puzzle->numberOfRows; i++)
    {
        for (short j = 0; j < puzzle->numberOfColumns; j++)
        {
            cout << "|" << puzzle->puzzleNumbers[i][j] << "|";
        }
        cout << endl;
    }
    cout << "+-------------------------+" << endl;
}
//check if numbers are more

//1st missing number


Coordinates firstMissingNumber(SudokuPuzzle* puzzle) {
    for (short i = 0; i < puzzle->numberOfRows; i++)
    {
        for (short j = 0; j < puzzle->numberOfColumns; j++)
        {
            if (puzzle->puzzleNumbers[i][j] == 0)
            {
                return Coordinates{ i,j };

            }
        }
    }
    return { -1,-1 };
}

Coordinates lastMissingNumber(SudokuPuzzle* puzzle)
{
    for (short i = puzzle->numberOfRows - 1; i >= 0; i--)
    {
        for (short j = puzzle->numberOfColumns - 1; j >= 0; j--)
        {
            if (puzzle->puzzleNumbers[i][j] == 0)
            {
                return Coordinates{ i,j };

            }
        }
    }

    return { -1,-1 };

}

void missing(SudokuPuzzle* puzzle) {
    short missingNumbers = 0;
    for (short i = 0; i < puzzle->numberOfRows; i++)
    {
        for (short j = 0; j < puzzle->numberOfColumns; j++)
        {
            if (puzzle->puzzleNumbers[i][j] == 0)
            {
                missingNumbers = missingNumbers + 1;

            }
            else
            {
                missingNumbers = missingNumbers + 0;
            }

        }
    }
    cout << "The number of missing numbers is:" << missingNumbers << endl;

}

template <typename T>
struct Stack {

    T _data[MAX_SIZE];
    unsigned short _top = 0;


    bool isFull()
    {
        return _top == MAX_SIZE;
    }

    bool isEmpty()
    {
        return _top == 0;
    }

    void push(T newValue)
    {
        if (!isFull())
        {
            _data[_top] = newValue;
            _top++;
        }
    }
    void pop()
    {
        if (!isEmpty())
        {
            _top--;
        }
    }

    T& top()
    {
        if (!isEmpty())
        { 
        return _data[_top - 1];
        }
    }
    
};

bool match(Coordinates a, Coordinates b) {
    return (a.Row == b.Row && a.Column == b.Column);
}



void sudokuSolver(SudokuPuzzle* puzzle)
{

    Stack<CorrectNumber>temporary;
    //Stack<Coordinates>temporary;
    Stack<CorrectNumber>saved;
    SudokuSolver solver;
    solver.puzzle = puzzle;



    //this loop is to put all the coordinates that have a number 0 in the puzzle
    //the loop starts from the last coordinate of the puzzle so that the first missing number is on the top of the "temporaryStack"
    for (short i = puzzle->numberOfRows - 1; i >= 0; i--)
    {
        for (short j = puzzle->numberOfColumns - 1; j >= 0; j--)
        {
            if (puzzle->puzzleNumbers[i][j] == 0)
            {
                temporary.push({ i,j,0 });
                cout << "in temporary stack:" << temporary.top().Row << "," << temporary.top().Column << "," << temporary.top().numberTried << "";
                cout << endl;
                //cout << "pushed:" << i<<","<<j;

            }
        }
    }


    while (!temporary.isEmpty())
    {
        CorrectNumber& currentCoords = temporary.top();// Working on the top of the Stack
        short row = currentCoords.Row;
        short col = currentCoords.Column;
        short numberAlreadyTried = currentCoords.numberTried;


        bool isANumberPlaced = false;

        for (short num = numberAlreadyTried; num <= 9; num++)
        {
            if (solver.isValidMove(row, col, num))//if the number doesn't already exist
            {
                puzzle->puzzleNumbers[row][col] = num; //place that number in those coordinates          
                currentCoords.numberTried = num;
                
                saved.push(currentCoords);//save it to the permanent stack
                temporary.pop();

                cout << "In the saved stack-------" << " " << saved.top().Row << "," << saved.top().Column << "," << saved.top().numberTried << endl;
                isANumberPlaced = true;

                break;
            }

        }


        //if a numbes is not valid to be put on a coordinate it means that the last assignment was wrong
        //get back the coordinates to the temporary stack and try another number
        if (!isANumberPlaced)
        {
            puzzle->puzzleNumbers[row][col] = 0;
            temporary.pop();


            if (!saved.isEmpty())
            {
                CorrectNumber previous = saved.top();
                saved.pop();

                if (previous.numberTried < 9)
                {
                    previous.numberTried++;

                    puzzle->puzzleNumbers[previous.Row][previous.Column] = 0;
                    temporary.push(previous);
                    cout << "Now in the temporary Stack>>>>" << " " << temporary.top().Row << "," << temporary.top().Column << "," << temporary.top().numberTried << endl;
                    // break;
                     /*puzzle->puzzleNumbers[currentCoords.Row][currentCoords.Column] = 0;
                      currentCoords.numberTried++;*/

                      //temporary.pop();
                }
                else
                {
                    puzzle->puzzleNumbers[previous.Row][previous.Column] = 0;

                }
            }
        }
    }
    
    cout << "Sudoku Solved" << endl;
    
    
    /*if (temporary.isEmpty())
    {
        cout << "Sudoku Solved" << endl;
    }*/

    //if (match(stack.top(), Coordinates{ lastMissingNumber(puzzle) }))
    //      {
    //         cout << "Sudoku Solved" << endl;
//}

}




int main()
{

    SudokuPuzzle* puzzle = loadPuzzle("sudoku2.txt");

    printPuzzle(puzzle);

    SudokuSolver solver;
    solver.puzzle = puzzle;
    Coordinates c, c1;
    c = firstMissingNumber(puzzle);
    cout << "First missing number" << "(" << c.Row << "," << c.Column << ")" << endl;
    c1 = lastMissingNumber(puzzle);
    cout << "Last missing number" << "(" << c1.Row << "," << c1.Column << ")" << endl;
    missing(puzzle);
    sudokuSolver(puzzle);
    printPuzzle(puzzle);
}


