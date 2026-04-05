#include <stdio.h>
#include <stdlib.h>
#include <time.h>

int main(int argc, char *argv[])
{
    if (argc != 3)
    {
        fprintf(stderr, "Usage: %s <n> <max_length>\n", argv[0]);
        return 1;
    }

    int n = atoi(argv[1]);
    int max_length = atoi(argv[2]);

    srand((unsigned int)time(NULL));

    printf("%d\n", n);
    for (int i = 0; i < n; i++)
    {
        int make_palindrome;
        if (i == 0)
            make_palindrome = 1;
        else if (i == 1)
            make_palindrome = 0;
        else
            make_palindrome = rand() % 2;

        int len;
        if (!make_palindrome && max_length > 1)
            len = rand() % (max_length - 1) + 2;
        else
            len = rand() % max_length + 1;

        printf("%d\n", len);

        char *a = malloc(sizeof(char) * (len + 1));
        if (a == NULL)
            return 1;

        if (make_palindrome)
        {
            for (int j = 0; j < (len + 1) / 2; j++)
            {
                a[j] = (char)(rand() % (126 - 33 + 1) + 33);
                a[len - 1 - j] = a[j];
            }
        }
        else
        {
            for (int j = 0; j < len; j++)
            {
                a[j] = (char)(rand() % (126 - 33 + 1) + 33);
            }

            if (len > 1)
            {
                int is_pal = 1;
                for (int j = 0; j < len / 2; j++)
                {
                    if (a[j] != a[len - 1 - j])
                    {
                        is_pal = 0;
                        break;
                    }
                }

                if (is_pal)
                {
                    a[len - 1] = (a[len - 1] == '!') ? '"' : '!';
                }
            }
        }

        a[len] = '\0';
        printf("%s\n", a);
        free(a);
    }
    return 0;
}