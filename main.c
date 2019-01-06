#include <stdio.h>
#include <stdlib.h>
#include "player.h"
#include <time.h>
#include <windows.h>

void maximizeWindow(void){
    HWND hWnd;
    SetConsoleTitle("Battleship");
    hWnd = FindWindow(NULL, "Battleship");
    HANDLE hOut = GetStdHandle(STD_OUTPUT_HANDLE);
    COORD NewSBSize = GetLargestConsoleWindowSize(hOut);
    SMALL_RECT DisplayArea = {0, 0, 0, 0};

    SetConsoleScreenBufferSize(hOut, NewSBSize);

    DisplayArea.Right = NewSBSize.X - 1;
    DisplayArea.Bottom = NewSBSize.Y - 1;

    SetConsoleWindowInfo(hOut, TRUE, &DisplayArea);

    ShowWindow(hWnd, SW_MAXIMIZE);
}

int main()
{
    maximizeWindow();
    srand(time(NULL));
    clearScreen();
    Player p = createPlayer();
    printGrids(p);
    freePlayer(p);
    while(getchar()!= '\n');
    return 0;
}
