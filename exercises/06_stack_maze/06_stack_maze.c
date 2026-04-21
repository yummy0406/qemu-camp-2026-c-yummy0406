#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_ROW 5
#define MAX_COL 5
#define MAX_PATH 100

int maze[MAX_ROW][MAX_COL] = {
    0, 1, 0, 0, 0, 0, 1, 0, 1, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 0, 0, 0, 0, 1, 0,
};

typedef struct {
  int row;
  int col;
} Position;

int visited[MAX_ROW][MAX_COL];
Position path[MAX_PATH];
int path_index = 0;
int found = 0;

// 四个方向：上、下、左、右
int directions[4][2] = {
    {-1, 0}, // 上
    {1, 0},  // 下
    {0, -1}, // 左
    {0, 1}   // 右
};

// DFS 递归函数
void dfs(int row, int col) {
  // 如果已经找到路径，直接返回
  if (found)
    return;

  // 标记为已访问
  visited[row][col] = 1;

  // 记录当前位置
  path[path_index++] = (Position){row, col};

  // 检查是否到达目标(4,4)
  if (row == MAX_ROW - 1 && col == MAX_COL - 1) {
    found = 1;
    return;
  }

  // 尝试四个方向
  for (int i = 0; i < 4; i++) {
    int new_row = row + directions[i][0];
    int new_col = col + directions[i][1];

    // 检查边界和有效性
    if (new_row >= 0 && new_row < MAX_ROW && new_col >= 0 &&
        new_col < MAX_COL && maze[new_row][new_col] == 0 &&
        !visited[new_row][new_col]) {

      dfs(new_row, new_col);

      if (found)
        return;
    }
  }

  // 回溯：将当前位置从路径中移除
  path_index--;
}

// 迷宫求解函数
void solve_maze(void) {
  memset(visited, 0, sizeof(visited));
  path_index = 0;
  found = 0;

  // 从起点(0,0)开始DFS
  dfs(0, 0);

  if (found) {
    printf("Path found!\n");
    // 从终点到起点输出路径
    for (int i = path_index - 1; i >= 0; i--) {
      printf("(%d, %d)\n", path[i].row, path[i].col);
    }
  } else {
    printf("No path!\n");
  }
}

int main(void) {
  solve_maze();
  return 0;
}