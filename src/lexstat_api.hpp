
#ifndef _LEXSTAT_API_H_
#define _LEXSTAT_API_H_

#ifdef __cplusplus
extern "C" {
#endif // __cplusplus

#include "lexstat.h"

extern int lexstat_by_url(const char* url, const int urlsiz, 
        LS_SWORD** plexres, int* plexressiz);

#ifdef __cplusplus
}
#endif // __cplusplus

#endif // #define _LEXSTAT_API_H_

