#include "Board.h"
#include <cstdlib>
#include <ctime>
#include <queue>
#include <utility>
#include <assert.h>

//�ɹ� ������ ���� ���� ������ �ǰ� �صα�
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
	if (x < 0 || x >= m_size || y < 0 || y >= m_size)//���� ���
		return false;//false ��ȯ
	if (tile[y][x] == Wall)//����
		return false;// false ��ȯ
	return true;// ��ȿ�� ��ġ
}
//=========================================
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
//=========================================
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
                cout << "\033[42m  \033[0m";
            else
                cout << "\033[40m  \033[0m";
        }
        cout << endl;
    }
}
//=========================================
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

        cout << "\n��θ� ã�ҽ��ϴ�!" << endl;
    }
    else {
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
        cout << "����" << endl;
    }
    }
}