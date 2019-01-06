#ifndef PLAYER_H_INCLUDED
#define PLAYER_H_INCLUDED

struct player;
typedef struct player *Player;

struct enemy;
typedef struct enemy *Enemy;

Player createPlayer(void);
void freePlayer(Player p);

Enemy createEnemy(void);
void freeEnemy(Enemy e);

///Below are TODO functions

void playerSetup(Player p);
void enemySetup(Enemy e);

void playerTurn(Player p, Enemy e);
void enemyTurn(Enemy e, Player p);

///already done
void clearScreen(void);
void printGrids(struct player *p);


#endif // PLAYER_H_INCLUDED
