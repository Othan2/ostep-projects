#include <stdio.h>

int cat_from_file(char* filename)
{
    FILE *opened = fopen(filename, "r");
    if (opened == NULL)
    {
        printf("wcat: cannot open file\n");
        return 1;
    }

    char buf[100];
    while ((fgets(buf, sizeof(buf), opened)) != NULL)
    {
        // I think no errors are happening in my call to fgets but I'm not sure -
        // according to man fgets I'd need to use feof or ferror to tell.
        printf("%s", buf);
    }

    fclose(opened);
    return 0;
}

int main(int argc, char *argv[])
{
    if (argc == 1)
    {
        return 0;
    }

    for (int i = 1; i < argc; i++)
    {
        int ret = cat_from_file(argv[i]);

        if (ret != 0) {
            return ret;
        }
    }

    return 0;
}
