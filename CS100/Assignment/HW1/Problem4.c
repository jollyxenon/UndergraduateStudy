#include <stdio.h>
#include <limits.h>
long long n, Re;
int main()
{
    scanf("%lld", &n);
    int SignState = 0;
    if (n < 0)
    {
        SignState = 1;
        n = -n;
    }
    while (n > 0)
    {
        Re = Re * 10 + n % 10;
        n /= 10;
    }
    if (SignState)
        Re = -Re;
    if (Re > 2147483647 || Re < -2147483648)
        Re = 0;
    printf("%lld", Re);
    return 0;
}