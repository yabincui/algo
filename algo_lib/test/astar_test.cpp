#include "astar.h"

#include <stdio.h>

#include <algorithm>
#include <vector>

#include <gtest/gtest.h>

using namespace algo_lib;
using namespace std;

static void printPath(vector<AstarPoint>& path) {
    for (auto& p : path) {
        printf("(%d, %d) -> ", p.r, p.c);
    }
    printf("\n");
}

TEST(astar, smoke1) {
    vector<vector<bool>> board = {
        {true, true, false},
        {false, true, true},
        {false, false, true},
    };
    Astar star;
    vector<AstarPoint> path = star.getPath(board, AstarPoint(0, 0), AstarPoint(2, 2));
    ASSERT_EQ(5u, path.size());
    vector<AstarPoint> expected_path = {
        AstarPoint(0, 0), AstarPoint(0, 1), AstarPoint(1, 1), AstarPoint(1, 2), AstarPoint(2, 2)
    };
    ASSERT_EQ(path, expected_path);
}

TEST(astar, smoke2) {
    vector<vector<bool>> board = {
        {true, true, true, true},
        {true, false, false, true},
        {true, true, true, true},
        {false, false, true, true},
    };
    Astar star;
    vector<AstarPoint> path = star.getPath(board, AstarPoint(1, 0), AstarPoint(0, 3));
    ASSERT_EQ(5u, path.size());
    vector<AstarPoint> expected_path = {
        AstarPoint(1, 0), AstarPoint(0, 0), AstarPoint(0, 1), AstarPoint(0, 2), AstarPoint(0, 3)
    };
    ASSERT_EQ(path, expected_path);
}

