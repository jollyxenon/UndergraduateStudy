#include <stdio.h>
int main()
{
    // Calculate the average power of all energy cores.

    int count = 0;
    int total = 0;

    printf("How many energy cores are there?\n");
    scanf("%d", &count);
    printf("Enter the power level of each core:\n");

    // We programmers count from zero!
    for (int i = 0; i < count; i++)
    {
        int power = 0;
        scanf("%d", &power);
        total += power;
    }

    double average = (double)(total) / count;
    if (average - 50.0 < 1e-6 && average - 50.0 > -1e-6)
        printf("Sufficient!\n");
    else if (average - 50.0 > 1e-6)
        printf("Overcharged!\n");
    else
        printf("Low Power!\n");

    printf("Average power is %.2f.", average);

    return 0;
}
