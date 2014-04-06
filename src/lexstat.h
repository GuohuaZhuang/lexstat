
#ifndef _LEXSTAT_H_
#define _LEXSTAT_H_

#if defined(__cplusplus)
extern "C" {
#endif

#ifndef _GNU_SOURCE
#define _GNU_SOURCE
#endif

#include <cabin.h>
#include <villa.h>
#include "stemming.h"

#define STOPWORD_DICT   DATA_DIR"/STOPWORD.DICT"
#define DICT_NUM        7
#define DICT_CET_4      DATA_DIR"/CET-4.DICT"
#define DICT_CET_6      DATA_DIR"/CET-6.DICT"
#define DICT_TOFEL      DATA_DIR"/TOFEL.DICT"
#define DICT_PG_E       DATA_DIR"/PG-E.DICT"
#define DICT_GRE        DATA_DIR"/GRE.DICT"
#define DICT_IELTS      DATA_DIR"/IELTS.DICT"
#define DICT_OXFORD     DATA_DIR"/OXFORD.DICT"

#define PUBLIC
#define PRIVATE static

// use Porter stemming algorithm.
typedef struct stemmer STEMMER;
// use Red-Black tree as hash map.
typedef CBMAP HASHMAP;
// use QDBM villa struct as DB.
typedef VILLA DB;

// OK
// lexical dict type
typedef enum {
    DT_UNKNOWN = 0,                // lexical unknown dict
    DT_CET_4   = 1 << 0,           // lexical dict of CET-4
    DT_CET_6   = 1 << 1,           // lexical dict of CET-6
    DT_PG_E    = 1 << 2,           // lexical dict of Post graduate English
    DT_TOFEL   = 1 << 3,           // lexical dict of TOFEL
    DT_GRE     = 1 << 4,           // lexical dict of GRE
    DT_IELTS   = 1 << 5,           // lexical dict of IELTS
    DT_OXFORD  = 1 << 6            // lexical dict of IELTS
} DICT_TYPE;

// OK
typedef struct _LS_SWORD {
    char* sword;                 // after stemming word
    char* desc;                  // lexical desc or blank
    DICT_TYPE type;              // lexical dict name or others
    struct _LS_SWORD* next;      // use as QUEUE
    struct _LS_SWORD* prev;      // use as QUEUE
} LS_SWORD;

// OK
typedef struct _LS_LISTMAP {
    int num;                     // count of element
    LS_SWORD* list;              // 用来按插入顺序输出结果
    HASHMAP* map;                // 用来鉴定重复
} LS_LISTMAP;

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
PUBLIC void lexstat_clean(LEXSTAT* lexstat);

#if defined(__cplusplus)
}
#endif

#endif // _LEXSTAT_H_

