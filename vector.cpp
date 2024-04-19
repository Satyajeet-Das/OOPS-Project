#include <iostream>
#include <vector>
#include <fstream>
#include <string>
#include <stdlib.h>
#include <conio.h>
#include <windows.h>
#include <ctime>
using namespace std;

// ANSI escape codes for text colors
#define ANSI_COLOR_RED     "\x1b[31m"
#define ANSI_COLOR_YELLOW  "\x1b[33m"
#define ANSI_COLOR_RESET   "\x1b[0m"

enum Direction
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
    char pac = 'O';

    int x;
    int y;
    Direction dirPacman;

    void PacInput()
    {
        if (_kbhit())
        {
            switch (_getch())
            {
            case 'w':
                dirPacman = UP;
                break;
            case 'a':
                dirPacman = LEFT;
                break;
            case 's':
                dirPacman = DOWN;
                break;
            case 'd':
                dirPacman = RIGHT;
                break;
            }
        }
    }
    void Pacmove()
    {
        switch (dirPacman)
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
class Ghost
{
public:
    char ghost = '@';
    int x, y;
    Direction dirGhost;

    void setGhostDirectionscatter()
    {
        srand(time(0));

        int val = rand() % 4 + 1;
        switch (val)
        {
        case 1:
            dirGhost = LEFT;
            break;

        case 2:
            dirGhost = RIGHT;
            break;

        case 3:
            dirGhost = UP;
            break;

        case 4:
            dirGhost = DOWN;
            break;
        }
    }
    void Ghostmove()
    {

        switch (dirGhost)
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
    inline int distance(Pacman &P, int x2, int y2)
    {
        return (P.x - x2) * (P.x - x2) + (P.y - y2) * (P.y - y2);
    }
    void chase(Pacman &P, vector<Direction> &dir)
    {
        int minDistance = INT_MAX;

        Direction finalDir;

        for (int i = 0; i < dir.size(); i++)
        {
            // cout <<distance(P,x,y)<<"\n";
            if (dir[i] == UP)
            {
                if (minDistance > distance(P, x, y - 1))
                {
                    minDistance = distance(P, x, y - 1);
                    finalDir = UP;
                }
            }
            else if (dir[i] == DOWN)
            {
                if (minDistance > distance(P, x, y + 1))
                {
                    minDistance = distance(P, x, y + 1);
                    finalDir = DOWN;
                }
            }
            else if (dir[i] == LEFT)
            {
                if (minDistance > distance(P, x - 1, y))
                {
                    minDistance = distance(P, x - 1, y);
                    finalDir = LEFT;
                }
            }
            else if (dir[i] == RIGHT)
            {
                if (minDistance > distance(P, x + 1, y))
                {
                    minDistance = distance(P, x + 1, y);
                    finalDir = RIGHT;
                }
            }
        }
        dirGhost = finalDir;
    }
};

class Map
{
public:
    vector<vector<char>> a; // this is 2-d vector for the map
    int height;
    int width;
    int noFruits=0;

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
                if (c == '.')
                {
                    noFruits++;
                }
            }
            a.push_back(templine);
        }
        height = a.size();
        width = a[0].size(); // initialize the heigth and width
    }

    void displayMap(Pacman P, Ghost gh)
    {
        for (int i = 0; i < a.size(); i++)
        {
            for (int j = 0; j < a[i].size(); j++) // till the size of the row
            {
                if (i == P.y && j == P.x) // i represents the y and j represents the x
                {
                    cout << P.pac;
                }
                else if (i == gh.y && j == gh.x)
                {
                    cout << gh.ghost;
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
    bool isWon;
    int score;
    Game()
    {
        gameOver = false;
        isWon = false;
        score = 0;
    }

    ~Game()
    {
        if (isWon == true)
        {
            cout << "You Won :-) :-) \n";
        }
        else
        {
            cout << "You Loose :-(  :-( \n";
        }
        cout << "Game Over\n";
    }

    void hideCursor() // to prevent the cursor from glitching
    {
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

    void wallCollision(Map &m, Pacman &p, Ghost &gh)
    {
        // Pacman
        if (m.a[(p.y)][(p.x + 1)] == '#' && p.dirPacman == RIGHT)
        {
            p.dirPacman = STOP;
            // p.x--;
        }
        else if (m.a[(p.y)][(p.x - 1)] == '#' && p.dirPacman == LEFT)
        {
            p.dirPacman = STOP;
            // p.x++;
        }
        else if (m.a[(p.y - 1)][(p.x)] == '#' && p.dirPacman == UP)
        {
            p.dirPacman = STOP;
            // p.y++;
        }
        else if (m.a[(p.y + 1)][(p.x)] == '#' && p.dirPacman == DOWN)
        {
            p.dirPacman = STOP;
            // p.y--;
        }

        // Ghost
        if (m.a[(gh.y)][(gh.x + 1)] == '#' && gh.dirGhost == RIGHT)
        {
            gh.dirGhost = STOP;
            // gh.x--;
        }
        else if (m.a[(gh.y)][(gh.x - 1)] == '#' && gh.dirGhost == LEFT)
        {
            gh.dirGhost = STOP;
            // gh.x++;
        }
        else if (m.a[(gh.y - 1)][(gh.x)] == '#' && gh.dirGhost == UP)
        {
            gh.dirGhost = STOP;
            // gh.y++;
        }
        else if (m.a[(gh.y + 1)][(gh.x)] == '#' && gh.dirGhost == DOWN)
        {
            gh.dirGhost = STOP;
            // p.y--;
        }
    }
    void fruitCollision(Map &m, Pacman &p)
    {

        if (m.a[(p.y)][(p.x + 1)] == '.' && p.dirPacman == RIGHT)
        {
            m.a[(p.y)][(p.x + 1)] = ' ';
            score += 10; // this is the property of game so it remembers the updated value
            // p.x--;
            m.noFruits--;
        }
        else if (m.a[(p.y)][(p.x - 1)] == '.' && p.dirPacman == LEFT)
        {
            m.a[(p.y)][(p.x - 1)] = ' ';
            score += 10;
            m.noFruits--;
            // p.x++;
        }
        else if (m.a[(p.y - 1)][(p.x)] == '.' && p.dirPacman == UP)
        {
            m.a[(p.y - 1)][(p.x)] = ' ';
            score += 10;
            m.noFruits--;
            // p.y++;
        }
        else if (m.a[(p.y + 1)][(p.x)] == '.' && p.dirPacman == DOWN)
        {
            m.a[(p.y + 1)][(p.x)] = ' ';
            score += 10;
            m.noFruits--;
            // p.y--;
        }
    }

    void Ghostcollision(Pacman &P, Ghost &gh)
    {
        if (P.x == gh.x && P.y == gh.y)
        {

            gameOver = true;
            P.pac = ' ';
        }
    }
    void fruitfinish(Map &m)
    {
        if (m.noFruits == 0)
        {
            gameOver = true;
            isWon = true;
        }
    }
    vector<Direction> possibleDirection(Map m, Ghost gh)
    {
        vector<Direction> temp; // vector of enum datatype

        if (m.a[gh.y][gh.x + 1] != '#') // right case
        {
            temp.push_back(RIGHT);
        }
        if (m.a[gh.y][gh.x - 1] != '#') // LEFT case
        {
            temp.push_back(LEFT);
        }
        if (m.a[gh.y - 1][gh.x] != '#') // UP case
        {
            temp.push_back(UP);
        }
        if (m.a[gh.y + 1][gh.x] != '#') // DOWN case
        {
            temp.push_back(DOWN);
        }

        return temp;
    }

    void logic(Map &m, Pacman &p, Ghost &gh)
    { // pass by reference as it's not the property of the object as without reference it sends a copy of the object and doesnt remember
        wallCollision(m, p, gh);
        fruitCollision(m, p);
    }
};

class Stats
{
public:
    void showStats(Game &g) // if we don't put reference then a copy is created and hence destructor will be called twice
    {
        cout << "Score: " << g.score << endl;
    }
};

int main()
{
    Game g;
    Map m;
    Pacman P;
    Ghost gh;
    Stats s;
    // char ghost[3]={

    // };
    
    int countTime = 0;
    P.x = 26;
    P.y = 5;

    gh.x = 5;
    gh.y = 5;

    g.hideCursor();
    m.inialiseMap("map.txt"); // fetchs it
    while (!g.gameOver)
    {
        countTime++;
        g.clearScreen();

        if ((P.dirPacman == UP && gh.dirGhost == UP) || (P.dirPacman == DOWN && gh.dirGhost == UP))
        {
            Sleep(150); // increasing the delay (inbuilt from library)
        }
        else
        {
            Sleep(60); // delay for 40ms  (inbuilt from library)
        }
        P.PacInput();
        if (countTime >= 56)
        {
            vector<Direction> dir = g.possibleDirection(m, gh);
            gh.chase(P, dir);
        }
        else
        {
            gh.setGhostDirectionscatter();
        }
        g.logic(m, P, gh);
        P.Pacmove();
        g.Ghostcollision(P, gh);
        g.fruitfinish(m);
        gh.Ghostmove();
        m.displayMap(P, gh);
        s.showStats(g);
        // for (int i = 0; i < dir.size(); i++)
        // {
        //     cout << dir.size() << " ";

        //     switch (dir[i])
        //     {
        //     case UP:
        //         cout << "UP ";
        //         break;
        //     case DOWN:
        //         cout << "DOWN ";
        //         break;
        //     case RIGHT:
        //         cout << "RIGHT ";
        //         break;
        //     case LEFT:
        //         cout << "LEFT ";
        //         break;
        //     }
        // }
        // dir.clear();
        // cout<<m.noFruits<<"\n";
    }
    return 0;
}
