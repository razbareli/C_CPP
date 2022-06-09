#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define LINE 20

char enc_dec(char str[], int k){
    int i;
    if (k < 0){
        k = abs((abs(k)%26)-26);
    }
    int number;
//    printf("%s\n", str);
    for (i=0; i <= (int)strlen(str) ; i++)
    {
        if ('a' <= str[i] && str[i] <= 'z')
        {
            number = (((int)str[i] - 97 + k) % 26) + 97;
            str[i] = (char)number;
        }
        if ('A' <= str[i] && str[i] <= 'Z')
        {
            number = (((int)str[i] - 65 + k) % 26) + 65;
            str[i] = (char)number;
        }
    }
//    printf("%s\n", str);
    return 0;
}


int main(int argc, char *argv[]) {
    if (argc == 1){
        fprintf(stderr, "The given command is invalid\n");
        return EXIT_FAILURE;
    }
    if (strcmp(argv[1], "encode") == 0) {
        if (argc != 5){
            fprintf(stderr, "Usage: cipher <encode|decode> <k> <source path file> <output path file>\n");
            return EXIT_FAILURE;
        }
        FILE *orig = fopen(argv[3], "r");
        if (orig == NULL) {
//            fclose(orig);
            fprintf(stderr, "The given file is invalid\n");
            return EXIT_FAILURE;
        }
        int k = atoi(argv[2]);
        FILE *coded = fopen(argv[4], "w");
        char temp[LINE];
        while (!feof(orig)) {
            fgets(temp, LINE, orig);
            enc_dec(temp, k);
            fputs(temp, coded);
        }
        fclose(orig);
        fclose(coded);
        return 0;
    }
    if (strcmp(argv[1], "decode") == 0) {
        if (argc != 5) {
            fprintf(stderr, "Usage: cipher <encode|decode> <k> <source path file> <output path file>\n");
            return EXIT_FAILURE;
        }
        FILE *orig = fopen(argv[3], "r");
        if (orig == NULL) {
//            fclose(orig);
            fprintf(stderr, "The given file is invalid\n");
            return EXIT_FAILURE;
        }
        int k = atoi(argv[2]);
        k = -1 * k;
        FILE *coded = fopen(argv[4], "w");
        char temp[LINE];
        while (!feof(orig)) {
            fgets(temp, LINE, orig);
            enc_dec(temp, k);
            fputs(temp, coded);
        }
        fclose(orig);
        fclose(coded);
        return 0;
    }
    if (strcmp(argv[1], "check") == 0) {
        if (argc != 4) {
            fprintf(stderr, "Usage: cipher <check> <source path file> <output path file>\n");
            return EXIT_FAILURE;
        }
        FILE *orig = fopen(argv[2], "r");
        FILE *cryp = fopen(argv[3], "r");
        if (orig == NULL || cryp == NULL) {
            fclose(orig);
            fclose(cryp);
            fprintf(stderr, "The given file is invalid\n");
            return EXIT_FAILURE;
        }
        int i;
        for (i = 0; i <= 25; i++) {
            char t_orig[LINE];
            char t_cryp[LINE];
            char B = 0;
            while (fgets(t_orig, LINE, orig) != NULL && fgets(t_cryp, LINE, cryp) != NULL) {
                enc_dec(t_orig, i);
                if (strcmp(t_orig, t_cryp) != 0) {
                    B = 1;
                    rewind(orig);
                    rewind(cryp);
                    break;
                }
            }
            if (B == 0) {
                fprintf(stdout,"Valid encrypting with k = %d\n", i);
                fclose(cryp);
                fclose(orig);
                return EXIT_SUCCESS;
            }
        }
        fprintf(stdout, "Invalid encrypting\n");
        fclose(cryp);
        fclose(orig);
        return EXIT_SUCCESS;
    }
    else{
        fprintf(stderr, "The given command is invalid\n");
        return EXIT_FAILURE;
    }
}


