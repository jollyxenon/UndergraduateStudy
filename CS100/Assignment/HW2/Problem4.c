#include <stdio.h>
#include <stdbool.h>
#include <math.h>
int n, Q;
unsigned int s[1001];
int p[1001];
bool Heap[4001][1001];
unsigned int CyclicLeftShift(unsigned int rs, int n)
{
    n = n % 32;
    if (n == 0)
        return rs;
    return (rs << n) | (rs >> (32 - n));
}
void modify(int x, int n, int r, unsigned int recents)
{
    if (Heap[r][x] == 1)
        return;
    else
    {
        Heap[r][x] = 1;
        s[x] = s[x] ^ CyclicLeftShift(recents, n);
        modify(p[x], n + 1, r, recents);
    }
    return;
}
int main(void)
{
    scanf("%d%d", &n, &Q);
    for (int i = 1; i <= n; i++)
        scanf("%u", &s[i]);
    for (int i = 1; i <= n; i++)
        p[i] = i;
    for (int i = 0; i < Q; i++)
    {
        int x, y;
        scanf("%d%d", &x, &y);
        p[x] = y;
        Heap[i][x] = 1;
        modify(p[x], 1, i, s[x]);
    }
    int minpos = 1;
    unsigned int minx = 4294967295u;
    for (int i = 1; i <= n; i++)
        if (s[i] < minx)
        {
            minx = s[i];
            minpos = i;
        }
    printf("%d\n%u", minpos, minx);
    return 0;
}