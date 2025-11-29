#include "TextManager.h"
#include <iostream>
#include <windows.h> // Sleep 함수용

using namespace std;

void TextManager::PrintAlgorithmInfo(int choice) {
    int ex_timer = 10; // 원본 코드의 타이머 설정

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
        Sleep(ex_timer);
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

        Sleep(ex_timer);
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

        Sleep(ex_timer);
        break;
    }
    case 4:
    {
        cout << "=== Hunt and Kill 알고리즘 ===" << endl << endl;
        cout << "[ 작동 원리 ]" << endl;
        cout << "두 가지 모드를 번갈아 사용하여 미로를 생성합니다:" << endl;
        cout << "  * Hunt 모드: 방문한 셀에 인접한 미방문 셀 탐색" << endl;
        cout << "  * Kill 모드: 발견한 셀에서 랜덤 워크 시작" << endl;
        cout << "  * 모든 셀을 방문할 때까지 반복" << endl << endl;
        cout << "[ 알고리즘 단계 ]" << endl;
        cout << "1. 시작 셀을 선택하고 방문 표시" << endl;
        cout << "2. Kill 모드 - 랜덤 워크 수행:" << endl;
        cout << "   * 현재 셀의 미방문 이웃을 찾음" << endl;
        cout << "   * 이웃이 있으면:" << endl;
        cout << "     - 랜덤하게 이웃 선택" << endl;
        cout << "     - 현재 셀과 이웃 사이의 벽 제거" << endl;
        cout << "     - 이웃을 현재 셀로 설정하고 반복" << endl;
        cout << "   * 이웃이 없으면 Hunt 모드로 전환" << endl;
        cout << "3. Hunt 모드 - 새로운 시작점 탐색:" << endl;
        cout << "   * 미로를 순차적으로 스캔" << endl;
        cout << "   * 미방문 셀 중 방문한 셀과 인접한 셀 발견" << endl;
        cout << "   * 발견한 셀과 인접 셀 사이의 벽 제거" << endl;
        cout << "   * Kill 모드로 전환하여 다시 랜덤 워크 시작" << endl << endl;
        cout << "[ 특징 ]" << endl << endl;
        cout << "장점:" << endl;
        cout << "  * 긴 복도와 구불구불한 경로 생성" << endl;
        cout << "  * 막다른 골목이 적고 흐름이 자연스러움" << endl;
        cout << "  * 추가 메모리 불필요 (스택이나 큐 불필요)" << endl;
        cout << "  * 항상 완벽한 미로 생성 (모든 셀 연결됨)" << endl << endl;
        cout << "단점:" << endl;
        cout << "  * Hunt 단계에서 전체 미로 스캔 필요" << endl;
        cout << "  * 큰 미로에서 성능 저하 가능 (O(n²))" << endl;
        cout << "  * 구현이 다소 복잡함" << endl;
        cout << "  * 생성 속도가 다른 알고리즘보다 느림" << endl;
        Sleep(ex_timer);
        break;
    }
    case 5: {
        cout << "=== Cellular Automata(세포자동자) 알고리즘 ===" << endl << endl;
        cout << "[ 작동 원리 ]" << endl;
        cout << "무작위로 초기화된 그리드에서 시작하여 규칙에 따라 셀 상태를 반복적으로 업데이트합니다:" << endl;
        cout << "  * 각 셀은 벽 또는 빈 공간 상태를 가짐" << endl;
        cout << "  * 주변 셀의 상태에 따라 현재 셀의 상태 결정" << endl;
        cout << "  * 여러 세대에 걸쳐 그리드가 진화하며 미로 형태 생성" << endl << endl;
        cout << "[ 알고리즘 단계 ]" << endl;
        cout << "1. 그리드를 무작위로 초기화 (벽과 빈 공간 혼합)" << endl;
        cout << "2. 여러 세대에 걸쳐 다음 규칙 적용:" << endl;
        cout << "   * 각 셀에 대해 주변 8칸의 벽 개수 계산" << endl;
        cout << "   * 벽 셀인 경우:" << endl;
        cout << "     - 주변 벽이 4개 이상이면 벽 유지" << endl;
        cout << "     - 그렇지 않으면 빈 공간으로 변경" << endl;
        cout << "   * 빈 공간 셀인 경우:" << endl;
        cout << "     - 주변 벽이 5개 이상이면 벽으로 변경" << endl;
        cout << "     - 그렇지 않으면 빈 공간 유지" << endl << endl;
        cout << "[ 특징 ]" << endl << endl;
        cout << "장점:" << endl;
        cout << "  * 자연스럽고 유기적인 미로 형태 생성" << endl;
        cout << "  * 구현이 비교적 간단함" << endl;
        cout << "  * 다양한 패턴과 구조 생성 가능" << endl;
        cout << "  * 매개변수 조정을 통해 미로 특성 변경 가능" << endl << endl;
        cout << "단점:" << endl;
        cout << "  * 초기 무작위 상태에 민감함" << endl;
        cout << "나는 그냥 만들고 싶게 생겨서 만든거임" << endl;
        Sleep(ex_timer);
        break;

    }
    case 6: {
        cout << "===주인장의 Blank test world===" << endl << endl;
        break;
    }
    case 7: {
        cout << "===주인장의 Custom Maze===" << endl << endl;
        break;
    }
    case 8: { // 원본 코드에서 BFS 선택 시 (choice + 7)로 호출되던 부분
        cout << "=== BFS (너비 우선 탐색) 알고리즘 ===" << endl << endl;

        cout << "[ 작동 원리 ]" << endl;
        cout << "시작점에서 가까운 곳부터 순차적으로 탐색합니다:" << endl;
        cout << "  * 시작점에서 1칸 거리 → 2칸 거리 → 3칸 거리 순으로 탐색" << endl;
        cout << "  * 큐(Queue)를 사용하여 탐색 순서 관리" << endl;
        cout << "  * 최단 경로를 보장하는 알고리즘" << endl << endl;

        cout << "[ 알고리즘 단계 ]" << endl;
        cout << "1. 시작점을 큐에 넣고 방문 표시" << endl;
        cout << "2. 큐에서 위치를 하나 꺼냄 (선입선출)" << endl;
        cout << "3. 현재 위치가 목적지인지 확인:" << endl;
        cout << "   * 목적지라면 탐색 종료" << endl;
        cout << "   * 아니라면 다음 단계 진행" << endl;
        cout << "4. 현재 위치의 상하좌우 4방향 탐색:" << endl;
        cout << "   * 유효한 위치인지 확인 (범위 내, 벽 아님)" << endl;
        cout << "   * 미방문 위치라면:" << endl;
        cout << "     - 방문 표시" << endl;
        cout << "     - 부모 노드 기록 (역추적용)" << endl;
        cout << "     - 큐에 추가" << endl;
        cout << "5. 큐가 빌 때까지 2~4단계 반복" << endl;
        cout << "6. 목적지 도달 시 부모 노드를 따라 역추적하여 경로 표시" << endl << endl;

        cout << "[ 특징 ]" << endl << endl;
        cout << "장점:" << endl;
        cout << "  * 항상 최단 경로를 찾음 (가중치 없는 그래프)" << endl;
        cout << "  * 구현이 비교적 간단함" << endl;
        cout << "  * 모든 경로를 공평하게 탐색" << endl;
        cout << "  * 경로 존재 여부를 확실히 판단 가능" << endl << endl;
        cout << "단점:" << endl;
        cout << "  * 메모리 사용량이 많음 (큐, 방문 배열, 부모 배열)" << endl;
        cout << "  * 목적지가 멀리 있으면 불필요한 탐색 증가" << endl;
        cout << "  * 시간 복잡도 O(V+E) - 모든 정점과 간선 탐색" << endl;
        cout << "  * 가중치가 있는 그래프에서는 최단 경로 보장 안 됨" << endl;
        Sleep(ex_timer);
        break;
    }
    case 9: { // 원본 코드에서 DFS 선택 시 호출되던 부분
        cout << "=== DFS (깊이 우선 탐색) 알고리즘 ===" << endl << endl;

        cout << "[ 작동 원리 ]" << endl;
        cout << "한 방향으로 끝까지 탐색한 후 되돌아와 다른 경로를 탐색합니다:" << endl;
        cout << "  * 막다른 길에 도달할 때까지 계속 진행" << endl;
        cout << "  * 스택(Stack) 또는 재귀를 사용하여 탐색" << endl;
        cout << "  * 백트래킹을 통해 다른 경로 탐색" << endl << endl;

        cout << "[ 알고리즘 단계 ]" << endl;
        cout << "1. 시작점을 스택에 넣고 방문 표시" << endl;
        cout << "2. 스택에서 위치를 하나 꺼냄 (후입선출)" << endl;
        cout << "3. 현재 위치가 목적지인지 확인:" << endl;
        cout << "   * 목적지라면 탐색 종료" << endl;
        cout << "   * 아니라면 다음 단계 진행" << endl;
        cout << "4. 현재 위치의 상하좌우 4방향 탐색:" << endl;
        cout << "   * 유효한 위치인지 확인 (범위 내, 벽 아님)" << endl;
        cout << "   * 미방문 위치라면:" << endl;
        cout << "     - 방문 표시" << endl;
        cout << "     - 부모 노드 기록 (역추적용)" << endl;
        cout << "     - 스택에 추가" << endl;
        cout << "5. 스택이 빌 때까지 2~4단계 반복" << endl;
        cout << "6. 목적지 도달 시 부모 노드를 따라 역추적하여 경로 표시" << endl << endl;

        cout << "[ 특징 ]" << endl << endl;
        cout << "장점:" << endl;
        cout << "  * 메모리 사용량이 적음 (현재 경로만 저장)" << endl;
        cout << "  * 구현이 간단함 (재귀 사용 시 더욱 간단)" << endl;
        cout << "  * 목적지가 깊은 곳에 있을 때 빠르게 발견 가능" << endl;
        cout << "  * 백트래킹 알고리즘에 적합" << endl << endl;
        cout << "단점:" << endl;
        cout << "  * 최단 경로를 보장하지 않음" << endl;
        cout << "  * 무한 루프에 빠질 위험 (순환 그래프)" << endl;
        cout << "  * 목적지가 얕은 곳에 있어도 깊이 탐색 먼저 진행" << endl;
        cout << "  * 시간 복잡도 O(V+E) - 최악의 경우 모든 정점 방문" << endl;
        Sleep(ex_timer);
        break;
    }
    }
}