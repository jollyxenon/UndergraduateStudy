#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <string.h>
#define ll long long

int N;                // Number of days
char SpiderType[10];  // Type of spider
ll PlayerHP, EnemyHP; // Player and enemy health points
double SandTime;      // Time when sandstorm comes
ll Venom;             // Venom count
int MultiCast;        // Multicast count
void Initialize()
{
    scanf("%lld %lld %lf", &PlayerHP, &EnemyHP, &SandTime);
    Venom = 0;
    return;
}
void VenomDamageResolution(ll step)
{
    if (step % 4 == 0)
    {
        EnemyHP -= Venom;
    }
    return;
}
void SpiderAttackApplication(ll step)
{
    if (step % 16 == 0 && step >= 16)
    {
        Venom += 2;
    }
    else if (step >= 16 && step - (ll)(step / 16) * 16 <= MultiCast)
    {
        Venom += 2;
    }
    return;
}
void SandstormDamageResolution(ll step)
{
    if (step >= (ll)(SandTime / 0.25))
    {
        ll SandStormDamage = (step - (ll)(SandTime / 0.25) + 1) * 2 - 1;
        EnemyHP -= SandStormDamage;
        PlayerHP -= SandStormDamage;
    }
    return;
}
int DeathCheck()
{
    if (EnemyHP <= 0 && PlayerHP > 0)
    {
        return 1; // Win
    }
    else if (PlayerHP <= 0 && EnemyHP > 0)
    {
        return 2; // Lose
    }
    else if (PlayerHP <= 0 && EnemyHP <= 0)
    {
        return 3; // Draw
    }
    return 0; // Continue
}
void VictoryRewardCalculate()
{
    if (strcmp(SpiderType, "silver") == 0)
    {
        MultiCast += 1;
    }
    else if (strcmp(SpiderType, "gold") == 0)
    {
        MultiCast += 2;
    }
    else if (strcmp(SpiderType, "diamond") == 0)
    {
        MultiCast += 3;
    }

    if (strcmp(SpiderType, "silver") == 0 && MultiCast >= 4)
    {
        MultiCast = 0;
    }
    else if (strcmp(SpiderType, "gold") == 0 && MultiCast >= 6)
    {
        MultiCast = 0;
    }
    else if (strcmp(SpiderType, "diamond") == 0 && MultiCast >= 8)
    {
        MultiCast = 0;
    }
    return;
}
int main(void)
{
    scanf("%d %s", &N, SpiderType);
    for (int i = 0; i < N; i++)
    {
        Initialize();
        ll TimeStep = 0;
        while (1)
        {
            VenomDamageResolution(TimeStep);
            SpiderAttackApplication(TimeStep);
            SandstormDamageResolution(TimeStep);
            int Status = DeathCheck();
            if (Status == 1)
            {
                printf("Win at %.2fs", (double)(TimeStep) * 0.25);
                VictoryRewardCalculate();
                break;
            }
            else if (Status == 2)
            {
                printf("Lose at %.2fs", (double)(TimeStep) * 0.25);
                break;
            }
            else if (Status == 3)
            {
                printf("Draw at %.2fs", (double)(TimeStep) * 0.25);
                break;
            }
            TimeStep++;
        }
        if (i != N - 1)
        {
            printf("\n");
        }
    }
    return 0;
}