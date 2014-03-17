
#include "singleton_libxml2.hpp"

#include <libxml/HTMLparser.h>
#include <cstdlib>
#include <cstdio>
#include <cstring>
#include <string>

using namespace lexstat;

#ifdef _MSC_VER
#define COMPARE(a, b) (!stricmp((a), (b)))
#else
#define COMPARE(a, b) (!strcasecmp((a), (b)))
#endif

#define PATTERN_TAG "p"

SingletonLibxml2*  SingletonLibxml2::m_pInstance = NULL;

typedef struct TagContext {
    TagContext(): addtag(false) { }
    bool addtag;
    std::string content;
} TAG_CONTEXT;

static void StartElement(void *ctx, const xmlChar *name, const xmlChar **atts) {
    TAG_CONTEXT* tagctx = (TAG_CONTEXT*) ctx;
    if (COMPARE((char *)name, PATTERN_TAG)) {
        // tagctx->content = "";
        tagctx->addtag = true;
    }
}

static void EndElement(void *ctx, const xmlChar *name) {
    TAG_CONTEXT* tagctx = (TAG_CONTEXT*) ctx;
    if (COMPARE((char *)name, PATTERN_TAG)) {
        tagctx->addtag = false;
    }
}

static void handleCharacters(TAG_CONTEXT* tagctx, const xmlChar* ch, int len) {
    if (tagctx->addtag) {
        tagctx->content.append((char *)ch, len);
        tagctx->content.append(1, ' ');
    }
}

static void Characters(void* ctx, const xmlChar* ch, int len) {
    TAG_CONTEXT* tagctx = (TAG_CONTEXT*) ctx;
    handleCharacters(tagctx, ch, len);
}

static htmlSAXHandler _saxHandler = {
    NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL,
    NULL, NULL, StartElement, EndElement, NULL, Characters, NULL, NULL, NULL,
    NULL, NULL, NULL, NULL, NULL, NULL
};

SingletonLibxml2::MemRelease SingletonLibxml2::m_MemRelease;

SingletonLibxml2::SingletonLibxml2() {
}

SingletonLibxml2::~SingletonLibxml2() {
}

int SingletonLibxml2::GetExtract(const char* html, const int htmlsiz, 
        char** pext, int* pextsiz) {
    if (!pext || !pextsiz) {
        fprintf(stderr, "%s:%d input invalid!\n", __FILE__, __LINE__);
        return -1;
    }

    TAG_CONTEXT tagctx;
    htmlParserCtxtPtr ctxt = htmlCreatePushParserCtxt(
            &_saxHandler, &tagctx, "", 0, "", XML_CHAR_ENCODING_NONE);
    htmlParseChunk(ctxt, html, htmlsiz, 0);
    htmlParseChunk(ctxt, "", 0, 1);
    if ((*pextsiz = tagctx.content.length()) > 0) {
        *pext = strdup(tagctx.content.data());
    } else {
        *pext = NULL;
    }
    htmlFreeParserCtxt(ctxt);
    return 0;
}

