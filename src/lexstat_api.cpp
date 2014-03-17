
#include "lexstat_api.hpp"
#include "singleton_curl.hpp"
#include "singleton_libxml2.hpp"
#include "singleton_lexstat.hpp"
#include <cstdio>
#include <cstring>

using namespace lexstat;

int lexstat_by_url(const char* url, const int urlsiz, 
        LS_SWORD** plexres, int* plexressiz) {
    if (!plexres || !plexressiz) {
        fprintf(stderr, "%s:%d input invalid!\n", __FILE__, __LINE__);
        return -1;
    }

    char* html = NULL, *ext = NULL;
    int htmlsiz = 0, extsiz = 0;

    do {
//        html = strdup("<html><head></head><body><p>Loading preliminari</p></body></html>");
//        htmlsiz = strlen(html);
        if (SingletonCurl::GetPage(
                    url, urlsiz, &html, &htmlsiz) < 0) {
            fprintf(stderr, "curl process err!\n"); return -1;
        }
        if (!html) { break; }
        do {
            if (SingletonLibxml2::GetExtract(
                        html, htmlsiz, &ext, &extsiz) < 0) {
                fprintf(stderr, "libxml process err!\n"); break;
            }
            if (!ext) { break; }
            do {
                if (SingletonLexstat::GetResult(
                            ext, extsiz, (void**)plexres, plexressiz) < 0) {
                    fprintf(stderr, "curl process err!\n"); break;
                }
                if (!(*plexres)) { break; }
            } while (0);
            if (ext) { free(ext); }
        } while (0);
        if (html) { free(html); }
    } while (0);

    return 0;
}

