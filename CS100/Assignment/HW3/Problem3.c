#include <stddef.h>

int hw3_islower(int ch)
{
    return (ch >= 'a' && ch <= 'z');
}

int hw3_isupper(int ch)
{
    return (ch >= 'A' && ch <= 'Z');
}

int hw3_isalpha(int ch)
{
    return ((ch >= 'a' && ch <= 'z') || (ch >= 'A' && ch <= 'Z'));
}

int hw3_isdigit(int ch)
{
    return (ch >= '0' && ch <= '9');
}

int hw3_tolower(int ch)
{
    return (ch >= 'A' && ch <= 'Z') ? (ch - 'A' + 'a') : ch;
}

int hw3_toupper(int ch)
{
    return (ch >= 'a' && ch <= 'z') ? (ch - 'a' + 'A') : ch;
}

size_t hw3_strlen(const char *str)
{
    size_t len = 0;
    while (*str != '\0')
    {
        len++;
        str++;
    }
    return len;
}

char *hw3_strchr(const char *str, int ch)
{
    while (*str != (char)ch)
    {
        if (*str == '\0')
            return NULL;
        str++;
    }
    return (char *)str;
}

char *hw3_strcpy(char *dest, const char *src)
{
    char *ret = dest;
    while ((*dest++ = *src++) != '\0')
        ;
    return ret;
}

char *hw3_strcat(char *dest, const char *src)
{
    char *ret = dest;
    while (*dest != '\0')
    {
        dest++;
    }
    while ((*dest++ = *src++) != '\0')
        ;
    return ret;
}

int hw3_strcmp(const char *lhs, const char *rhs)
{
    while (*lhs != '\0' && *lhs == *rhs)
    {
        lhs++;
        rhs++;
    }
    return (unsigned char)(*lhs) - (unsigned char)(*rhs);
}