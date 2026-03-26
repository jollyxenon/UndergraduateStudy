#include <stdio.h>
int main()
{
    int n = 0;
    long long sum = 0, MX = 0, inp = 0;
    scanf("%d", &n);
    for (int i = 0; i < n; i++)
    {
        scanf("%lld", &inp);
        sum += inp;
        if (inp > MX)
            MX = inp;
    }
    printf("%lld\n%lld", MX, sum * 8);
    return 0;
}