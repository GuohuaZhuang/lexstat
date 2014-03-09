// This is just presudo code for implement lexstat project.
// About main operator router.
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <unistd.h>
#include <errno.h>

#define PUBLIC
#define PRIVATE static

// use Red-Black tree as hash map.
typedef RBT HASHMAP;
// use QDBM villa struct as DB.
typedef VILLA DB;

// OK
// lexical dict type
typedef enum {
	CET_4 = 1 << 0, 		// lexical dict of CET-4
	CET_6 = 1 << 1, 		// lexical dict of CET-6
	PG_E  = 1 << 2, 		// lexical dict of Post graduate English
	TOFEL = 1 << 3, 		// lexical dict of TOFEL
	GRE   = 1 << 4, 		// lexical dict of GRE
	IELTS = 1 << 5  		// lexical dict of IELTS
} DICT_TYPE;

// OK
typedef struct _LS_SWORD {
	char* sword;            // after stemming word
	char* desc;             // lexical desc or blank
	DICT_TYPE type;         // lexical dict name or others
	struct _sword* next;    // use as QUEUE
	struct _sword* prev;    // use as QUEUE
} LS_SWORD;

// OK
typedef struct _LS_LISTMAP {
	SWORD* list;            // 用来按插入顺序输出结果
	HASHMAP* map;           // 用来鉴定重复
} LS_LISTMAP;

// TODO
int list_map_push(
	LS_LISTMAP* listmap, const char* sword, char* desc, DICT_TYPE type);

// TODO
int list_map_find(
	LS_LISTMAP* listmap, const char* sword);

// TODO
int db_search(
	DB* db, const char* sword);

// TODO
int stopword_filter(
	DB* stopwords, const char* sword) {
	return db_search(stopwords, sword);
}

// TODO
typedef struct _LEXSTAT {
	int			dnum;
	DB** 		dicts;
	DB*			stopwords;
	LS_LISTMAP* result;
} LEXSTAT;

// PUBLIC functions
PUBLIC LEXSTAT* lexstat_init(void);
PUBLIC void lexstat_release(LEXSTAT* lexstat);
PUBLIC int lexstat_statistic(LEXSTAT* lexstat, const char* s);
PUBLIC int lexstat_output_result(LEXSTAT* lexstat);

// OK
LEXSTAT* lexstat_init(void) {
	LEXSTAT* lexstat = (LEXSTAT*) malloc(sizeof(LEXSTAT));
	memset(lexstat, 0, sizeof(LEXSTAT));
	return lexstat;
}

// OK
void lexstat_release(LEXSTAT* lexstat) {
	free(lexstat);
}

// TODO
int lexstat_statistic(LEXSTAT* lexstat, const char* s) {
	LEXSTAT lex = lexstat;
	char* desc = NULL;
	DICT_TYPE type = 0;
	for (word in string) {                              // split
		sword = stemming(word);                         // stemming
		if (list_map_find(lex->result, sword) > 0) { continue; }
		if (stopword_filter(lex->stopword, sword) > 0) { continue; }
		if (lexical_dict_find(lex->dicts, lex->dnum, sword, &desc, &type) > 0) {
			list_map_push(lex->result, sword, desc, type);
		} else {
			list_map_push(lex->result, sword, NULL, 0);
		}
	}
	return -1;
}

// TODO
int lexstat_output_result(LEXSTAT* lexstat) {
	return -1;
}

// OK
void lexstat_file_test(const char* filename) {
	FILE* fp = fopen(filename, "r");
	if (!fp) {
		fprintf("fopen %s failed\n", filename); return;
	}
	LEXSTAT* lexstat = lexstat_init();
	size_t _line_len = 0;
	char* _line = NULL;
	while (-1 != (_line_len = getline(&_line, NULL, fp))) {
		line[_line_len-1] = '\0';
		if ('\0' != line[0]) {
			lexstat_statistic(lexstat, line);
		}
		free(_line);
	}
	lexstat_output_result(lexstat);
	lexstat_release(lexstat);
	fclose(fp);
}

// OK
int main(int argc, const char *argv[])
{
	if (argc < 2) {
		fprintf(stderr, "Input invalid! Run it like this:\n\t");
		fprintf(stdout, "%s [filename]\n\n", argv[0]);
		return -1;
	}
	const char* filename = argv[1];
	if (-1 == access(filename, R_OK)) {
		switch (errno) {
			case EACCES: 
				fprintf(stderr, "No permission to read %s\n", filename); break;
			case ENAMETOOLONG:
				fprintf(stderr, "pathname is too long\n"); break;
			case ENOENT:
				fprintf(stderr, "%s is not exist\n", filename); break;
			default:
				frpintf(stderr, "can not access %s\n", filename);
		}
	}
	lexstat_file_test(filename);

	return 0;
}
