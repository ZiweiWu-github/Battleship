#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <string.h>

#define bool uint8_t
#define false 0
#define true 1

typedef struct ship{
    int length;
    int hp;
    char name[11];
} *Ship;

typedef struct grid{
    Ship s;
    char gridChar; ///at first, the char will be '-' for unknown but will change to 'H' or 'M' for hit or miss, respectively
} Grid;


void clearScreen(void){
    for(int i =0; i<300; ++i){
        printf("\n");
    }
}


typedef enum shipArrangement{
    Vertical, Horizontal
}ShipArrangement;

/**
*** Below are the struct and functions for the player
*/
struct player{
    Grid playerGrid[10][10];
    char enemyGrid[10][10]; ///used to keep track of hits or misses, should not actually contain the enemy's info
                            ///character arrangement same as the gridChar's
    Ship carrier, battleship, cruiser, submarine, destroyer;
    uint8_t shipsLeft;
};


struct player* createPlayer(void){
    struct player* p = malloc(sizeof *p);

    p->carrier = malloc(sizeof *p->carrier);
    p->carrier->length = p->carrier->hp = 5;
    strcpy(p->carrier->name, "Carrier");

    p->battleship = malloc(sizeof *p->battleship);
    p->battleship->length = p->battleship->hp = 4;
    strcpy(p->battleship->name, "Battleship");

    p->cruiser = malloc(sizeof *p->cruiser);
    p->cruiser->length = p->cruiser->hp = 3;
    strcpy(p->cruiser->name, "Cruiser");

    p->submarine = malloc(sizeof *p->submarine);
    p->submarine->length = p->submarine->hp = 3;
    strcpy(p->submarine->name, "Submarine");

    p->destroyer = malloc(sizeof *p->destroyer);
    p->destroyer->length = p->destroyer->hp = 2;
    strcpy(p->destroyer->name, "Destroyer");

    p->shipsLeft = 5;

    for(int i = 0; i<10; ++i){
        for(int j=0; j<10; ++j){
            p->playerGrid[i][j].s = NULL;
            p->playerGrid[i][j].gridChar = '-';
            p->enemyGrid[i][j] = '-';
        }
    }

    return p;
}

void freePlayer(struct player *p){
    free(p->carrier);
    free(p->battleship);
    free(p->cruiser);
    free(p->submarine);
    free(p->destroyer);
    free(p);
}


/**
*** Below are the struct and functions for the enemy AI
*/

struct enemy{
    struct player *enemyPlayer;
    bool lastShotHit;
    bool lastShotSank;
    uint8_t lastXHit, lastYHit;
};

struct enemy* createEnemy(void){
    struct enemy* e= malloc(sizeof *e);
    e->enemyPlayer = createPlayer();
    e->lastShotHit = false;
    e->lastShotSank = false;
    return e;
}

void freeEnemy(struct enemy* e){
    freePlayer(e->enemyPlayer);
    free(e);
}

char intToChar(int i){
    switch(i){
        case 1: return 'A';
        case 2: return 'B';
        case 3: return 'C';
        case 4: return 'D';
        case 5: return 'E';
        case 6: return 'F';
        case 7: return 'G';
        case 8: return 'H';
        case 9: return 'I';
        default: return 'J';
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

///setup and turn functions need to be below here because of the print function above
