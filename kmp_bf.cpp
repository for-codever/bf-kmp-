#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int bf_compare_count = 0;
int kmp_compare_count = 0;

int bf_match(const char *main_str, int n, const char *pattern, int m)
{
    int i = 0, j = 0;
    while (i < n && j < m)
    {
        bf_compare_count++;
        if (main_str[i] == pattern[j])
        {
            i++;
            j++;
        }
        else
        {
            i = i - j + 1;
            j = 0;
        }
    }
    return j == m ? i - j : -1;
}

void get_next(const char *pattern, int m, int *next)
{
    int j = 0, k = -1;
    next[0] = -1;
    while (j < m - 1)
    {
        if (k == -1 || pattern[j] == pattern[k])
        {
            j++;
            k++;
            next[j] = k;
        }
        else
        {
            k = next[k];
        }
    }
}

int kmp_match(const char *main_str, int n, const char *pattern, int m, const int *next)
{
    int i = 0, j = 0;
    while (i < n && j < m)
    {
        kmp_compare_count++;
        if (j == -1 || main_str[i] == pattern[j])
        {
            i++;
            j++;
        }
        else
        {
            j = next[j];
        }
    }
    return j == m ? i - j : -1;
}
