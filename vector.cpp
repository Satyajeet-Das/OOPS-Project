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
                    cout << "O";
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

    void hideCursor() {
        HANDLE consoleHandle = GetStdHandle(STD_OUTPUT_HANDLE);
        CONSOLE_CURSOR_INFO cursorInfo;
        GetConsoleCursorInfo(consoleHandle, &cursorInfo);
        cursorInfo.bVisible = FALSE; // Set cursor visibility to false
        SetConsoleCursorInfo(consoleHandle, &cursorInfo);
    }

    void clearScreen()
    {
        COORD cursorPosition;
        cursorPosition.X = 0;
        cursorPosition.Y = 0;
        SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), cursorPosition);
    }

    void wallCollision(Map &m, Pacman &p)
    {

        if (m.a[(p.y)][(p.x + 1)] == '#' && p.dir == RIGHT)
        {
            p.dir = STOP;
            // p.x--;
        }
        else if (m.a[(p.y)][(p.x - 1)] == '#' && p.dir == LEFT)
        {
            p.dir = STOP;
            // p.x++;
        }
        else if (m.a[(p.y - 1)][(p.x)] == '#' && p.dir == UP)
        {
            p.dir = STOP;
            // p.y++;
        }
        else if (m.a[(p.y + 1)][(p.x)] == '#' && p.dir == DOWN)
        {
            p.dir = STOP;
            // p.y--;
        }
    }
    void fruitCollision(Map &m, Pacman &p)
    {

        if (m.a[(p.y)][(p.x + 1)] == '.' && p.dir == RIGHT)
        {
            m.a[(p.y)][(p.x + 1)] = ' ';
            score += 10; //this is the property of game so it remembers the updated value
            // p.x--;
        }
        else if (m.a[(p.y)][(p.x - 1)] == '.' && p.dir == LEFT)
        {
            m.a[(p.y)][(p.x - 1)] = ' ';
            score += 10;

            // p.x++;
        }
        else if (m.a[(p.y - 1)][(p.x)] == '.' && p.dir == UP)
        {
            m.a[(p.y - 1)][(p.x)] = ' ';
            score += 10;
            // p.y++;
        }
        else if (m.a[(p.y + 1)][(p.x)] == '.' && p.dir == DOWN)
        {
            m.a[(p.y + 1)][(p.x)] = ' ';
            score += 10;
            // p.y--;
        }
    }

    void logic(Map &m, Pacman &p)
    { // pass by reference as it's not the property of the object as without reference it sends a copy of the object and doesnt remember
        wallCollision(m, p);
        fruitCollision(m, p);
    }
};

class Stats {
    public:

    void showStats(Game g){
        cout << "Score: " << g.score << endl;
    }
};

int main()
{
    Game g;
    Map m;
    Pacman P;
    Stats s;
    P.x = 10;
    P.y = 10;

    g.hideCursor();
    m.inialiseMap("map1.txt");
    while (!g.gameOver)
    { 
        g.clearScreen();
        m.displayMap(P);
        s.showStats(g);
        
        if (P.dir == UP || P.dir == DOWN)
        {
            Sleep(150); // increasing the delay (inbuilt from library)
        }
        else
        {
            Sleep(60); // delay for 40ms  (inbuilt from library)
        }
        P.PacInput();
        g.logic(m, P);
        P.Pacmove();
    }
    return 0;
}