// This is just presudo code for implement lexstat project.
// About main operator router.

#define _GNU_SOURCE

#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <string.h>
#include <unistd.h>
#include <errno.h>
#include <ctype.h>

#include <cabin.h>
#include <villa.h>

#include "stemming.h"

#define PUBLIC
#define PRIVATE static

// use Porter stemming algorithm.
typedef struct stemmer STEMMER;
// use Red-Black tree as hash map.
typedef CBMAP HASHMAP;
// use QDBM villa struct as DB.
typedef VILLA DB;

#define STOPWORD_DICT   "../data/STOPWORD.DICT"
#define DICT_NUM        6
#define DICT_CET_4      "../data/CET-4.DICT"
#define DICT_CET_6      "../data/CET-6.DICT"
#define DICT_TOFEL      "../data/TOFEL.DICT"
#define DICT_PG_E       "../data/PG-E.DICT"
#define DICT_GRE        "../data/GRE.DICT"
#define DICT_IELTS      "../data/IELTS.DICT"

// OK
// lexical dict type
typedef enum {
    DT_CET_4 = 1 << 0,         // lexical dict of CET-4
    DT_CET_6 = 1 << 1,         // lexical dict of CET-6
    DT_PG_E  = 1 << 2,         // lexical dict of Post graduate English
    DT_TOFEL = 1 << 3,         // lexical dict of TOFEL
    DT_GRE   = 1 << 4,         // lexical dict of GRE
    DT_IELTS = 1 << 5          // lexical dict of IELTS
} DICT_TYPE;

DICT_TYPE GET_DICT_TYPE(const char* file) {
    if (strcasestr(file, "CET")) {
        if (strchr(file, '4')) { return DT_CET_4; }
        else if (strchr(file, '6')) { return DT_CET_6; }
    }
    if (strcasestr(file, "TOFEL")) { return DT_TOFEL; }
    if (strcasestr(file, "GRE")) { return DT_GRE; }
    if (strcasestr(file, "IELTS")) { return DT_IELTS; }
    if (strcasestr(file, "PG")) { return DT_PG_E; }
    return 0;
}

// OK
typedef struct _LS_SWORD {
    char* sword;                 // after stemming word
    char* desc;                  // lexical desc or blank
    DICT_TYPE type;              // lexical dict name or others
    struct _LS_SWORD* next;    // use as QUEUE
    struct _LS_SWORD* prev;    // use as QUEUE
} LS_SWORD;

// OK
typedef struct _LS_LISTMAP {
    LS_SWORD* list;         // 用来按插入顺序输出结果
    HASHMAP* map;           // 用来鉴定重复
} LS_LISTMAP;

// OK
int list_map_push(LS_LISTMAP** plistmap,
    const char* sword, char* desc, DICT_TYPE type) {
    if (!sword || !plistmap) {
        fprintf(stderr, "list_map_push err\n"); return -1;
    }
    if (NULL == *plistmap) {
        *plistmap = (LS_LISTMAP*) malloc(sizeof(LS_LISTMAP));
        if (!(*plistmap)) { fprintf(stderr, "malloc error!\n"); return -1; }
        memset(*plistmap, 0, sizeof(LS_LISTMAP));
    }
    if (!(*plistmap)->map) { (*plistmap)->map = cbmapopen(); }
    int _fv = 1;
    cbmapput((*plistmap)->map, sword, -1, (const char*)&_fv, sizeof(_fv), 0);
    
    LS_SWORD* _ls = (LS_SWORD*) malloc(sizeof(LS_SWORD));
    if (!_ls) { fprintf(stderr, "malloc error!\n"); return -1; }
    memset(_ls, 0, sizeof(LS_SWORD));
    _ls->sword = strdup(sword); _ls->desc = desc; _ls->type = type;
    if (!(*plistmap)->list) { (*plistmap)->list = _ls; }
    else {
        _ls->next = (*plistmap)->list;
        (*plistmap)->list->prev = _ls;
        (*plistmap)->list = _ls;
    }
    return 0;
}

// OK
int list_map_find(LS_LISTMAP* listmap, const char* sword) {
    if (!listmap) { return 0; }
    if (!listmap->map) { return 0; }
    return cbmapget(listmap->map, sword, -1, NULL) ? 1 : 0;
}

// OK
int list_map_close(LS_LISTMAP* listmap) {
    if (!listmap) { return -1; }
    if (listmap->map) { cbmapclose(listmap->map); }
    LS_SWORD* _ls = NULL;
    while (listmap->list) {
        _ls = listmap->list; listmap->list = _ls->next;
        if (_ls->sword) { free(_ls->sword); }
        if (_ls->desc) { free(_ls->desc); }
        free(_ls);
    }
    free(listmap);
    return 0;
}

// OK
int list_map_output(LS_LISTMAP* listmap) {
    if (!listmap) { return 0; }
    if (!listmap->list) { return 0; }
    LS_SWORD* _ls = listmap->list;
    while (_ls->next) { _ls = _ls->next; }
    while (_ls) {
        if (_ls->desc) {
            fprintf(stdout, "%s (%d)\n", _ls->desc, _ls->type);
        } else {
            fprintf(stdout, "%s [OTHERS]\n", _ls->sword);
        }
        _ls = _ls->prev;
    }
    return 1;
}

// OK
int db_search(DB* db, const char* sword) {
    return (db && sword) ? (vlget(db, sword, -1, NULL) ? 1 : 0) : -1;
}

// OK
typedef struct _LEXSTAT {
    int         dnum;
    DICT_TYPE*  dtypes;
    DB**        dicts;
    DB*         stopwords;
    LS_LISTMAP* result;
    STEMMER*    stemmer;
} LEXSTAT;

// PUBLIC functions
PUBLIC LEXSTAT* lexstat_init(const char* fstopword, int dnum, ...);
PUBLIC void lexstat_release(LEXSTAT* lexstat);
PUBLIC int lexstat_statistic(LEXSTAT* lexstat, const char* s);
PUBLIC int lexstat_output_result(LEXSTAT* lexstat);

// OK
LEXSTAT* lexstat_init(const char* fstopword, int dnum, ...) {
    LEXSTAT* lexstat = (LEXSTAT*) malloc(sizeof(LEXSTAT));
    if (!lexstat) { fprintf(stderr, "malloc err\n"); return NULL; }
    memset(lexstat, 0, sizeof(LEXSTAT));
    if (!(lexstat->stopwords = vlopen(fstopword, VL_OREADER, VL_CMPLEX))) {
        fprintf(stderr, "vlopen '%s': %s\n", fstopword, dperrmsg(dpecode));
    }

    lexstat->dnum = dnum;
    lexstat->dicts = (DB**) malloc(sizeof(DB*) * dnum);
    if (!lexstat->dicts) { fprintf(stderr, "malloc err\n"); return NULL; }
    lexstat->dtypes = (DICT_TYPE*) malloc(sizeof(DICT_TYPE) * dnum);
    if (!lexstat->dtypes) { fprintf(stderr, "malloc err\n"); return NULL; }
    memset(lexstat->dicts, 0, sizeof(DB*) * dnum);
    memset(lexstat->dtypes, 0, sizeof(DICT_TYPE) * dnum);
    char* dbfile = NULL;
    va_list ap; int _i = dnum;
    va_start(ap, dnum);
    while (-- _i >= 0) {
        dbfile = va_arg(ap, char*);
        if (!(lexstat->dicts[_i] = vlopen(dbfile, VL_OREADER, VL_CMPLEX))) {
            fprintf(stderr, "vlopen '%s': %s\n", dbfile, dperrmsg(dpecode));
        }
        lexstat->dtypes[_i] = GET_DICT_TYPE(dbfile);
    }
    va_end(ap);

    lexstat->stemmer = create_stemmer();

    return lexstat;
}

// OK
void lexstat_release(LEXSTAT* lexstat) {
    if (lexstat->dicts && lexstat->dnum > 0) {
        while (-- lexstat->dnum >= 0) {
            if (lexstat->dicts[lexstat->dnum] 
                && !vlclose(lexstat->dicts[lexstat->dnum])) {
                fprintf(stderr, "vlclose: %s\n", dperrmsg(dpecode));
            }
        }
        free(lexstat->dicts);
    }
    if (lexstat->dtypes) {
        free(lexstat->dtypes);
    }
    if (lexstat->stopwords) {
        if(!vlclose(lexstat->stopwords)){
            fprintf(stderr, "vlclose: %s\n", dperrmsg(dpecode));
        }
    }
    if (lexstat->result) {
        list_map_close(lexstat->result);
    }

    free_stemmer(lexstat->stemmer);

    free(lexstat);
}

int lexstat_dict_find(LEXSTAT* lexstat, const char* sword, 
    char** pdesc, DICT_TYPE* ptype) {
    if (!lexstat || !sword || !pdesc || !ptype) {
        fprintf(stderr, "lexstat_dict_find err\n"); return -1;
    }
    if (!lexstat->dicts || lexstat->dnum < 1) { return 0; }
    *ptype = 0;
    char* val = NULL;
    int i = 0;
    for (i = 0; i < lexstat->dnum; i ++) {
        if (NULL == lexstat->dicts[i]) { continue; }
        if ((val = vlget(lexstat->dicts[i], sword, -1, NULL))) {
            *ptype |= lexstat->dtypes[i];
            if (NULL == *pdesc) { *pdesc = val; }
            else { free(val); }
        }
    }
    return ((0 == *ptype) ? 0 : 1);
}

// OK
char* lexstat_stemming(LEXSTAT* lexstat, char* word) {
    if (!lexstat || !word) { return NULL; }
    int n = strlen(word);
    char* p = word;
    while (*p) { *p = tolower(*p); p ++; }
    word[stem(lexstat->stemmer, word, n - 1) + 1] = '\0';
    return word;
}

// OK
int lexstat_statistic(LEXSTAT* lexstat, const char* s) {
    if (!lexstat || !s) { return -1; }
    LEXSTAT* lex = lexstat;
    char* desc = NULL, * sword = NULL;
    DICT_TYPE type = 0;
    const char* delim = " \n\t\r,.\"";
    char* word = NULL, * sdup = strdup(s), * _sdup = NULL, * _ssave = NULL;
    for (_sdup = sdup; (word = strtok_r(_sdup, delim, &_ssave)); _sdup = NULL) {
        sword = lexstat_stemming(lex, word); desc = NULL; type = 0;
        if (list_map_find(lex->result, sword) > 0) { continue; }
        if (db_search(lex->stopwords, sword) > 0) { continue; }
        if (lexstat_dict_find(lex, sword, &desc, &type) > 0) {
            list_map_push(&(lex->result), sword, desc, type);
        } else {
            list_map_push(&(lex->result), sword, NULL, 0);
        }
    }
    free(sdup);
    return -1;
}

// OK
int lexstat_output_result(LEXSTAT* lexstat) {
    if (!lexstat) { return -1; }
    if (!lexstat->result) { return 0; }
    list_map_output(lexstat->result);
    return 0;
}

#define DEFAULT_LINE_LENGTH 1024

// OK
void lexstat_file_test(const char* filename) {
    FILE* fp = fopen(filename, "r");
    if (!fp) {
        fprintf(stderr, "fopen %s failed\n", filename); return;
    }
    LEXSTAT* lexstat = lexstat_init(STOPWORD_DICT, DICT_NUM, DICT_CET_4, 
        DICT_CET_6, DICT_TOFEL, DICT_PG_E, DICT_GRE, DICT_IELTS);
    int _line_len = 0; size_t _line_lenmax = DEFAULT_LINE_LENGTH;
    char* _line = (char*) malloc(_line_lenmax);
    while ((_line_len = getline(&_line, &_line_lenmax, fp)) > 0) {
        _line[_line_len-1] = '\0';
        if ('\0' != _line[0]) {
            lexstat_statistic(lexstat, _line);
        }
    }
    free(_line);
    lexstat_output_result(lexstat);
    lexstat_release(lexstat);
    fclose(fp);
}

// OK
int main(int argc, const char *argv[])
{
    if (argc < 2) {
        fprintf(stderr, "Input invalid! Run it like this:\n\t");
        fprintf(stdout, "%s [filename]\n\n", argv[0]);
        return -1;
    }
    const char* filename = argv[1];
    if (-1 == access(filename, R_OK)) {
        switch (errno) {
            case EACCES: 
                fprintf(stderr, "No permission to read %s\n", filename); break;
            case ENAMETOOLONG:
                fprintf(stderr, "pathname is too long\n"); break;
            case ENOENT:
                fprintf(stderr, "%s is not exist\n", filename); break;
            default:
                fprintf(stderr, "can not access %s\n", filename);
        }
        return -1;
    }
    lexstat_file_test(filename);

    return 0;
}
