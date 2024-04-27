#include <iostream>
#include <vector>
#include <fstream>
#include <string>
#include <stdlib.h>
#include <conio.h>
#include <windows.h>
#include <ctime>
using namespace std;

// ANSI(American National Standards Institute) escape codes for text colors
#define RED "\x1b[31m"
#define BRIGHT_RED "\x1b[91m"
#define YELLOW "\x1b[33m"
#define RESET "\x1b[0m"
#define BLUE "\x1b[34m"
#define MAGENTA "\x1b[35m"
#define CYAN "\x1b[36m"
#define GREEN "\x1b[32m"
#define ORANGE "\x1b[38;2;255;165;0m"

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
        // if (_kbhit())
        // {
        //     switch (_getch())
        //     {
        //     case 'w':
        //         dirPacman = UP;
        //         break;
        //     case 'a':
        //         dirPacman = LEFT;
        //         break;
        //     case 's':
        //         dirPacman = DOWN;
        //         break;
        //     case 'd':
        //         dirPacman = RIGHT;
        //         break;
        //     }
        // }

        if (GetAsyncKeyState(VK_UP) & 0x8000)
        {
            dirPacman = UP;
            // std::cout << "Up arrow pressed\n";
        }
        if (GetAsyncKeyState(VK_DOWN) & 0x8000)
        {
            dirPacman = DOWN;
            // std::cout << "Down arrow pressed\n";
        }
        if (GetAsyncKeyState(VK_LEFT) & 0x8000)
        {
            dirPacman = LEFT;
            // std::cout << "Left arrow pressed\n";
        }
        if (GetAsyncKeyState(VK_RIGHT) & 0x8000)
        {
            dirPacman = RIGHT;
            // std::cout << "Right arrow pressed\n";
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
    bool isKillable = false;
    bool isTrapped = false;
    Direction dirGhost;

    void setGhostDirectionFrightened()
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
    inline int distance(int x1, int y1, int x2, int y2)
    {
        return (x1 - x2) * (x1 - x2) + (y1 - y2) * (y1 - y2);
    }
    void chase(Pacman &P, vector<Direction> &dir)
    {
        int minDistance = INT_MAX;

        Direction finalDir;

        for (int i = 0; i < dir.size(); i++)
        {
            // cout <<distance(P,x,y)<<"\n";
            if (dir[i] == UP && dirGhost != DOWN) // because ghost cannot turn 180 degrees
            {
                if (minDistance > distance(P.x, P.y, x, y - 1))
                {
                    minDistance = distance(P.x, P.y, x, y - 1);
                    finalDir = UP;
                }
            }
            else if (dir[i] == DOWN && dirGhost != UP)
            {
                if (minDistance > distance(P.x, P.y, x, y + 1))
                {
                    minDistance = distance(P.x, P.y, x, y + 1);
                    finalDir = DOWN;
                }
            }
            else if (dir[i] == LEFT && dirGhost != RIGHT)
            {
                if (minDistance > distance(P.x, P.y, x - 1, y))
                {
                    minDistance = distance(P.x, P.y, x - 1, y);
                    finalDir = LEFT;
                }
            }
            else if (dir[i] == RIGHT && dirGhost != LEFT)
            {
                if (minDistance > distance(P.x, P.y, x + 1, y))
                {
                    minDistance = distance(P.x, P.y, x + 1, y);
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
    int noFruits = 0;

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

    void displayMap(Pacman P, Ghost ghy, Ghost ghr)
    {
        for (int i = 0; i < a.size(); i++)
        {
            for (int j = 0; j < a[i].size(); j++) // till the size of the row
            {
                if (i == P.y && j == P.x) // i represents the y and j represents the x
                {
                    cout << YELLOW << P.pac << RESET;
                }

                else if (i == ghy.y && j == ghy.x)
                {
                    if (ghy.isKillable)
                    {

                        cout << MAGENTA << ghy.ghost << RESET;
                    }
                    else
                    {
                        cout << GREEN << ghy.ghost << RESET;
                    }
                }
                else if (i == ghr.y && j == ghr.x)
                {
                    if (ghy.isKillable)
                    {

                        cout << MAGENTA << ghy.ghost << RESET;
                    }
                    else
                    {
                        cout << BRIGHT_RED << ghy.ghost << RESET;
                    }
                }

                else
                {
                    if (a[i][j] == '.')
                    {
                        cout << a[i][j];
                    }
                    else if (a[i][j] == '*')
                    {
                        cout << ORANGE << a[i][j] << RESET;
                    }
                    else
                    {
                        cout << BLUE << a[i][j] << RESET;
                    }
                }
            }
            cout << "\n";
        }
    }
};

class YellowGhost : public Ghost
{
public:
    YellowGhost(int a, int b)
    {
        x = a;
        y = b;
    }
    void chase(Pacman &P, vector<Direction> &dir)
    {
        int minDistance = INT_MAX;

        Direction finalDir;

        for (int i = 0; i < dir.size(); i++)
        {
            //  cout << P.x << " " << P.y << endl;
            // cout <<distance(P,x,y)<<"\n";
            if (dir[i] == UP && dirGhost != DOWN) // because ghost cannot turn 180 degrees
            {
                // cout << distance(P.x, P.y-4, x + 1, y)<< endl;
                if (minDistance > distance(P.x, P.y, x, y - 1))
                {
                    minDistance = distance(P.x, P.y, x, y - 1);
                    finalDir = UP;
                }
            }
            else if (dir[i] == DOWN && dirGhost != UP)
            {
                // cout << distance(P.x, (P.y)+4, x + 1, y)<< endl;
                if (minDistance > distance(P.x, (P.y), x, y + 1)) // check why +4 doesnot work
                {
                    minDistance = distance(P.x, (P.y), x, y + 1);
                    finalDir = DOWN;
                }
            }
            else if (dir[i] == LEFT && dirGhost != RIGHT)
            {
                // cout << distance(P.x - 4, P.y, x + 1, y)<< endl;
                if (minDistance > distance(P.x, P.y, x - 1, y))
                {
                    minDistance = distance(P.x, P.y, x - 1, y);
                    finalDir = LEFT;
                }
            }
            else if (dir[i] == RIGHT && dirGhost != LEFT)
            {
                // cout << distance(P.x + 4, P.y, x + 1, y)<< endl;
                if (minDistance > distance(P.x, P.y, x + 1, y))
                {
                    minDistance = distance(P.x, P.y, x + 1, y);
                    finalDir = RIGHT;
                }
            }
        }
        dirGhost = finalDir;
    }

    void setGhostDirectionscatter(Map m, vector<Direction> &dir)
    {
        int minDistance = INT_MAX;

        Direction finalDir;

        for (int i = 0; i < dir.size(); i++)
        {
            // cout <<distance(P,x,y)<<"\n";
            int cornerX = 1;
            int cornerY = m.height - 1;
            if (dir[i] == UP && dirGhost != DOWN) // because ghost cannot turn 180 degrees
            {
                if (minDistance > distance(cornerX, cornerY, x, y - 1))
                {
                    minDistance = distance(cornerX, cornerY, x, y - 1);
                    finalDir = UP;
                }
            }
            else if (dir[i] == DOWN && dirGhost != UP)
            {
                if (minDistance > distance(cornerX, cornerY, x, y + 1))
                {
                    minDistance = distance(cornerX, cornerY, x, y + 1);
                    finalDir = DOWN;
                }
            }
            else if (dir[i] == LEFT && dirGhost != RIGHT)
            {
                if (minDistance > distance(cornerX, cornerY, x - 1, y))
                {
                    minDistance = distance(cornerX, cornerY, x - 1, y);
                    finalDir = LEFT;
                }
            }
            else if (dir[i] == RIGHT && dirGhost != LEFT)
            {
                if (minDistance > distance(cornerX, cornerY, x + 1, y))
                {
                    minDistance = distance(cornerX, cornerY, x + 1, y);
                    finalDir = RIGHT;
                }
            }
        }
        dirGhost = finalDir;
    }
};

class RedGhost : public Ghost
{
public:
    RedGhost(int a, int b)
    {
        x = a;
        y = b;
    }
    void chase(Pacman &P, vector<Direction> &dir)
    {
        int minDistance = INT_MAX;

        Direction finalDir;

        for (int i = 0; i < dir.size(); i++)
        {
            // {    cout << P.x << " " << P.y << endl;
            // cout <<distance(P,x,y)<<"\n";
            if (dir[i] == UP && dirGhost != DOWN) // because ghost cannot turn 180 degrees
            {
                // cout << distance(P.x, P.y-4, x + 1, y)<< endl;
                if (minDistance > distance(P.x, P.y, x, y - 1))
                {
                    minDistance = distance(P.x, P.y, x, y - 1);
                    finalDir = UP;
                }
            }
            else if (dir[i] == DOWN && dirGhost != UP)
            {
                // cout << distance(P.x, (P.y)+4, x + 1, y)<< endl;
                if (minDistance > distance(P.x, (P.y), x, y + 1)) // check why +4 doesnot work
                {
                    minDistance = distance(P.x, (P.y), x, y + 1);
                    finalDir = DOWN;
                }
            }
            else if (dir[i] == LEFT && dirGhost != RIGHT)
            {
                // cout << distance(P.x - 4, P.y, x + 1, y)<< endl;
                if (minDistance > distance(P.x, P.y, x - 1, y))
                {
                    minDistance = distance(P.x, P.y, x - 1, y);
                    finalDir = LEFT;
                }
            }
            else if (dir[i] == RIGHT && dirGhost != LEFT)
            {
                // cout << distance(P.x + 4, P.y, x + 1, y)<< endl;
                if (minDistance > distance(P.x, P.y, x + 1, y))
                {
                    minDistance = distance(P.x, P.y, x + 1, y);
                    finalDir = RIGHT;
                }
            }
        }
        dirGhost = finalDir;
    }

    void setGhostDirectionscatter(Map m, vector<Direction> &dir)
    {
        int minDistance = INT_MAX;

        Direction finalDir;

        for (int i = 0; i < dir.size(); i++)
        {
            // cout <<distance(P,x,y)<<"\n";
            int cornerX = m.width - 2;
            int cornerY = m.height - 1;
            if (dir[i] == UP && dirGhost != DOWN) // because ghost cannot turn 180 degrees
            {
                if (minDistance > distance(cornerX, cornerY, x, y - 1))
                {
                    minDistance = distance(cornerX, cornerY, x, y - 1);
                    finalDir = UP;
                }
            }
            else if (dir[i] == DOWN && dirGhost != UP)
            {
                if (minDistance > distance(cornerX, cornerY, x, y + 1))
                {
                    minDistance = distance(cornerX, cornerY, x, y + 1);
                    finalDir = DOWN;
                }
            }
            else if (dir[i] == LEFT && dirGhost != RIGHT)
            {
                if (minDistance > distance(cornerX, cornerY, x - 1, y))
                {
                    minDistance = distance(cornerX, cornerY, x - 1, y);
                    finalDir = LEFT;
                }
            }
            else if (dir[i] == RIGHT && dirGhost != LEFT)
            {
                if (minDistance > distance(cornerX, cornerY, x + 1, y))
                {
                    minDistance = distance(cornerX, cornerY, x + 1, y);
                    finalDir = RIGHT;
                }
            }
        }
        dirGhost = finalDir;
    }
};

class Game
{
public:
    bool gameOver;
    bool isWon;
    int score;
    bool firstFruitTime;
    bool firstTrappedTime;
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
            cout << GREEN << "You Won" << CYAN << " :-) :-) \n"
                 << RESET;
        }
        else
        {
            cout << RED << "You Loose" << CYAN << " :-( :-( \n"
                 << RESET;
        }
        cout << ORANGE << "Game Over\n"
             << RESET;
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
    void starFruitCollision(Map &m, Pacman &p, Ghost &ghy, Ghost &ghr)
    {

        if (m.a[(p.y)][(p.x + 1)] == '*' && p.dirPacman == RIGHT)
        {
            m.a[(p.y)][(p.x + 1)] = ' ';
            ghy.isKillable = true;
            ghr.isKillable = true;
            firstFruitTime = true;
        }
        else if (m.a[(p.y)][(p.x - 1)] == '*' && p.dirPacman == LEFT)
        {
            m.a[(p.y)][(p.x - 1)] = ' ';
            ghy.isKillable = true;
            ghr.isKillable = true; // score += 10;
            firstFruitTime = true;
            // m.noFruits--;
            // p.x++;
        }
        else if (m.a[(p.y - 1)][(p.x)] == '*' && p.dirPacman == UP)
        {
            m.a[(p.y - 1)][(p.x)] = ' ';
            ghy.isKillable = true;
            ghr.isKillable = true; // score += 10;
            firstFruitTime = true;
            // m.noFruits--;
            // p.y++;
        }
        else if (m.a[(p.y + 1)][(p.x)] == '*' && p.dirPacman == DOWN)
        {
            m.a[(p.y + 1)][(p.x)] = ' ';
            ghy.isKillable = true;
            ghr.isKillable = true; // score += 10;
            firstFruitTime = true;
            // m.noFruits--;
            // p.y--;
        }
    }

    void Ghostcollision(Pacman &P, Ghost &gh, Map &m)
    {
        if (P.x == gh.x && P.y == gh.y && gh.isKillable == false)
        {

            gameOver = true;
            P.pac = ' ';
        }
        else if (P.x == gh.x && P.y == gh.y && gh.isKillable == true)
        {
            gh.x = m.width / 2;
            gh.y = m.height / 2;
            gh.isTrapped = true;
            firstTrappedTime = true;
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
        cout << MAGENTA << "Score: " << CYAN << g.score << RESET << endl;
    }
};
class mainMenu
{
    string name;

public:
    void getNameUser()
    {
        cout << "Enter your name\n";
        cin >> name;
    }
    void homepg()
    {
        system("cls");
        cout << BRIGHT_RED << "\nHello " << YELLOW << name << RESET << RED << " welcome to our pacman game :-) !!!\n"
             << RESET;
        cout << "Enter y/n/h \n";
    }
    void highScoreAdding(int score)
    {

        ofstream highscores("highscore.txt", ios::app); // opens the file in the output in the append mode
        if (!highscores.is_open())
        {
            cerr << "Error opening file" << endl;
            exit(1);
        }
        highscores << name << " " << score << "\n"
                   << "\n";

        highscores.close();
    }
    void highscoreDisplay()
    {
        ifstream high("highscore.txt");
        string line;
        while (getline(high, line))
        {
            string tempname = "";
            string score = "";
            for (char c : line)
            {
                if (c != ' ')
                {
                    tempname += c;
                }
                else
                {
                    break;
                }
            }
            if (tempname == name)
            {
                cout << "Your scores are:\n";
                cout << line << "\n";
            }
        }
    }
};
int main()
{
    mainMenu main;
    char ch;
    main.getNameUser();

    main.homepg();

    while (1)
    {

        cin >> ch;
        if (ch == 'y')
        {
            Game g;

            system("cls");
            Map m;
            Pacman P;
            // YellowGhost ghy(5,19);
            // RedGhost ghr(14,10);

            YellowGhost ghy(1, 1);
            RedGhost ghr(1, 2);
            Stats s;

            int countTime = 0;
            P.x = 26;
            P.y = 5;

            g.hideCursor();
            m.inialiseMap("map4.txt"); // fetchs it
            int isStartkill = 0;
            int isStarTrapped = 0;
            while (!g.gameOver)
            {
                if ((ghy.isKillable == true || ghr.isKillable == true) && g.firstFruitTime == true)
                {
                    isStartkill = countTime;
                    g.firstFruitTime = false;
                }
                if ((ghy.isTrapped == true || ghr.isTrapped == true) && g.firstTrappedTime == true)
                {
                    isStarTrapped = countTime;
                    g.firstTrappedTime = false;
                }
                countTime++;
                g.clearScreen();

                if ((P.dirPacman == UP && ghy.dirGhost == UP && ghr.dirGhost == UP) || (P.dirPacman == DOWN && ghy.dirGhost == UP && ghr.dirGhost == UP))
                {
                    Sleep(150); // increasing the delay (inbuilt from library)
                }
                else
                {
                    Sleep(60); // delay for 40ms  (inbuilt from library)
                }
                P.PacInput();
                vector<Direction> diry = g.possibleDirection(m, ghy);
                vector<Direction> dirr = g.possibleDirection(m, ghr);

                int state = (countTime / 56) % 2;
                if (ghy.isKillable)
                {
                    ghy.setGhostDirectionFrightened();
                    ghr.setGhostDirectionFrightened();
                }
                else if (state)
                {
                    ghy.chase(P, diry);
                    ghr.chase(P, dirr);
                }
                else
                {
                    ghy.setGhostDirectionscatter(m, diry);
                    ghr.setGhostDirectionscatter(m, dirr);
                }

                g.logic(m, P, ghr);
                g.logic(m, P, ghy);

                P.Pacmove();

                g.Ghostcollision(P, ghr, m);
                g.Ghostcollision(P, ghy, m);

                g.fruitfinish(m);

                g.starFruitCollision(m, P, ghy, ghr);
                // Time period for which it can kill and after time ends, it resets
                if (countTime - isStartkill == 56 && (ghy.isKillable == true || ghr.isKillable == true))
                {
                    ghr.isKillable = false;
                    ghy.isKillable = false;
                }

                if (countTime - isStarTrapped == 26 && (ghy.isTrapped == true || ghr.isTrapped == true))
                {
                    if (ghy.isTrapped == true)
                    {
                        ghy.x = m.width / 2;
                        ghy.y = (m.height / 2) - 3; // goes up
                        ghy.isTrapped = false;
                    }

                    if (ghr.isTrapped == true)
                    {
                        ghr.x = m.width / 2;
                        ghr.y = (m.height / 2) + 4; // goes down
                        ghr.isTrapped = false;
                    }
                }

                ghr.Ghostmove();
                ghy.Ghostmove();

                m.displayMap(P, ghy, ghr);
                s.showStats(g);

                // cout << countTime << endl;
            }
            main.highScoreAdding(g.score);
        }
        else if (ch == 'n')
        {
            cout << ORANGE << "Visit Again! \n"
                 << RESET;
            exit(0);
        }
        else if (ch == 'h')
        {

            main.highscoreDisplay();
        }
        if (ch == 'y')
        {
            cout << "Do you want to continue playing? y/n/h \n";
        }
        if (ch == 'h')
        {
            cout << "Do you want to continue playing? y/n/h \n";
        }
    }
    return 0;
}
