#include "Board.h"
#include <cstdlib>
using namespace std;

int main() {
	int mapsize = 49;//보드 크기 저장//최대 50//홀수로 선언 해야함
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
    board.Initialize(mapsize, choice, true);
    SetCursorPosition(0, 0);
    board.Render();
    board.BasicMazeSearch(1, 1, mapsize-2, mapsize-2);
	cout << "\n1. Restart\n0. Exit\n";
    cin >> choice;
    switch (choice)
    {
	case 0:
		cout << "Goodbye!\n";
		break;
	case 1:
		system("cls");
		main();
    }
    return 0;
}
