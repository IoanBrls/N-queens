#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <ctime>
#include <math.h>

using namespace std;

void printBoard(int n, int *board);
int *randomPos(int n);
int numOfConflicts(int n, int *board);
int findMinConflictions(int n, int *board);
int *hillClimbing(int n);
int checkBoard(int n, int *board);
int *InitBoard(int n);
int *dfs(int n, int *board, int col);
int *mainDfs(int n);


int main()
{
    // num of queens
    int n;
    // just a variable for algorithm selection
    int help;
    // the time variables
    clock_t startclock, endclock;
    //srand(time(0));
    // some simple interface
    cout << "Insert the number of queens (board size): ";
    cin >> n;
    cout << endl << "Choose the preferred search algorithm: " << endl;
    cout << "(1) Hills Climbing Search Algorithm (Local Search)" << endl;
    cout << "(2) Depth First Search Algorithm (Online Search)" << endl;
    cout << "Press 1 or 2: ";
    cin >> help;

    // print the board size
    cout << endl << "The board size is: " << n << "x" << n << endl;

    // create and initialize the board
    int *board=new int[n];
    for (int i=0; i<n; i++)
        board[i]=-1;

    // start time of the algorithm
    startclock=clock();

    // search algorithm execution
    if (help==1)
        board=hillClimbing(n);
    else if (help==2)
        board=mainDfs(n);


    //print the solution
    cout << endl << "The solution is the following: " << endl;
    printBoard(n, board);

    // end time of the algorithm
    endclock=clock();

    // print the algorithm executed time
    cout << endl << "Executed in " << ((float)endclock-startclock)/CLOCKS_PER_SEC << " seconds." << endl << endl;
    return 0;
}

/// prints the chess board
/// Queens are pressented with the letter Q
/// Empty squares are pressented with * symbol
void printBoard(int n, int *board)
{
    for (int i=0; i<n; i++)
        for (int j=0; j<n; j++)
            if (j<n-1)
                if (board[j]!=-1 && board[j]==i)
                    cout << "Q" << " ";
                else
                    cout << "*" << " ";
            else
                if (board[j]!=-1 && board[j]==i)
                    cout << "Q" << endl;
                else
                    cout << "*" << endl;
}

/// Puts queens randomly in the board
int *randomPos(int n)
{
    int *board=new int[n];
    for (int i=0; i<n; i++)
        board[i]=rand()%n;
    return board;
}

/// Computes and returns the number of conflicts in the board
int numOfConflicts(int n, int *board)
{
    int conflicts=0;
    for (int i=0; i<n; i++)
        for (int j=i+1; j<n; j++)
            if ((board[i]==board[j] || (board[i]!=n+1 && board[j]!=n+1 && abs(board[i]-board[j])==abs(i-j))) && board[i]!=n+1 && board[j]!=n+1)
                conflicts++;
    //cout << endl << "CONFLICTS " << conflicts << endl;
    return conflicts;
}

int findMinConflictions(int n, int *board)
{
    int conflicts=numOfConflicts(n, board);
    if (conflicts==0)
        return 0;
    int nrow=-1;
    int ncol=-1;
    for (int i=0; i<n; i++)
    {
        int temp=board[i];
        for (int j=0; j<n; j++)
        {
            if (j==temp)
                continue;
            board[i]=j;
            int newConflicts=numOfConflicts(n, board);
            if (newConflicts<conflicts)
            {
                conflicts=newConflicts;
                nrow=i;
                ncol=j;
            }
        }
        board[i]=temp;
    }
    if (nrow==-1 && ncol==-1)
        return -1;
    board[nrow]=ncol;
    return conflicts;
}

/// hill climbing search algorithm
int *hillClimbing(int n)
{
    int *board=new int[n];
    int searches=0;
    while (true)
    {
        int adjust_times=0;
        // Put queens randomly in the board
        board=randomPos(n);
        // return the board if there are not conflicts
        if (numOfConflicts(n, board)==0)
        {
            cout << endl << "Total number of searches: " << searches;
            cout << endl << "Number of steps in this solution: " << adjust_times << endl;
            return board;
        }
        while (true)
        {
            int conflictions=findMinConflictions(n, board);
            adjust_times++;
            if (conflictions==0)
            {
                cout << endl << "Total number of searches: " << searches;
                cout << endl << "Number of steps in this solution: " << adjust_times << endl;
                return board;
            }
            else if (conflictions==-1)
                break; // Re-initialization
        }
        delete board;
        searches++;
    }
}


/// Initialize board
int *InitBoard(int n)
{
    int *board=new int[n];
    for (int i=0; i<n; i++)
        board[i]=n+1;
    return board;
}

/// Place a queen
// places an queen in the column con and in indx row
int *Putin(int col, int indx, int *board)
{
    board[col]=indx;
    return board;
}

/// Checks if there are queens in the board
// returns 0 if there are not queens in the board
// returns 1 if there are some queens in the board
int checkBoard(int n, int *board)
{
    int tmp=0;
    for (int i=0; i<n; i++)
        if (board[i]==n+1)
            tmp++;

    // checks if there are any queens in the board
    if (tmp==n)
        return 0;
    else
        return 1;
}

/// The dfs algorithm
int *dfs(int n, int *board, int col)
{
    int t=0;
    if (col==(n+1))
        return board;
    else
    {
        for (int i=0; i<n; i++)
        {
            if (numOfConflicts(n, board)==0)
            {
                board[col]=i;
                col++;
                int *tmp=new int[n];
                t=0;
                tmp=dfs(n, board, col);
                if (checkBoard(n, tmp)==1)
                {
                    return tmp;
                }
                if (col>0 && t!=1)
                {
                    col--;
                    board[col]=n+1;
                    t=1;
                }
            }
        }
    }
    return InitBoard(n);
}

//
// Calls the dfs algorithm and returns the solution
int *mainDfs(int n)
{
    int col=0;
    int *board=new int[n];
    board=dfs(n, InitBoard(n), col);
    return board;
}


