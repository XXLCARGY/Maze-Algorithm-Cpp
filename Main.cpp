#include "Board.h"
#include <cstdlib>
using namespace std;

int main() {
    int choice;
    cout << "====================================\n";
    cout << "     Maze Generation Algorithms     \n";
    cout << "====================================\n\n";
    cout << "1. Binary Tree Algorithm\n";
    cout << "2. Growing Binary Tree Algorithm\n";
    cout << "0. Exit\n\n";
    cout << "Select algorithm (0-2): ";
    cin >> choice;

    if (choice == 0) {
        cout << "Goodbye!\n";
        return 0;
    }

    if (choice < 1 || choice > 2) {
        cout << "Invalid choice!\n";
        return 0;
    }

    cout << "\n";

    HideCursor();
    system("cls");
    Board board;
    board.Algorithm(choice);
    system("cls");
    board.Initialize(49, choice, true);
    SetCursorPosition(0, 0);
    board.Render();

    return 0;
}
