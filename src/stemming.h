#ifndef _STEMMING_H
#define _STEMMING_H

#if defined(__cplusplus)
extern "C" {
#endif

struct stemmer;

extern struct stemmer * create_stemmer(void);
extern void free_stemmer(struct stemmer * z);
extern int stem(struct stemmer * z, char * b, int k);

#if defined(__cplusplus)
}
#endif

#endif // _STEMMING_h

