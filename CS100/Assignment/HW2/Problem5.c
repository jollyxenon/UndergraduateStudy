#include <stdio.h>
#include <stdbool.h>
unsigned int l[4], b[4];
bool evaluate_packet(unsigned int A, unsigned int B, unsigned int C, unsigned int D, char **type_str)
{
    *type_str = "Invalid";
    if (A > 0 && B > 0 && C > 0)
    {
        if (D == (A * B + C))
        {
            *type_str = "Product";
            return 1;
        }
        else if (D == (A + B + C))
        {
            *type_str = "Sum";
            return 1;
        }
    }
    return 0;
}
int main(void)
{
    unsigned char bytes[16];
    unsigned int temp;
    for (int i = 0; i < 16; ++i)
    {
        scanf("%x", &temp);
        bytes[i] = (unsigned char)temp;
    }

    for (int i = 0; i < 4; ++i)
    {
        int idx = i * 4;
        l[i] = ((unsigned int)bytes[idx]) |
               ((unsigned int)bytes[idx + 1] << 8) |
               ((unsigned int)bytes[idx + 2] << 16) |
               ((unsigned int)bytes[idx + 3] << 24);
        b[i] = ((unsigned int)bytes[idx] << 24) |
               ((unsigned int)bytes[idx + 1] << 16) |
               ((unsigned int)bytes[idx + 2] << 8) |
               ((unsigned int)bytes[idx + 3]);
    }
    int l_valid, b_valid;
    char *l_type, *b_type;
    l_valid = evaluate_packet(l[0], l[1], l[2], l[3], &l_type);
    b_valid = evaluate_packet(b[0], b[1], b[2], b[3], &b_type);
    printf("Little: %u %u %u %u %s\n", l[0], l[1], l[2], l[3], l_type);
    printf("Big: %u %u %u %u %s\n", b[0], b[1], b[2], b[3], b_type);
    if (l_valid && b_valid)
    {
        printf("Result: Both");
    }
    else if (l_valid)
    {
        printf("Result: Little %s", l_type);
    }
    else if (b_valid)
    {
        printf("Result: Big %s", b_type);
    }
    else
    {
        printf("Result: None");
    }
    return 0;
}