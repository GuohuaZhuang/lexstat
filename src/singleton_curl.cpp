
#include "singleton_curl.hpp"

#include <curl/curl.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

using namespace lexstat;

SingletonCurl*  SingletonCurl::m_pInstance = NULL;
void*           SingletonCurl::m_curl = NULL;

typedef struct _MemoryBuffer {
    char* mem;
    size_t siz;
} MEMORY_BUFFER;

static size_t writememoryhandle(void* ctx, size_t size, size_t nmemb, void *u) {
    size_t realsize = size * nmemb;
    MEMORY_BUFFER* pu = (MEMORY_BUFFER *) u;
    if(!(pu->mem = (char*) realloc(pu->mem, pu->siz + realsize + 1))) {
        fprintf(stderr, "realloc err\n"); return 0;
    }
    memcpy(pu->mem+pu->siz, ctx, realsize);
    pu->siz += realsize;
    pu->mem[pu->siz] = '\0';
    return realsize;
}

SingletonCurl::MemRelease SingletonCurl::m_MemRelease;

SingletonCurl::SingletonCurl() {
    curl_global_init(CURL_GLOBAL_ALL);
    m_curl = (void*) curl_easy_init();
}

SingletonCurl::~SingletonCurl() {
    if (m_curl) { curl_easy_cleanup((CURL*)m_curl); }
    curl_global_cleanup();
}

int SingletonCurl::GetPage(const char* url, const int urlsiz, 
        char** phtml, int* phtmlsiz) {
    if (!phtml || !phtmlsiz || !url) {
        fprintf(stderr, "%s:%d input invalid!\n", __FILE__, __LINE__);
        return -1;
    }

    MEMORY_BUFFER membuf; membuf.mem = NULL; membuf.siz = 0;
    curl_easy_setopt(m_curl, CURLOPT_URL, url);
    curl_easy_setopt(m_curl, CURLOPT_FOLLOWLOCATION, 1L);
    curl_easy_setopt(m_curl, CURLOPT_WRITEFUNCTION, writememoryhandle);
    curl_easy_setopt(m_curl, CURLOPT_WRITEDATA, (void *)&membuf);
    curl_easy_setopt(m_curl, CURLOPT_USERAGENT, "libcurl-agent/1.0");
    CURLcode res = curl_easy_perform(m_curl);
    if(CURLE_OK != res) {
        fprintf(stderr, "curl_easy_perform() failed: %s\n", 
                curl_easy_strerror(res));
        if (membuf.mem) { free(membuf.mem); }
        return -1;
    } else {
        *phtml = membuf.mem; *phtmlsiz = (int)(membuf.siz);
    }
    return 0;
}

