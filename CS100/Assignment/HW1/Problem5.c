#include <stdio.h>
int E, S, N, X, Y;
int main()
{
    scanf("%d%d%d", &E, &S, &N);
    char c = getchar();
    while (c != 'Q')
    {
        if (c == 's')
        {
            if (E >= 1)
            {
                E -= 1;
                if (S + 6 >= 0)
                {
                    X += (S + 6);
                }
            }
        }
        else if (c == 'b')
        {
            if (E >= 1)
            {
                Y += 5;
                E -= 1;
            }
        }
        else if (c == 'c')
        {
            if (E >= 1)
            {
                N += 1;
                E -= 1;
            }
        }
        else if (c == 't')
        {
            E += 2;
            S -= 1;
        }
        else if (c == 'e')
        {
            if (E >= 2)
            {
                X += (8 * N);
                N = 0;
                E -= 2;
            }
        }
        c = getchar();
    }
    X += (N * 3);
    printf("%d %d\n", X, Y);
    printf("%d %d %d", E, S, N);
    return 0;
}