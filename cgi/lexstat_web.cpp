/**
 * @compile: g++ [code_file.cpp] -lfcgi -o [output.fcgi]
 */

#include "fcgi_common.h"
#include "../src/lexstat_api.hpp"

#define ITOA(num) (sprintf(_SN,"%d",num), _SN)

// #define _CGI_DEBUG_

int lexstat_web(const char* url, const int urlsiz, ext_string& result) {
    LS_SWORD* lexres = NULL, *pl = NULL;
    int lexressiz = 0, i = 0;
    if (!lexstat_by_url(url, urlsiz, &lexres, &lexressiz)) {
        pl = lexres;
        while (pl->next) { pl = pl->next; }
        while (pl) {
            if (0 != i ++) { result += ", "; }
            result += "{\"word\": \"";
            result += (pl->sword?pl->sword:"(null)");
            result += "\", \"content\": \"";
            result += (pl->desc?pl->desc:"(null)");
            result += "\", \"type\": \"";
            // result += pl->type;
            result += "\", \"offset\": \"0\"}";
            pl = pl->prev;
        }
    } else {
        result = "{\"ret\": \"10011\", \"msg\": \"lexstat process error\"}";
    }
    return lexressiz;
}

#ifdef _CGI_DEBUG_

#include <stdio.h>

int main(int argc, char* argv[]) {

    if (argc < 2) {
        fprintf(stderr, "%s [url]\n\n", argv[0]); return 0;
    }
	ext_string enurl = argv[1];
	
	ext_string result = "";
    result += "{\"success\": \"加载完成";
    result += "\", \"enurl\": \"";
    result += enurl;
    result += "\", \"d\": [";
    int num = lexstat_web(enurl.data(), enurl.length(), result);
    char _SN[24] = {0};
    result += "], \"num\": \"";
    result += ITOA(num);
    result += "\"}";

	printf("%s", result.data());
}

#else // _CGI_DEBUG_

int main(void) {

	_FCGI_START_
	
	ext_string callback = query_map["callback"];
	ext_string debug = query_map["debug"];
	ext_string enurl = query_map["enurl"];
    char* _enurl = basedecode(enurl.data(), NULL);
    enurl = _enurl; if (_enurl) { free(_enurl); }
	
	ext_string result = "";
    result += "{\"success\": \"加载完成";
    result += "\", \"enurl\": \"";
    result += enurl;
    result += "\", \"d\": [";
    int num = lexstat_web(enurl.data(), enurl.length(), result);
    char _SN[24] = {0};
    result += "], \"num\": \"";
    result += ITOA(num);
    result += "\"}";

	if (0 == debug.compare("1")) result = "{\"ret\": \"0\", \"task_id\": 2543821345}";
	if (result.length() == 0) result = "{\"ret\": \"10008\", \"msg\": \"Param error, see doc for more info\"}";
	if (callback.length() > 0) result = callback + "(" + result + ")";
	
	fcgi_printf("%s", result.data());
	
	_FCGI_END_
	
	return EXIT_SUCCESS;
}

#endif // _CGI_DEBUG_

