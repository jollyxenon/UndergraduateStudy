#include <stdio.h>
#include <string.h>
#include <stdbool.h>

bool ends_with(const char *str, const char *suffix)
{
    int str_len = strlen(str);
    int suffix_len = strlen(suffix);
    if (str_len >= suffix_len)
    {
        return strcmp(str + str_len - suffix_len, suffix) == 0;
    }
    return false;
}

void parse_std(const char *xxx, char *output)
{
    if (strncmp(xxx, "gnu++", 5) == 0)
    {
        sprintf(output, "GNU dialect of C++%s", &xxx[5]);
    }
    else if (strncmp(xxx, "gnu", 3) == 0)
    {
        sprintf(output, "GNU dialect of C%s", &xxx[3]);
    }
    else if (strncmp(xxx, "c++", 3) == 0)
    {
        sprintf(output, "ISO C++%s", &xxx[3]);
    }
    else if (strncmp(xxx, "c", 1) == 0)
    {
        sprintf(output, "ISO C%s", &xxx[1]);
    }
}

const char *parse_file_type(const char *arg)
{
    if (ends_with(arg, ".c"))
        return "C source code";
    if (ends_with(arg, ".h"))
        return "C/C++ header file";
    if (ends_with(arg, ".cpp") || ends_with(arg, ".C") || ends_with(arg, ".cc") || ends_with(arg, ".cxx"))
        return "C++ source code";
    if (ends_with(arg, ".hpp") || ends_with(arg, ".hxx"))
        return "C++ header file";
    return "";
}

int main(int argc, char *argv[])
{
    for (int i = 1; i < argc; ++i)
    {
        const char *arg = argv[i];

        if (strcmp(arg, "-Wall") == 0)
        {
            printf("-Wall: Enable all the warnings about constructions that some users consider questionable, and that are easy to avoid (or modify to prevent the warning).\n");
        }
        else if (strcmp(arg, "-Wpedantic") == 0)
        {
            printf("-Wpedantic: Issue all the warnings demanded by strict ISO C and ISO C++ and reject all programs that use forbidden extensions.\n");
        }
        else if (strcmp(arg, "-Wextra") == 0)
        {
            printf("-Wextra: Enable some extra warning flags that are not enabled by -Wall.\n");
        }
        else if (strcmp(arg, "-Werror") == 0)
        {
            printf("-Werror: Make all warnings into errors.\n");
        }
        else if (strcmp(arg, "-o") == 0)
        {
            i++;
            const char *xxx = argv[i];
            printf("-o %s: Place the primary output in file %s.\n", xxx, xxx);
        }
        else if (strcmp(arg, "-I") == 0)
        {
            i++;
            const char *xxx = argv[i];
            printf("-I %s: Add the directory %s to the list of directories to be searched for header files during preprocessing.\n", xxx, xxx);
        }
        else if (strncmp(arg, "-std=", 5) == 0)
        {
            const char *xxx = &arg[5];
            char yyy[100] = {0};
            parse_std(xxx, yyy);
            printf("-std=%s: Set the language standard to %s.\n", xxx, yyy);
        }
        else
        {
            const char *type = parse_file_type(arg);
            printf("%s: %s as input file.\n", arg, type);
        }
    }

    return 0;
}