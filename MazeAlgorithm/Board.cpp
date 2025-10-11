#include "Board.h"
#include <cstdlib>
#include <ctime>
#include <queue>
#include <utility>
#include <assert.h>

//맴버 변수랑 로컬 변수 구분좀 되게 해두기
void SetCursorPosition(int x, int y) {
    COORD coord;
    coord.X = x;
    coord.Y = y;
    SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), coord);//이게이게뭐에요? 이게뭐에요?
}
//=========================================
void HideCursor() {
    CONSOLE_CURSOR_INFO cursorInfo;
    GetConsoleCursorInfo(GetStdHandle(STD_OUTPUT_HANDLE), &cursorInfo);
    cursorInfo.bVisible = FALSE;
    SetConsoleCursorInfo(GetStdHandle(STD_OUTPUT_HANDLE), &cursorInfo);
}
//=========================================
Board::Node::Node(int x, int y, int g, int h, Node* parent)
    : m_x(x), m_y(y), m_g(g), m_h(h), m_parent(parent) {
    m_f = g + h;
}
//=========================================
bool Board::Node::operator>(const Node& other) const {
    return m_f > other.m_f;
}
//=========================================
int Board::CalculateH(int x1, int y1, int x2, int y2) {
    return abs(x1 - x2) + abs(y1 - y2);
}
//=========================================
bool Board::IsValid(int x, int y) {
	if (x < 0 || x >= m_size || y < 0 || y >= m_size)//범위 벗어남
		return false;//false 반환
	if (tile[y][x] == Wall)//벽임
		return false;// false 반환
	return true;// 유효한 위치
}
//=========================================
void Board::GenerateBinaryTree(bool showProcess) {
    //맵 초기화 x y가 짝수일때 wall로 지정 else 빈칸
    for (int y = 0; y < m_size; y++) {
        for (int x = 0; x < m_size; x++) {
            if (x % 2 == 0 || y % 2 == 0)
                tile[y][x] = Wall;
            else
                tile[y][x] = Empty;
        }
    }
    if (showProcess) {
        SetCursorPosition(0, 0);
        Render();
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
                tile[y + 1][x] = Empty;//비워버려
            else if (!canGoDown)
                tile[y][x + 1] = Empty;//비워버려
            else {
                if (rand() % 2 == 0)//둘다 가능하면 랜덤
                    tile[y][x + 1] = Empty;
                else
                    tile[y + 1][x] = Empty;
            }
            if (showProcess) {
                SetCursorPosition(0, 0);
                Render();
                Sleep(0);
            }
        }
    }
}
//=========================================
void Board::GenerateGrowingBinaryTree(bool showProcess) {
    for (int y = 0; y < m_size; y++) {
        for (int x = 0; x < m_size; x++) {
            tile[y][x] = Wall;
        }
    }

    if (showProcess) {
        SetCursorPosition(0, 0);
        Render();
    }

    srand(unsigned(time(NULL)));

    bool visited[50][50] = { false };//레전드 정상화
    int startX = 1, startY = 1;//생성 시작지점
    tile[startY][startX] = Empty;//시작을 비우고
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
            tile[wallY][wallX] = Empty;
            tile[newY][newX] = Empty;
            visited[newY][newX] = true;
            activeList.push_back({ newX, newY });

            if (showProcess) {
                SetCursorPosition(0, 0);
                Render();
                Sleep(0);
            }
        }
        else {
            activeList.erase(activeList.begin() + idx);//리스트 비우기
        }
    }
}
//=========================================
void Board::GenerateRecursiveBacktracking(bool showProcess) {
    for (int y = 0; y < m_size; y++) {
        for (int x = 0; x < m_size; x++) {
            tile[y][x] = Wall;
        }
    }
    if (showProcess) {
        SetCursorPosition(0, 0);
        Render();
    }

    srand(unsigned(time(NULL)));

    bool visited[50][50] = { false };//모두 방문 안했고
    int startX = 1, startY = 1;//생성 시작지점
    tile[startY][startX] = Empty;//시작을 비우고
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

            tile[wallY][wallX] = Empty;
            tile[newY][newX] = Empty;
            visited[newY][newX] = true;

            activeList.push_back({ newX, newY });  // 스택에 추가

            if (showProcess) {
                tile[newY][newX] = Path;
                SetCursorPosition(0, 0);
                Render();
                Sleep(10);
                tile[newY][newX] = Empty;
            }
        }
        else {
            int backX = activeList.back().first;
            int backY = activeList.back().second;

            activeList.pop_back();

            if (showProcess) {
                tile[backY][backX] = Back;  // 백트래킹 표시
                SetCursorPosition(0, 0);
                Render();
                Sleep(50);
                tile[backY][backX] = Empty;
            }
        }
    }
}
//=========================================
void Board::GenerateHuntandKill(bool showProcess) {
    for (int y = 0; y < m_size; y++) {
        for (int x = 0; x < m_size; x++) {
            tile[y][x] = Wall;
        }
    }
    
    if (showProcess) {
        SetCursorPosition(0, 0);
        Render();
    }

    srand(unsigned(time(NULL)));

    bool visited[50][50] = { false };//모두 방문 안했고
    int startX = 1, startY = 1;//생성 시작지점
    tile[startY][startX] = Empty;//시작을 비우고
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
            
            tile[wallY][wallX] = Empty;

            assert(newX > 0 && newX < m_size - 1 &&
                newY > 0 && newY < m_size - 1);
            
            tile[newY][newX] = Empty;
            visited[newY][newX] = true;

            curX = newX;  // 현재 위치 업데이트
            curY = newY;

            if (showProcess) {
                tile[newY][newX] = Path;
                SetCursorPosition(0, 0);
                Render();
                tile[newY][newX] = Empty;
            }
        }
        else {
            bool found = false;
            //이 전설적인 포문을 봐라 이게 뭔 개판이란말인가!!!
            for (int y = 1; y < m_size - 1 && !found; y += 2) {
                if (showProcess) {
                    //스캔 시각화 
                    for (int x = 1; x < m_size - 1; x += 2) {
                        tile[y][x] = Scan;
                    }
                    SetCursorPosition(0, 0);
                    Render();
                    for (int x = 1; x < m_size - 1; x +=2) {
                        tile[y][x] = (visited[y][x]) ? Empty : Wall;
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

                                tile[wallY][wallX] = Empty;
                                tile[y][x] = Empty;
                                visited[y][x] = true;

                                curX = x;  // 새 시작점
                                curY = y;
                                found = true;

                                if (showProcess) {
                                    tile[y][x] = Back;  // Hunt 표시
                                    SetCursorPosition(0, 0);
                                    Render();
                                    tile[y][x] = Empty;
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
void Board::Initialize(int boardSize, int algorithm, bool showProcess) {
    assert(boardSize % 2&&"보드 사이즈 개똥임 정상화 하셈 ㅇㅇ");
    m_size = boardSize;
    m_startX = 1;
    m_startY = 1;
    m_goalX = m_size - 2;
    m_goalY = m_size - 2;
    switch (algorithm) {
    case 1:
        GenerateBinaryTree(showProcess);
        break;
    case 2:
        GenerateGrowingBinaryTree(showProcess);
        break;
    case 3:
        GenerateRecursiveBacktracking(showProcess);
        break;
    case 4:
        GenerateHuntandKill(showProcess);
    }
}
//=========================================
void Board::Render() {
    for (int y = 0; y < m_size; y++) {
        for (int x = 0; x < m_size; x++) {
            if (x == m_startX && y == m_startY)
                cout << "\033[42m  \033[0m";
            else if (x == m_goalX && y == m_goalY)
                cout << "\033[41m  \033[0m";
            else if (tile[y][x] == Wall)
                cout << "\033[47m  \033[0m";
            else if (tile[y][x] == Path)
                cout << "\033[44m  \033[0m";// Path는 파란색으로 표시
            else if (tile[y][x] == Loot)
                cout << "\033[43m  \033[0m";// Loot는 노란색으로 표시
            else if (tile[y][x] == Back)
                cout << "\033[41m  \033[0m";// Back은 붉은색으로 표시
            else if (tile[y][x] == Scan)
                cout << "\033[42m  \033[0m";
            else
                cout << "\033[40m  \033[0m";
        }
        cout << endl;
    }
}
//=========================================
void Board::Breadthfirstsearch(int sX, int sY, int gX, int gY) {
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
			if (IsValid(newX, newY) && !visited[newY][newX]) {//만약 유효한 위치이고 방문하지 않았으며 벽이 아닌 경우
				visited[newY][newX] = true;//방문 표시
				parent[newY][newX] = { curX, curY };//부모 노드 기록
				q.push({ newX, newY });//큐에 새 위치 추가
				tile[newY][newX] = Path;//경로 표시
                SetCursorPosition(0, 0);
                Render();
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
				tile[curY][curX] = Loot;//경로 표시
            }
            //부모 노드 좌표 가져오기
			int prevX = parent[curY][curX].first;//이전 좌표에 부모 노드 좌표 저장
			int prevY = parent[curY][curX].second;
            curX = prevX;
            curY = prevY;
            Sleep(1);
            SetCursorPosition(0, 0);
            Render();
        }

        cout << "\n경로를 찾았습니다!" << endl;
    }
    else {
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
void Board::Algorithm(int choice) {
    
    switch (choice) {
    case 1: {
        cout << "=== 이진 트리 미로 알고리즘 ===" << endl << endl;

        cout << "[ 작동 원리 ]" << endl;
        cout << "각 셀(칸)마다 두 방향 중 하나만 선택해서 통로를 만듭니다:" << endl;
        cout << "  * 일반적으로 북쪽(위) 또는 동쪽(오른쪽) 중 하나를 랜덤하게 선택" << endl;
        cout << "  * 선택한 방향의 벽을 제거하여 통로 생성" << endl << endl;

        cout << "[ 알고리즘 단계 ]" << endl;
        cout << "1. 그리드의 모든 셀을 순회합니다" << endl;
        cout << "2. 각 셀에서:" << endl;
        cout << "   * 북쪽과 동쪽 방향이 모두 가능하면 → 둘 중 하나를 랜덤 선택" << endl;
        cout << "   * 한 방향만 가능하면 → 그 방향 선택" << endl;
        cout << "   * 둘 다 불가능하면 (가장자리 코너) → 아무것도 안 함" << endl << endl;

        cout << "[ 특징 ]" << endl << endl;

        cout << "장점:" << endl;
        cout << "  * 구현이 매우 간단함" << endl;
        cout << "  * 빠른 생성 속도 (O(n))" << endl;
        cout << "  * 메모리 효율적" << endl << endl;

        cout << "단점:" << endl;
        cout << "  * 명확한 편향(bias)이 생김 - 한쪽 방향으로 치우친 패턴" << endl;
        cout << "  * 북동쪽 코너가 항상 막다른 골목이 됨" << endl;
        cout << "  * 미로가 너무 단순하고 예측 가능함" << endl;
        cout << "  * 긴 직선 복도가 많이 생성됨" << endl;
        Sleep(6000);
        break;
    }
    case 2: {
        cout << "=== Growing Binary Tree 알고리즘 ===" << endl << endl;

        cout << "[ 작동 원리 ]" << endl;
        cout << "트리가 성장하듯이 미로를 확장해 나가는 방식입니다:" << endl;
        cout << "  * 시작 셀에서부터 랜덤하게 인접한 셀로 확장" << endl;
        cout << "  * 방문하지 않은 셀을 만날 때마다 트리에 추가" << endl;
        cout << "  * 더 이상 확장할 수 없으면 백트래킹" << endl << endl;

        cout << "[ 알고리즘 단계 ]" << endl;
        cout << "1. 시작 셀을 선택하고 활성 셀 리스트에 추가" << endl;
        cout << "2. 활성 셀 리스트가 비어있지 않는 동안:" << endl;
        cout << "   * 리스트에서 랜덤하게 셀 선택" << endl;
        cout << "   * 선택한 셀의 미방문 이웃을 찾음" << endl;
        cout << "   * 이웃이 있으면:" << endl;
        cout << "     - 랜덤하게 이웃 선택" << endl;
        cout << "     - 현재 셀과 이웃 사이의 벽 제거" << endl;
        cout << "     - 이웃을 활성 리스트에 추가" << endl;
        cout << "   * 이웃이 없으면 리스트에서 제거" << endl << endl;

        cout << "[ 특징 ]" << endl << endl;

        cout << "장점:" << endl;
        cout << "  * 균형잡힌 미로 생성 (편향이 적음)" << endl;
        cout << "  * 다양한 패턴의 미로 생성 가능" << endl;
        cout << "  * Prim's Algorithm의 변형으로 효율적" << endl;
        cout << "  * 막다른 골목이 적절히 분산됨" << endl << endl;

        cout << "단점:" << endl;
        cout << "  * Binary Tree보다 구현이 복잡함" << endl;
        cout << "  * 활성 셀 리스트 관리를 위한 추가 메모리 필요" << endl;
        cout << "  * 생성 속도가 상대적으로 느림 (O(n log n))" << endl;
        cout << "  * 단순한 생성 과정으로써 구조가 단순함" << endl;

        Sleep(6000);
        break;
    }
    case 3: {
        cout << "=== Recursive Backtracking 알고리즘 ===" << endl << endl;

        cout << "[ 작동 원리 ]" << endl;
        cout << "깊이 우선 탐색(DFS) 방식으로 미로를 생성합니다:" << endl;
        cout << "  * 한 방향으로 끝까지 파고들어가며 길을 만듦" << endl;
        cout << "  * 막다른 곳에 도달하면 이전 위치로 되돌아감(백트래킹)" << endl;
        cout << "  * 모든 경로를 탐색할 때까지 반복" << endl << endl;
        cout << "[ 알고리즘 단계 ]" << endl;
        cout << "1. 시작 셀을 선택하고 스택에 추가" << endl;
        cout << "2. 스택이 비어있지 않는 동안:" << endl;
        cout << "   * 스택의 최상단 셀을 확인 (pop하지 않음)" << endl;
        cout << "   * 현재 셀의 미방문 이웃을 찾음" << endl;
        cout << "   * 이웃이 있으면:" << endl;
        cout << "     - 랜덤하게 이웃 선택" << endl;
        cout << "     - 현재 셀과 이웃 사이의 벽 제거" << endl;
        cout << "     - 이웃을 스택에 추가 (push)" << endl;
        cout << "   * 이웃이 없으면:" << endl;
        cout << "     - 스택에서 현재 셀 제거 (pop)" << endl;
        cout << "     - 이전 셀로 백트래킹" << endl << endl;

        cout << "[ 특징 ]" << endl << endl;

        cout << "장점:" << endl;
        cout << "  * 구현이 직관적이고 이해하기 쉬움" << endl;
        cout << "  * 항상 완벽한 미로 생성 (모든 셀 연결됨)" << endl;
        cout << "  * 긴 복도와 구불구불한 경로가 많음" << endl;
        cout << "  * 메모리 효율적 (스택만 사용)" << endl << endl;
        cout << "단점:" << endl;
        cout << "  * 한 방향으로 편향된 경로 생성" << endl;
        cout << "  * 막다른 골목이 많이 생성됨" << endl;
        cout << "  * 생성 과정이 예측 가능함 (패턴 반복)" << endl;
        cout << "  * 시작점에서 먼 곳일수록 복잡도 증가" << endl;

        Sleep(6000);
        break;
    }
    case 4:
    {
        cout << "개똥" << endl;
    }
    }
}