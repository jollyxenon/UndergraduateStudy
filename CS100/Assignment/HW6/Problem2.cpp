#include <iostream>
#include <string>
#include <vector>
#include <algorithm>
class playerinfo
{
    std::string name;
    int goal;
    int minute;
    double efficiency;

public:
    playerinfo(const std::string &raw_info)
        : name(""), goal(0), minute(0), efficiency(0.0)
    {
        long unsigned int i = 0;
        for (; i < raw_info.size(); ++i)
        {
            if (raw_info[i] == '#')
            {
                ++i;
                break;
            }
            if (raw_info[i] == '_')
                name += ' ';
            else
                name += raw_info[i];
        }
        for (; i < raw_info.size(); ++i)
        {
            if (raw_info[i] == '#')
            {
                ++i;
                break;
            }
            goal = goal * 10 + (raw_info[i] - '0');
        }
        for (; i < raw_info.size(); ++i)
        {
            if (raw_info[i] == '#')
            {
                ++i;
                break;
            }
            minute = minute * 10 + (raw_info[i] - '0');
        }
        efficiency = goal * 90.0 / minute;
    }
    void display() const
    {
        std::cout << name << " (" << goal << ") - ";
        printf("%.2f", efficiency);
        std::cout << std::endl;
    }
    int get_goal() const
    {
        return goal;
    }
    double get_efficiency() const
    {
        return efficiency;
    }
    std::string get_name() const
    {
        return name;
    }
};
void getname(std::string &name)
{
    char c = getchar();
    while (c != '\n' && c != '\r' && c != EOF)
    {
        name.push_back(c);
        c = getchar();
    }
    return;
}
bool cmp(const playerinfo &a, const playerinfo &b)
{
    if (a.get_goal() != b.get_goal())
        return a.get_goal() > b.get_goal();
    else if (a.get_efficiency() != b.get_efficiency())
        return a.get_efficiency() > b.get_efficiency();
    else
        return a.get_name() < b.get_name();
}
std::vector<playerinfo> players;
int main()
{
    int N;
    scanf("%d", &N);
    do
    {
        if (N <= 0)
            break;
        char c = getchar();
        if (c == '\n' || c == '\r')
            continue;
        if (c == EOF)
            break;
        std::string raw_info;
        raw_info.push_back(c);
        getname(raw_info);
        N--;
        players.push_back(playerinfo(raw_info));
    } while (1);
    std::sort(players.begin(), players.end(), cmp);
    for (const auto &player : players)
        player.display();
    return 0;
}