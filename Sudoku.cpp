// Sudoku.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include <string>
#include <fstream>



typedef struct SudokuPuzzle {
    int numberOfRows;
    int numberOfColumns;
    int puzzleNumbers[9][9] = {};
    int missingNumbers;  //how many numbers are left to fill the boxes
    int firstMissingNumber; //position of first missing number in the array
    int lastMissingNumber; //position of last missing number in the array

};


SudokuPuzzle* loadPuzzle(string filename) {
    //load the sample puzzles 
    ifstream inputfile("sudoku.txt");
    string line; //store each line from txt in the variable line

    while (getline(inputfile, line))   //read each line from the text
    {
        cout << line << endl;
        //print number of missing numbers
        //int numberOfMisingDigits;
        //cout<<

        //print position of first missing number
        //print position of last missing number
        //print the numbers that exist & their position 


    }
}

template <typename T>
struct Stack {

    T _data[MAX_SIZE];
    unsigned int _top = 0;

    bool isFull(Stack& s)
    {
        return _top == MAX_SIZE - 1;
    }

    bool isEmpty(Stack& s)
    {
        return _top == 0;
    }

    void push(T newValue);
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

    T top(Stack& s)
    {
        return _data[top - 1];
    }

};


int main()
{



}

