#include <stdbool.h>
#include <stdio.h>

#define MAX_PEOPLE 50

typedef struct {
  int id;
} People;

typedef struct {
  People data[MAX_PEOPLE];
  int head;
  int tail;
  int count;
} Queue;

// 初始化队列
void queue_init(Queue *q) {
  q->head = 0;
  q->tail = 0;
  q->count = 0;
}

// 入队
void queue_push(Queue *q, People p) {
  if (q->count < MAX_PEOPLE) {
    q->data[q->tail] = p;
    q->tail = (q->tail + 1) % MAX_PEOPLE;
    q->count++;
  }
}

// 出队
People queue_pop(Queue *q) {
  People p = q->data[q->head];
  q->head = (q->head + 1) % MAX_PEOPLE;
  q->count--;
  return p;
}

// 获取队列大小
int queue_size(Queue *q) { return q->count; }

int main() {
  Queue q;
  int total_people = 50;
  int report_interval = 5;

  // 初始化队列，加入所有的人
  queue_init(&q);
  for (int i = 1; i <= total_people; i++) {
    People p = {i};
    queue_push(&q, p);
  }

  // 模拟约瑟夫环
  int count = 0;
  while (queue_size(&q) > 1) {
    People p = queue_pop(&q); // 取出一个人
    count++;

    if (count == report_interval) {
      // 达到报数间隔，淘汰这个人
      printf("淘汰: %d\n", p.id);
      count = 0; // 重置计数
    } else {
      // 没有达到间隔，放回队列
      queue_push(&q, p);
    }
  }

  // 输出最后剩下的人
  printf("最后剩下的人是: %d\n", q.data[q.head].id);

  return 0;
}