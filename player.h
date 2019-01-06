#ifndef PLAYER_H_INCLUDED
#define PLAYER_H_INCLUDED

struct player;
typedef struct player *Player;

struct enemy;
typedef struct enemy *Enemy;

Player createPlayer(void);
void freePlayer(struct player* p);

Enemy createEnemy(void);
void freeEnemy(struct enemy* e);

void resetPlayer(struct player *p);
void resetEnemy(struct enemy *e);


void playerSetup(struct player* p);
void enemySetup(struct enemy *e);

void playerTurn(struct player* p , struct enemy *e);

///Below are TODO functions
void enemyTurn(struct enemy *e, struct player* p );

///already done
void clearScreen(void);
void printGrids(struct player *p);


#endif // PLAYER_H_INCLUDED
