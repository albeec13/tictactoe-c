#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define LEN_BOARD_SIDE  (3)
#define LEN_BOARD_TOTAL (LEN_BOARD_SIDE * LEN_BOARD_SIDE)
#define NUM_DIAGS       (2)

typedef enum {
    BLANK,
    X_BOX,
    O_BOX
} tBox;

typedef struct {
    tBox box[LEN_BOARD_TOTAL];
    int full;
    int gameOver;
    int rowScore[LEN_BOARD_SIDE];
    int colScore[LEN_BOARD_SIDE];
    int diagScore[NUM_DIAGS];
} tGameBoard;

void PrintBoard(tGameBoard *board);
int AI_Move(tGameBoard *board);
tBox CheckGameOver(tGameBoard *board);

int main()
{
    tGameBoard gameBoard = { .full = 0 , .gameOver = 0 };
    tBox winner = BLANK;
    
    for(int i=0; i < LEN_BOARD_TOTAL; i++)
    {
        gameBoard.box[i] = BLANK;
    }
    
    for(int i=0; i < LEN_BOARD_SIDE; i++)
    {
        gameBoard.rowScore[i] = 0;
        gameBoard.colScore[i] = 0;
        if(i < NUM_DIAGS)
            gameBoard.diagScore[i] = 0;
    }
    
    srand(time(NULL));
    
    PrintBoard(&gameBoard);
    
    while(!gameBoard.full && !gameBoard.gameOver)
    {
        static int odd = 1;
        int move = -1;
        
        if(odd)
        {
            printf("Enter box (1-%d): ", LEN_BOARD_TOTAL);
            
            if(scanf("%d", &move) < 1)
                break;
            
            if((move < 1) || (move > LEN_BOARD_TOTAL))
            {
                printf("Invalid box. ");
            }
            else if(gameBoard.box[move - 1] != BLANK)
            {
                printf("That box is taken. ");
            }
            else
            {
                gameBoard.box[move - 1] = odd ? X_BOX : O_BOX;
                odd = !odd;
                
                PrintBoard(&gameBoard);
                winner = CheckGameOver(&gameBoard);
            }
        }
        else
        {
            int AImove = AI_Move(&gameBoard);
            gameBoard.box[AImove - 1] = odd ? X_BOX : O_BOX;
            odd = !odd;
            PrintBoard(&gameBoard);
            printf("AI chose box %d.\n", AImove);
            winner = CheckGameOver(&gameBoard);
        }
    }
    
    printf("The winner is %s.\n", winner == X_BOX ? "X" : (winner == O_BOX ? "O" : "nobody"));

    return 0;
}

void PrintBoard(tGameBoard *board)
{
    board->full = 1;
    
    printf("%*s\n", ((LEN_BOARD_SIDE * 2 + 2) * 3 + 1 + 11)/2, "TIC-TAC-TOE");
    
    for(int i=0; i<LEN_BOARD_TOTAL; i++)
    {
        if((i % LEN_BOARD_SIDE) == 0)
        {
            printf("|");
            for(int j=0; j<LEN_BOARD_SIDE; j++)
            {
                if(board->box[i + j] == BLANK)
                    printf("%2d|", i + j + 1);
                else
                    printf("  |");
            }
            printf(" --> ");
        }
        
        switch(board->box[i])
        {
            case X_BOX:
                printf("| X");
                break;
            case O_BOX:
                printf("| O");
                break;
            default:
                printf("|  ");
                board->full = 0;
                break;
        }
        
        if(((i + 1) % LEN_BOARD_SIDE) == 0)
        {
            printf("|\n");
        }
    }
}

int AI_Move(tGameBoard *board)
{
    int move = rand() % LEN_BOARD_TOTAL + 1;
    
    while((!board->full) && (board->box[move - 1] != BLANK))
    {
        move = rand() % LEN_BOARD_TOTAL + 1;
    }
    
    return move;
}

tBox CheckGameOver(tGameBoard *board)
{
    tBox ret = BLANK;
    
    for(int i=0; i < LEN_BOARD_SIDE; i++)
    {
        if((board->rowScore[i] == LEN_BOARD_SIDE) || (board->colScore[i] == LEN_BOARD_SIDE))
        {
            ret = X_BOX;
            board->gameOver = 1;
            break;
        }
        
        if((board->rowScore[i] == -LEN_BOARD_SIDE) || (board->colScore[i] == -LEN_BOARD_SIDE))
        {
            ret = O_BOX;
            board->gameOver = 1;
            break;
        }
            
        if(i < NUM_DIAGS)
        {
            if(board->diagScore[i] == LEN_BOARD_SIDE)
            {
                ret = X_BOX;
                board->gameOver = 1;
                break;
            }
            
            if(board->diagScore[i] == -LEN_BOARD_SIDE)
            {
                ret = O_BOX;
                board->gameOver = 1;
                break;
            }
        }
    }
    
    return ret;
}

