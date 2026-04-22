#include <execinfo.h>
#include <stdio.h>
#include <stdlib.h>

#if DEBUG_LEVEL == 1
#define DEBUG_PRINT(fmt, ...) printf("DEBUG: func=%s, line=%d\n", __func__, __LINE__)
#elif DEBUG_LEVEL == 2
#define DEBUG_PRINT(fmt, ...) printf("DEBUG: func=%s, line=%d, " fmt "\n", __func__, __LINE__, ##__VA_ARGS__)
#else
#define DEBUG_PRINT(fmt, ...)                                    \
    do {                                                         \
        void *trace[16];                                         \
        int n = backtrace(trace, 16);                            \
        printf("DEBUG: func=%s, line=%d\n", __func__, __LINE__); \
        backtrace_symbols_fd(trace, n, 1);                       \
    } while (0)
#endif
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//

//! MUST BE ENSURE THE DEBUG_PRINT("x=%d", x) AT THE 48 LINE

// 测试代码
void test() {
    int x = 42;
    DEBUG_PRINT("x=%d", x);
}

int main() {
    test();
    return 0;
}
