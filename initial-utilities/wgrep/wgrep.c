#include <stdio.h>
#include <regex.h>
#include <string.h>

int main(int argc, char *argv[])
{
    if (argc < 2)
    {
        printf("wgrep: searchterm [file ...]\n");
        return 1;
    }

    // get the term into a compiled regex
    regex_t re;
    int rc = regcomp(&re, argv[1], REG_EXTENDED);
    if (rc != 0)
    {
        printf("wgrep: invalid regex\n");
        return 1;
    }

    if (argc == 2)
    {
        char buf[200];
        regmatch_t match[1];
        while (fgets(buf, sizeof(buf), stdin))
        {
            int found = regexec(&re, buf, /*nmatch*/ 1, match, /*no flags*/ 0);

            if (found == 0)
            {
                printf("%s", buf);
            }
        }

        return 0;
    }

    for (int i = 2; i < argc; i++)
    {
        FILE *file = fopen(argv[i], "r");
        if (file == NULL)
        {
            printf("wgrep: cannot open file\n");
            return 1;
        }

        char *line = NULL;
        size_t linecap = 0;
        ssize_t linelen;
        regmatch_t match[1];
        while ((linelen = getline(&line, &linecap, file)) > 0)
        {
            // use regexec, "man 3 regex"
            int found = regexec(&re, line, /*nmatch*/ 1, match, /*no flags*/ 0);

            if (found == 0)
            {
                printf("%s", line);
            }
            // look at re eval errors:
            // else {
            //     char errbuf[100];
            //     regerror(found, &re, errbuf, sizeof(errbuf));
            //     printf("%s\n", errbuf);
            // }
        }

        fclose(file);
    }
    regfree(&re);

    return 0;
}