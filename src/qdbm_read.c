#include <depot.h>
#include <cabin.h>
#include <villa.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "stemming.h"

#define WORD_LEN 1024

int main(int argc, char **argv){
    if (argc < 3) {
        printf("[ERR] invalid args!\n");
        printf("%s [dictfile] [word]\n\n", argv[0]);
        return -1;
    }

    char* word = (char*) malloc(WORD_LEN);
    strcpy(word, argv[2]);
    int n = strlen(argv[2]);
    struct stemmer* stemmer = create_stemmer();
    word[stem(stemmer, word, n - 1) + 1] = '\0';

    VILLA *villa;
    char *val;

    /* open the database */
    if(!(villa = vlopen(argv[1], VL_OWRITER | VL_OCREAT, VL_CMPLEX))){
        fprintf(stderr, "vlopen: %s\n", dperrmsg(dpecode));
        return 1;
    }

    /* retrieve the record */
    if(!(val = vlget(villa, word, -1, NULL))){
        fprintf(stderr, "vlget: %s\n", dperrmsg(dpecode));
    } else {
        printf("key: [%s]\n", word);
        printf("value: [%s]\n", val);
        free(val);
    }

    /* close the database */
    if(!vlclose(villa)){
        fprintf(stderr, "vlclose: %s\n", dperrmsg(dpecode));
        return 1;
    }

    free_stemmer(stemmer);
    free(word);

    return 0;
}
