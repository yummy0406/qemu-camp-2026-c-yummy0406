#include <stdio.h>
#include <stdlib.h>

/*
 * 19 字符串分割器（类似 strtok_r）
 * 目标：实现线程安全的字符串分割函数，不依赖静态全局状态
 * 原型：char* strtok_r(char *str, const char *delim, char **saveptr)
 * 示例：输入 "hello,world test"，分隔符 ", " → 依次得到 hello、world、test
 */

/* 判断字符 c 是否在分隔符集合 delim 中 */
static int is_delim(char c, const char *delim) {
    for (; *delim != '\0'; ++delim) {
        if (c == *delim) {
            return 1;
        }
    }
    return 0;
}

/* 线程安全版本：通过 saveptr 维护调用状态，不使用静态变量 */
char *strtok_r(char *str, const char *delim, char **saveptr) {
    char *p;

    /* 如果 str 不为 NULL，则开始新的分割 */
    if (str != NULL) {
        *saveptr = str;
    }

    p = *saveptr;
    if (p == NULL) {
        return NULL;
    }

    /* 跳过开头的分隔符 */
    while (*p != '\0' && is_delim(*p, delim)) {
        p++;
    }

    /* 如果到达字符串结尾，返回 NULL */
    if (*p == '\0') {
        *saveptr = NULL;
        return NULL;
    }

    /* 标记的开始位置 */
    char *token_start = p;

    /* 找到标记的结束位置 */
    while (*p != '\0' && !is_delim(*p, delim)) {
        p++;
    }

    /* 如果当前字符是分隔符，用 '\0' 替换，并移动指针到下一个字符 */
    if (*p != '\0') {
        *p = '\0';
        p++;
    }

    /* 更新 saveptr 指向剩余部分 */
    *saveptr = p;

    return token_start;
}

int main(void) {
    char buf[] = "hello,world test";
    const char *delim = ", ";
    char *save = NULL;

    char *tok = strtok_r(buf, delim, &save);
    while (tok) {
        printf("%s\n", tok);
        tok = strtok_r(NULL, delim, &save);
    }
    return 0;
}
