#include <iostream>
#include <vector>
#include <fstream>
#include <string>
#include <stdlib.h>
#include <conio.h>
#include <windows.h>
#include <ctime>
using namespace std;

enum Directionpacman
{
    STOP = 0,
    LEFT,
    RIGHT,
    UP,
    DOWN
};

class Pacman
{
public:
    int x;
    int y;

    Directionpacman dir;
    void PacInput()
    {
        if (_kbhit())
        {
            switch (_getch())
            {
            case 'w':
                dir = UP;
                break;
            case 'a':
                dir = LEFT;
                break;
            case 's':
                dir = DOWN;
                break;
            case 'd':
                dir = RIGHT;
                break;
            }
        }
    }
    void Pacmove()
    {
        switch (dir)
        {
        case UP:
            y--;
            cout << "the ycord" << y << "\n";
            break;
        case DOWN:
            y++;
            break;
        case RIGHT:
            x++;
            break;
        case LEFT:
            x--;
            break;
        }
    }
};

class Map
{
public:
    vector<vector<char>> a;
    int height;
    int width;

    void inialiseMap(string filename)
    {
        string line;
        ifstream map(filename); // ifstream is a predefined class and we create an object oftype ifstream
        while (getline(map, line))
        {
            // so currently  my line has one by one my rows
            vector<char> templine;
            for (char c : line) // like foreach
            {
                templine.push_back(c);
            }
            a.push_back(templine);
        }
        height = a.size();
        width = a[0].size(); // initialize the heigth and width
    }

    void displayMap(Pacman P)
    {
        for (int i = 0; i < a.size(); i++)
        {
            for (int j = 0; j < a[i].size(); j++) // till the size of the row
            {
                if (i == P.y && j == P.x) // i represents the y and j represents the x
                {
                    cout << "P";
                }
                else
                {
                    cout << a[i][j];
                }
            }
            cout << "\n";
        }
    }
};
class Game
{
public:
    bool gameOver;
    int score;
    Game()
    {
        gameOver = false;
        score = 0;
    }
    void clearScreen()
    {
        COORD cursorPosition;
        cursorPosition.X = 0;
        cursorPosition.Y = 0;
        SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), cursorPosition);
    }

    void wallCollision(Map m, Pacman p)
    {
        if(m.a[(p.x)+1][p.y]=='#' && p.dir==RIGHT)
        {
            p.dir=STOP;
        }
    }

    void logic(Map m, Pacman p){
        wallCollision(m, p);
    }
};
int main()
{
    Game g;
    Map m;
    Pacman P;
    P.x = 10;
    P.y = 10;

    m.inialiseMap("map.txt");
    while (!g.gameOver)
    {
        g.clearScreen();
        m.displayMap(P);
        if (P.dir == UP || DOWN)
        {
            Sleep(100); // increasing the delay (inbuilt from library)
        }else{
            Sleep(50); // delay for 40ms  (inbuilt from library)
        }
        P.PacInput();
        P.Pacmove();
        g.logic(m, P);
    }
    return 0;
}