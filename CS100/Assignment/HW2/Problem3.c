#include <stdio.h>
#include <stdbool.h>
int gomoku[15][15];
int PieceCount;
bool isLine(int Layer, int x, int y, int color, int dx, int dy)
{
    if (Layer == 0)
        return true;
    else if (x < 0 || x >= 15 || y < 0 || y >= 15 || gomoku[x][y] != color)
        return false;
    else
        return isLine(Layer - 1, x + dx, y + dy, color, dx, dy);
}
bool checkWin()
{
    for (int i = 0; i < 15; i++)
    {
        for (int j = 0; j < 15; j++)
        {
            if (gomoku[i][j] != -1)
            {
                int color = gomoku[i][j];
                if (isLine(5, i, j, color, 0, 1) || isLine(5, i, j, color, 1, 1) ||
                    isLine(5, i, j, color, 1, 0) || isLine(5, i, j, color, 1, -1) ||
                    isLine(5, i, j, color, 0, -1) || isLine(5, i, j, color, -1, -1) ||
                    isLine(5, i, j, color, -1, 0) || isLine(5, i, j, color, -1, 1))
                {
                    return true;
                }
            }
        }
    }
    return false;
}
int main(void)
{
    for (int i = 0; i < 15; i++)
    {
        char temp;
        for (int j = 0; j < 15; j++)
        {
            temp = getchar();
            if (temp == '.')
            {
                gomoku[i][j] = -1;
            }
            else
            {
                PieceCount++;
                if (temp == 'X')
                    gomoku[i][j] = 0;
                else if (temp == 'O')
                    gomoku[i][j] = 1;
            }
        }
        if (i < 14)
            getchar();
    }

    if (checkWin())
    {
        printf("AlreadyWin!\n");
        return 0;
    }

    for (int i = 0; i < 15; i++)
    {
        for (int j = 0; j < 15; j++)
        {
            if (gomoku[i][j] == -1)
            {
                gomoku[i][j] = PieceCount % 2;
                if (checkWin())
                {
                    printf("Win!\n");
                    printf("%d %d\n", i + 1, j + 1);
                    return 0;
                }
                gomoku[i][j] = -1;
            }
        }
    }

    printf("CannotWin!");
    return 0;
}