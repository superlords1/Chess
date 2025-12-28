#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <time.h>


#define SIZE 8
#define INPUTSIZE 4
typedef enum {White, Black, Non} Color;
typedef enum {Menu, PvP, PvC, CvC, End} GameState;


typedef struct {
    Color color;
    char piece;
}Pos;   //Position




typedef struct {
    int x;
    int y;
} Cord;




//Menu functions
GameState menu_select();




//Board functions
void gen_blank_board(Pos board[SIZE][SIZE]);
void init_board(Pos board[SIZE][SIZE]);
void print_board();




//Game functions
void move_piece(Pos board[SIZE][SIZE], int *input, int *computerInput);
void player_movement(Pos board[SIZE][SIZE], int *input, int *computerInput, int repeat);
int move_legality (Pos board[SIZE][SIZE], int *input);
int pawn_promotion(Pos board[SIZE][SIZE], Pos pieceToMove, int endRank);
Cord get_king_cord(Pos board[SIZE][SIZE], Color c);
bool is_checked(Pos board[SIZE][SIZE], int *input, Color c);




//Computer functions
void computer_ai_movement(Pos board[SIZE][SIZE], int *computerInput);








int main() {




    Pos chessBoard[SIZE][SIZE];
    int playerInput[INPUTSIZE] = {-1, -1, -1, -1};
    int computerInput[INPUTSIZE] = {-1, -1, -1, -1};
    GameState currentState = Menu;
    Color winner = Non;
    bool inCheckMate = false;
    bool playerIsCheck = false;
    bool computerIsCheck = false;




    init_board(chessBoard);
   
    while(1) {
        switch (currentState) {
            case Menu:  //Menu functions
   
                currentState = menu_select();
                break;


            case PvP: //Player vs Player
                currentState = End;
                break;


            case PvC:  //Player vs Computer
                print_board(chessBoard);
                while(!inCheckMate) {
                    player_movement(chessBoard, playerInput, computerInput, 0);
                    computer_ai_movement(chessBoard, computerInput);
                }
                currentState = End;
                break;


            case CvC: //Computer vs Computer
                currentState = End;
                break;


            case End:   //End functions
                switch(winner) {
                    case White:
                        printf("Checkmate! White wins!");
                    case Black:
                        printf("Checkmate! Black wins!");
                }
                return 0;
        }
    }




    return 0;




}




bool is_checked(Pos board[SIZE][SIZE], int *input, Color c) {


    Cord king = get_king_cord(board, c);


    int atkMove[4] = {input[2], input[3], king.x, king.y};


    if (move_legality(board, atkMove) == 1) {
        return true;
    } else {
        return false;
    }


}








Cord get_king_cord(Pos board[SIZE][SIZE], Color c) {




    Cord data;




    for (int i = 0; i < SIZE; i++) {
        for (int j = 0; j < SIZE; j++)
            if(board[i][j].color == c && board[i][j].piece == 'K') {
                data.x = j;
                data.y = i;
            }
    }




    return data;
}




void player_movement(Pos board[SIZE][SIZE], int *input, int *computerInput, int repeat) {


    char charArr[INPUTSIZE];


    if(repeat == 0) {
        printf("===================================================\n");
        printf("Player: White\n");
        printf("===================================================\n\n");
    }
     
    //Get coordinates
    printf("Enter a piece to move: ");
    scanf(" %c%c", &charArr[0], &charArr[1]);
    printf("Enter a valid move: ");
    scanf(" %c%c", &charArr[2], &charArr[3]);
    printf("\n");


    input[0] = charArr[0] - 'a';
    input[1] = charArr[1] - '1';
    input[2] = charArr[2] - 'a';
    input[3] = charArr[3] - '1';


    move_piece(board, input, computerInput);
}




void move_piece(Pos board[SIZE][SIZE], int *input, int *computerInput) {


    int startFile = input[0];
    int startRank = input[1];
    int endFile = input[2];
    int endRank = input[3];


    bool playerIsCheck = is_checked(board, computerInput, White);


    if (move_legality(board, input) == 1 && !playerIsCheck) {
        Pos temp = board[startRank][startFile];
        board[startRank][startFile].piece = ' ';
        board[startRank][startFile].color = Non;
        board[endRank][endFile] = temp;
    } else if (playerIsCheck) {
        printf("Can't move that piece. King is in check!\n\n");
        player_movement(board, input, computerInput, 1);
    } else {
        printf("Invalid move. Try again. [ex. 'a1']\n\n");
        player_movement(board, input, computerInput, 1);
    }
}




int move_legality (Pos board[SIZE][SIZE], int *input){


    int startFile = input[0];
    int startRank = input[1];
    int endFile = input[2];
    int endRank = input[3];


    Pos pieceToMove = board[startRank][startFile];
    Pos endPosition = board[endRank][endFile];


    //Check if end position is same color
    if (pieceToMove.color == endPosition.color) {
        return 0;
    }
   
    //Validate piece move
    switch (pieceToMove.piece) {
        case 'P':   //Pawn
            if (endFile == startFile && (abs(endRank - startRank) == 1) && endPosition.piece == ' '){ // pawn moves up 1
                break;
            } else if (startRank == 1 && pieceToMove.color == White && (abs(endRank - startRank) == 2) && endPosition.piece == ' '){ // first pawn move of white
                break;
            } else if (startRank == 6 && pieceToMove.color == Black && (abs(endRank - startRank) == 2) && endPosition.piece == ' '){ // first pawn move of black
                break;
            } else if (((pieceToMove.color == White && endPosition.color == Black) || (pieceToMove.color == Black && endPosition.color == White)) && abs(endRank - startRank) == 1 && abs(endFile - startFile) == 1){ // capture diagonally
                break;
            } else{
                return 0;
            }
            break;


       
        case 'H':   //Horse
            if ((abs(endFile - startFile) == 2 && abs(endRank - startRank) == 1) || (abs(endFile - startFile) == 1 && abs(endRank - startRank) == 2)) {
                return 1;
            }
            break;


        case 'R':   //Rook
            if (!(startRank == endRank || startFile == endFile)){
                return 0;
            }
            break;


        case 'B':   //Bishop
            if (!(abs(endRank - startRank) == abs(endFile - startFile))){
                return 0;
            }
            break;


        case 'Q':   //Queen
            if (!(abs(endFile - startFile) == abs(endRank - startRank) || startFile == endFile || startRank == endRank)){
                return 0;
            }
            break;


        case 'K':   //King
            if (!(abs(endFile - startFile) <= 1 && abs(endRank - startRank) <= 1)){
                return 0;
            }
            break;
    }
   
    int rankDirection;
    int fileDirection;
   
    if (endRank - startRank > 0) {
        rankDirection = 1;
    } else if (endRank - startRank < 0) {
        rankDirection = -1;
    } else {
        rankDirection = 0;
    }


    if (endFile - startFile > 0) {
        fileDirection = 1;
    } else if (endFile - startFile < 0) {
        fileDirection = -1;
    } else {
        fileDirection = 0;
    }
   
    for (int r = startRank + rankDirection, f = startFile + fileDirection; r != endRank || f != endFile; r += rankDirection, f += fileDirection) {
        if (board[r][f].piece != ' '){
            return 0;
        }
    }
   
    // Pawn Promotion
    char newPiece;
    if (pieceToMove.piece == 'P'){
        if (pawn_promotion(board, pieceToMove, endRank) == 1){
            printf("Select a new piece(R, H, B, Q): ");
            scanf(" %c", &newPiece);
            board[startRank][startFile].piece = newPiece;
        }
    }
    return 1;


}


//Checks if a pawn has gotten to the other side of the board
int pawn_promotion(Pos board[SIZE][SIZE], Pos pieceToMove, int endRank){
    if (pieceToMove.color == White && endRank == 7){
        return 1;
    } else if (pieceToMove.color == Black && endRank == 0){
        return 1;
    }
    return 0;
}


void computer_ai_movement (Pos board [SIZE][SIZE], int *input) {


    char computerInput[4];


    while (1) {


        srand(time(NULL));


        unsigned char x = (rand() % 8);
        unsigned char y = (rand() % 8);


        computerInput[0] = y + 'a';
        computerInput[1] = x + '1';


        if (board[x][y].color == Black && (board[x][y].piece == 'H' || board[x][y].piece ==  'P' || board[x][y].piece ==  'R' || board[x][y].piece ==  'B')) {
            switch (board [x][y].piece) {
                case 'P':  //Pawn        
                    computerInput[2] = computerInput[0];
                    computerInput[3] = computerInput [1] - 1;
                    break;


                case 'H': //Horse
                    do {
                        int possibleMoves[8][2] = {
                            {-2, -1}, {-2, 1}, {-1, -2}, {-1, 2},
                            {1, -2}, {1, 2}, {2, -1}, {2, 1}
                        };  
                        int randomMove = rand() % 8;
                        computerInput[2] = computerInput[0] + possibleMoves[randomMove][0];
                        computerInput[3] = computerInput[1] + possibleMoves[randomMove][1];
                    } while (computerInput[2] > 'h' || computerInput[2] < 'a' || computerInput[3] > '8' || computerInput [3] < '1' || board[computerInput[3] - '1'][computerInput[2]-'a'].color == Black);
                    break;


                case 'R': //Rook
                    do {
                        int verticalorHorizontalMovement = rand() % 2;
                        int randommove = rand() % 8;
                        if (verticalorHorizontalMovement == 0) {
                            computerInput[2] = randommove + 'a';
                            computerInput[3] = computerInput[1];
                        }
                        else {
                            computerInput[2] = computerInput[0];
                            computerInput[3] = randommove + '1';
                        }
                    } while (computerInput[2] > 'h' || computerInput[2] < 'a' || computerInput[3] > '8' || computerInput [3] < '1'  || board[computerInput[3] - '1'][computerInput[2]-'a'].color == Black);
                    break;


                case 'B': //Bishop
                    do {
                        int diagonal = rand() % 4; // Choose a random diagonal (0 to 3)
                        int diagonalMagnitude = (rand() % 6) + 1;
                        switch (diagonal) {
                            case 0: //Top-left diagonal
                                computerInput[3] = computerInput[1] + diagonalMagnitude;
                                computerInput[2] = computerInput[0] - diagonalMagnitude;
                                break;
                            case 1: //Top-right diagonal
                                computerInput[3] = computerInput[1] + diagonalMagnitude;
                                computerInput[2] = computerInput[0] + diagonalMagnitude;
                                break;
                            case 2: //Bottom-left diagonal
                                computerInput[3] = computerInput[1] - diagonalMagnitude;
                                computerInput[2] = computerInput[0] - diagonalMagnitude;
                                break;
                            case 3: //Bottom-right diagonal
                                computerInput[3] = computerInput[1] - diagonalMagnitude;
                                computerInput[2] = computerInput[0] + diagonalMagnitude;
                                break;
                        }
                    } while (computerInput[2] > 'h' || computerInput[2] < 'a' || computerInput[3] > '8' || computerInput [3] < '1' || board[computerInput[3] - '1'][computerInput[2]-'a'].color == Black);
                    break;


                case 'Q': //Queen
                    do {
                        int diagonalOrVerticalorHorizontalMovement = rand() % 6;
                        int diagonalMagnitude = (rand() % 6) + 1;
                        int randommove = rand() % 8;
                        switch (diagonalOrVerticalorHorizontalMovement) {  
                            case 0: // Top-left diagonal
                                computerInput[3] = computerInput[1] + diagonalMagnitude;
                                computerInput[2] = computerInput[0] - diagonalMagnitude;
                                break;
                            case 1: // Top-right diagonal
                                computerInput[3] = computerInput[1] + diagonalMagnitude;
                                computerInput[2] = computerInput[0] + diagonalMagnitude;
                                break;
                            case 2: // Bottom-left diagonal
                                computerInput[3] = computerInput[1] - diagonalMagnitude;
                                computerInput[2] = computerInput[0] - diagonalMagnitude;
                                break;
                            case 3: // Bottom-right diagonal
                                computerInput[3] = computerInput[1] - diagonalMagnitude;
                                computerInput[2] = computerInput[0] + diagonalMagnitude;
                                break;
                            case 4:
                               computerInput[2] = randommove + 'a';
                                computerInput[3] = computerInput[1];
                                break;
                            case 5:
                                computerInput[2] = computerInput[0];
                                computerInput[3] = randommove + '1';
                            break;
                        }
                    } while (computerInput[2] > 'h' || computerInput[2] < 'a' || computerInput[3] > '8' || computerInput [3] < '1' || board[computerInput[3] - '1'][computerInput[2]-'a'].color == Black);
                    break;


                case 'K': //King
                    do {
                        int diagonalOrVerticalorHorizontalMovement = rand() % 6;
                        int diagonalMagnitude =  1;
                        int randommove = 1;
                        switch (diagonalOrVerticalorHorizontalMovement) {  
                            case 0: // Top-left diagonal
                                computerInput[3] = computerInput[1] + diagonalMagnitude;
                                computerInput[2] = computerInput[0] - diagonalMagnitude;
                                break;
                            case 1: // Top-right diagonal
                                computerInput[3] = computerInput[1] + diagonalMagnitude;
                                computerInput[2] = computerInput[0] + diagonalMagnitude;
                                break;
                            case 2: // Bottom-left diagonal
                                computerInput[3] = computerInput[1] - diagonalMagnitude;
                                computerInput[2] = computerInput[0] - diagonalMagnitude;
                                break;
                            case 3: // Bottom-right diagonal
                                computerInput[3] = computerInput[1] - diagonalMagnitude;
                                computerInput[2] = computerInput[0] + diagonalMagnitude;
                                break;
                            case 4:
                                computerInput[2] = randommove + 'a';
                                computerInput[3] = computerInput[1];
                                break;
                            case 5:
                                computerInput[2] = computerInput[0];
                                computerInput[3] = randommove + '1';
                                break;
                        }
                    } while (computerInput[2] > 'h' || computerInput[2] < 'a' || computerInput[3] > '8' || computerInput [3] < '1' || board[computerInput[3] - '1'][computerInput[2]-'a'].color == Black);
                    break;
            }
           
            int startFile = computerInput[0] - 'a';
            int startRank = computerInput[1] - '1';
            int endFile = computerInput[2] - 'a';
            int endRank = computerInput[3] - '1';


		//return computer input as int array
            input[0] = startFile;
            input[1] = startRank;
            input[2] = endFile;
            input[3] = endRank;


            board[endRank][endFile].piece = board[startRank][startFile].piece;
            board[endRank][endFile].color = board[startRank][startFile].color;
            board[startRank][startFile].color = Non;
            board[startRank][startFile].piece = ' ';    


            printf("===================================================\n");
            printf("Computer: Black\n");
            printf("===================================================\n\n");
            printf("b%c has moved from %c%c to %c%c\n\n", board[endRank][endFile].piece, computerInput[0], computerInput[1], computerInput[2],computerInput[3]);
            print_board(board);
            break;
        }    
    }
}


GameState menu_select()
{


    int select;


    // Print menu
    printf("===================================================\n");
    printf("CHESS GAME\n");
    printf("team 20: Queen's Gambit \n");
    printf("===================================================\n\n");
    printf("* Select a number to choose gamemode   *\n");
    printf("* (1) Player vs Player                 *\n");
    printf("* (2) Player vs Computer               *\n");
    printf("* (3) Computer vs Computer             *\n");
    printf("* (4) Quit                             *\n\n");
    printf("===================================================\n\n");
   
    while(1) {
        printf("Selection: ");
        scanf("%d", &select);
        printf("\n");


        switch (select) {
            case 1:
                return PvP;
            case 2:
                return PvC;
            case 3:
                return CvC;
            case 4:
                return End;
            default:
                printf("Invalid choice. Try again.\n\n");
        }
    }
}




void print_board(Pos board[SIZE][SIZE]) {
    for (int i = SIZE - 1; i >= 0; i--) {
        printf("  +-----+-----+-----+-----+-----+-----+-----+-----+\n");
        printf("%d ", i + 1);
        for (int j = 0; j < SIZE; j++) {      //Print squares  
            switch (board[i][j].color) {
                case White:
                    printf("| %c%c  ", 'w', board[i][j].piece);
                    break;
                case Black:
                    printf("| %c%c  ", 'b', board[i][j].piece);
                    break;
                default:
                    printf("|     ");
            }  
        }
        printf("|\n");
    }
    printf("  +-----+-----+-----+-----+-----+-----+-----+-----+\n");
    printf("     a     b     c     d     e     f     g     h\n\n");
}




void gen_blank_board(Pos board[SIZE][SIZE]) {


    Pos p = {Non, ' '};


    for (int i = 0; i < SIZE; i++) {
        for (int j = 0; j < SIZE; j++)
            board[i][j] = p;
    }




}




void init_board(Pos board[SIZE][SIZE]) {


    //Pieces
    Pos R = {White, 'R'};
    Pos H = {White, 'H'};
    Pos B = {White, 'B'};
    Pos Q = {White, 'Q'};
    Pos K = {White, 'K'};
    Pos P = {White, 'P'};
    //Blank space
    Pos X = {Non, ' '};


    Pos tempBoard[SIZE][SIZE] = {
        {R, H, B, Q, K, B, H, R},
        {P, P, P, P, P, P, P, P},
        {X, X, X, X, X, X, X, X},
        {X, X, X, X, X, X, X, X},
        {X, X, X, X, X, X, X, X},
        {X, X, X, X, X, X, X, X},
        {P, P, P, P, P, P, P, P},
        {R, H, B, Q, K, B, H, R}
    };


    for (int i = 0; i < SIZE; i++) {
        for (int j = 0; j < SIZE; j++) {
            if(i == 6 || i == 7)
                tempBoard[i][j].color = Black;
        }
    }


    for (int i = 0; i < SIZE; i++) {
        for (int j = 0; j < SIZE; j++)
            board[i][j] = tempBoard[i][j];
    }


}
