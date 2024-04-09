#include <iostream>
#include <vector>
#include <fstream>
#include <string>
using namespace std;

class Pacman {
    public:
    int x;
    int y;
};

class Map
{
public:
    vector<vector<char>> a;

    void displayMap(string filename)
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
        for (vector<char> templine : a)
        {
            for (char c : templine)
            {
                cout << c;
            }
            cout << "\n";
        }
    }
};


int main()
{
    Map m;
    // string line;
    // ifstream map("map.txt"); // ifstream is a predefined class and we create an object oftype ifstream
    // while (getline(map, line))
    // {
    //     // so currently  my line has one by one my rows
    //     vector<char> templine;
    //     for (char c : line) // like foreach
    //     {
    //         templine.push_back(c);
    //     }
    //     a.push_back(templine);
    // }
    // for (vector<char> templine : a)
    // {
    //     for (char c : templine)
    //     {
    //         cout << c;
    //     }
    //     cout << "\n";
    // }
    m.displayMap("map.txt");
    return 0;
}