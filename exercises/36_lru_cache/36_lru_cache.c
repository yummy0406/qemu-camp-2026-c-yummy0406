#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/*
 * 16 LRU 缓存淘汰算法（哈希表 + 双向链表）
 *  - put(k,v)、get(k) 均为 O(1)
 *  - 容量满时淘汰最久未使用（LRU）的元素
 */

typedef struct LRUNode {
    int key;
    int value;
    struct LRUNode* prev;
    struct LRUNode* next;
} LRUNode;

typedef struct HashEntry {
    int key;
    LRUNode* node;
    struct HashEntry* next;
} HashEntry;

typedef struct {
    int capacity;
    int size;
    LRUNode* head; /* 最近使用（MRU） */
    LRUNode* tail; /* 最久未使用（LRU） */
    /* 简单链式哈希表 */
    size_t bucket_count;
    HashEntry** buckets;
} LRUCache;

static unsigned hash_int(int key) {
    /* 简单的整数哈希函数 */
    unsigned h = (unsigned)key;
    h = ((h >> 16) ^ h) * 0x45d9f3b;
    h = ((h >> 16) ^ h) * 0x45d9f3b;
    h = (h >> 16) ^ h;
    return h;
}

static HashEntry* hash_find(LRUCache* c, int key, HashEntry*** pprev_next) {
    unsigned h = hash_int(key);
    size_t idx = h % c->bucket_count;

    HashEntry** prev_next = &c->buckets[idx];
    HashEntry* entry = *prev_next;

    while (entry) {
        if (entry->key == key) {
            /* 找到键 */
            if (pprev_next) {
                *pprev_next = prev_next;
            }
            return entry;
        }
        prev_next = &entry->next;
        entry = entry->next;
    }

    /* 未找到键 */
    if (pprev_next) {
        *pprev_next = prev_next;
    }
    return NULL;
}

static void list_add_to_head(LRUCache* c, LRUNode* node) {
    node->prev = NULL;
    node->next = c->head;

    if (c->head) {
        c->head->prev = node;
    } else {
        /* 链表为空，尾指针也指向新节点 */
        c->tail = node;
    }
    c->head = node;
}

static void list_remove(LRUCache* c, LRUNode* node) {
    if (node->prev) {
        node->prev->next = node->next;
    } else {
        /* 节点是头节点 */
        c->head = node->next;
    }

    if (node->next) {
        node->next->prev = node->prev;
    } else {
        /* 节点是尾节点 */
        c->tail = node->prev;
    }

    /* 断开节点的指针 */
    node->prev = NULL;
    node->next = NULL;
}

static void list_move_to_head(LRUCache* c, LRUNode* node) {
    /* 如果节点已经是头节点，无需移动 */
    if (node == c->head) {
        return;
    }

    /* 先将节点从链表中移除，然后添加到头部 */
    list_remove(c, node);
    list_add_to_head(c, node);
}

static LRUNode* list_pop_tail(LRUCache* c) {
    LRUNode* tail_node = c->tail;
    if (!tail_node) {
        return NULL;
    }

    /* 从链表中移除尾节点 */
    list_remove(c, tail_node);
    return tail_node;
}

/* LRU 接口实现 */
static LRUCache* lru_create(int capacity) {
    if (capacity <= 0) {
        return NULL;
    }

    LRUCache* c = malloc(sizeof(LRUCache));
    if (!c) {
        return NULL;
    }

    c->capacity = capacity;
    c->size = 0;
    c->head = NULL;
    c->tail = NULL;

    /* 哈希桶数量：容量的2倍，但至少为16 */
    c->bucket_count = (size_t)capacity * 2;
    if (c->bucket_count < 16) {
        c->bucket_count = 16;
    }

    c->buckets = calloc(c->bucket_count, sizeof(HashEntry*));
    if (!c->buckets) {
        free(c);
        return NULL;
    }

    return c;
}

static void lru_free(LRUCache* c) {
    if (!c) {
        return;
    }

    /* 释放所有哈希条目和LRU节点 */
    for (size_t i = 0; i < c->bucket_count; ++i) {
        HashEntry* entry = c->buckets[i];
        while (entry) {
            HashEntry* next = entry->next;
            free(entry->node);  /* 释放LRU节点 */
            free(entry);        /* 释放哈希条目 */
            entry = next;
        }
    }

    free(c->buckets);
    free(c);
}

static int lru_get(LRUCache* c, int key, int* out_value) {
    HashEntry* entry = hash_find(c, key, NULL);
    if (!entry) {
        return 0; /* 键不存在 */
    }

    *out_value = entry->node->value;
    list_move_to_head(c, entry->node);
    return 1;
}

static void lru_put(LRUCache* c, int key, int value) {
    HashEntry** prev_next;
    HashEntry* entry = hash_find(c, key, &prev_next);

    if (entry) {
        /* 键已存在：更新值并移动到头部 */
        entry->node->value = value;
        list_move_to_head(c, entry->node);
        return;
    }

    /* 键不存在：需要插入新条目 */
    if (c->size == c->capacity) {
        /* 缓存已满：淘汰最久未使用的节点（尾部） */
        LRUNode* tail_node = list_pop_tail(c);
        if (tail_node) {
            /* 从哈希表中删除对应的条目 */
            HashEntry** prev_next2;
            HashEntry* old_entry = hash_find(c, tail_node->key, &prev_next2);
            if (old_entry) {
                *prev_next2 = old_entry->next;
                free(old_entry);
            }
            free(tail_node);
            c->size--;
        }
    }

    /* 创建新节点 */
    LRUNode* new_node = malloc(sizeof(LRUNode));
    if (!new_node) {
        return; /* 内存分配失败 */
    }
    new_node->key = key;
    new_node->value = value;

    /* 创建新哈希条目 */
    HashEntry* new_entry = malloc(sizeof(HashEntry));
    if (!new_entry) {
        free(new_node);
        return; /* 内存分配失败 */
    }
    new_entry->key = key;
    new_entry->node = new_node;

    /* 将节点添加到链表头部 */
    list_add_to_head(c, new_node);

    /* 将条目插入哈希表 */
    new_entry->next = *prev_next;
    *prev_next = new_entry;

    c->size++;
}

/* 打印当前缓存内容（从头到尾） */
static void lru_print(LRUCache* c) {
    LRUNode* p = c->head;
    int first = 1;
    while (p) {
        if (!first) printf(", ");
        first = 0;
        printf("%d:%d", p->key, p->value);
        p = p->next;
    }
    printf("\n");
}

int main(void) {
    /* 容量 3：put(1,1), put(2,2), put(3,3), put(4,4), get(2), put(5,5) */
    LRUCache* c = lru_create(3);
    if (!c) {
        fprintf(stderr, "创建 LRU 失败\n");
        return 1;
    }

    lru_put(c, 1, 1); /* 缓存：1 */
    lru_put(c, 2, 2); /* 缓存：2,1 */
    lru_put(c, 3, 3); /* 缓存：3,2,1 (满) */
    lru_put(c, 4, 4); /* 淘汰 LRU(1)，缓存：4,3,2 */

    int val;
    if (lru_get(c, 2, &val)) {
        /* 访问 2：缓存：2,4,3 */
        (void)val; /* 演示无需使用 */
    }

    lru_put(c, 5, 5); /* 淘汰 LRU(3)，缓存：5,2,4 */

    /* 期望最终键集合：{2,4,5}，顺序无关。此处按最近->最久打印：5:5, 2:2, 4:4 */
    lru_print(c);

    lru_free(c);
    return 0;
}
