
#ifndef DEBUG
#define DEBUG 0

#define printdebugfmt(fmt, ...) \
    do {if (DEBUG) fprintf(stderr, "%s:%d:%s()------> " fmt "\n", __FILE__, __LINE__, \
                            __FUNCTION__, __VA_ARGS__); } while (0);

#define printdebug(fmt) \
    do {if (DEBUG) fprintf(stderr, "%s:%d:%s()------> " fmt "\n", __FILE__, __LINE__, \
                            __FUNCTION__); } while (0);

#define ASSERT(cond) assert(cond)

#endif

#define printNewLine() printf("\n")
#define print(fmt) printf(fmt"\n");
#define printargs(fmt, ...) printf(fmt"\n", __VA_ARGS__);
#define printint(fmt) printf("%d\n", fmt)
