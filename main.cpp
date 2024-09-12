/*
 * Name: [Isabella]
 * Description: Sudoku is a game where given a 9x9 grid, you fill in the missing numbers insuring each column, row, and 
 *              quadrant contains numbers 1 - 9. However, the catch is that each row, column, and 3 x 3 quadrant may not have 
 *              a duplicate number. This program allows a user to input an unsolved Sudoku puzzle to be solved 
 *              by a recursive algorithm.
 * Input: User inputs an unsolved Sudoku puzzle with the empty spaces listed as 0's
 * Output: The program will output the unsolved and solved puzzle for comparison, will not stop unless user inputs NO to file prompt
 */

#include <iostream>  //various basic functions
#include <fstream>   //file stream for reading in file with board set up
#include <iomanip>   //operations for formatting output

using namespace std;

/****Helper Functions***/
bool backTrack(int i, int j, int puzzle[][9]);
bool checkSpace(int i, int j, int n, int puzzle[][9]);
bool sectorCheck(int i, int j, int n, int puzzle[][9]);
void printBoardUnsolved(int puzzle[][9]);
void printBoardSolved(int puzzle[][9]);
void printLine();

int main()
{
    ifstream inFile;       //stream for input file
    string fileName;       //name of file
    bool endofGame = false; //for flagging end of game 

    int sudokuGame[9][9];   //game array 

    /*GAME LOOP*/
    while (!endofGame) {

        bool fileOpen = false; //check for file opening

        //File Prompt and check
        while (!fileOpen) {

            //User prompt for file
            cout << "Enter initial sudoku board file: ";
            cin >> fileName;
            cout << endl; //formatting

            inFile.open(fileName);

            //program will terminate if user enters NO in file prompt
            if (fileName == "NO") {
                endofGame = true;
                return 0;
            }

            //check if opened
            if (inFile.is_open()) {

                cout << "File read successfully." << endl;
                cout << endl; //formatting
                fileOpen = true;
            }
            else {

                //reprompt if file did not open
                cout << "ERROR: File \"" << fileName << "\" not found or could not be opened!" << endl;
                cout << "Please try again." << endl;
                fileOpen = false;

            }
        }

        int temp = 0; //temporary integer for reading in from file

        //Read in
        while (!inFile.eof()) {
            
            //iterate through array and assign numbers
            for (int i = 0; i < 9; i++) {
                for (int j = 0; j < 9; j++) {
                    inFile >> temp;
                    sudokuGame[i][j] = temp;
                }
            }
            inFile >> temp; //for flagging eof
        }

        //close file
        inFile.close();

        cout << "Initial board read in: " << endl;
        printBoardUnsolved(sudokuGame);
        cout << endl;

        //call backtrack algorithm to solve the puzzle
        //start at coord. (0,0) 
        bool gameSolved = backTrack(0, 0, sudokuGame);
        
        //final loop to check if the puzzle was solved
        if (gameSolved) {
            cout << "Sudoku puzzle solved: " << endl;
            printBoardSolved(sudokuGame);
            cout << endl;
        }
        else {
            //last resort if algorithm fails
            cout << "Error solving puzzle, but this is as far as we got: " << endl;
            printBoardSolved(sudokuGame);
            cout << endl;
        }
    }

}

/*
* function_identifier: Acts as a check for the backTrack function to make sure no duplicate numbers end up in the puzzle.
* parameters: takes 2 integer values that work as rows and columns, takes another int as the number to check, takes 9 x 9 array of ints
* return value: returns false if:
*               the same number is found in the 3 x 3 matrix that the backTrack function is working in
*               Othewise returns true.
*/
bool sectorCheck(int i, int j, int n, int puzzle[][9]) {

    //check 3x3
    //find the matrix by checking the bounds

    //boundary variables
    int rowMin = ((i / 3) * 3);
    int colMin = ((j / 3) * 3);

    //matrix row check and column check
    for (int x = 0; x < 3; x++) {
        for (int y = 0; y < 3; y++) {

            if (puzzle[x + rowMin][y + colMin] == n) {
                return false;
            }
        }
    }
    return true;

}

/*
* function_identifier: Acts as a check for the backTrack function to make sure no duplicate numbers end up in the puzzle.
* parameters: takes 2 integer values that work as rows and columns, takes another int as the number to check, takes 9 x 9 array of ints
* return value: returns false if: 
*               the same number is found in that row (aka invalid number), 
*               the same number is found in that column (aka invalid number);
*               Othewise returns true.
*/
bool checkSpace(int i, int j, int n, int puzzle[][9]) {

    //check column
    for (int x = 0; x < 9; x++) {
        //if the number x is also the number 9, we have a duplicate
        //therefore this number n will not work in this row / column
        if (puzzle[x][j] == n) {
            return false;
        }
    }

    //check row 
    for (int x = 0; x < 9; x++) {

        if (puzzle[i][x] == n) {
            return false;
        }
    }

    return true;
}

/*
* function_identifier: Solves the puzzle by iterating throw the array and trying numbers in each location 
*                      using recursion to find the valid solution.
* parameters: takes 2 integer values that work as rows and columns, takes 9 by 9 integer array
* return value: returns true if the puzzle was solved, returns false if the algorithm did not solve the puzzle
*/
bool backTrack(int i, int j, int puzzle[][9]) {

    //i is row
    //j is column

    //if we are outside the board index
    //the board must be solved
    if (i == 9) {
        return true;
    }

    //iterate to a blank space
    if (puzzle[i][j] != 0){
        return backTrack(i + (j + 1) / 9, (j + 1) % 9, puzzle);
    }

    //main recursion loop
    //checks the space to see if we can add this number
    //set new number in and call next row (within boundaries)
    //otherwise we can't add a number there
    for (int x = 1; x <= 9; x++) {
        if (sectorCheck(i, j, x, puzzle)) {

            if (checkSpace(i, j, x, puzzle)) {

                puzzle[i][j] = x;
                if (backTrack(i + ( j + 1) / 9, (j + 1) % 9, puzzle)) {
                    return true;
                }
            }
            puzzle[i][j] = 0;
        }
    }
    return false;    
}

/*
* function_identifier: outputs the UNSOLVED puzzle to the terminal
* parameters: takes a 9 by 9 array of integers
* return value: no return value
*/
void printBoardUnsolved(int puzzle[][9]) {

    printLine();
    for (int x = 0; x < 9; x++) {
        cout << "|";
        for (int y = 0; y < 9; y++) {
            if (puzzle[x][y] == 0) {

                if (y == 3 || y == 6) { cout << " | "; }
                cout << " - ";
            }
            else {

                if (y == 3 || y == 6) { cout << " | "; }
                cout << " " << puzzle[x][y] << " ";
            }
        }
        if (x == 2 || x == 5) {
            cout << "|" << endl;
            printLine();
        }
        else { cout << "|" << endl; } 
    }
    printLine();
}

/*
* function_identifier: outputs the SOLVED puzzle to the terminal
* parameters: takes a 9 by 9 array of integers
* return value: no return value
*/
void printBoardSolved(int puzzle[][9]) {

    printLine();
    for (int x = 0; x < 9; x++) {
        cout << "|";
        for (int y = 0; y < 9; y++){
            if (y == 3 || y == 6) { cout << " | "; }
            cout << " " << puzzle[x][y] << " ";
        }
        if (x == 2 || x == 5) {
            cout << "|" << endl;
            printLine();
        }
        else { cout << "|" << endl; }
    }
    printLine();

}

/*
* function_identifier: outputs a line of dashes to the terminal. Cleans up code a bit.
* parameters: no params
* return value: no return value
*/
void printLine() {
    cout << "-----------------------------------" << endl;
}
