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
    cout << "3. Recursive Backtracking Algorithm\n";
    cout << "4. Hunt and Kill Algorithm\n";
    cout << "0. Exit\n\n";
    cout << "Select algorithm (0-4): ";
    cin >> choice;
    
    if (choice == 0) {
        cout << "Goodbye!\n";
        return 0;
    }

    if (choice < 1 || choice > 4) {
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
    //미로 탐색 함수가 추가되는 대로 그냥 여기다가 선택하게 추가하기
    board.Breadthfirstsearch(1, 1, mapsize - 2, mapsize - 2);
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
