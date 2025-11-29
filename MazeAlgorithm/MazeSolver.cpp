#include "MazeSolver.h"
#include "Common.h" // SetCursorPosition, TileType 등
#include <queue>
#include <vector>
#include <utility> // pair
#include <cmath>   // abs
#include <iostream>

using namespace std;

//====================맨해탄 거리=====================
int MazeSolver::CalculateH(int x1, int y1, int x2, int y2) {
    return abs(x1 - x2) + abs(y1 - y2);
}//A*

//====================미로 탐색=====================
void MazeSolver::Breadthfirstsearch(MazeBoard& board) {
    int m_size = board.GetSize();
    int sX = board.startX;
    int sY = board.startY;
    int gX = board.goalX;
    int gY = board.goalY;

    bool** visited = new bool* [m_size];//Initialize에서 사이즈 정함 ㅇㅇ
    pair<int, int>** parent = new pair<int, int>* [m_size];

    for (int i = 0; i < m_size; i++) {
        visited[i] = new bool[m_size]();  // () 붙이면 false로 초기화
        parent[i] = new pair<int, int>[m_size];
        for (int j = 0; j < m_size; j++) {
            parent[i][j] = { -1, -1 };
        }
    }

    queue<pair<int, int>> q;//큐 아직 이해 못함 //BFS에 쓰이는 queue(선입선출). pair<int,int>에는 (x, y)를 담습니다.

    q.push({ sX, sY });//시작점에 큐 넣기
    visited[sY][sX] = true;//방문 표시

    // 4방향 이동
    int dx[] = { 0, 0, -1, 1 };
    int dy[] = { -1, 1, 0, 0 };

    // 메인 알고리즘
    bool found = false;//목적지 도달 여부 초기화
    while (!q.empty() && !found) {//큐가 비어있지 않고 목적지에 도달하지 않았으면 계속 반복
        int curX = q.front().first;//큐에서 현재 위치 꺼내기
        int curY = q.front().second;
        q.pop();//큐에서 현재 위치 꺼내기

        // 목적지에 도달했는지 확인
        if (curX == gX && curY == gY) {
            found = true;
            break;
        }

        // 4방향 탐색
        for (int i = 0; i < 4; i++) {
            int newX = curX + dx[i];//현재 위치에서 4방향으로 이동한 위치 계산
            int newY = curY + dy[i];

            // 유효한 위치이고 방문하지 않았으며 벽이 아닌 경우
            if (board.IsValid(newX, newY) && !visited[newY][newX]) {//만약 유효한 위치이고 방문하지 않았으며 벽이 아닌 경우
                visited[newY][newX] = true;//방문 표시
                parent[newY][newX] = { curX, curY };//부모 노드 기록
                q.push({ newX, newY });//큐에 새 위치 추가
                board.SetTile(newX, newY, Path);//경로 표시
                SetCursorPosition(0, 0);
                board.Render();
            }
        }
    }
    //그니깐 벽 검사하고 벽아님? 바로 let's go 하고 큐에 넣고 방문표시하고 부모노드 기록하고 경로표시
    // 경로가 발견된 경우 역추적하여 경로 표시
    if (found) {
        int curX = gX;//현재 위치를 목표 지점으로 설정
        int curY = gY;

        // 시작점까지 역추적
        while (!(curX == sX && curY == sY)) {//현재 위치가 시작점이 아닐 때까지 반복
            if (!(curX == gX && curY == gY) && !(curX == sX && curY == sY)) {//현재 위치가 시작점이나 목표점이 아닐 때
                board.SetTile(curX, curY, Loot);//경로 표시
            }
            //부모 노드 좌표 가져오기
            int prevX = parent[curY][curX].first;//이전 좌표에 부모 노드 좌표 저장
            int prevY = parent[curY][curX].second;
            curX = prevX;
            curY = prevY;
            Sleep(1);
            SetCursorPosition(0, 0);
            board.Render();
        }
        system("cls");
        SetCursorPosition(0, 0);
        board.Render();
        cout << "\n경로를 찾았습니다!" << endl;
    }
    else {
        system("cls");
        SetCursorPosition(0, 0);
        board.Render();
        cout << "\n경로를 찾을 수 없습니다!" << endl;
    }

    //메모리 해제
    for (int i = 0; i < m_size; i++) {
        delete[] visited[i];
        delete[] parent[i];
    }
    delete[] visited;
    delete[] parent;
}
//=========================================
void MazeSolver::Depthfirstsearch(MazeBoard& board) {
    int m_size = board.GetSize();
    int sX = board.startX;
    int sY = board.startY;
    int gX = board.goalX;
    int gY = board.goalY;
    bool found = false;
    bool visited[50][50] = { false };

    visited[sY][sX] = true;
    vector<pair<int, int>> Stack;
    Stack.push_back({ sX, sY });

    int dx[] = { 0, 0, -1, 1 };
    int dy[] = { -1, 1, 0, 0 };

    while (!Stack.empty() && !found) {
        int curX = Stack.back().first;
        int curY = Stack.back().second;

        if (curX == gX && curY == gY) {
            found = true;
            board.SetTile(curY, curX, Path);
            break;
        }

        vector<int> validDirections;
        for (int i = 0; i < 4; i++) {
            int newX = curX + dx[i];
            int newY = curY + dy[i];

            if (board.IsValid(newX, newY) && !visited[newY][newX]) {
                validDirections.push_back(i);
            }
        }

        if (!validDirections.empty()) {
            int dir = validDirections[0];
            int newX = curX + dx[dir];
            int newY = curY + dy[dir];

            visited[newY][newX] = true;
            Stack.push_back({ newX, newY });

            board.SetTile(newX, newY, Path);
            SetCursorPosition(0, 0);
            board.Render();
        }
        else {
            int backX = Stack.back().first;
            int backY = Stack.back().second;
            Stack.pop_back();

            // 시작점/목적지가 아니면 백트래킹 표시
            if (!(backX == sX && backY == sY) && !(backX == gX && backY == gY)) {
				board.SetTile(backX, backY, Back);
                SetCursorPosition(0, 0);
                board.Render();
            }
        }
    }

    if (found) {
        system("cls");
        SetCursorPosition(0, 0);
        board.Render();
        cout << "\n경로를 찾았습니다!" << endl;
    }
    else {
        system("cls");
        SetCursorPosition(0, 0);
        board.Render();
        cout << "\n경로를 찾을 수 없습니다!" << endl;
    }
}
//=========================================
void MazeSolver::AStarsearch(MazeBoard& board) {
    //난이제 모르겠다 시간이 없다
}