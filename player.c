#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <ctype.h>

#define bool uint8_t
#define false 0
#define true 1

typedef struct ship{
    int length;
    int hp;
    char name[12];
    char shipAlpha;
} *Ship;

typedef struct grid{
    Ship s;
    char gridChar; ///at first, the char will be '-' for unknown but will change to the ship type
                    ///if the ship is hit, then will turn to 'H', if the enemy missed, then will turn to 'M'
} Grid;


void clearScreen(void){
    for(int i =0; i<300; ++i){
        printf("\n");
    }
}

typedef enum shipArrangement{ ///enums for the arrangement of the ship
    Vertical, Horizontal
}ShipArrangement;

typedef enum hitDirection{ ///enums to direct the enemy AI where to shoot at
    Up, Left, Right, Down, None
}HitDirection;

/**
*** Below are the struct and functions for the player
*/
struct player{
    Grid playerGrid[10][10];
    char enemyGrid[10][10]; ///used to keep track of hits or misses, should not actually contain the enemy's info
                            ///character arrangement same as the gridChar's
    Ship ships[5];///carrier, battleship, cruiser, submarine, destroyer
    uint8_t shipsLeft;
};

void resetPlayer(struct player *p){
    p->shipsLeft = 5;

    for(int i = 0; i<10; ++i){
        for(int j=0; j<10; ++j){
            p->playerGrid[i][j].s = NULL;
            p->playerGrid[i][j].gridChar = '-';
            p->enemyGrid[i][j] = '-';
        }
    }
}

struct player* createPlayer(void){
    struct player* p = malloc(sizeof *p);

    for(int i = 0; i<5; ++i){
        p->ships[i] = malloc(sizeof * p->ships[i]);
        int length;
        switch(i){
            case 0:
                length = 5;
                p->ships[i]->length = p->ships[i]->hp = length;
                strcpy(p->ships[i]->name, "CARRIER");
                p->ships[i]->shipAlpha = 'C';
                break;
            case 1:
                length = 4;
                p->ships[i]->length = p->ships[i]->hp = length;
                strcpy(p->ships[i]->name, "BATTLESHIP");
                p->ships[i]->shipAlpha = 'B';
                break;
            case 2:
                length = 3;
                p->ships[i]->length = p->ships[i]->hp = length;
                strcpy(p->ships[i]->name, "DESTROYER");
                p->ships[i]->shipAlpha = 'D';
                break;
            case 3:
                length = 3;
                p->ships[i]->length = p->ships[i]->hp = length;
                strcpy(p->ships[i]->name, "SUBMARINE");
                p->ships[i]->shipAlpha = 'S';
                break;
            default:
                length = 2;
                p->ships[i]->length = p->ships[i]->hp = length;
                strcpy(p->ships[i]->name, "PATROL BOAT");
                p->ships[i]->shipAlpha = 'P';
                break;
        }
    }
    resetPlayer(p);

    return p;
}

void freePlayer(struct player *p){
    for(int i = 0; i<5; ++i){
        free(p->ships[i]);
    }
    free(p);
}


/**
*** Below are the struct and functions for the enemy AI
*/

struct enemy{
    struct player *enemyPlayer;
    bool lastShotHit;
    uint8_t lastRowHit, lastColHit, firstRowHit, firstColHit;
    HitDirection lastSuccessfulHitDirection;
    bool rightSideAlreadyHit, leftSideAlreadyHit, upSideAlreadyHit, downSideAlreadyHit;
};

void resetEnemyBools(struct enemy *e){
    e->lastShotHit = false;
    e->downSideAlreadyHit = false;
    e->upSideAlreadyHit = false;
    e->leftSideAlreadyHit = false;
    e->rightSideAlreadyHit = false;
    e->lastSuccessfulHitDirection = None;
}

void resetEnemy(struct enemy *e){
    resetPlayer(e->enemyPlayer);
    resetEnemyBools(e);
}

struct enemy* createEnemy(void){
    struct enemy* e= malloc(sizeof *e);
    e->enemyPlayer = createPlayer();
    e->lastShotHit = false;
    e->downSideAlreadyHit = false;
    e->upSideAlreadyHit = false;
    e->leftSideAlreadyHit = false;
    e->rightSideAlreadyHit = false;
    return e;
}

void freeEnemy(struct enemy* e){
    freePlayer(e->enemyPlayer);
    free(e);
}

/**
*** The below functions are to print the grids out
*/
void printPlayerGrids(struct player *p){
    printf("     A  B  C  D  E  F  G  H  I  J\n\n");
    for(int row = 0; row <10; ++row){
        ///print the row number first
        if(row +1 <10) printf("  %d  ", row +1);
        else printf(" %d  ", row+1);

        for(int col = 0; col <10; ++col){
            ///print the row of chars
            printf("%c  ", p->playerGrid[row][col].gridChar);
        }
        printf("\n\n");
    }
}

void printGrids(struct player *p){
    printf("===================The Enemy Grid=====================\n\n");
    printf("     A  B  C  D  E  F  G  H  I  J\n\n");
    for(int row = 0; row <10; ++row){
        ///print the row number first
        if(row +1 <10) printf("  %d  ", row +1);
        else printf(" %d  ", row+1);

        for(int col = 0; col <10; ++col){
            ///print the row of chars
            printf("%c  ", p->enemyGrid[row][col]);
        }
        printf("\n\n");
    }
    printf("\n\n===================Your Grid=====================\n\n");
    printPlayerGrids(p);
}

/**
*** The two functions below are helpers for input
*/

int charToInt(char c){
    switch(toupper(c)){
        case 'A': return 0;
        case 'B': return 1;
        case 'C': return 2;
        case 'D': return 3;
        case 'E': return 4;
        case 'F': return 5;
        case 'G': return 6;
        case 'H': return 7;
        case 'I': return 8;
        case 'J': return 9;
        default: return -1;
    }
}


void getRowAndColInput(int *row, int *intCol){
    char col;
    while(1){
        int check = scanf(" %c%d", &col, row);
        if(check !=2 || (*intCol = charToInt(col)) == -1|| *row > 10 || *row <= 0){
            printf("\n\nInvalid input, please try again\n\n");
            while(getchar() != '\n');
            continue;
        }
        break;
    }
    *row-=1;
}


/**
*** below are the setup functions for the player and enemy AI
*/

void playerSetup(struct player* p){
    clearScreen();
    for(int i = 0; i<5; ++i){
        clearScreen();
        printPlayerGrids(p);
        ///for each ship, choose the ship's orientation (vertical or horizontal)
        ShipArrangement arrangement;
        while(1){
            printf("\nChoose the %s's orientation: [H]orizontal or [V]ertical\n\n\n", p->ships[i]->name);
            char option;
            scanf(" %c", &option);
            option = toupper(option);
            if(option != 'H' && option != 'V'){
                printf("Invalid input, please type \"H\" for horizontal and \"V\" for vertical (both upper and lowercase are fine)\n\n\n");
                continue;
            }
            else{
                arrangement = (option == 'H')? Horizontal:Vertical;
                break;
            }
        }

        ///after the arrangement has been chosen, have the user choose the location of the ship
       /// clearScreen();
       /// printPlayerGrids(p);
        printf("\n\n\n\nPlease input the location of %s's starting point(For example, J10)\n", p->ships[i]->name);
        printf("%s's length is %d\n", p->ships[i]->name, p->ships[i]->length);
        printf("Ships that are vertical will have their length downwards from their starting point\n");
        printf("Ships that are horizontal will have their length rightwards from their starting point\n\n\n\n");


        while(1){ ///while loop for input checking
            ///bounds checking
            int intCol, row;
            getRowAndColInput(&row, &intCol);
            if(arrangement == Vertical){
                ///check row
                if(row + p->ships[i]->length > 10){
                    printf("The ship is too long to start at this location! Please adjust the ship's location!\n\n");
                    while(getchar() != '\n');
                    continue;
                }
                else{
                    bool collide = false;
                    for(int index = 0; index < p->ships[i]->length; ++index ){
                        if(p->playerGrid[row+index][intCol].gridChar != '-'){
                            collide = true;
                            break;
                        }
                    }
                    if(collide){
                        printf("This ship's position will cause it to collide with another ship! Readjust your position!\n\n");
                        while(getchar() != '\n');
                        continue;
                    }
                    for(int index =0; index < p->ships[i]->length; ++index){ ///todo: check for ship collisions
                        p->playerGrid[row+index][intCol].gridChar = p->ships[i]->shipAlpha;
                        p->playerGrid[row+index][intCol].s = p->ships[i];
                    }
                }
            }
            else{///the arrangement is horizontal
                if(intCol + p->ships[i]->length> 10){
                    printf("The ship is too long to start at this location! Please adjust the ship's location!\n\n");
                    while(getchar() != '\n');
                    continue;
                }
                else{
                    bool collide = false;
                    for(int index =0; index<p->ships[i]->length; ++index){
                        if(p->playerGrid[row][intCol + index].gridChar != '-'){
                            collide = true;
                            break;
                        }
                    }
                    if(collide){
                        printf("This ship's position will cause it to collide with another ship! Readjust your position!\n\n");
                        while(getchar() != '\n');
                        continue;
                    }
                    for(int index =0; index<p->ships[i]->length; ++index){
                        p->playerGrid[row][intCol + index].gridChar = p->ships[i]->shipAlpha;
                        p->playerGrid[row][intCol+index].s = p->ships[i];
                    }
                }
            }
            break;
        }
    }
}


void enemySetup(struct enemy *e){
    for(int i = 0; i<5; ++i){
        int horiOrVert = rand()%2;
        ShipArrangement arrangement = (horiOrVert == 0)? Horizontal:Vertical;
        Ship s = e->enemyPlayer->ships[i];

        while(1){
            ///get random numbers for row and column
            int row = rand()%10;
            int col = rand()%10;

            ///check for length bounds
            if(arrangement == Vertical){
                if(row + s->length > 10) continue;
                bool collide = false;
                for(int index = 0; index < s->length; ++index){
                    if(e->enemyPlayer->playerGrid[row+index][col].gridChar != '-'){
                        collide = true;
                        break;
                    }
                }
                if(collide) continue;
                for(int index = 0; index < s->length; ++index){
                    e->enemyPlayer->playerGrid[row+index][col].gridChar = s->shipAlpha;
                    e->enemyPlayer->playerGrid[row+index][col].s = s;
                }
            }
            else{
                if(col + s->length > 10) continue;
                bool collide = false;
                for(int index = 0; index < s->length; ++index){
                    if(e->enemyPlayer->playerGrid[row][col+index].gridChar != '-'){
                        collide = true;
                        break;
                    }
                }
                if(collide) continue;
                for(int index = 0; index < s->length; ++index){
                    e->enemyPlayer->playerGrid[row][col+index].gridChar = s->shipAlpha;
                    e->enemyPlayer->playerGrid[row][col+index].s = s;
                }
            }
            break;
        }

    }
}

/**
*** The below functions are for the player's and enemy's turn
*/

void playerTurn(struct player *p, struct enemy *e){
    clearScreen();
    printGrids(p);

    ///have the player input location to attack
    printf("\n\nInput your attack location\n\n");
    int row, col;

    ///check if the player has already shot at this location
    while(1){
        getRowAndColInput(&row, &col);
        char hitOrMiss = p->enemyGrid[row][col];
        if(hitOrMiss == 'H' || hitOrMiss == 'M'){
            printf("You've already shot here! Please input a new location\n\n");
            continue;
        }
        break;
    }

    Ship s = e->enemyPlayer->playerGrid[row][col].s;

    if(s != NULL){ ///there is a ship at that location
        p->enemyGrid[row][col] = 'H';
        --s->hp;
        clearScreen();
        printGrids(p);

        if(s->hp == 0){
            printf("\n\nYOU'VE SUNK THE ENEMY'S %s!\n\n", s->name);
            --e->enemyPlayer->shipsLeft;
        }
        else{
            printf("\n\nYou've hit an enemy ship!\n\n");
        }
    }
    else{ ///There is no ship at that location
        p->enemyGrid[row][col] = 'M';
        clearScreen();
        printGrids(p);
        printf("\n\nYou've missed your shot!\n\n");
    }

    while(getchar()!='\n');
    printf("Press ENTER to YOUR turn\n\n\n"); ///allow the player the ability to see the text before it disappears
    while(getchar()!='\n');
}


char intToChar(int i){
    switch(i){
        case 0: return 'A';
        case 1: return 'B';
        case 2: return 'C';
        case 3: return 'D';
        case 4: return 'E';
        case 5: return 'F';
        case 6: return 'G';
        case 7: return 'H';
        case 8: return 'I';
        default: return 'J';
    }
}


void nextRowAndCol(int lastRow, int lastCol, HitDirection dir, int *nextRow, int *nextCol){
    switch(dir){
        case Up:
            *nextRow = lastRow - 1;
            *nextCol = lastCol;
            break;
        case Down:
            *nextRow = lastRow + 1;
            *nextCol = lastCol;
            break;
        case Left:
            *nextRow = lastRow;
            *nextCol = lastCol - 1;
            break;
        case Right:
            *nextRow = lastRow;
            *nextCol = lastCol + 1;
            break;
        default: break;
    }
}

bool hasBeenHitByEnemy(struct player *p, int row, int col){
    if(p->playerGrid[row][col].gridChar == 'H' || p->playerGrid[row][col].gridChar == 'M'){
        return true;
    }
    else return false;
}

void enemyTurn(struct enemy *e, struct player *p){
    int row = 0,col = 0;

    ///if the last shot did not hit, randomly choose a location to target (that has not been used yet)
    if(!e->lastShotHit){
        while(1){
            row = rand() % 10;
            col = rand() % 10;

            ///if the shot has been taken already, choose another random location
            if(hasBeenHitByEnemy(p, row, col)) continue;
            break;
        }
        ///determine if the shot has missed or hit
        Ship s = p->playerGrid[row][col].s;

        if(s != NULL){ ///shot has hit something
            --s->hp;
            p->playerGrid[row][col].gridChar = 'H';
            e->lastShotHit = true;
            e->firstRowHit = row;
            e->firstColHit = col;
            e->lastRowHit = row;
            e->lastColHit = col;
            clearScreen();
            printGrids(p);
            if(s->hp == 0){
                printf("\n\nThe enemy has shot at %c%d and has sunk your %s!\n\n", intToChar(col), row+1, s->name);
                --p->shipsLeft;
                resetEnemyBools(e);
            }
            else printf("\n\nThe enemy has shot at %c%d and hit your %s!\n\n", intToChar(col), row+1, s->name);
        }
        else{ ///the shot missed
            p->playerGrid[row][col].gridChar = 'M';
            clearScreen();
            printGrids(p);
            printf("\n\nThe enemy has shot at %c%d and missed!\n\n", intToChar(col), row+1);
        }
    }

    else{ ///have the AI try to guess where the rest of your ship is
        if(e->lastSuccessfulHitDirection == None){ ///we aint go a clue where to shoot next, boss
            ///first, create an array of options
            int optionsSize = 0;

            ///we have to determine the array size by doing annoying counting
            if(!e->downSideAlreadyHit && e->lastRowHit != 9 && !hasBeenHitByEnemy(p, e->lastRowHit+1, e->lastColHit)) ++optionsSize;
            if(!e->leftSideAlreadyHit && e->lastColHit != 0 && !hasBeenHitByEnemy(p, e->lastRowHit, e->lastColHit-1)) ++optionsSize;
            if(!e->rightSideAlreadyHit && e->lastColHit != 9 && !hasBeenHitByEnemy(p, e->lastRowHit, e->lastColHit+1)) ++optionsSize;
            if(!e->upSideAlreadyHit && e->firstRowHit != 0 && !hasBeenHitByEnemy(p, e->lastRowHit-1, e->lastColHit)) ++optionsSize;

            if(optionsSize == 0){ ///if for some reason there are no options to shoot, then go back to randomly shooting
                resetEnemyBools(e);
                enemyTurn(e, p);
                return;
            }

            HitDirection *options = malloc(optionsSize * sizeof *options);
            int optionsIndex = 0;

            ///then, actually put the options into the array
            if(!e->downSideAlreadyHit && e->lastRowHit != 9 && !hasBeenHitByEnemy(p, e->lastRowHit+1, e->lastColHit)) options[optionsIndex++] = Down;
            if(!e->leftSideAlreadyHit && e->lastColHit != 0 && !hasBeenHitByEnemy(p, e->lastRowHit, e->lastColHit-1)) options[optionsIndex++] = Left;
            if(!e->rightSideAlreadyHit && e->lastColHit != 9 && !hasBeenHitByEnemy(p, e->lastRowHit, e->lastColHit+1)) options[optionsIndex++] = Right;
            if(!e->upSideAlreadyHit && e->firstRowHit != 0 && !hasBeenHitByEnemy(p, e->lastRowHit-1, e->lastColHit)) options[optionsIndex++] = Up;


            ///Choose the direction to attack from the available options
            HitDirection decision = options[rand() % optionsSize];

            nextRowAndCol(e->lastRowHit, e->lastColHit, decision, &row, &col);

            ///the attack options have already been out-of-bounds checked, so we don't need to do that

            Ship s = p->playerGrid[row][col].s;

            if(s != NULL){ ///hit
                p->playerGrid[row][col].gridChar = 'H';
                e->lastRowHit = row;
                e->lastColHit = col;
                e->lastSuccessfulHitDirection = decision;
                --s->hp;
                clearScreen();
                printGrids(p);
                if(s->hp == 0){
                    printf("\n\nThe enemy has shot at %c%d and has sunk your %s!\n\n", intToChar(col), row+1, s->name);
                    --p->shipsLeft;
                    resetEnemyBools(e);
                }
                else printf("\n\nThe enemy has shot at %c%d and hit your %s!\n\n", intToChar(col), row+1, s->name);
            }
            else{///miss
                p->playerGrid[row][col].gridChar = 'M';
                clearScreen();
                printGrids(p);
                printf("\n\nThe enemy has shot at %c%d and missed!\n\n", intToChar(col), row+1);
            }

            free(options);

            switch(decision){
                case Up:
                    e->upSideAlreadyHit = true;
                    break;
                case Down:
                    e->downSideAlreadyHit = true;
                    break;
                case Left:
                    e->leftSideAlreadyHit = true;
                    break;
                case Right:
                    e->rightSideAlreadyHit = true;
                    break;
                default: break;
            }
        }

        else{ ///the AI knows where the rest of your ship is, or at least where it SHOULD be

            ///first of all, get the next shot based on previous success
            nextRowAndCol(e->lastRowHit, e->lastColHit, e->lastSuccessfulHitDirection, &row, &col);

            ///bounds checking
            if(row >= 10 || row < 0 || col >= 10 || col < 0){
                e->lastRowHit = e->firstRowHit;
                e->lastColHit = e->firstColHit;
                e->lastSuccessfulHitDirection = None;
                enemyTurn(e,p); ///take the shot again
                return;
            }
            else{
                ///if not out of bounds, then check for hit or miss
                Ship s= p->playerGrid[row][col].s;

                if(s!=NULL){///hit
                    p->playerGrid[row][col].gridChar = 'H';
                    e->lastRowHit = row;
                    e->lastColHit = col;
                    --s->hp;
                    clearScreen();
                    printGrids(p);
                    if(s->hp == 0){
                        printf("\n\nThe enemy has shot at %c%d and has sunk your %s!\n\n", intToChar(col), row+1, s->name);
                        --p->shipsLeft;
                        resetEnemyBools(e);
                    }
                    else printf("\n\nThe enemy has shot at %c%d and hit your %s!\n\n", intToChar(col), row+1, s->name);
                }
                else{ ///miss
                    p->playerGrid[row][col].gridChar = 'M';
                    clearScreen();
                    printGrids(p);
                    printf("\n\nThe enemy has shot at %c%d and missed!\n\n", intToChar(col), row+1);
                    e->lastRowHit = e->firstRowHit;
                    e->lastColHit = e->firstColHit;
                    e->lastSuccessfulHitDirection = None;
                }
            }
        }
    }

    ///while(getchar()!='\n');
    printf("Press ENTER to end ENEMY'S turn\n\n"); ///allow the player the ability to see the text before it disappears
    while(getchar()!='\n');
}

bool gameOver(struct player *p, struct enemy *e){
    if(p->shipsLeft == 0){
        clearScreen();
        printGrids(p);
        printf("\n\nALL OF YOUR SHIPS HAVE BEEN SUNK!!! THE ENEMY IS VICTORIOUS!!!\n\n");
        return true;
    }
    else if(e->enemyPlayer->shipsLeft == 0){
        clearScreen();
        printGrids(p);
        printf("\n\nALL THE ENEMY'S SHIPS HAVE BEEN SUNK!!! YOU ARE VICTORIOUS!!!\n\n");
        return true;
    }
    else{
        return false;
    }
}
