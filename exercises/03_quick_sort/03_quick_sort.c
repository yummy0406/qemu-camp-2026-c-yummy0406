#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_STUDENTS 100
#define NAME_LEN 50

typedef struct {
  char name[NAME_LEN];
  int score;
} Student;

Student students[MAX_STUDENTS];

void quick_sort(int left, int right) {
  if (left >= right) {
    return;
  }

  Student pivot = students[right]; // 选择最后一个元素作为基准
  int i = left - 1;

  for (int j = left; j < right; j++) {
    if (students[j].score >= pivot.score) { // 按成绩从高到低排序
      i++;
      Student temp = students[i];
      students[i] = students[j];
      students[j] = temp;
    }
  }

  // 将基准元素放到正确的位置
  Student temp = students[i + 1];
  students[i + 1] = students[right];
  students[right] = temp;

  // 递归排序左右两部分
  quick_sort(left, i);
  quick_sort(i + 2, right);
}

int main(void) {
  FILE *file = fopen("03_students.txt", "r");
  if (!file) {
    printf("错误：无法打开文件 03_students.txt\n");
    return 1;
  }

  int n;
  fscanf(file, "%d", &n);

  if (n <= 0 || n > MAX_STUDENTS) {
    printf("学生人数无效：%d（应为 1-%d）\n", n, MAX_STUDENTS);
    fclose(file);
    return 1;
  }

  for (int i = 0; i < n; i++) {
    fscanf(file, "%s %d", students[i].name, &students[i].score);
  }
  fclose(file);

  quick_sort(0, n - 1);

  // 输出结果
  printf("\n快速排序后按成绩从高到低排序的学生名单：\n");
  for (int i = 0; i < n; i++) {
    printf("%s %d\n", students[i].name, students[i].score);
  }

  return 0;
}