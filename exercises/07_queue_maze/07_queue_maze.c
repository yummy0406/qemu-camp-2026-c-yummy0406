#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_ROW 5
#define MAX_COL 5
#define MAX_QUEUE 100
#define MAX_PATH 100

int maze[MAX_ROW][MAX_COL] = {
    0, 1, 0, 0, 0, 0, 1, 0, 1, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 0, 0, 0, 0, 1, 0,
};

typedef struct {
  int row;
  int col;
} Position;

typedef struct {
  Position items[MAX_QUEUE];
  int front;
  int rear;
} Queue;

// 队列操作函数
void queue_init(Queue *q) {
  q->front = 0;
  q->rear = 0;
}

void queue_enqueue(Queue *q, int row, int col) {
  if (q->rear < MAX_QUEUE) {
    q->items[q->rear].row = row;
    q->items[q->rear].col = col;
    q->rear++;
  }
}

int queue_dequeue(Queue *q, int *row, int *col) {
  if (q->front < q->rear) {
    *row = q->items[q->front].row;
    *col = q->items[q->front].col;
    q->front++;
    return 1;
  }
  return 0;
}

int queue_is_empty(Queue *q) { return q->front >= q->rear; }

// 迷宫求解函数（BFS）
void solve_maze(void) {
  Queue queue;
  int visited[MAX_ROW][MAX_COL];
  Position parent[MAX_ROW][MAX_COL];
  Position path[MAX_PATH];
  int path_index = 0;

  // 初始化
  queue_init(&queue);
  memset(visited, 0, sizeof(visited));
  memset(parent, -1, sizeof(parent));

  // 从起点(0,0)开始BFS
  queue_enqueue(&queue, 0, 0);
  visited[0][0] = 1;
  parent[0][0].row = -1;
  parent[0][0].col = -1;

  // 四个方向：上、下、左、右
  int directions[4][2] = {
      {-1, 0}, // 上
      {1, 0},  // 下
      {0, -1}, // 左
      {0, 1}   // 右
  };

  int target_found = 0;

  // BFS 遍历
  while (!queue_is_empty(&queue)) {
    int row, col;
    queue_dequeue(&queue, &row, &col);

    // 检查是否到达目标(4,4)
    if (row == MAX_ROW - 1 && col == MAX_COL - 1) {
      target_found = 1;
      break;
    }

    // 尝试四个方向
    for (int i = 0; i < 4; i++) {
      int new_row = row + directions[i][0];
      int new_col = col + directions[i][1];

      // 检查边界和有效性
      if (new_row >= 0 && new_row < MAX_ROW && new_col >= 0 &&
          new_col < MAX_COL && maze[new_row][new_col] == 0 &&
          !visited[new_row][new_col]) {

        queue_enqueue(&queue, new_row, new_col);
        visited[new_row][new_col] = 1;
        parent[new_row][new_col].row = row;
        parent[new_row][new_col].col = col;
      }
    }
  }

  if (target_found) {
    printf("Path found!\n");

    // 从目标点回溯到起点，构建路径
    int row = MAX_ROW - 1;
    int col = MAX_COL - 1;

    while (row != -1 && col != -1 && path_index < MAX_PATH) {
      path[path_index++] = (Position){row, col};
      int temp_row = parent[row][col].row;
      int temp_col = parent[row][col].col;
      row = temp_row;
      col = temp_col;
    }

    // 从终点到起点输出路径
    for (int i = 0; i < path_index; i++) {
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