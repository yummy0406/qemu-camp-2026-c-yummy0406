#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/**
 * URL参数解析器
 * 输入：包含http/https超链接的字符串
 * 输出：解析出所有的key-value键值对，每行显示一个
 */

int parse_url(const char *url) {
  int err = 0;

  // 找到查询字符串的开始位置 '?'
  const char *query_start = strchr(url, '?');
  if (query_start == NULL) {
    return 0;
  }
  query_start++; // 跳过 '?'

  // 复制查询字符串以便使用 strtok 处理
  size_t len = strlen(query_start);
  char *query_copy = (char *)malloc(len + 1);
  if (query_copy == NULL) {
    return ENOMEM;
  }
  strcpy(query_copy, query_start);

  // 使用 '&' 分割不同的参数对
  char *pair = strtok(query_copy, "&");
  while (pair != NULL) {
    // 使用 '=' 分割 key 和 value
    char *sep = strchr(pair, '=');
    if (sep != NULL) {
      *sep = '\0';
      char *key = pair;
      char *value = sep + 1;
      printf("key = %s, value = %s\n", key, value);
    }
    pair = strtok(NULL, "&");
  }

  free(query_copy);

  return err;
}

int main() {
  const char *test_url =
      "https://cn.bing.com/search?name=John&age=30&city=New+York";

  printf("Parsing URL: %s\n", test_url);
  printf("Parameters:\n");

  parse_url(test_url);

  return 0;
}