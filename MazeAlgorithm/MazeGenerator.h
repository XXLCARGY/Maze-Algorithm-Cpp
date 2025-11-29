#pragma once
#include "MazeBoard.h"

class MazeGenerator {
public:
    // 외부에서 호출하는 메인 함수
    static void Generate(MazeBoard& board, int algoType, bool showProcess);

private:
    // cpp 파일의 함수명과 대소문자까지 정확히 일치해야 합니다.
    static void GenerateBinaryTree(MazeBoard& board, bool showProcess);
    static void GenerateGrowingBinaryTree(MazeBoard& board, bool showProcess);
    static void GenerateRecursiveBacktracking(MazeBoard& board, bool showProcess);
    static void GenerateHuntAndKill(MazeBoard& board, bool showProcess); // 'a'nd 소문자 주의
    static void GenerateCellularAutomata(MazeBoard& board, bool showProcess);
};