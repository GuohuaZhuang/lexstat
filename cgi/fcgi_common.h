
#ifndef _FCGI_COMMON_H_
#define _FCGI_COMMON_H_

#include <cstring>
#include "ext_string.h"
#include <string>
#include <vector>
#include <map>
#include <cstdio>
#include <cstdlib>
#include <fcgiapp.h>

using std::string;
using std::ext_string;
using std::vector;
using std::map;
using std::iterator;
using std::pair;

/**
 * @brief decode url to utf-8 character.
 * @param url [input] the url to be decode.
 * @return returns a pointer to cn code url. 
 	The caller should deallocate this code url using free(3)..
 */
extern char* UrlDecode(const char* url);

/**
* @brief Encode a serial object with Base64 encoding.
*/
char* baseencode(const char* ptr, int size);

/**
* @brief Decode a string encoded with Base64 encoding.
*/
char* basedecode(const char* str, int *sp);


#define printff(...) FCGX_FPrintF(request->out, __VA_ARGS__) 
#define get_param(KEY) FCGX_GetParam(KEY, request->envp)
#define fcgi_printf(...) FCGX_FPrintF(request.out, __VA_ARGS__) 


void handle_request(FCGX_Request* request, ext_string& query_str)
{
	char *value;
	printff("Content-Type: text/plain\r\n\r\n");
	if ((value = get_param("QUERY_STRING")) != NULL)
	{
		char* cncode = UrlDecode(value);
		if(!cncode) return;
		query_str = cncode;
		free(cncode);
	}
}


#define _FCGI_START_	FCGX_Request request;									\
						FCGX_Init();											\
						FCGX_InitRequest(&request, 0, 0);						\
						while (FCGX_Accept_r(&request) >= 0)					\
						{														\
							ext_string query_str = "";							\
							handle_request(&request, query_str);				\
							map< ext_string, ext_string > query_map;			\
							vector<ext_string> querys = query_str.split('&');	\
							vector<ext_string>::iterator it;					\
							for (it = querys.begin(); it != querys.end(); it++)	\
							{													\
								vector<ext_string> element = it->split('=');	\
								if (element.size() > 1)							\
									query_map.insert(pair<ext_string, 			\
										ext_string>(element[0], element[1]));	\
							}


#define _FCGI_END_		FCGX_Finish_r(&request);								\
						}


extern char* UrlDecode(const char* url)
{
	int n_len = 0;
	if (NULL == url || (n_len = strlen(url)) <= 0) return NULL;
	
	char* ret = new char[n_len + 1];
	memset(ret, '\0', n_len + 1);
	
	char tmp[3] = {0};
	char* prt = ret;
	const char* pus = url;
	const char* pue = url + n_len;
	
	while (pus <= pue)
	{
		if('%' == pus[0] 
			&& (memcpy(tmp, pus + 1, 2), *prt = strtol(tmp, NULL, 16), 0 != *prt))
			pus += 3;
		else
			*prt = *pus, pus++;
		prt++;
	}
	
	return ret;
}

/**
* @brief Encode a serial object with Base64 encoding.
*/
char* baseencode(const char* ptr, int size) {
    if (!ptr) { return NULL; }
    const char* tbl = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";
    char* buf = NULL, * wp = NULL;
    const unsigned char *obj = NULL;
    int i = 0;
    if (size < 0) { size = strlen(ptr); }
    buf = (char*) malloc(4 * (size + 2) / 3 + 1);
    obj = (const unsigned char *)ptr;
    wp = buf;
    for (i = 0; i < size; i += 3) {
        switch (size - i) {
            case 1:
                *wp++ = tbl[obj[0] >> 2];
                *wp++ = tbl[(obj[0] & 3) << 4];
                *wp++ = '=';
                *wp++ = '=';
                break;
            case 2:
                *wp++ = tbl[obj[0] >> 2];
                *wp++ = tbl[((obj[0] & 3) << 4) + (obj[1] >> 4)];
                *wp++ = tbl[(obj[1] & 0xf) << 2];
                *wp++ = '=';
                break;
            default:
                *wp++ = tbl[obj[0] >> 2];
                *wp++ = tbl[((obj[0] & 3) << 4) + (obj[1] >> 4)];
                *wp++ = tbl[((obj[1] & 0xf) << 2) + (obj[2] >> 6)];
                *wp++ = tbl[obj[2] & 0x3f];
                break;
        }
        obj += 3;
    }
    *wp = '\0';
    return buf;
}

/**
* @brief Decode a string encoded with Base64 encoding.
*/
char* basedecode(const char* str, int *sp) {
    if (!str) { return NULL; }
    unsigned char* obj = NULL, * wp = NULL;
    int len = 0, cnt = 0, bpos = 0, i = 0, bits = 0, eqcnt = 0;
    len = strlen(str);
    obj = (unsigned char*) malloc(len + 4);
    wp = obj;
    while (bpos < len && eqcnt == 0) {
        bits = 0;
        for (i = 0; bpos < len && i < 4; bpos++) {
            if (str[bpos] >= 'A' && str[bpos] <= 'Z') {
                bits = (bits << 6) | (str[bpos] - 'A');
                i++;
            } else if (str[bpos] >= 'a' && str[bpos] <= 'z') {
                bits = (bits << 6) | (str[bpos] - 'a' + 26);
                i++;
            } else if (str[bpos] >= '0' && str[bpos] <= '9') {
                bits = (bits << 6) | (str[bpos] - '0' + 52);
                i++;
            } else if (str[bpos] == '+') {
                bits = (bits << 6) | 62;
                i++;
            } else if (str[bpos] == '/') {
                bits = (bits << 6) | 63;
                i++;
            } else if (str[bpos] == '=') {
                bits <<= 6;
                i++;
                eqcnt++;
            }
        }
        if (i == 0 && bpos >= len) { continue; }
        switch (eqcnt) {
            case 0:
                *wp++ = (bits >> 16) & 0xff;
                *wp++ = (bits >> 8) & 0xff;
                *wp++ = bits & 0xff;
                cnt += 3;
                break;
            case 1:
                *wp++ = (bits >> 16) & 0xff;
                *wp++ = (bits >> 8) & 0xff;
                cnt += 2;
                break;
            case 2:
                *wp++ = (bits >> 16) & 0xff;
                cnt += 1;
                break;
        }
    }
    obj[cnt] = '\0';
    if (sp) { *sp = cnt; }
    return (char *)obj;
}

#endif // _FCGI_COMMON_H_

