
#include "singleton_lexstat.hpp"

#include "lexstat.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

using namespace lexstat;

SingletonLexstat*   SingletonLexstat::m_pInstance = NULL;
void*               SingletonLexstat::m_lexstat = NULL;

SingletonLexstat::MemRelease SingletonLexstat::m_MemRelease;

SingletonLexstat::SingletonLexstat() {
    m_lexstat = (void*) lexstat_init(STOPWORD_DICT, DICT_NUM, 
        DICT_CET_4, DICT_CET_6, DICT_TOFEL, DICT_PG_E, DICT_GRE, DICT_IELTS, 
        DICT_OXFORD);
}

SingletonLexstat::~SingletonLexstat() {
    lexstat_release((LEXSTAT*)m_lexstat);
    m_lexstat = NULL;
}

int SingletonLexstat::GetResult(const char* ext, const int extsiz, 
        void** plexres, int* plexressiz) {
    if (!plexres || !plexressiz) {
        fprintf(stderr, "%s:%d input invalid!\n", __FILE__, __LINE__);
        return -1;
    }

    lexstat_clean((LEXSTAT*)m_lexstat);
    lexstat_statistic((LEXSTAT*)m_lexstat, ext);
    *plexres = ((LEXSTAT*)m_lexstat)->result->list;
    *plexressiz = ((LEXSTAT*)m_lexstat)->result->num;
    return 0;
}

