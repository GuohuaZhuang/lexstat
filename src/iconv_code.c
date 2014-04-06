#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <iconv.h>

char* iconv_impl(const char *ptr, int size,
        const char *icode, const char *ocode, int *sp, int *mp){
    iconv_t ic;
    char *obuf, *wp, *rp;
    size_t isiz, osiz;
    int miss;
    if(size < 0) size = strlen(ptr);
    isiz = size;
    if((ic = iconv_open(ocode, icode)) == (iconv_t)-1) return NULL;
    osiz = isiz * 5; 
    if(!(obuf = malloc(osiz + 1))){
        iconv_close(ic);
        return NULL;
    }
    wp = obuf;
    rp = (char *)ptr;
    miss = 0; 
    while(isiz > 0){
        if(iconv(ic, (void *)&rp, &isiz, &wp, &osiz) == -1){ 
            if(errno == EILSEQ && (*rp == 0x5c || *rp == 0x7e)){
                *wp = *rp; 
                wp++;
                rp++;
                isiz--;
            } else if(errno == EILSEQ || errno == EINVAL){
                rp++;
                isiz--;
                miss++;
            } else {
                break;
            }    
        }    
    }
    *wp = '\0';
    if(iconv_close(ic) == -1){ 
        free(obuf);
        return NULL;
    }
    if(sp) *sp = wp - obuf;
    if(mp) *mp = miss;
    return obuf;
}

#define MAXLINE_BUFF 65535

int main(int argc, const char *argv[])
{
    if (argc < 2) { return -1; }
    FILE* fp = fopen(argv[1], "r");
    if (!fp) { return -1; }
    char* s = malloc(MAXLINE_BUFF);
    if (!s) { return -1; }
    memset(s, 0, MAXLINE_BUFF);
    int outputsiz = 0, misssiz = 0;

    while (fgets(s, MAXLINE_BUFF, fp)) {
        outputsiz = 0; misssiz = 0;
        char* output = iconv_impl(s, -1, 
                "GB18030", "UTF-8", &outputsiz, &misssiz);
        printf("%s", output);
        if (output) { free(output); }
    }

    free(s);
    fclose(fp);
    return 0;
}
