#include <bits/stdc++.h>

/// @brief Define the Skill and Pokemon classes, and declare the necessary variables and functions for reading Pokemon information and combat information.
class Skill
{
public:
    std::string SkillName;
    int Power;
    int pp;
    Skill(std::string SkillName_, int Power_, int pp_) : SkillName(SkillName_), Power(Power_), pp(pp_) {}
};

/// @brief Define the Pokemon class, which contains the Pokemon's name, health points, number of skills, and a vector of skills.
class Pokemon
{
public:
    std::string PokemonName;
    int HealthPoints;
    int SkillNumber;
    std::vector<Skill> Skills;
    Pokemon(std::string PokemonName_, int HealthPoints_, int SkillNumber_, std::vector<Skill> Skills_)
        : PokemonName(PokemonName_), HealthPoints(HealthPoints_), SkillNumber(SkillNumber_), Skills(Skills_) {}
    Pokemon() : PokemonName(""), HealthPoints(0), SkillNumber(0) {}
    void PrintInfo()
    {
        std::cout << PokemonName << " " << HealthPoints << std::endl;
        if (SkillNumber > 0)
            for (const auto &Skill : Skills)
            {
                std::cout << Skill.SkillName << " " << Skill.pp << std::endl;
            }
    }
};

std::vector<Pokemon> Pokemons;
Pokemon Player1, Player2;

/// @brief Read Pokemon information until "Ditto 0 0" is encountered, and store the information in the Pokemons vector.
void ReadPokemon()
{
    do
    {
        std::string PokemonName;
        int HealthPoints;
        int SkillNumber;
        std::cin >> PokemonName >> HealthPoints >> SkillNumber;
        std::vector<Skill> Skills;
        for (int i = 0; i < SkillNumber; i++)
        {
            std::string SkillName;
            int Power, pp;
            std::cin >> SkillName >> Power >> pp;
            Skill NewSkill(SkillName, Power, pp);
            Skills.push_back(NewSkill);
        }
        Pokemon NewPokemon(PokemonName, HealthPoints, SkillNumber, Skills);
        Pokemons.push_back(NewPokemon);
    } while (Pokemons.back().PokemonName != "Ditto" || Pokemons.back().HealthPoints != 0 || Pokemons.back().SkillNumber != 0);
    Pokemons.pop_back();
    return;
}

/// @brief Read the names of the two players and find their corresponding Pokemon in the Pokemons vector, and store them in Player1 and Player2.
void ReadCombat()
{
    std::string Player1Name, Player2Name;
    std::cin >> Player1Name >> Player2Name;
    for (const auto &Pokemon : Pokemons)
    {
        if (Pokemon.PokemonName == Player1Name)
            Player1 = Pokemon;

        if (Pokemon.PokemonName == Player2Name)
            Player2 = Pokemon;
    }
    return;
}

/// @brief Process the attack action between two Pokemon.
/// @param Attacker
/// @param Defender
/// @param SkillIndex
/// @return The name of the champion if a Pokemon faints, otherwise an empty string.
std::string AttackProcess(Pokemon &Attacker, Pokemon &Defender, const int SkillIndex)
{
    if (SkillIndex >= 0 && Attacker.SkillNumber - 1 >= SkillIndex && Attacker.Skills[SkillIndex].pp > 0)
    {
        Attacker.Skills[SkillIndex].pp--;
        if (Defender.HealthPoints <= Attacker.Skills[SkillIndex].Power)
        {
            Defender.HealthPoints = 0;
            return Attacker.PokemonName;
        }
        else
        {
            Defender.HealthPoints -= Attacker.Skills[SkillIndex].Power;
        }
    }
    return "";
}

/// @brief Process the combat between two Pokemon for a specified number of rounds.
/// @param RoundNumber
void Combat(int RoundNumber)
{
    bool Player1Turn = true;
    std::string ChampionName = "";
    for (int i = 0; i < RoundNumber * 2; i++)
    {
        int SkillIndex;
        scanf("%d", &SkillIndex);
        if (Player1Turn)
            ChampionName = AttackProcess(Player1, Player2, SkillIndex);
        else
            ChampionName = AttackProcess(Player2, Player1, SkillIndex);
        if (ChampionName != "")
            break;
        Player1Turn = !Player1Turn;
    }

    Player1.PrintInfo();
    std::cout << std::endl;
    Player2.PrintInfo();
    std::cout << std::endl;
    if (ChampionName != "")
        std::cout << ChampionName << " Win";
    else
        std::cout << "Draw";
}

int main()
{
    ReadPokemon();
    ReadCombat();
    int CombatRoundNumber;
    scanf("%d", &CombatRoundNumber);
    Combat(CombatRoundNumber);
    return 0;
}