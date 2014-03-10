#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#include <villa.h>

#include "stemming.h"

typedef struct stemmer STEMMER;
typedef VILLA DB;

int dict_add_word(STEMMER* stemmer, DB* dict, char* word, char* desc) {
    int n = strlen(word);
    char* p = word;
    while (*p) {
        if (*p == '\n') { *p = '\0'; break; }
        *p = tolower(*p); p ++;
    }
    word[stem(stemmer, word, n - 1) + 1] = '\0';
    if (!vlput(dict, word, -1, desc, -1, VL_DDUP)) {
        fprintf(stderr, "vlput: %s\n", dperrmsg(dpecode));
        return -1;
    }
    return 0;
}

#define DEFAULT_LINE_LENTGHT 1024

void generate_dict(const char* rawfile, const char* dbfile) {
    if (!rawfile || !dbfile) {
        fprintf(stderr, "generate_dict err\n"); return;
    }

    DB* dict = vlopen(dbfile, VL_OWRITER | VL_OCREAT, VL_CMPLEX);
    if (!dict) { fprintf(stderr, "vlopen: %s\n", dperrmsg(dpecode)); return; }
    STEMMER* stemmer = create_stemmer();

    do {
        FILE* fp = fopen(rawfile, "r");
        if (!fp) { fprintf(stderr, "fopen err\n"); break; }
        int n = 0, sub_n = 0;
        size_t nmax = DEFAULT_LINE_LENTGHT, sub_nmax = DEFAULT_LINE_LENTGHT;
        char* line = (char*) malloc(nmax), *sub_line = (char*) malloc(sub_nmax);
        char desc[1024] = {0};
        while ((n = getline(&line, &nmax, fp)) > 0) {
            line[n-1] = '\0';
            if ((sub_n = getline(&sub_line, &sub_nmax, fp)) <= 0) { break; }
            sub_line[sub_n-1] = '\0';
            sprintf(desc, "%s  %s", line, sub_line);
            dict_add_word(stemmer, dict, line, desc);
        }
        free(line);
        fclose(fp);
    } while (0);

    free_stemmer(stemmer);
    if (!vlclose(dict)) {
        fprintf(stderr, "vlclose: %s\n", dperrmsg(dpecode)); return;
    }
}

int main(int argc, const char *argv[])
{
    if (argc < 3) {
        fprintf(stdout, "input like this:\n\t");
        fprintf(stdout, "%s [rawfile] [dbfile]\n\n", argv[0]);
        return 0;
    }
    const char* rawfile = argv[1];
    const char* dbfile = argv[2];
    generate_dict(rawfile, dbfile);

    return 0;
}
