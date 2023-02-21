// Klondike Solitaire in console
//  - Author: Tomash Mikulevich
//  - Created with: Visual Studio Code 1.55.0 | g++.exe (x86_64-posix-seh-rev0, Built by MinGW-W64 project) 8.1.0 | GNU gdb (GDB) 8.1
//  - Last edited: 08.04.2021

#include <iostream>
#include <iomanip>
#include <cstring>  
#include <ctime>  
#include <vector>  
#include <queue>  
#include <stack>  

using namespace std;


const int MAX_CARDS = 52;


struct Card
{
    int numValue, numSuit, numColor;
    string aboutCard;
    bool isHidden = true;

    Card() {}

    Card(int s, int v) {
        numSuit = s;
        numValue = v;

        switch (numValue)
        {
        case 1:
            aboutCard += "A";
            break;
        case 11:
            aboutCard += "J";
            break;
        case 12:
            aboutCard += "Q";
            break;
        case 13:
            aboutCard += "K";
            break;
        default:
            char charValueT, charValueO;

            charValueT = ('0' + numValue / 10);
            charValueO = ('0' + numValue % 10);

            if (numValue / 10)
                aboutCard += charValueT;

            aboutCard += charValueO;
        }

        switch (numSuit)
        {
        case 0:
            aboutCard += "h";
            numColor = 1;
            break;
        case 1:
            aboutCard += "c";
            numColor = 0;
            break;
        case 2:
            aboutCard += "s";
            numColor = 0;
            break;
        case 3:
            aboutCard += "d";
            numColor = 1;
            break;
        }
    }

    Card& operator = (Card c) {
        numValue = c.numValue;
        numSuit = c.numSuit;
        numColor = c.numColor;

        aboutCard = c.aboutCard;
        isHidden = c.isHidden;

        return *this;
    }
};


class Klondike
{
    Card deck[MAX_CARDS];

    queue<Card> v0;
    vector<Card> v1, v2, v3, v4, v5, v6, v7;
    vector<vector<Card>> v1_7 = { v1, v2, v3, v4, v5, v6, v7 };
    stack<Card> v8, v9, v10, v11;
    vector<stack<Card>> v8_11 = { v8, v9, v10, v11 };

    int movesNum = 0;
    string whichCard;
    int a, b;

    bool isSameSuit, isSameColor, isCorrectValue;

public:
    Klondike();

    void game();

    void shuffleDeck();
    void createV();
    void createQ();

    int maxSizeV();

    void printInit();
    void printNum1();
    void printNum2();
    void printLines();
    void printSQ();
    void printV();

    void moveInit();
    void move0();
    void move1();
    void move2();
    void move3();
    void move4();

    bool isWon();

    void errors(int n);
};


Klondike::Klondike() {
    for (int i = 0; i < MAX_CARDS; i++) {
        Card temp(i / 13, i % 13 + 1);
        deck[i] = temp;
    }
}


void Klondike::game() {
    for (int i = 0; i < 4; i++)
        shuffleDeck();

    createV();
    createQ();

    cout << endl << setw(40) << "Klondike Solitaire" << endl;

    while (true) {
        printInit();

        cout << "MOVEMENT NO " << ++movesNum << endl;
        cout << "---> Input format: X ZZ Y, so the ZZ card will be taken from field X and moved to field Y (e.g. 1 7h 8) <---"
             << "\n---> To change a card in the stack, type 0 0 0 <---"
             << "\n---> To leave, write x EXIT x, where x - any number of integer type <---"
             << "\n Choice: ";

        cin >> a >> whichCard >> b;

        while (cin.fail()) {
            cin.clear();
            cin.ignore(32768, '\n');

            errors(1);

            cin >> a >> whichCard >> b;
        }

        if (whichCard == "EXIT" || whichCard == "exit")
            break;

        moveInit();

        if (isWon())
            break;
    }
}


void Klondike::shuffleDeck() {
    int j;

    for (int i = 0; i < MAX_CARDS; i++) {
        j = rand() % MAX_CARDS;

        Card temp = deck[i];
        deck[i] = deck[j];
        deck[j] = temp;
    }
}

void Klondike::createV() {
    int count = 0;

    for (int i = 0; i < 7; i++) {
        for (int j = count; j < count + i + 1; j++)
            v1_7[i].push_back(deck[j]);

        count += i + 1;

        v1_7[i].back().isHidden = false;
    }
}

void Klondike::createQ() {
    for (int i = 28; i < MAX_CARDS; i++)
        v0.push(deck[i]);
}


int Klondike::maxSizeV() {
    int max = 0;

    for (int i = 0; i < 7; i++)
        if (v1_7[i].size() > max)
            max = v1_7[i].size();

    return max;
}


void Klondike::printInit() {
    printNum1();
    printLines();
    printSQ();
    printLines();
    printV();
    printLines();
    printNum2();
}

void Klondike::printNum1() {
    cout << endl << setw(7) << "(0)" << setw(33) << " ";

    for (int i = 8; i < 12; i++)
        cout << setw(3) << "(" << i << ")";

    cout << endl;
}

void Klondike::printNum2() {
    for (int i = 1; i < 8; i++)
        cout << setw(5) << "(" << i << ")" << setw(2) << " ";

    cout << endl << endl;;
}

void Klondike::printLines() {
    for (int i = 0; i < 64; i++)
        cout << "-";

    cout << endl;
}

void Klondike::printSQ() {
    if (!v0.empty())
        cout << setw(7) << v0.front().aboutCard << setw(33) << " ";
    else
        cout << setw(7) << "0" << setw(33) << " ";

    for (int i = 0; i < 4; i++) {
        if (!v8_11[i].empty())
            cout << setw(5) << v8_11[i].top().aboutCard;
        else
            cout << setw(5) << "0";
    }

    cout << endl;
}

void Klondike::printV() {
    for (int i = 0; i < maxSizeV(); i++) {
        cout << "|";

        for (int j = 0; j < 7; j++) {
            if (i < v1_7[j].size()) {
                if (v1_7[j][i].isHidden == false)
                    cout << setw(6) << v1_7[j][i].aboutCard;
                else
                    cout << setw(6) << "===";
            }
            else
                cout << setw(6) << "  ";

            cout << setw(3) << "|";
        }

        cout << endl << endl;
    }
}


void Klondike::moveInit() {
    if (a == 0) {
        if (b == 0)
            move0();
        else if (b >= 1 && b <= 7)
            move1();
        else if (b >= 8 && b <= 11)
            move2();
        else
            errors(3);
    }
    else if (a >= 1 && a <= 7) {
        if (b >= 1 && b <= 7)
            move3();
        else if (b >= 8 && b <= 11)
            move4();
        else
            errors(3);
    }
    else
        errors(2);
}

void Klondike::move0() {
    if (!v0.empty()) {
        v0.emplace(v0.front());
        v0.pop();
    }
    else
        errors(4);
}

void Klondike::move1() {
    if (!v0.empty()) {
        if (v0.front().aboutCard == whichCard) {
            if (!v1_7[b - 1].empty()) {
                isSameColor = (v0.front().numColor == v1_7[b - 1].back().numColor);
                isCorrectValue = ((v1_7[b - 1].back().numValue - v0.front().numValue) == 1);

                if (!isSameColor && isCorrectValue) {
                    v1_7[b - 1].push_back(v0.front());
                    v0.pop();
                    v1_7[b - 1].back().isHidden = false;
                }
                else
                    errors(5);
            }
            else if (v0.front().numValue == 13) {
                v1_7[b - 1].push_back(v0.front());
                v0.pop();
                v1_7[b - 1].back().isHidden = false;
            }
            else
                errors(6);
        }
        else
            errors(7);
    }
    else
        errors(4);
}

void Klondike::move2() {
    if (!v0.empty()) {
        if (v0.front().aboutCard == whichCard) {
            if (!v8_11[b - 8].empty()) {
                isSameSuit = (v8_11[b - 8].top().numSuit == v0.front().numSuit);
                isCorrectValue = ((v0.front().numValue - v8_11[b - 8].top().numValue) == 1);

                if (isSameSuit && isCorrectValue) {
                    v8_11[b - 8].push(v0.front());
                    v0.pop();
                    v8_11[b - 8].top().isHidden = false;
                }
                else
                    errors(5);
            }
            else if (v0.front().numValue == 1) {
                v8_11[b - 8].push(v0.front());
                v0.pop();
                v8_11[b - 8].top().isHidden = false;
            }
            else
                errors(8);
        }
        else
            errors(7);
    }
    else
        errors(4);
}

void Klondike::move3() {
    if (!v1_7[a - 1].empty()) {
        int ind = -1;

        for (int i = v1_7[a - 1].size() - 1; i >= 0; i--) {
            if (v1_7[a - 1][i].aboutCard == whichCard && v1_7[a - 1][i].isHidden == false) {
                ind = i;
                break;
            }
        }

        if (ind != -1) {
            if (!v1_7[b - 1].empty()) {
                isSameColor = (v1_7[a - 1][ind].numColor == v1_7[b - 1].back().numColor);
                isCorrectValue = ((v1_7[b - 1].back().numValue - v1_7[a - 1][ind].numValue) == 1);

                if (!isSameColor && isCorrectValue) {
                    for (int i = ind; i < v1_7[a - 1].size(); i++)
                        v1_7[b - 1].push_back(v1_7[a - 1][i]);

                    v1_7[a - 1].erase(v1_7[a - 1].begin() + ind, v1_7[a - 1].end());

                    if (!v1_7[a - 1].empty())
                        v1_7[a - 1].back().isHidden = false;
                }
                else
                    errors(5);
            }
            else if (v1_7[a - 1][ind].numValue == 13) {
                for (int i = ind; i < v1_7[a - 1].size(); i++)
                    v1_7[b - 1].push_back(v1_7[a - 1][i]);

                v1_7[a - 1].erase(v1_7[a - 1].begin() + ind, v1_7[a - 1].end());

                if (!v1_7[a - 1].empty())
                    v1_7[a - 1].back().isHidden = false;
            }
            else
                errors(6);
        }
        else
            errors(7);
    }
    else
        errors(4);
}

void Klondike::move4() {
    if (!v1_7[a - 1].empty()) {
        if (v1_7[a - 1].back().aboutCard == whichCard) {
            if (!v8_11[b - 8].empty()) {
                isSameSuit = (v8_11[b - 8].top().numSuit == v1_7[a - 1].back().numSuit);
                isCorrectValue = ((v1_7[a - 1].back().numValue - v8_11[b - 8].top().numValue) == 1);

                if (isSameSuit && isCorrectValue) {
                    v8_11[b - 8].push(v1_7[a - 1].back());
                    v1_7[a - 1].pop_back();
                    v1_7[a - 1].back().isHidden = false;
                }
                else
                    errors(5);
            }
            else if (v1_7[a - 1].back().numValue == 1) {
                v8_11[b - 8].push(v1_7[a - 1].back());
                v1_7[a - 1].pop_back();
                v1_7[a - 1].back().isHidden = false;
            }
            else
                errors(8);
        }
        else
            errors(7);
    }
    else
        errors(4);
}


bool Klondike::isWon() {
    int count = 0;

    for (int i = 0; i < 4; i++)
        if (v8_11[i].size() == 13)
            count++;

    if (count == 4) {
        printLines();
        cout << "You win! Number of movements: " << movesNum << endl;

        return true;
    }

    return false;
}


void Klondike::errors(int n) {
    cout << endl;

    switch (n)
    {
    case 1:
        cout << "!!! You gave the wrong data! Try again: ";
        break;
    case 2:
        cout << "!!! You gave the wrong number of the first field to move the card! Try again." << endl;
        break;
    case 3:
        cout << "!!! You gave the wrong number of the second field to move the card! Try again." << endl;
        break;
    case 4:
        cout << "!!! The stack (or column a) is empty!" << endl;
        break;
    case 5:
        cout << "!!! The card does not spread with a color or / and the card is not lower by one point!" << endl;
        break;
    case 6:
        cout << "!!! When a column is empty, only the king can be placed in it to start a new column!" << endl;
        break;
    case 7:
        cout << "!!! You played the wrong card! (probably not in the selected field or the field is empty)" << endl;
        break;
    case 8:
        cout << "!!! There must be an ace at the bottom of each stack!" << endl;
        break;
    }
}


int main() {
    srand(time(NULL));
    Klondike solitare;

    solitare.game();

    return 0;
}