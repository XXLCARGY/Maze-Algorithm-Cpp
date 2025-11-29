#include "Common.h"
#include "MazeBoard.h"
#include "MazeGenerator.h"
#include "MazeSolver.h"
#include "TextManager.h" // 텍스트 매니저 추가

using namespace std;

int main() {
    // 윈도우 콘솔 커서 숨기기 (Common.h에 있는 함수 사용)
    HideCursor();

    while (true) {
        int mapsize = 49;
        int choice;

        // 1. 미로 생성 알고리즘 선택 메뉴
        system("cls");
        cout << "====================================\n";
        cout << "     Maze Generation Algorithms     \n";
        cout << "====================================\n\n";
        cout << "1. Binary Tree Algorithm\n";
        cout << "2. Growing Binary Tree Algorithm\n";
        cout << "3. Recursive Backtracking Algorithm\n";
        cout << "4. Hunt and Kill Algorithm\n";
        cout << "5. CellularAutomata Algorithm(not maze just Solo practice)\n";
        cout << "6. Blank Map(Just for Test when PathFinding Algorithm)\n";
        cout << "0. Exit\n\n";
        cout << "Select algorithm (0-6): ";
        cin >> choice;

        if (choice == 0) {
            cout << "Goodbye!\n";
            break;
        }
        if (choice < 1 || choice > 6) continue;

        system("cls");

        // 2. 선택한 생성 알고리즘 설명 출력 (TextManager 사용)
        TextManager::PrintAlgorithmInfo(choice);
        Sleep(2000);
        system("cls");

        // 3. 보드 초기화 및 미로 생성
        MazeBoard board;
        board.Init(mapsize);

        // 생성 과정 시각화 (true)
        MazeGenerator::Generate(board, choice, true);

        // 4. 길 찾기 알고리즘 선택 메뉴
        system("cls");
        board.Render(); // 생성된 미로 한 번 보여주고 메뉴 출력

        int solveChoice;
        cout << "\n====================================\n";
        cout << "     Maze Finding Algorithms     \n";
        cout << "====================================\n\n";
        cout << "1. Breadthfirstsearch(BFS) Algorithm\n";
        cout << "2. Depthfirstsearch(DFS) Algorithm\n";
        cout << "3. Dijkstra(Still Dev) Algorithm\n";
        cout << "4. Astar Algorithm\n";
        cout << "0. Exit\n\n";
        cout << "Select algorithm (0-4): ";
        cin >> solveChoice;

        if (solveChoice == 0) {
            cout << "Goodbye!\n";
            break;
        }

        // 5. 길 찾기 설명 출력 (TextManager 사용)
        // TextManager에 8번(BFS), 9번(DFS)으로 등록되어 있으므로 매핑
        if (solveChoice == 1) { // BFS
            system("cls");
            TextManager::PrintAlgorithmInfo(8);
            Sleep(3000);
        }
        else if (solveChoice == 2) { // DFS
            system("cls");
            TextManager::PrintAlgorithmInfo(9);
            Sleep(3000);
        }
        // A*(4)나 Dijkstra(3) 설명이 TextManager에 있다면 여기서 추가 호출

        // 6. 길 찾기 실행
        system("cls");
        board.Render(); // 배경 렌더링

        switch (solveChoice) {
        case 1:
            MazeSolver::Breadthfirstsearch(board);
            break;
        case 2:
            MazeSolver::Depthfirstsearch(board);
            break;
        case 3:
            cout << "\nStill Dev...\n";
            Sleep(1000);
            break;
        case 4:
            cout << "\nStill Dev...\n";
            break;
        }

        // 7. 재시작 여부 확인
        cout << "\n1. Restart  0. Exit\n";
        int retry;
        cin >> retry;
        if (retry == 0) {
            cout << "Goodbye!\n";
            break;
        }
        // 루프 돌면서 system("cls")로 초기화됨
    }
    return 0;
}