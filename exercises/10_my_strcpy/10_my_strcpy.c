#include <stdio.h>

// 自定义字符串拷贝函数
void my_strcpy(char *dest, char *src) {
  while (*src != '\0') {
    *dest = *src;
    dest++;
    src++;
  }
  *dest = '\0'; // 添加结束符
}

int main() {
  char source[] =
      "Hello, World! This is a test string for my_strcpy function.\n";
  char destination[101];

  my_strcpy(destination, source);

  printf("拷贝后的字符串: %s", destination);

  return 0;
}