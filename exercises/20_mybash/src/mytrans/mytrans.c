// mytrans.c
#include "myhash.h"
#include <ctype.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void trim(char *str) {
  if (!str)
    return;

  // 去除前导空格
  char *start = str;
  while (*start && isspace((unsigned char)*start)) {
    start++;
  }

  // 去除末尾空格
  char *end = str + strlen(str) - 1;
  while (end >= start && isspace((unsigned char)*end)) {
    end--;
  }

  // 复制修剪后的字符串
  memmove(str, start, end - start + 1);
  str[end - start + 1] = '\0';
}

int load_dictionary(const char *filename, HashTable *table,
                    uint64_t *dict_count) {
  FILE *file = fopen(filename, "r");
  if (!file) {
    perror("无法打开词典文件");
    return -1;
  }

  char line[1024];
  char current_word[100] = {0};
  char current_translation[1024] = {0};
  int in_entry = 0;

  while (fgets(line, sizeof(line), file)) {
    line[strcspn(line, "\n")] = '\0';
    trim(line);

    if (strlen(line) == 0) {
      continue;
    }

    // 检查是否是词条开始（以单词开头，后跟制表符）
    char *tab_pos = strchr(line, '\t');
    if (tab_pos) {
      // 如果有前一个词条，保存它
      if (in_entry && strlen(current_word) > 0) {
        hash_table_insert(table, current_word, current_translation);
        (*dict_count)++;
      }

      // 新词条
      int word_len = tab_pos - line;
      strncpy(current_word, line, word_len);
      current_word[word_len] = '\0';

      strcpy(current_translation, tab_pos + 1);
      in_entry = 1;
    } else if (in_entry) {
      // 继续当前词条的翻译
      strcat(current_translation, "@");
      strcat(current_translation, line);
    }
  }

  // 保存最后一个词条
  if (in_entry && strlen(current_word) > 0) {
    hash_table_insert(table, current_word, current_translation);
    (*dict_count)++;
  }

  fclose(file);
  return 0;
}

void to_lowercase(char *str) {
  for (; *str; ++str)
    *str = tolower((unsigned char)*str);
}

int __cmd_mytrans(const char *filename) {
  HashTable *table = create_hash_table();
  if (!table) {
    fprintf(stderr, "无法创建哈希表\n");
    return 1;
  }

  printf("=== 哈希表版英语翻译器（支持百万级数据）===\n");
  uint64_t dict_count = 0;
  if (load_dictionary("/workspace/exercises/20_mybash/src/mytrans/dict.txt",
                      table, &dict_count) != 0) {
    fprintf(stderr, "加载词典失败，请确保 dict.txt 存在。\n");
    free_hash_table(table);
    return 1;
  }
  printf("词典加载完成，共计%ld词条。\n", dict_count);

  FILE *file = fopen(filename, "r");
  if (file == NULL) {
    fprintf(stderr, "无法打开文件 dict.txt。\n");
    free_hash_table(table);
    return 1;
  }

  char line[256];
  while (fgets(line, sizeof(line), file) != NULL) {
    line[strcspn(line, "\n")] = '\0';

    if (strlen(line) == 0) {
      continue;
    }

    // 使用 strtok 按空格分割单词
    char *word = strtok(line, " ");
    while (word != NULL) {
      const char *translation = hash_table_lookup(table, word);
      printf("原文: %s\t", word);
      if (translation) {
        printf("翻译: %s\n", translation);
      } else {
        printf("未找到该单词的翻译。\n");
      }

      word = strtok(NULL, " ");
    }
  }

  free_hash_table(table);
  return 0;
}