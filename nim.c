/****************************************************************************
Student Name: Quinn Meiszinger
Class ID: CIS*2520 (0101) F21 - Data Structures
Instructor: Stefan Kremer
****************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "nim.h"


int *new_board( int board_size )
{

    //Allocate enough memory to store board_size integers
    int *board = malloc(sizeof(int) * board_size);
    if(board == NULL)
    {//If memory not allocated, print error and exit program
        fprintf(stderr, "     [ERROR] [*new_board()]: board returned NULL, exiting program...\n");
        exit(-1);
    }

    //Return pointer to board
    return board;

}

struct node *mk_nim_hash( int max_hash, int board_size, int *start_board )
{

    //Declare pointer array of node structs
    struct node *nimHash = malloc(sizeof(struct node) * max_hash);
    if(nimHash == NULL)
    {//If nimHash is NULL, throw an error and exit program
        fprintf(stderr, "     [ERROR] [*mk_nim_hash()]: nimHash returned NULL, exiting program...\n");
        exit(-1);
    } 

    for(int i = 0; i < max_hash; i++)
    {//Iterate through each index in nimHash

        //Set moves, move, and nimsum to default values
        nimHash[i].moves = -1;
        nimHash[i].move = NULL;
        nimHash[i].nimsum = -1;

        //Get board using hash2board function
        nimHash[i].board = hash2board(board_size, start_board, i);
        if(nimHash[i].board == NULL)
        {//If board is NULL, throw an error and exit program
            fprintf(stderr, "     [ERROR] [*mk_nim_hash()]: nimHash[i]->board returned NULL, exiting program...\n");
            exit(-1);
        }

    }

    //Return pointer to nimHash
    return nimHash;

}

void free_board( int *board )
{

    //Free memory allocated to board
    free(board);

}

void free_nim_hash( int max_hash, struct node *nim_hash )
{

    for(int i = 0; i < max_hash; i++)
    {//Iterate through each node struct in nim_hash

        //Free each move struct and board array
        free(nim_hash[i].move);
        free(nim_hash[i].board);

    }

    //Free memory allocated to nim_hash
    free(nim_hash);

}

int *board_from_argv( int board_size, char **argv )
{

    //Allocate sufficient memory to store a new board
    int *board = new_board( board_size );

    for(int i = 0; i < board_size; i++)
    {//Initialize each integer with command line input
        board[i] = atoi(argv[i]);
    }

    //Return pointer to board
    return board;

}

int *copy_board( int board_size, int *board )
{

    //Allocate sufficient memory to a new board
    int *newBoard = new_board( board_size );

    for(int i = 0; i < board_size; i++)
    {//Iterate through both boards and copy values
        newBoard[i] = board[i];
    }

    //Return pointer to newBoard
    return newBoard;

}

int game_over( int board_size, int *board )
{

    //Integer value to store total amount of matches
    int totalMatches = 0;

    for(int i = 0; i < board_size; i++)
    {//Add the number of matches of each line to totalMatches
        totalMatches += board[i];
    }

    if(totalMatches <= 1)
    {//If only one match remaining, return 1

        #ifdef DEBUG
        {
            printf("     [INFO] [game_over()]: game is over\n");
        }
        #endif

        return 1;
    }
    else
    {//Otherwise, return 0
        return 0;
    }

}

void join_graph( struct node *nim_hash, int hash, int board_size, int *start_board )
{

    //Calculate nimsum
    nim_hash[hash].nimsum = compute_nimsum( board_size, nim_hash[hash].board );

    if(nim_hash[hash].moves != -1)
    {//if moves is -1, return from function
        return;
    }

    //Integer to keep track of total moves
    int possibleMoves = 0;

    for(int i = 0; i < board_size; i++)
    {//Iterate through each board and computer possibleMoves
        possibleMoves += nim_hash[hash].board[i];
    }

    //Assign possible moves to moves in nim_hash
    nim_hash[hash].moves = possibleMoves;

    //Allocate memory to store all possible moves
    struct move *moveArray = malloc(sizeof(struct move) * possibleMoves);
    if(moveArray == NULL)
    {//If memory not allocated, throw an error and exit program
        fprintf(stderr, "     [ERROR] [join_graph()]: moveArray return NULL, exiting program...\n");
        exit(-1);
    }

    //Integer to keep track of which move to store
    int counter = 0;
    for(int i = 0; i < board_size; i++)
    {//For loop for iterating through each row

        for(int j = 0; j < nim_hash[hash].board[i]; j++)
        {//Nested for loop for iterating through each move

            //Set row to i
            moveArray[counter].row = i;

            //Set number of matches to remove to j+1
            moveArray[counter].matches = j + 1;

            #ifdef DEBUG
            {
                printf("\n");
                printf("     [INFO] [join_graph()]: moveArray[%d].row     = %d\n", counter, moveArray[counter].row);
                printf("                            moveArray[%d].matches = %d\n", counter, moveArray[counter].matches);
                printf("\n");
            }
            #endif

            //Increment counter
            counter++;

        }

    }

    for(int i = 0; i < nim_hash[hash].moves; i++)
    {//Iterate through each move in nim_hash

        //Copy board from current board
        int *board = copy_board( board_size, nim_hash[hash].board );

        //Apply generated move to board
        board[moveArray[i].row] -= moveArray[i].matches;

        //Calculate hash from board
        int hash = board2hash( board_size, start_board, board );

        //Set move hash to calculated hash
        moveArray[i].hash = hash;

        #ifdef DEBUG
        {//Debug print to show HASH numbers
            printf("     [INFO] [join_graph()]: moveArray[%d].hash    = %d\n", i, moveArray[i].hash);
        }
        #endif

        //Free memory allocated to copied board
        free(board);

    }

    //Store moveArray into nim_hash
    nim_hash[hash].move = moveArray;

    for(int i = 0; i < nim_hash[hash].moves; i++)
    {//Iterate through each move in moveArray

        //Recursively call join_graph for each move
        join_graph(nim_hash, nim_hash[hash].move[i].hash, board_size, start_board );

    }

}

int compute_nimsum( int board_size, int *board )
{  

    //Variable to keep track of whether the negation should be taken
    int isNegation = 1;

    //Variable to track the nimsum, initialized to the first value in board
    int nimsum = 0;

    for(int i = 0; i < board_size; i++)
    {

        //Take bitwise XOR or each value in board
        nimsum = nimsum ^ board[i];

        if(board[i] > 1)
        {//If any value in board is greater than 1, set isNegation to 0
            isNegation = 0;
        }

    }

    if(isNegation)
    {//If no values in board are greater than 1, take the negation of the nimsum
        nimsum = !nimsum;
    }

    #ifdef DEBUG
    {
        printf("     [INFO] [computer_nimsum()]: nimsum          = %d\n", nimsum);
    }
    #endif

    //Return the nimsum
    return nimsum;

}

