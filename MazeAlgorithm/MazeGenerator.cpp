#include "MazeGenerator.h"
#include "Common.h" // SetCursorPosition, TileType 등 정의
#include <cstdlib>
#include <ctime>
#include <vector>
#include <utility>
#include <assert.h>

using namespace std;

void MazeGenerator::Generate(MazeBoard& board, int algorithm, bool showProcess) {
    // 알고리즘 선택 전 랜덤 시드 초기화 (원본 Initialize에 있던 부분 대신 여기서 한 번 호출)
    // 각 함수 내부에도 srand가 있지만, 안전장치로 둡니다.
    srand(unsigned(time(NULL)));

    switch (algorithm) {
    case 1:
        GenerateBinaryTree(board, showProcess);
        break;
    case 2:
        GenerateGrowingBinaryTree(board, showProcess);
        break;
    case 3:
        GenerateRecursiveBacktracking(board, showProcess);
        break;
    case 4:
        GenerateHuntAndKill(board, showProcess);
        break;
    case 5:
        GenerateCellularAutomata(board, showProcess);
        break;
    case 6:
        // Case 6: 가장자리 벽 설정 (Initialize 함수에 있던 로직)
        int m_size = board.GetSize();
        // 전체 초기화는 필요하다면 밖에서 수행, 여기서는 6번 로직만 수행
        for (int y = 0; y < m_size; y++) {
            for (int x = 0; x < m_size; x++) {
                if (y == 0 || y == m_size - 1 || x == 0 || x == m_size - 1) {
                    board.SetTile(x, y, Wall); // 가장자리 벽 설정
                }
                else {
                    board.SetTile(x, y, Empty); // 내부는 비움 (기본 초기화)
                }
            }
        }
        break;
    }
}

//===================미로 생성 알고리즘 파트======================
void MazeGenerator::GenerateBinaryTree(MazeBoard& board, bool showProcess) {
    int m_size = board.GetSize();

    //맵 초기화 x y가 짝수일때 wall로 지정 else 빈칸
    for (int y = 0; y < m_size; y++) {
        for (int x = 0; x < m_size; x++) {
            if (x % 2 == 0 || y % 2 == 0)
                board.SetTile(x, y, Wall);
            else
                board.SetTile(x, y, Empty);
        }
    }
    if (showProcess) {
        SetCursorPosition(0, 0);
        board.Render();
    }
    srand(unsigned(time(NULL)));
    for (int y = 0; y < m_size; y++) {// 모든 칸을 소모할때까지
        for (int x = 0; x < m_size; x++) {
            if (x % 2 == 0 || y % 2 == 0)//2배 때리는건죽음임
                continue;
            bool canGoRight = (x < m_size - 2);//벽인지 검사
            bool canGoDown = (y < m_size - 2);
            if (!canGoRight && !canGoDown)
                continue;
            else if (!canGoRight)
                board.SetTile(x, y + 1, Empty);//비워버려
            else if (!canGoDown)
                board.SetTile(x + 1, y, Empty);//비워버려
            else {
                if (rand() % 2 == 0)//둘다 가능하면 랜덤
                    board.SetTile(x + 1, y, Empty);
                else
                    board.SetTile(x, y + 1, Empty);
            }
            if (showProcess) {
                SetCursorPosition(0, 0);
                board.Render();
                Sleep(0);
            }
        }
    }
}
//=========================================
void MazeGenerator::GenerateGrowingBinaryTree(MazeBoard& board, bool showProcess) {
    int m_size = board.GetSize();

    for (int y = 0; y < m_size; y++) {
        for (int x = 0; x < m_size; x++) {
            board.SetTile(x, y, Wall);
        }
    }

    if (showProcess) {
        SetCursorPosition(0, 0);
        board.Render();
    }

    srand(unsigned(time(NULL)));

    bool visited[50][50] = { false };//레전드 정상화
    int startX = 1, startY = 1;//생성 시작지점
    board.SetTile(startX, startY, Empty);//시작을 비우고
    visited[startY][startX] = true;// 시작에 방문 표시를 하며
    vector<pair<int, int>> activeList;//2차원 좌표 활성화 리스트에 넣음 동적 할당임
    activeList.push_back({ startX, startY });//넣고
    //4방향 백터
    int dx[] = { 0, 0, -2, 2 };
    int dy[] = { -2, 2, 0, 0 };

    while (!activeList.empty()) {//모든 활성화 리스트가 비워질때까지
        int idx = rand() % activeList.size();
        int curX = activeList[idx].first;
        int curY = activeList[idx].second;

        vector<int> validDirections;
        for (int i = 0; i < 4; i++) {
            int newX = curX + dx[i];
            int newY = curY + dy[i];
            if (newX > 0 && newX < m_size - 1 &&
                newY > 0 && newY < m_size - 1 &&
                !visited[newY][newX]) {
                validDirections.push_back(i);
            }
        }

        if (!validDirections.empty()) {//
            int dir = validDirections[rand() % validDirections.size()];
            int newX = curX + dx[dir];
            int newY = curY + dy[dir];
            int wallX = curX + dx[dir] / 2;
            int wallY = curY + dy[dir] / 2;
            board.SetTile(wallX, wallY, Empty);
            board.SetTile(newX, newY, Empty);
            visited[newY][newX] = true;
            activeList.push_back({ newX, newY });

            if (showProcess) {
                SetCursorPosition(0, 0);
                board.Render();
                Sleep(0);
            }
        }
        else {
            activeList.erase(activeList.begin() + idx);//리스트 비우기
        }
    }
}
//=========================================
void MazeGenerator::GenerateRecursiveBacktracking(MazeBoard& board, bool showProcess) {
    int m_size = board.GetSize();

    for (int y = 0; y < m_size; y++) {
        for (int x = 0; x < m_size; x++) {
            board.SetTile(x, y, Wall);
        }
    }
    if (showProcess) {
        SetCursorPosition(0, 0);
        board.Render();
    }

    srand(unsigned(time(NULL)));

    bool visited[50][50] = { false };//모두 방문 안했고
    int startX = 1, startY = 1;//생성 시작지점
    board.SetTile(startX, startY, Empty);//시작을 비우고
    visited[startY][startX] = true;// 시작에 방문 표시를 하며
    vector<pair<int, int>> activeList;//아마 스택처럼 써야하지 않을까? 길찾기인데 뒤로 돌아가야한다는점을 착안해서 ㅇㅇ
    activeList.push_back({ startX, startY });//넣고

    //4방향 백터
    int dx[] = { 0, 0, -2, 2 };
    int dy[] = { -2, 2, 0, 0 };

    while (!activeList.empty()) {
        int curX = activeList.back().first;
        int curY = activeList.back().second;

        vector<int> validDirections;
        for (int i = 0; i < 4; i++) {
            int newX = curX + dx[i];
            int newY = curY + dy[i];
            if (newX > 0 && newX < m_size - 1 &&
                newY > 0 && newY < m_size - 1 &&
                !visited[newY][newX]) {
                validDirections.push_back(i);
            }
        }
        if (!validDirections.empty()) {
            int dir = validDirections[rand() % validDirections.size()];
            int newX = curX + dx[dir];
            int newY = curY + dy[dir];

            int wallX = curX + dx[dir] / 2;
            int wallY = curY + dy[dir] / 2;

            board.SetTile(wallX, wallY, Empty);
            board.SetTile(newX, newY, Empty);
            visited[newY][newX] = true;

            activeList.push_back({ newX, newY });  // 스택에 추가

            if (showProcess) {
                board.SetTile(newX, newY, Path);
                SetCursorPosition(0, 0);
                board.Render();
                Sleep(10);
                board.SetTile(newX, newY, Empty);
            }
        }
        else {
            int backX = activeList.back().first;
            int backY = activeList.back().second;

            activeList.pop_back();

            if (showProcess) {
                board.SetTile(backX, backY, Back);  // 백트래킹 표시
                SetCursorPosition(0, 0);
                board.Render();
                Sleep(50);
                board.SetTile(backX, backY, Empty);
            }
        }
    }
}
//=========================================
void MazeGenerator::GenerateHuntAndKill(MazeBoard& board, bool showProcess) {
    int m_size = board.GetSize();

    for (int y = 0; y < m_size; y++) {
        for (int x = 0; x < m_size; x++) {
            board.SetTile(x, y, Wall);
        }
    }

    if (showProcess) {
        SetCursorPosition(0, 0);
        board.Render();
    }

    srand(unsigned(time(NULL)));

    bool visited[50][50] = { false };//모두 방문 안했고
    int startX = 1, startY = 1;//생성 시작지점
    board.SetTile(startX, startY, Empty);//시작을 비우고
    visited[startY][startX] = true;// 시작에 방문 표시를 하며

    int curX = startX;
    int curY = startY;

    //4방향 백터
    int dx[] = { 0, 0, -2, 2 };
    int dy[] = { -2, 2, 0, 0 };

    while (true) {

        vector<int> validDirections;
        for (int i = 0; i < 4; i++) {
            int newX = curX + dx[i];
            int newY = curY + dy[i];
            if (newX > 0 && newX < m_size - 1 &&
                newY > 0 && newY < m_size - 1 &&
                !visited[newY][newX]) {
                validDirections.push_back(i);
            }
        }
        if (!validDirections.empty()) {
            int dir = validDirections[rand() % validDirections.size()];
            int newX = curX + dx[dir];
            int newY = curY + dy[dir];

            int wallX = curX + dx[dir] / 2;
            int wallY = curY + dy[dir] / 2;

            board.SetTile(wallX, wallY, Empty);

            assert(newX > 0 && newX < m_size - 1 &&
                newY > 0 && newY < m_size - 1);

            board.SetTile(newX, newY, Empty);
            visited[newY][newX] = true;

            curX = newX;  // 현재 위치 업데이트
            curY = newY;

            if (showProcess) {
                board.SetTile(newX, newY, Path);
                SetCursorPosition(0, 0);
                board.Render();
                board.SetTile(newX, newY, Empty);
            }
        }
        else {
            bool found = false;
            //이 전설적인 포문을 봐라 이게 뭔 개판이란말인가!!!
            for (int y = 1; y < m_size - 1 && !found; y += 2) {
                if (showProcess) {
                    //스캔 시각화 
                    for (int x = 1; x < m_size - 1; x += 2) {
                        board.SetTile(x, y, Scan);
                    }
                    SetCursorPosition(0, 0);
                    board.Render();
                    for (int x = 1; x < m_size - 1; x += 2) {
                        board.SetTile(x, y, (visited[y][x]) ? Empty : Wall);
                    }
                }
                for (int x = 1; x < m_size - 1 && !found; x += 2) {

                    // 방문 안 한 셀 중에서
                    if (!visited[y][x]) {
                        // 인접한 방문한 셀이 있는지 체크
                        for (int i = 0; i < 4; i++) {
                            int adjX = x + dx[i];
                            int adjY = y + dy[i];

                            if (adjX > 0 && adjX < m_size - 1 &&
                                adjY > 0 && adjY < m_size - 1 &&
                                visited[adjY][adjX]) {

                                int wallX = x + dx[i] / 2;
                                int wallY = y + dy[i] / 2;

                                board.SetTile(wallX, wallY, Empty);
                                board.SetTile(x, y, Empty);
                                visited[y][x] = true;

                                curX = x;  // 새 시작점
                                curY = y;
                                found = true;

                                if (showProcess) {
                                    board.SetTile(x, y, Back);  // Hunt 표시
                                    SetCursorPosition(0, 0);
                                    board.Render();
                                    board.SetTile(x, y, Empty);
                                }
                                break;
                            }
                        }
                    }
                }
            }
            if (!found) {
                break;
            }

        }
    }
}
//=========================================
void MazeGenerator::GenerateCellularAutomata(MazeBoard& board, bool showProcess) {
    int m_size = board.GetSize();

    //맵 초기화 x y가 짝수일때 wall로 지정 else 빈칸
    for (int y = 0; y < m_size; y++) {
        for (int x = 0; x < m_size; x++) {
            board.SetTile(x, y, (rand() % 100 < 45) ? Wall : Empty); // 45% 확률로 벽 생성
        }
    }
    for (int y = 0; y < m_size; y++) {
        for (int x = 0; x < m_size; x++) {
            if (y == 0 || y == m_size - 1 || x == 0 || x == m_size - 1) {
                board.SetTile(x, y, Wall); // 가장자리 벽 설정
            }
        }
    }
    if (showProcess) {
        SetCursorPosition(0, 0);
        board.Render();
        Sleep(500);
    }
    srand(unsigned(time(NULL)));
    //smoothmap
    for (int iteration = 0; iteration < 10; iteration++) { //5번 반복
        TileType newTile[50][50];
        for (int y = 1; y < m_size - 1; y++) {
            for (int x = 1; x < m_size - 1; x++) {
                int wallCount = 0;
                //주변 8칸 검사
                for (int dy = -1; dy <= 1; dy++) {
                    for (int dx = -1; dx <= 1; dx++) {
                        if (dx == 0 && dy == 0) continue;
                        if (board.GetTile(x + dx, y + dy) == Wall)
                            wallCount++;
                    }
                }
                //규칙 적용
                if (board.GetTile(x, y) == Wall) {
                    newTile[y][x] = (wallCount >= 4) ? Wall : Empty;
                }
                else {
                    newTile[y][x] = (wallCount >= 5) ? Wall : Empty;
                }
            }
        }
        //업데이트
        for (int y = 1; y < m_size - 1; y++) {
            for (int x = 1; x < m_size - 1; x++) {
                board.SetTile(x, y, newTile[y][x]);
            }
        }
        if (showProcess) {
            SetCursorPosition(0, 0);
            board.Render();
            Sleep(200);
        }
    }
}