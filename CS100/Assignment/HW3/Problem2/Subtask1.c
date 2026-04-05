#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
int n;
int main(void)
{
    scanf("%d", &n);
    int len;
    char erc;
    for (int i = 0; i < n; i++)
    {
        scanf("%d", &len);
        char *a = malloc(sizeof(char) * len);
        while ((erc = getchar()) != EOF && (erc == ' ' || erc == '\r' || erc == '\n' || erc == '\t'))
            ;
        for (int j = 0; j < (len + 1) / 2; j++)
        {
            *(a + j) = erc;
            erc = getchar();
        }
        bool tag = 1;
        for (int j = (len + 1) / 2; j < len; j++)
        {
            if (erc != *(a + len - j - 1))
                tag = 0;
            erc = getchar();
        }
        if (tag == 0)
            printf("No\n");
        else
            printf("Yes\n");
        free(a);
    }
    return 0;
}