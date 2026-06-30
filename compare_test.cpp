#include <stdio.h>
#include <stdlib.h>
#include <string.h>

extern int bf_compare_count;
extern int kmp_compare_count;
extern int bf_match(const char *main_str, int n, const char *pattern, int m);
extern void get_next(const char *pattern, int m, int *next);
extern int kmp_match(const char *main_str, int n, const char *pattern, int m, const int *next);

void reset_count()
{
    bf_compare_count = 0;
    kmp_compare_count = 0;
}

void print_bar(int count, int max_count)
{
    int bar_len = count * 40 / max_count;
    if (bar_len < 1 && count > 0) bar_len = 1;
    for (int i = 0; i < bar_len; i++)
        printf("█");
}

void run_test(const char *title, const char *s, int n, const char *p, int m)
{
    printf("=== %s ===\n", title);
    printf("  主串长度: %d, 模式串长度: %d\n", n, m);

    reset_count();
    int bf_res = bf_match(s, n, p, m);
    int bf_cnt = bf_compare_count;

    reset_count();
    int *next = (int *)malloc(m * sizeof(int));
    get_next(p, m, next);
    int kmp_res = kmp_match(s, n, p, m, next);
    int kmp_cnt = kmp_compare_count;
    free(next);

    printf("  BF  : 匹配位置=%2d, 比较次数=%5d  ", bf_res, bf_cnt);
    print_bar(bf_cnt, bf_cnt > kmp_cnt ? bf_cnt : kmp_cnt);
    printf("\n");

    printf("  KMP : 匹配位置=%2d, 比较次数=%5d  ", kmp_res, kmp_cnt);
    print_bar(kmp_cnt, bf_cnt > kmp_cnt ? bf_cnt : kmp_cnt);
    printf("\n");

    if (bf_cnt > 0 && kmp_cnt > 0)
        printf("  KMP 比 BF 少做了 %.1f%% 的比较\n", (1 - (double)kmp_cnt / bf_cnt) * 100);

    printf("\n");
}

int main()
{
    printf("========== 字符串匹配算法对比（基于比较次数） ==========\n\n");

    // 测试1: 普通匹配
    char s1[] = "ababcabcacbab";
    char p1[] = "abcac";
    run_test("测试1: 普通匹配", s1, strlen(s1), p1, strlen(p1));

    // 测试2: 不匹配（快速失败）
    char s2[] = "aaaaabbbbb";
    char p2[] = "cc";
    run_test("测试2: 完全不匹配", s2, strlen(s2), p2, strlen(p2));

    // 测试3: 极端情况（大量重复字符，最能体现时间复杂度差异）
    int s3_len = 10000;
    char *s3 = (char *)malloc(s3_len + 1);
    memset(s3, 'a', s3_len);
    s3[s3_len] = '\0';

    int p3_len = 100;
    char *p3 = (char *)malloc(p3_len + 1);
    memset(p3, 'a', p3_len - 1);
    p3[p3_len - 1] = 'b';
    p3[p3_len] = '\0';

    run_test("测试3: 极端重复（O(n*m) vs O(n+m)）", s3, s3_len, p3, p3_len);

    free(s3);
    free(p3);

    printf("=======================================================\n");
    printf("结论: 比较次数直接反映了算法的时间复杂度\n");
    printf("  BF  最坏情况: O(n*m)  —— 每次都要回溯\n");
    printf("  KMP 最坏情况: O(n+m)  —— 利用已匹配信息，不回溯\n");

    return 0;
}
