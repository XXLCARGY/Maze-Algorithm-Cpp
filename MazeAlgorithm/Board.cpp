#include "Board.h"
#include <cstdlib>
#include <ctime>
#include <queue>
#include <utility>
#include <assert.h>

using namespace std;
void SetCursorPosition(int x, int y) {
    COORD coord;
    coord.X = x;
    coord.Y = y;
    SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), coord);//�̰��̰Թ�����? �̰Թ�����?
}
//=========================================
void HideCursor() {
    CONSOLE_CURSOR_INFO cursorInfo;
    GetConsoleCursorInfo(GetStdHandle(STD_OUTPUT_HANDLE), &cursorInfo);
    cursorInfo.bVisible = FALSE;
    SetConsoleCursorInfo(GetStdHandle(STD_OUTPUT_HANDLE), &cursorInfo);
}
//=====================A* ���====================
Board::Node::Node(int x, int y, int g, int h, Node* parent)
    : m_x(x), m_y(y), m_g(g), m_h(h), m_parent(parent) {
    m_f = g + h;
}//A*
//=========================================
bool Board::Node::operator>(const Node& other) const {
    return m_f > other.m_f;
}//A*
//====================����ź �Ÿ�=====================
int Board::CalculateH(int x1, int y1, int x2, int y2) {
    return abs(x1 - x2) + abs(y1 - y2);
}//A*
//====================�� �˻�=====================
bool Board::IsValid(int x, int y) {
	if (x < 0 || x >= m_size || y < 0 || y >= m_size)//���� ���
		return false;//false ��ȯ
	if (tile[y][x] == Wall)//����
		return false;// false ��ȯ
	return true;// ��ȿ�� ��ġ
}
//===================�̷� ���� �˰��� ��Ʈ======================
void Board::GenerateBinaryTree(bool showProcess) {
    //�� �ʱ�ȭ x y�� ¦���϶� wall�� ���� else ��ĭ
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
    for (int y = 0; y < m_size; y++) {// ��� ĭ�� �Ҹ��Ҷ�����
        for (int x = 0; x < m_size; x++) {
            if (x % 2 == 0 || y % 2 == 0)//2�� �����°�������
                continue;
            bool canGoRight = (x < m_size - 2);//������ �˻�
            bool canGoDown = (y < m_size - 2);
            if (!canGoRight && !canGoDown)
                continue;
            else if (!canGoRight)
                tile[y + 1][x] = Empty;//�������
            else if (!canGoDown)
                tile[y][x + 1] = Empty;//�������
            else {
                if (rand() % 2 == 0)//�Ѵ� �����ϸ� ����
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

    bool visited[50][50] = { false };//������ ����ȭ
    int startX = 1, startY = 1;//���� ��������
    tile[startY][startX] = Empty;//������ ����
    visited[startY][startX] = true;// ���ۿ� �湮 ǥ�ø� �ϸ�
    vector<pair<int, int>> activeList;//2���� ��ǥ Ȱ��ȭ ����Ʈ�� ���� ���� �Ҵ���
    activeList.push_back({ startX, startY });//�ְ�
    //4���� ����
    int dx[] = { 0, 0, -2, 2 };
    int dy[] = { -2, 2, 0, 0 };

    while (!activeList.empty()) {//��� Ȱ��ȭ ����Ʈ�� �����������
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
            activeList.erase(activeList.begin() + idx);//����Ʈ ����
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

    bool visited[50][50] = { false };//��� �湮 ���߰�
    int startX = 1, startY = 1;//���� ��������
    tile[startY][startX] = Empty;//������ ����
    visited[startY][startX] = true;// ���ۿ� �湮 ǥ�ø� �ϸ�
    vector<pair<int, int>> activeList;//�Ƹ� ����ó�� ������� ������? ��ã���ε� �ڷ� ���ư����Ѵٴ����� �����ؼ� ����
    activeList.push_back({ startX, startY });//�ְ�

    //4���� ����
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

            activeList.push_back({ newX, newY });  // ���ÿ� �߰�

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
                tile[backY][backX] = Back;  // ��Ʈ��ŷ ǥ��
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

    bool visited[50][50] = { false };//��� �湮 ���߰�
    int startX = 1, startY = 1;//���� ��������
    tile[startY][startX] = Empty;//������ ����
    visited[startY][startX] = true;// ���ۿ� �湮 ǥ�ø� �ϸ�

    int curX = startX;
    int curY = startY;

    //4���� ����
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

            curX = newX;  // ���� ��ġ ������Ʈ
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
            //�� �������� ������ ���� �̰� �� �����̶����ΰ�!!!
            for (int y = 1; y < m_size - 1 && !found; y += 2) {
                if (showProcess) {
                    //��ĵ �ð�ȭ 
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
                    
                    // �湮 �� �� �� �߿���
                    if (!visited[y][x]) {
                        // ������ �湮�� ���� �ִ��� üũ
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

                                curX = x;  // �� ������
                                curY = y;
                                found = true;

                                if (showProcess) {
                                    tile[y][x] = Back;  // Hunt ǥ��
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
//=====================�ʱ�ȭ====================
void Board::Initialize(int boardSize, int algorithm, bool showProcess) {
    assert(boardSize % 2&&"���� ������ ������ ����ȭ �ϼ� ����");
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
void Board::PathFindingChoose(int mapsize) {
    int choice;
    cout << "====================================\n";
    cout << "     Maze Finding Algorithms     \n";
    cout << "====================================\n\n";
    cout << "1. Breadthfirstsearch(BFS) Algorithm\n";
    cout << "2. Depthfirstsearch(DFS) Algorithm\n";
    cout << "3. Dijkstra(Still Dev) Algorithm\n";
    cout << "4. Astar(Still Dev) Algorithm\n";
    cout << "0. Exit\n\n";
    cout << "Select algorithm (0-4): ";
    cin >> choice;
    switch (choice)
    {
    case 0: {
        cout << "Goodbye!\n";
    }
    case 1:
        system("cls");
        Algorithm(choice+4);
        Sleep(6000);
        system("cls");
        Breadthfirstsearch(1, 1, mapsize - 2, mapsize - 2);
        break;
    case 2:
        system("cls");
        Algorithm(choice+4);
        Sleep(6000);
        system("cls");
        Depthfirstsearch(1, 1, mapsize - 2, mapsize - 2);
        break;
    case 3:
        system("cls");
        Algorithm(choice+4);
        Sleep(6000);
        system("cls");
        cout << "Still Dev\n";
        break;
    case 4:
        system("cls");
        Algorithm(choice+4);
        Sleep(6000);
        system("cls");
        cout << "Still Dev\n";
        break;
    }
}
//=====================������====================
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
                cout << "\033[44m  \033[0m";// Path�� �Ķ������� ǥ��
            else if (tile[y][x] == Loot)
                cout << "\033[43m  \033[0m";// Loot�� ��������� ǥ��
            else if (tile[y][x] == Back)
                cout << "\033[41m  \033[0m";// Back�� ���������� ǥ��
            else if (tile[y][x] == Scan)
                cout << "\033[42m  \033[0m";// Scan�� �ʷϻ����� ǥ��
            else
                cout << "\033[40m  \033[0m";
        }
        cout << endl;
    }
}
//====================�̷� Ž��=====================
void Board::Breadthfirstsearch(int sX, int sY, int gX, int gY) {
    bool** visited = new bool* [m_size];//Initialize���� ������ ���� ����
    pair<int, int>** parent = new pair<int, int>* [m_size];

    for (int i = 0; i < m_size; i++) {
        visited[i] = new bool[m_size]();  // () ���̸� false�� �ʱ�ȭ
        parent[i] = new pair<int, int>[m_size];
        for (int j = 0; j < m_size; j++) {
            parent[i][j] = { -1, -1 };
        }
    }

    queue<pair<int, int>> q;//ť ���� ���� ���� //BFS�� ���̴� queue(���Լ���). pair<int,int>���� (x, y)�� ����ϴ�.

    q.push({ sX, sY });//�������� ť �ֱ�
    visited[sY][sX] = true;//�湮 ǥ��

    // 4���� �̵�
    int dx[] = { 0, 0, -1, 1 };
    int dy[] = { -1, 1, 0, 0 };

    // ���� �˰���
	bool found = false;//������ ���� ���� �ʱ�ȭ
	while (!q.empty() && !found) {//ť�� ������� �ʰ� �������� �������� �ʾ����� ��� �ݺ�
		int curX = q.front().first;//ť���� ���� ��ġ ������
        int curY = q.front().second;
		q.pop();//ť���� ���� ��ġ ������

        // �������� �����ߴ��� Ȯ��
        if (curX == gX && curY == gY) {
            found = true;
            break;
        }

        // 4���� Ž��
        for (int i = 0; i < 4; i++) {
			int newX = curX + dx[i];//���� ��ġ���� 4�������� �̵��� ��ġ ���
            int newY = curY + dy[i];

            // ��ȿ�� ��ġ�̰� �湮���� �ʾ����� ���� �ƴ� ���
			if (IsValid(newX, newY) && !visited[newY][newX]) {//���� ��ȿ�� ��ġ�̰� �湮���� �ʾ����� ���� �ƴ� ���
				visited[newY][newX] = true;//�湮 ǥ��
				parent[newY][newX] = { curX, curY };//�θ� ��� ���
				q.push({ newX, newY });//ť�� �� ��ġ �߰�
				tile[newY][newX] = Path;//��� ǥ��
                SetCursorPosition(0, 0);
                Render();
            }
        }
    }
	//�״ϱ� �� �˻��ϰ� ���ƴ�? �ٷ� let's go �ϰ� ť�� �ְ� �湮ǥ���ϰ� �θ��� ����ϰ� ���ǥ��
    // ��ΰ� �߰ߵ� ��� �������Ͽ� ��� ǥ��
    if (found) {
		int curX = gX;//���� ��ġ�� ��ǥ �������� ����
        int curY = gY;

        // ���������� ������
		while (!(curX == sX && curY == sY)) {//���� ��ġ�� �������� �ƴ� ������ �ݺ�
			if (!(curX == gX && curY == gY) && !(curX == sX && curY == sY)) {//���� ��ġ�� �������̳� ��ǥ���� �ƴ� ��
				tile[curY][curX] = Loot;//��� ǥ��
            }
            //�θ� ��� ��ǥ ��������
			int prevX = parent[curY][curX].first;//���� ��ǥ�� �θ� ��� ��ǥ ����
			int prevY = parent[curY][curX].second;
            curX = prevX;
            curY = prevY;
            Sleep(1);
            SetCursorPosition(0, 0);
            Render();
        }
        system("cls");
        SetCursorPosition(0, 0);
        Render();
        cout << "\n��θ� ã�ҽ��ϴ�!" << endl;
    }
    else {
        system("cls");
        SetCursorPosition(0, 0);
        Render();
        cout << "\n��θ� ã�� �� �����ϴ�!" << endl;
    }

    //�޸� ����
    for (int i = 0; i < m_size; i++) {
        delete[] visited[i];
        delete[] parent[i];
    }
    delete[] visited;
    delete[] parent;
}
//=========================================
void Board::Depthfirstsearch(int sX, int sY, int gX, int gY) {
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
            tile[curY][curX] = Path;  // �������� ǥ��
            break;
        }

        vector<int> validDirections;
        for (int i = 0; i < 4; i++) {
            int newX = curX + dx[i];
            int newY = curY + dy[i];

            if (IsValid(newX, newY) && !visited[newY][newX]) {
                validDirections.push_back(i);
            }
        }

        if (!validDirections.empty()) {
            int dir = validDirections[0];
            int newX = curX + dx[dir];
            int newY = curY + dy[dir];

            visited[newY][newX] = true;
            Stack.push_back({ newX, newY });

            tile[newY][newX] = Path;  // ��� ǥ��
            SetCursorPosition(0, 0);
            Render();
            Sleep(10);
        }
        else {
            int backX = Stack.back().first;
            int backY = Stack.back().second;
            Stack.pop_back();

            // ������/�������� �ƴϸ� ��Ʈ��ŷ ǥ��
            if (!(backX == sX && backY == sY) && !(backX == gX && backY == gY)) {
                tile[backY][backX] = Back;
                SetCursorPosition(0, 0);
                Render();
                Sleep(50);
            }
        }
    }

    if (found) {
        system("cls");
        SetCursorPosition(0, 0);
        Render();
        cout << "\n��θ� ã�ҽ��ϴ�!" << endl;
    }
    else {
        system("cls");
        SetCursorPosition(0, 0);
        Render();
        cout << "\n��θ� ã�� �� �����ϴ�!" << endl;
    }
}
//====================�˰��� ����=====================
void Board::Algorithm(int choice) {
    
    switch (choice) {
    case 1: {
        cout << "=== ���� Ʈ�� �̷� �˰��� ===" << endl << endl;

        cout << "[ �۵� ���� ]" << endl;
        cout << "�� ��(ĭ)���� �� ���� �� �ϳ��� �����ؼ� ��θ� ����ϴ�:" << endl;
        cout << "  * �Ϲ������� ����(��) �Ǵ� ����(������) �� �ϳ��� �����ϰ� ����" << endl;
        cout << "  * ������ ������ ���� �����Ͽ� ��� ����" << endl << endl;

        cout << "[ �˰��� �ܰ� ]" << endl;
        cout << "1. �׸����� ��� ���� ��ȸ�մϴ�" << endl;
        cout << "2. �� ������:" << endl;
        cout << "   * ���ʰ� ���� ������ ��� �����ϸ� �� �� �� �ϳ��� ���� ����" << endl;
        cout << "   * �� ���⸸ �����ϸ� �� �� ���� ����" << endl;
        cout << "   * �� �� �Ұ����ϸ� (�����ڸ� �ڳ�) �� �ƹ��͵� �� ��" << endl << endl;

        cout << "[ Ư¡ ]" << endl << endl;

        cout << "����:" << endl;
        cout << "  * ������ �ſ� ������" << endl;
        cout << "  * ���� ���� �ӵ� (O(n))" << endl;
        cout << "  * �޸� ȿ����" << endl << endl;

        cout << "����:" << endl;
        cout << "  * ��Ȯ�� ����(bias)�� ���� - ���� �������� ġ��ģ ����" << endl;
        cout << "  * �ϵ��� �ڳʰ� �׻� ���ٸ� ����� ��" << endl;
        cout << "  * �̷ΰ� �ʹ� �ܼ��ϰ� ���� ������" << endl;
        cout << "  * �� ���� ������ ���� ������" << endl;
        Sleep(6000);
        break;
    }
    case 2: {
        cout << "=== Growing Binary Tree �˰��� ===" << endl << endl;

        cout << "[ �۵� ���� ]" << endl;
        cout << "Ʈ���� �����ϵ��� �̷θ� Ȯ���� ������ ����Դϴ�:" << endl;
        cout << "  * ���� ���������� �����ϰ� ������ ���� Ȯ��" << endl;
        cout << "  * �湮���� ���� ���� ���� ������ Ʈ���� �߰�" << endl;
        cout << "  * �� �̻� Ȯ���� �� ������ ��Ʈ��ŷ" << endl << endl;

        cout << "[ �˰��� �ܰ� ]" << endl;
        cout << "1. ���� ���� �����ϰ� Ȱ�� �� ����Ʈ�� �߰�" << endl;
        cout << "2. Ȱ�� �� ����Ʈ�� ������� �ʴ� ����:" << endl;
        cout << "   * ����Ʈ���� �����ϰ� �� ����" << endl;
        cout << "   * ������ ���� �̹湮 �̿��� ã��" << endl;
        cout << "   * �̿��� ������:" << endl;
        cout << "     - �����ϰ� �̿� ����" << endl;
        cout << "     - ���� ���� �̿� ������ �� ����" << endl;
        cout << "     - �̿��� Ȱ�� ����Ʈ�� �߰�" << endl;
        cout << "   * �̿��� ������ ����Ʈ���� ����" << endl << endl;

        cout << "[ Ư¡ ]" << endl << endl;

        cout << "����:" << endl;
        cout << "  * �������� �̷� ���� (������ ����)" << endl;
        cout << "  * �پ��� ������ �̷� ���� ����" << endl;
        cout << "  * Prim's Algorithm�� �������� ȿ����" << endl;
        cout << "  * ���ٸ� ����� ������ �л��" << endl << endl;

        cout << "����:" << endl;
        cout << "  * Binary Tree���� ������ ������" << endl;
        cout << "  * Ȱ�� �� ����Ʈ ������ ���� �߰� �޸� �ʿ�" << endl;
        cout << "  * ���� �ӵ��� ��������� ���� (O(n log n))" << endl;
        cout << "  * �ܼ��� ���� �������ν� ������ �ܼ���" << endl;

        Sleep(6000);
        break;
    }
    case 3: {
        cout << "=== Recursive Backtracking �˰��� ===" << endl << endl;

        cout << "[ �۵� ���� ]" << endl;
        cout << "���� �켱 Ž��(DFS) ������� �̷θ� �����մϴ�:" << endl;
        cout << "  * �� �������� ������ �İ���� ���� ����" << endl;
        cout << "  * ���ٸ� ���� �����ϸ� ���� ��ġ�� �ǵ��ư�(��Ʈ��ŷ)" << endl;
        cout << "  * ��� ��θ� Ž���� ������ �ݺ�" << endl << endl;
        cout << "[ �˰��� �ܰ� ]" << endl;
        cout << "1. ���� ���� �����ϰ� ���ÿ� �߰�" << endl;
        cout << "2. ������ ������� �ʴ� ����:" << endl;
        cout << "   * ������ �ֻ�� ���� Ȯ�� (pop���� ����)" << endl;
        cout << "   * ���� ���� �̹湮 �̿��� ã��" << endl;
        cout << "   * �̿��� ������:" << endl;
        cout << "     - �����ϰ� �̿� ����" << endl;
        cout << "     - ���� ���� �̿� ������ �� ����" << endl;
        cout << "     - �̿��� ���ÿ� �߰� (push)" << endl;
        cout << "   * �̿��� ������:" << endl;
        cout << "     - ���ÿ��� ���� �� ���� (pop)" << endl;
        cout << "     - ���� ���� ��Ʈ��ŷ" << endl << endl;

        cout << "[ Ư¡ ]" << endl << endl;

        cout << "����:" << endl;
        cout << "  * ������ �������̰� �����ϱ� ����" << endl;
        cout << "  * �׻� �Ϻ��� �̷� ���� (��� �� �����)" << endl;
        cout << "  * �� ������ ���ұ����� ��ΰ� ����" << endl;
        cout << "  * �޸� ȿ���� (���ø� ���)" << endl << endl;
        cout << "����:" << endl;
        cout << "  * �� �������� ����� ��� ����" << endl;
        cout << "  * ���ٸ� ����� ���� ������" << endl;
        cout << "  * ���� ������ ���� ������ (���� �ݺ�)" << endl;
        cout << "  * ���������� �� ���ϼ��� ���⵵ ����" << endl;

        Sleep(6000);
        break;
    }
    case 4:
    {
        cout << "=== Hunt and Kill �˰��� ===" << endl << endl;
        cout << "[ �۵� ���� ]" << endl;
        cout << "�� ���� ��带 ������ ����Ͽ� �̷θ� �����մϴ�:" << endl;
        cout << "  * Hunt ���: �湮�� ���� ������ �̹湮 �� Ž��" << endl;
        cout << "  * Kill ���: �߰��� ������ ���� ��ũ ����" << endl;
        cout << "  * ��� ���� �湮�� ������ �ݺ�" << endl << endl;
        cout << "[ �˰��� �ܰ� ]" << endl;
        cout << "1. ���� ���� �����ϰ� �湮 ǥ��" << endl;
        cout << "2. Kill ��� - ���� ��ũ ����:" << endl;
        cout << "   * ���� ���� �̹湮 �̿��� ã��" << endl;
        cout << "   * �̿��� ������:" << endl;
        cout << "     - �����ϰ� �̿� ����" << endl;
        cout << "     - ���� ���� �̿� ������ �� ����" << endl;
        cout << "     - �̿��� ���� ���� �����ϰ� �ݺ�" << endl;
        cout << "   * �̿��� ������ Hunt ���� ��ȯ" << endl;
        cout << "3. Hunt ��� - ���ο� ������ Ž��:" << endl;
        cout << "   * �̷θ� ���������� ��ĵ" << endl;
        cout << "   * �̹湮 �� �� �湮�� ���� ������ �� �߰�" << endl;
        cout << "   * �߰��� ���� ���� �� ������ �� ����" << endl;
        cout << "   * Kill ���� ��ȯ�Ͽ� �ٽ� ���� ��ũ ����" << endl << endl;
        cout << "[ Ư¡ ]" << endl << endl;
        cout << "����:" << endl;
        cout << "  * �� ������ ���ұ����� ��� ����" << endl;
        cout << "  * ���ٸ� ����� ���� �帧�� �ڿ�������" << endl;
        cout << "  * �߰� �޸� ���ʿ� (�����̳� ť ���ʿ�)" << endl;
        cout << "  * �׻� �Ϻ��� �̷� ���� (��� �� �����)" << endl << endl;
        cout << "����:" << endl;
        cout << "  * Hunt �ܰ迡�� ��ü �̷� ��ĵ �ʿ�" << endl;
        cout << "  * ū �̷ο��� ���� ���� ���� (O(n��))" << endl;
        cout << "  * ������ �ټ� ������" << endl;
        cout << "  * ���� �ӵ��� �ٸ� �˰��򺸴� ����" << endl;
        Sleep(6000);
        break;
    }
    case 5: {
        cout << "=== BFS (�ʺ� �켱 Ž��) �˰��� ===" << endl << endl;

        cout << "[ �۵� ���� ]" << endl;
        cout << "���������� ����� ������ ���������� Ž���մϴ�:" << endl;
        cout << "  * ���������� 1ĭ �Ÿ� �� 2ĭ �Ÿ� �� 3ĭ �Ÿ� ������ Ž��" << endl;
        cout << "  * ť(Queue)�� ����Ͽ� Ž�� ���� ����" << endl;
        cout << "  * �ִ� ��θ� �����ϴ� �˰���" << endl << endl;

        cout << "[ �˰��� �ܰ� ]" << endl;
        cout << "1. �������� ť�� �ְ� �湮 ǥ��" << endl;
        cout << "2. ť���� ��ġ�� �ϳ� ���� (���Լ���)" << endl;
        cout << "3. ���� ��ġ�� ���������� Ȯ��:" << endl;
        cout << "   * ��������� Ž�� ����" << endl;
        cout << "   * �ƴ϶�� ���� �ܰ� ����" << endl;
        cout << "4. ���� ��ġ�� �����¿� 4���� Ž��:" << endl;
        cout << "   * ��ȿ�� ��ġ���� Ȯ�� (���� ��, �� �ƴ�)" << endl;
        cout << "   * �̹湮 ��ġ���:" << endl;
        cout << "     - �湮 ǥ��" << endl;
        cout << "     - �θ� ��� ��� (��������)" << endl;
        cout << "     - ť�� �߰�" << endl;
        cout << "5. ť�� �� ������ 2~4�ܰ� �ݺ�" << endl;
        cout << "6. ������ ���� �� �θ� ��带 ���� �������Ͽ� ��� ǥ��" << endl << endl;

        cout << "[ Ư¡ ]" << endl << endl;
        cout << "����:" << endl;
        cout << "  * �׻� �ִ� ��θ� ã�� (����ġ ���� �׷���)" << endl;
        cout << "  * ������ ���� ������" << endl;
        cout << "  * ��� ��θ� �����ϰ� Ž��" << endl;
        cout << "  * ��� ���� ���θ� Ȯ���� �Ǵ� ����" << endl << endl;
        cout << "����:" << endl;
        cout << "  * �޸� ��뷮�� ���� (ť, �湮 �迭, �θ� �迭)" << endl;
        cout << "  * �������� �ָ� ������ ���ʿ��� Ž�� ����" << endl;
        cout << "  * �ð� ���⵵ O(V+E) - ��� ������ ���� Ž��" << endl;
        cout << "  * ����ġ�� �ִ� �׷��������� �ִ� ��� ���� �� ��" << endl;

        
        break;
    }
    case 6: {
        cout << "=== DFS (���� �켱 Ž��) �˰��� ===" << endl << endl;

        cout << "[ �۵� ���� ]" << endl;
        cout << "�� �������� ������ Ž���� �� �ǵ��ƿ� �ٸ� ��θ� Ž���մϴ�:" << endl;
        cout << "  * ���ٸ� �濡 ������ ������ ��� ����" << endl;
        cout << "  * ����(Stack) �Ǵ� ��͸� ����Ͽ� Ž��" << endl;
        cout << "  * ��Ʈ��ŷ�� ���� �ٸ� ��� Ž��" << endl << endl;

        cout << "[ �˰��� �ܰ� ]" << endl;
        cout << "1. �������� ���ÿ� �ְ� �湮 ǥ��" << endl;
        cout << "2. ���ÿ��� ��ġ�� �ϳ� ���� (���Լ���)" << endl;
        cout << "3. ���� ��ġ�� ���������� Ȯ��:" << endl;
        cout << "   * ��������� Ž�� ����" << endl;
        cout << "   * �ƴ϶�� ���� �ܰ� ����" << endl;
        cout << "4. ���� ��ġ�� �����¿� 4���� Ž��:" << endl;
        cout << "   * ��ȿ�� ��ġ���� Ȯ�� (���� ��, �� �ƴ�)" << endl;
        cout << "   * �̹湮 ��ġ���:" << endl;
        cout << "     - �湮 ǥ��" << endl;
        cout << "     - �θ� ��� ��� (��������)" << endl;
        cout << "     - ���ÿ� �߰�" << endl;
        cout << "5. ������ �� ������ 2~4�ܰ� �ݺ�" << endl;
        cout << "6. ������ ���� �� �θ� ��带 ���� �������Ͽ� ��� ǥ��" << endl << endl;

        cout << "[ Ư¡ ]" << endl << endl;
        cout << "����:" << endl;
        cout << "  * �޸� ��뷮�� ���� (���� ��θ� ����)" << endl;
        cout << "  * ������ ������ (��� ��� �� ���� ����)" << endl;
        cout << "  * �������� ���� ���� ���� �� ������ �߰� ����" << endl;
        cout << "  * ��Ʈ��ŷ �˰��� ����" << endl << endl;
        cout << "����:" << endl;
        cout << "  * �ִ� ��θ� �������� ����" << endl;
        cout << "  * ���� ������ ���� ���� (��ȯ �׷���)" << endl;
        cout << "  * �������� ���� ���� �־ ���� Ž�� ���� ����" << endl;
        cout << "  * �ð� ���⵵ O(V+E) - �־��� ��� ��� ���� �湮" << endl;

        break;
    }
    }
}