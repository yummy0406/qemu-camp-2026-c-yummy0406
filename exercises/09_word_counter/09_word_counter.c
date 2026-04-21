#include <stdio.h>

int main() {
  char str[] = "Don't ask what your country can do for you, but ask what you "
               "can do for your country.";
  int wordCount = 0;

  // 使用状态变量标记是否在单词中
  int inWord = 0;
  for (int i = 0; str[i] != '\0'; i++) {
    if (str[i] != ' ' && str[i] != '\n' && !inWord) {
      // 进入单词状态
      inWord = 1;
      wordCount++;
    } else if (str[i] == ' ') {
      // 退出单词状态
      inWord = 0;
    }
  }

  printf("单词数量: %d\n", wordCount);

  return 0;
}